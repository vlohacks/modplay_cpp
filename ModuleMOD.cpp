/*
 * Copyright 2015 Florian Feucht
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "ModuleMOD.hpp"
#include <cstdio>
#include <cstring>

namespace vmp
{
    
    ModuleMOD::ModuleMOD() 
    {
        
    }
    
    ModuleMOD::~ModuleMOD() 
    {
        int i;
        
        if (songTitle)
            delete[] songTitle;
            
        if (orders)
            delete[] orders;
            
        if (initialPanning)
            delete[] initialPanning;
            
        if (patterns) {
            for (i = 0; i < numPatterns; ++) {
                delete[] patterns[i].data;
            }
            delete[] patterns;
        }
            
    }
    
    bool ModuleMOD::loadCheck(Io* io) 
    {
        int i;
        u32 signature = 0;
        size_t saved_pos;
        char tmp[5];

        saved_pos = io->tell();
        io->seek(0x438, Io::IO_SEEK_SET);
        signature = io->readU32();
        io->seek(saved_pos, Io::IO_SEEK_SET);

        // Probe for standard MOD types
        for (i = 0; i < numModTypes; i++) {
            if (signature == modTypes[i].signature) 
                return true;
        }

        // Probe for FT2 xxCH 10 channel+ signature
        for (i = 10; i <= 32; i += 2) {
            sprintf(tmp, "%02iCH", i);
            if (memcmp (&signature, tmp, 4) == 0) 
                return true;
        }

        /* TODO: Check if the file MIGHT BE a STK (get sample sizes, compare to file
         * size since there is no magic/header
         */

        return false;
        
    }
    
    
    void ModuleMOD::load(Io* io)
    {
        u32 signature;
        u8 tmp8;
        int i;
        char tmp[5];
        
        // Default values for MOD files
        initialBpm = 125;
        initialSpeed = 6;


        // Determine mod file type by checking the signature (M.K., nCHN...)
        io->seek(0x438, Io::SEEK_SET);
        signature = io->readU32();
        
        numSamples = 0;
        
        // Probe for standard MOD types
        for (i = 0; i < numModTypes; i++) {
            if (signature == modTypes[i].signature) {
                // valid signature means protracker mod with 31 sample slots
                numTracks = modTypes[i].numTracks;
                numSamples = 31;
                break;
            }
        }

        if (numSamples == 0) {
            // Probe for FT2 xxCH 10 channel+ signature
            for (i = 10; i <= 32; i += 2) {
                sprintf(tmp, "%02iCH", i);
                if (memcmp (&signature, tmp, 4) == 0) {
                    this->numTracks = i;
                    this->numSamples = 31;
                    break;
                }
            }
        }

        // no valid signature found means old SoundTracker MOD (STK) with 15 sample
        // slots and 4 channels
        if (numSamples == 0) {
            numSamples = 15;
            numTracks = 4;
        }

        // now we know what kind of MOD it is, so we can start with REAL loading
        songTitle = new char[20];
        io->seek(0, Io::IO_SEEK_SET);
        io->read(songTitle, 1, 20);

        // load sample header data (aka song message :-))
        module->samples = (module_sample_t *)malloc(sizeof(module_sample_t) * module->num_samples);
        for (i = 0; i < module->num_samples; i++) {
            if (loader_mod_read_sample_header(&(module->samples[i].header), h)) {
                free(module);
                return 0;
            }
        }

        // read number of orders in mod
        numOrders = io->readU8();

        // read not used "load patterns" / "loop position" / whatever
        (void)io->readU8();

        // read order list
        orders = new u8[128];
        io->read(&orders, 1, 128);

        // read signature again, just to move the filepointer - and only if the
        // file is not a STK not having a signature
        if (numSamples > 15) 
            (void)io->readU32();
        

        // determine number of patterns im MOD by fetching highest entry in orders
        numPatterns = 0;
        for (i = 0; i < numOrders; i++) {
            if (orders[i] > numPatterns)
                numPatterns = orders[i];
        }
        numPatterns++;

        // load pattern data - TODO: special FLT8 arrangement - currently broken
        module->patterns = (module_pattern_t *)malloc(sizeof(module_pattern_t) * module->num_patterns);
        for (i = 0; i < module->num_patterns; i++) {
            module->patterns[i].num_rows = 64;        // mod alwas has 64 rows per pattern
            module->patterns[i].rows = (module_pattern_row_t *)malloc(sizeof(module_pattern_row_t) * module->patterns[i].num_rows);
            for (j = 0; j < module->patterns[i].num_rows; j++) {
                module->patterns[i].rows[j].data = (module_pattern_data_t *)malloc(sizeof(module_pattern_data_t) * module->num_channels);
                for (k = 0; k < module->num_channels; k++) {
                    if(loader_mod_read_pattern_data (&(module->patterns[i].rows[j].data[k]), h)) {
                        free(module);
                        return 0;
                    }
                }
            }
        }

        // load sample pcm data
        for (i = 0; i < module->num_samples; i++) 
            loader_mod_read_sample_data(&(module->samples[i]), h);

        // initial pannings
        initialPanning = new u8[numTracks];
        for (i = 0; i < numTracks; i++) {
            if (((i % 4) == 1) || ((i % 4) == 2))           // LRRL LRRL ...
                initialPanning[i] = 0xff;
            else
                initialPanning[i] = 0x00;
        }
        
    }
}
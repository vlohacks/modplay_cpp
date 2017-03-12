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
#include "ByteSex.hpp"

#include <cstdio>
#include <cstring>


namespace vmp
{
    
    constexpr ModuleMOD::modtype_t ModuleMOD::modTypes[numModTypes];
    
    
    ModuleMOD::ModuleMOD()
        : Module(MODULE_TYPE_MOD)
    {
    }
    
    // check if the file is a valid MOD file
    bool ModuleMOD::loadCheck(Io& io) 
    {
        u32 signature = 0;
        size_t saved_pos;
        char tmp[5];
        int i;

        saved_pos = io.tell();
        io.seek(0x438, Io::IO_SEEK_SET);
        signature = io.readU32le();
        io.seek(saved_pos, Io::IO_SEEK_SET);

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
    
    
    void ModuleMOD::load(Io& io)
    {
        u32 signature;
        int i, j, k;
        char tmp[21];
        
        // Default values for MOD files
        initialBpm = 125;
        initialSpeed = 6;


        // Determine mod file type by checking the signature (M.K., nCHN...)
        io.seek(0x438, Io::IO_SEEK_SET);
        signature = io.readU32le();
        
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
        // slots and 4 channels... hopefully :-)
        if (numSamples == 0) {
            numSamples = 15;
            numTracks = 4;
        }
        
        samples = vector<Sample>(numSamples);

        // now we know what kind of MOD it is, so we can start with REAL loading
        io.seek(0, Io::IO_SEEK_SET);
        io.read(tmp, 1, 20);
        tmp[20] = 0;
        songTitle = string(tmp);
        
        // load sample header data (aka song message :-))
        for (i = 0; i < numSamples; i++)
            loadSampleHeader(samples[i], io);

        // read number of orders in mod
        numOrders = io.readU8();

        // read not used "load patterns" / "loop position" / whatever
        static_cast<void>(io.readU8());

        // read order list. The MOD contains always all possible 128 bytes
        // despite the actual count of orders.
        for (i = 0; i < 128; i++) {
            u8 tmp8 = io.readU8();
            if (i < numOrders)
                orders.push_back(tmp8);
        }

        // read signature again, just to move the filepointer - and only if the
        // file is not a STK not having a signature
        if (numSamples > 15) 
            static_cast<void>(io.readU32be());
        
        // determine number of patterns im MOD by finding highest entry in orders
        numPatterns = 0;
        for (i = 0; i < numOrders; i++) {
            if (orders[i] > numPatterns)
                numPatterns = orders[i];
        }
        numPatterns++;
        
        // load pattern data - TODO: special FLT8 arrangement - currently broken 
        // - need FLT8 example here
        for (i = 0; i < numPatterns; i++) {
            patterns.push_back(Pattern(64, numTracks));
            Pattern& pattern = getPattern(i);
            for (j = 0; j < pattern.getNumRows(); j++) {
                for (k = 0; k < numTracks; k++) {
                    PatternData& data = this->getData(i, j, k);
                    this->loadPatternData(data, io);
                }
            }
        }

        // read sample PCM data to samples
        for (i = 0; i < numSamples; i++)
            samples[i].loadIo(io, Sample::PCM_S8);

        // initial pannings
        initialPanning = vector<u8>(numTracks);
        for (i = 0; i < numTracks; i++) {
            if (((i % 4) == 1) || ((i % 4) == 2))           // LRRL LRRL ...
                initialPanning[i] = 0xff;
            else
                initialPanning[i] = 0x00;
        }
        
    }

    
    void ModuleMOD::loadSampleHeader(Sample& sample, Io& io)
    {
        char tmp[23];
        
        u32 l, ls, ll, le;

        io.read(tmp, 22, 1);
        tmp[22] = 0;
        sample.setName(string(tmp));
        
        l = static_cast<u32>(io.readU16be()) << 1;
        
        sample.setLength(l);
        sample.setFinetune(io.readS8() & 0x0f);
        sample.setDefaultVolume(io.readU8());
        
        ls = static_cast<u32>(io.readU16be()) << 1;
        ll = static_cast<u32>(io.readU16be()) << 1;
        
        le = (ll + ls) - 1;
        
        // There seem to be mods with loop length past sample length 
        // (example: moby - apology, sample #06)
        // assume last sample = loop end in this case....
        if (le > l)
            le = l;
        
        sample.setLoop(ll > 2, ls, le);
    }    
    
    
    void ModuleMOD::loadPatternData(PatternData& data, Io& io) 
    {
        u8  tmp8;
        u8  tmpFxCmd;
        u8  tmpFxVal;
                
        u32 dw;
        u16 tmp16;
        int tmp;

        /* retrieve the doubleword where all information for one dataset is in
        Byte 0   Byte 1   Byte 2   Byte 3
        SSSSPPPP pppppppp sssscccc FFFFffff
        SSSSssss     = sample number
        PPPPpppppppp = period value
        cccc         = effect command
        FFFFffff     = effect parameters
        */        
        dw = io.readU32be();

        // sample number (scattered around in 2 places)
        tmp8 = (static_cast<u8>(dw >> 24) & 0xf0) | (static_cast<u8>(dw >> 12) & 0x0f);
        if (tmp8 > 0)
            data.setInstrument(tmp8 - 1);
        
        // 24 bits period data
        tmp16 = static_cast<u16>((dw >> 16) & 0x0fff);
        if (tmp16) {
            tmp = lookupPeriodIndex(tmp16);
            if (tmp >= 0)
                data.setNote(static_cast<u8>(tmp));
            else
                fprintf(stderr, "Loader: WARNING: Non-standard period: %i @ %x\n", tmp, (unsigned int)io.tell());
        }
        
        // effect command and value
        tmpFxCmd = static_cast<u8>((dw & 0x0f00) >> 8);
        tmpFxVal = static_cast<u8>(dw & 0xff);
        
        // special case for .MOD: effect 000 = no effect, effect 0xy = arpeggio
        if ((tmpFxCmd > 0) || ((tmpFxCmd == 0) && tmpFxVal > 0))
            data.setEffectCmd(tmpFxCmd);
        
        if (data.hasEffectCmd())
            data.setEffectValue(tmpFxVal);
    }

    // lookup a note index in the period table
    s16 ModuleMOD::lookupPeriodIndex(const u16 period)
    {
        int i;
        for (i = 0; i < DefsMOD::numPeriods; i++) {
            if (DefsMOD::periods[i] == period)
                return i;
        }
        return -1;
    }
    
}


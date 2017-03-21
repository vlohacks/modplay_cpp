/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "ModuleS3M.hpp"
#include <cstring>
#include <vector>

namespace vmp
{

    ModuleS3M::ModuleS3M() 
        : Module(MODULE_TYPE_S3M)
    {}
    
    bool ModuleS3M::loadCheck(Io& io)
    {
        char    signature[4];
        size_t  savedPos;

        memset(signature, 0, 4);

        savedPos = io.tell();
        io.seek(0x2c, Io::IO_SEEK_SET);
        io.read(signature, 4, 1);
        io.seek(savedPos, Io::IO_SEEK_SET);

        if (!memcmp(signature, "SCRM", 4))
            return true;

        return false;
    }
        
    void ModuleS3M::load(Io& io)
    {
        int     r, i, j, k;
        u32     tmp_u32;
        u16     tmp_u16;
        u8      tmp_u8;
        u8      channel_map[32];
        u8      default_panning;
        u32     sample_memseg;
        char    tmp_name[32];

        u16 num_patterns_internal;
        vector<u16> parapointer_pattern;
        vector<u16> parapointer_sample;

        // check if we really deal with a S3M file. If not, bail out
        if (!loadCheck(io)) {
            // TODO throw some Exception
            return;
        }
        
        // read the song name
        io.seek(0, Io::IO_SEEK_SET);
        io.read(tmp_name, 1, 28);
        songTitle = string(tmp_name);
        
        // read num_orders, num_samples, num_patterns
        io.seek(0x20, Io::IO_SEEK_SET);
        numOrders = io.readU16le();
        numSamples = io.readU16le();
        num_patterns_internal = io.readU16le();

        // read flags
        tmp_u16 = io.readU16le();
        if (tmp_u16 & S3MFlags::ST2VIBRATO > 0)
            setFlag(S3MFlags::ST2VIBRATO);
        
        if (tmp_u16 & S3MFlags::ST2TEMPO > 0)
            setFlag(S3MFlags::ST2TEMPO);
        
        if (tmp_u16 & S3MFlags::AMIGASLIDES > 0)
            setFlag(S3MFlags::AMIGASLIDES);
        
        // TODO left out "0vol optimizations"   (8)
        
        if (tmp_u16 & S3MFlags::AMIGALIMITS > 0)
            setFlag(S3MFlags::AMIGALIMITS);
        
        // TODO left out "filter/sfx"  (32)
        if (tmp_u16 & S3MFlags::ST30VOLUMESLIDES > 0)
            setFlag(S3MFlags::ST30VOLUMESLIDES);
        
        // read version
        version = io.readU16le();

        
        /* Set version specific flags:
         * st3 3.0 suffers from bug where volume slides start also on tick 0.
         * Enable crippled volume slides to be compatible with files created with
         * this buggy version
         * This flag gets checked in effects */
        if (version == 0x1300)
            setFlag(S3MFlags::ST30VOLUMESLIDES);

        // TODO: currently ignoring sample format, as it is always unsigned

        io.seek(0x31, Io::IO_SEEK_SET);
        initialSpeed = io.readU8();
        initialBpm = io.readU8();
        initialMasterVolume = io.readU8();
        
        // skip ultraclick stuff... gus is dead.
        io.seek(0x35, Io::IO_SEEK_SET); 
        
        default_panning = io.readU8();
        
        /* TODO: we currently ignore GLOBAL_VOLUME .. maybe it will turn out that
         it is a good idea to deal with it... */

        /* initialize channel map. Since individual channels can be disabled in
         * ST3 (also in between enabled channels), active channels need to be 
         * mapped to their absolute channel num. The loader just skips disabled
         * channels
         */
        for (i = 0; i < 32; i++)
            channel_map[i] = 255;

        // read channel infos
        io.seek(0x40, Io::IO_SEEK_SET);
        numTracks = 0;
        
        for (i = 0; i < 32; i++) {
            tmp_u8 = io.readU8();
            // value < 16 means that this channel is enabled
            if (tmp_u8 < 16) {
                channel_map[i] = numTracks;
                
                if (tmp_u8 <= 7) 
                    initialPanning.push_back(0x33);
                else 
                    initialPanning.push_back(0xcc);

                numTracks++;
            }
        }

        /* read orders and determine REAL number of patterns. FS3MDOC.TXT says 
         * we cannot rely on what the header data says 
         */
        j = 0;
        numPatterns = 0;
        for (i = 0; i < numOrders; i++) {
            tmp_u8 = io.readU8();
            if (tmp_u8 < 254) {
                orders.push_back(tmp_u8);
                if (tmp_u8 > numPatterns) 
                    numPatterns = tmp_u8;
            }
        }
        numOrders = orders.size();
        numPatterns++;

        /*
        printf ("num orders: %i, num_patterns: %i, num_channels: %i\n", module->num_orders, module->num_patterns, module->num_channels);
        for (i = 0; i < module->num_orders; i++)
            printf(" %i\n", module->orders[i]);
         */

        /* read sample and pattern parapointers 
         * "Parapointers" are just offsets to the specific items in the file
         */
        parapointer_sample.resize(numSamples);
        parapointer_pattern.resize(num_patterns_internal);

        io.read(parapointer_sample.data(), sizeof(u16), numSamples);
        io.read(parapointer_pattern.data(), sizeof(u16), num_patterns_internal);


        /* read default pan positions 
         * TODO: This never worked as described in FS3MDOC: S3Ms were panned 
         * completely to 0 (left). Therefore it has been disabled, further
         * investigation is needed here.
         */
        if (default_panning == 0xfc) {
            for (i = 0; i < 32; i++)  {
                tmp_u8 = io.readU8();
                if (tmp_u8 & 16) {
                    tmp_u8 &= 0x0f;
                    // TODO set initial Panning to tmp_u8 as soon I know how this works
                }
            }
        }

        /* flag in the master volume byte indicates 
         * whether the song is stereo or mono 
         */
        if ((initialMasterVolume & 128) == 0) {
            /* make the song mono */
            for (i=0; i<initialPanning.size(); i++) 
                initialPanning[i] = 0x7f;

            setFlag(S3MFlags::MONO);
        }

        
        //samples = vector<Sample>(numSamples);
        // read sample headers (instruments)
        for (i = 0; i < numSamples; i++) {
            u8 sample_type;
            bool sample_16bit;
            u32 ls, le;
            
            samples.push_back(Sample());
            Sample& currentSample = samples.back();
            
            io.seek(parapointer_sample[i] << 4, Io::IO_SEEK_SET);
            // read sample type
            sample_type = io.readU8();

            /* skip the "dos filename" 
             * FS3MDOC.TXT is wrong here, it states this are 13 chars,
             * actually it's 12 chars according to ST3 TECH.DOC
             */
            io.seek(12, Io::IO_SEEK_CUR);

            // read sample "memseg" - which is stored in 3 bytes
            tmp_u8 = io.readU8();
            tmp_u16 = io.readU16le();
            sample_memseg = ((uint32_t)tmp_u8 << 16) + tmp_u16;
            
            // sample length
            tmp_u32 = io.readU32le();
            currentSample.setLength(tmp_u32);
            
            // loop start (upper 2 bytes might contain garbage)
            ls = io.readU32le() & 0xffff;

            // loop end (actually the first sample NOT being played in loop)
            le = io.readU32le() & 0xffff;
            
            // volume
            tmp_u8 = io.readU8();
            currentSample.setDefaultVolume(tmp_u8);

            // Skip unused byte and packing scheme
            io.seek(2, Io::IO_SEEK_CUR);

            /* flags 
             * (1)          = loop
             * (1<<1)       = stereo sample (never used, ignored for now)
             * (1<<2)       = 16 bit sample: supported by IT/Schism S3M Export 
             *              used by some Tracks of the UNREAL soundtrack
             *              (The EPIC shooter game, not the FC demo ;-))
             */
            tmp_u8 = io.readU8();
            currentSample.setLoop(tmp_u8 & 1, ls, le - 1) ;    
            sample_16bit = ((tmp_u8 & (1<<2)) != 0) ? true : false;

            // c2spd
            tmp_u32 = io.readU32le() & 0xffff;
            currentSample.setMiddleCSpeed(tmp_u32);
            
            // Skip unused bytes
            io.seek(12, Io::IO_SEEK_CUR);

            // sample name
            io.read(tmp_name, 1, 28);
            currentSample.setName(string(tmp_name));

            /* if we deal with a adlib instrument or a empty sample slot, continue
             * without loading data
             */
            if (sample_type != 1) {
                if (sample_type > 1)
                    fprintf(stderr, __FILE__ " sample %i - unsupported type: %i (most likely ADLIB)\n", i ,sample_type);
                continue;
            }

            // fetch sample data
            io.seek(sample_memseg << 4, Io::IO_SEEK_SET);
            if (sample_16bit)
                currentSample.loadIo(io, Sample::PCM_U16_LE);
            else
                currentSample.loadIo(io, Sample::PCM_U8);
        }

                       
        // read pattern data
        u8 packed_flags;
        u8 channel_num;
        u16 packed_size;
        PatternData tmp_data;

        for (i = 0; i < numPatterns; i++) {
            
            // S3M patterns always have 64 rows
            patterns.push_back(Pattern(64, numTracks));
            Pattern& currentPattern = patterns.back();
            
            io.seek(parapointer_pattern[i] << 4, Io::IO_SEEK_SET);
            packed_size = io.readU16le();

            for (j = 0; j < 64; j++) {
                
                do {
                    packed_flags = io.readU8();
                    tmp_data.clearAll();
                    
                    if (packed_flags > 0) {
                        channel_num = packed_flags & 31;

                        // check if data conains note and sample information
                        if (packed_flags & 32) {
                            tmp_u8 = io.readU8();
                            if (tmp_u8 == 255)                      // 255 = no note information
                                tmp_data.clearNote();
                            else if (tmp_u8 == 254)                 // 254 = note off command (the ^^^ thing in ST3) TODO: avoid magic value here
                                tmp_data.setNote(254);
                            else                                    //regular note, low nibble = note, high nibble = octave
                                tmp_data.setNote( ((tmp_u8 >> 4) * 12) + (tmp_u8 & 0x0f) );

                            tmp_u8 = io.readU8();
                            if (tmp_u8 > 0)
                                tmp_data.setInstrument(tmp_u8 - 1);
                        }

                        // check if data contains volume information
                        if (packed_flags & 64) {
                            tmp_u8 = io.readU8();
                            tmp_data.setVolume(tmp_u8);
                        }

                        // check if data contains effect information
                        if (packed_flags & 128) {
                            tmp_u8 = io.readU8();
                            tmp_data.setEffectCmd(tmp_u8);
                            tmp_u8 = io.readU8();
                            tmp_data.setEffectValue(tmp_u8);
                        }

                        if (channel_map[channel_num] < 255) {
                            PatternData& currentData = currentPattern.getRow(j)[channel_num];
                            if (tmp_data.hasNote())
                                currentData.setNote(tmp_data.getNote());
                            
                            if (tmp_data.hasInstrument())
                                currentData.setInstrument(tmp_data.getInstrument());
                            
                            if (tmp_data.hasVolume())
                                currentData.setVolume(tmp_data.getVolume());
                            
                            if (tmp_data.hasEffectCmd())
                                currentData.setEffectCmd(tmp_data.getEffectCmd());
                            
                            if (tmp_data.hasEffectValue())
                                currentData.setEffectValue(tmp_data.getEffectValue());
                        }                    
                    }

                } while (packed_flags);
            }
        }
        
    }
    

}
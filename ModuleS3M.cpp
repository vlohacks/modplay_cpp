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

        if (memcmp(signature, "SCRM", 4))
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
        uint16_t * parapointer_pattern;
        uint16_t * parapointer_sample;

        // check if we really deal with a S3M file. If not, bail out
        if (!loadCheck(io)) {
            // TODO throw some Exception
            return;
        }

        // read the song name
        io.seek(0, Io::IO_SEEK_SET);
        r = io.read(tmp_name, 1, 28);
        songTitle = string(tmp_name);
        
        // read num_orders, num_samples, num_patterns
        io.seek(0x20, Io::IO_SEEK_SET);
        numOrders = io.readU16le();
        numSamples = io.readU16le();
        numPatterns = io.readU16le();

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

        /* read orders and determine REAL number of patterns FS3MDOC.TXT says we
         * cannot rely on what the header data says 
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

        /* read sample and pattern parapointers */
        parapointer_sample = (uint16_t *)malloc(sizeof(uint16_t) * module->num_samples);
        parapointer_pattern = (uint16_t *)malloc(sizeof(uint16_t) * num_patterns_internal);

        h->read(parapointer_sample, sizeof(uint16_t), module->num_samples, h);
        h->read(parapointer_pattern, sizeof(uint16_t), num_patterns_internal, h);


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
                    //TODO set initial Panning as soon I know how this works
                }
            }
        }

        /* flag in the master volume byte indicates 
         * whether the song is stereo or mono */
        if ((module->initial_master_volume & 128) == 0) {
            /* make the song mono */
            for (i=0; i<32; i++) 
                module->initial_panning[i] = 0x7f;

            module->module_info.flags_s3m.mono = 1;
        } else {
            module->module_info.flags_s3m.mono = 0;
        }

        module->samples = malloc(sizeof(module_sample_t) * module->num_samples) ;
        /* read sample headers (instruments) */
        for (i = 0; i < module->num_samples; i++) {
            uint8_t sample_type;
            uint8_t sample_16bit;
            /* we use c2spd - initialize finetune with 0 */
            module->samples[i].header.finetune = 0;

            h->seek(h, parapointer_sample[i] << 4, io_seek_set);

            /* read sample type */
            h->read(&sample_type, sizeof(uint8_t), 1, h);

            /* skip the "dos filename" 
             * FS3MDOC.TXT is wrong here, it states this are 13 chars,
             * actually it's 12 chars according to ST3 TECH.DOC
             */
            h->seek(h, 12, io_seek_cur);

            /* read sample "memseg" - which is stored in 3 bytes */
            h->read(&tmp_u8, sizeof(uint8_t), 1, h);
            h->read(&tmp_u16, sizeof(uint16_t), 1, h);
            sample_memseg = ((uint32_t)tmp_u8 << 16) + tmp_u16;

            /* sample length */
            h->read(&tmp_u32, sizeof(uint32_t), 1, h);
            module->samples[i].header.length = tmp_u32 & 0xffff;

            /* loop start */
            h->read(&tmp_u32, sizeof(uint32_t), 1, h);
            module->samples[i].header.loop_start = tmp_u32 & 0xffff;

            /* loop end */
            h->read(&tmp_u32, sizeof(uint32_t), 1, h);
            module->samples[i].header.loop_end = (tmp_u32 & 0xffff) - 1;
            module->samples[i].header.loop_length = (tmp_u32 & 0xffff) - module->samples[i].header.loop_start;

            /* volume */
            h->read(&(module->samples[i].header.volume), sizeof(uint8_t), 1, h);

            /* Skip unused byte and packing scheme */
            h->seek(h, 2, io_seek_cur);

            /* flags 
             * (1)          = loop
             * (1<<1)       = stereo sample (never used, ignored for now)
             * (1<<2)       = 16 bit sample: supported by IT/Schism S3M Export 
             *              used by some Tracks of the UNREAL soundtrack
             *              (The EPIC shooter game, not the FC demo ;-))
             */
            h->read(&tmp_u8, sizeof(uint8_t), 1, h);
            module->samples[i].header.loop_enabled = tmp_u8 & 1;    // loop flag
            sample_16bit = tmp_u8 & (1<<2);

            /* c2spd */
            h->read(&tmp_u32, sizeof(uint32_t), 1, h);
            module->samples[i].header.c2spd = tmp_u32 & 0xffff;

            /* Skip unused bytes */
            h->seek(h, 12, io_seek_cur);

            /* sample name */
            h->read(module->samples[i].header.name, 1, 28, h);

            /* if we deal with a adlib instrument or a empty sample slot, continue
             * without loading data
             */
            module->samples[i].data = 0;
            if (sample_type != 1) {
                if (sample_type > 1)
                    fprintf(stderr, __FILE__ " sample %i - unsupported type: %i (most likely ADLIB)\n", i ,sample_type);
                continue;
            }

            /* fetch sample data */
            module->samples[i].data = malloc(module->samples[i].header.length * sizeof(sample_t));
            h->seek(h, sample_memseg << 4, io_seek_set);

            for (j=0; j<module->samples[i].header.length; j++) {
                if (sample_16bit) {
                    h->read(&tmp_u16, 2, 1, h);
                    tmp_u16 ^= 32768;
                    module->samples[i].data[j] = sample_from_s16((int16_t)tmp_u16);
                } else {
                    h->read(&tmp_u8, 1, 1, h);
                    /* we use signed samples interally */
                    tmp_u8 ^= 128;
                    module->samples[i].data[j] = sample_from_s8((int8_t)tmp_u8);
                }
            }

        }

        /* allocate patterns */
        module->patterns = (module_pattern_t *)malloc(sizeof(module_pattern_t) * module->num_patterns /*num_patterns_internal*/);
        for (i = 0; i < module->num_patterns /*num_patterns_internal*/; i++) 
            module->patterns[i].rows = 0;

        /* read pattern data */
        int pattern_nr = 0;
        uint8_t packed_flags;
        uint8_t channel_num;
        uint16_t packed_size;
        module_pattern_data_t tmp_data;

        for (i = 0; i < module->num_patterns /*num_patterns_internal*/; i++) {
            h->seek(h, parapointer_pattern[i] << 4, io_seek_set);

            h->read(&packed_size, sizeof(uint16_t), 1, h);

            /* s3m always has 64 rows per pattern */
            module->patterns[pattern_nr].rows = (module_pattern_row_t *)malloc(sizeof(module_pattern_row_t) * 64);
            module->patterns[pattern_nr].num_rows = 64;

            /* initialize all data with empty values */
            for (j = 0; j < 64; j++) {
                module->patterns[pattern_nr].rows[j].data = (module_pattern_data_t *)malloc(sizeof(module_pattern_data_t) * module->num_channels);
                for (k = 0; k < module->num_channels; k++) {
                    memset(&(module->patterns[pattern_nr].rows[j].data[k]), 0, sizeof(module_pattern_data_t));
                    module->patterns[pattern_nr].rows[j].data[k].period_index = -1;
                    module->patterns[pattern_nr].rows[j].data[k].volume = -1;
                }
            }

            for (j = 0; j < 64; j++) {

                do {
                    memset(&tmp_data, 0, sizeof(module_pattern_data_t));
                    tmp_data.period_index = -1;
                    tmp_data.volume = -1;

                    h->read(&packed_flags, sizeof(uint8_t), 1, h);

                    if (packed_flags > 0) {

                        channel_num = packed_flags & 31;

                        if (packed_flags & 32) {
                            h->read(&(tmp_u8), 1, 1, h);
                            if (tmp_u8 == 255)
                                tmp_data.period_index = -1;
                            else if (tmp_u8 == 254)
                                tmp_data.period_index = 254;
                            else //low nibble = note, high nibble = octave
                                tmp_data.period_index = (int)(((tmp_u8 >> 4) * 12) + (tmp_u8 & 0x0f));

                            h->read(&(tmp_data.sample_num), 1, 1, h);
                        }

                        if (packed_flags & 64)
                            h->read(&tmp_data.volume, 1, 1, h);

                        if (packed_flags & 128) {
                            h->read(&tmp_data.effect_num, 1, 1, h);
                            h->read(&tmp_data.effect_value, 1, 1, h);
                        }

                        if (channel_map[channel_num] < 255) {
                            //printf("r=%i, cn=%i, ch=%i\n", j, channel_num, channel_map[channel_num]);
                            memcpy(&(module->patterns[pattern_nr].rows[j].data[channel_map[channel_num]]), &tmp_data, sizeof(module_pattern_data_t));
                        }                    
                    }

                } while (packed_flags);
            }
            pattern_nr++;
        }


        /* free memory temporary occupied by parapointers */
        free (parapointer_sample);
        free (parapointer_pattern);

        //module_dump(module, stdout);
        module->song_message = 0;
        return module;
        
    }
    

}
/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "EffectsS3M.hpp"
#include "Player.hpp"
#include "DefsMOD.hpp"

namespace vmp
{
    
    EffectsS3M::EffectsS3M()
        : Effects(vector<effect_t>({
            
    }))
    {}
    
    EffectsS3M::~EffectsS3M()
    {}
    
    
    void EffectsS3M::setTrackFrequency(Player& player, Track& track, u16 period)
    {
        // no period? bail out
        if (!period)
            return;
        
        track.setFrequency(14317056L / period);
        track.setSampleStep(static_cast<precision_t>(track.getFrequency()) / static_cast<precision_t>(player.getSampleRate()));       
    }
    
    
    void EffectsS3M::ASetSpeed(Player& player, Track& track)
    {
        PatternData* data = track.getData();
        
        if (data->getEffectValue() == 0)
            return;

        if (player.getCurrentTick() == 0) 
            player.setSpeed(data->getEffectValue());

    }

    void EffectsS3M::BPositionJump(Player& player, Track& track)
    {
        PatternData* data = track.getData();
        
        // prevent egoist-mods from looping back if no single-mod-loop is enabled
        if (data->getEffectValue() < player.getCurrentOrder()) {
            if (!player.getLoop())
                return;
        }

        if (player.getCurrentTick() == 0) {
            player.setNextOrder(data->getEffectValue());
            player.setNextRow(0);
            player.setDoBreak();
        }
    }

    void EffectsS3M::CPatternBreak(Player& player, Track& track)
    {
        PatternData* data = track.getData();
        
        if (player.getCurrentTick() == 0) {
            player.setNextRow((data->getEffectValueUpper() * 10) + data->getEffectValueLower());
            player.setDoBreak();
        }
    }

    void EffectsS3M::DVolumeSlide(Player& player, Track& track)
    {
        s16 tmp;
        PatternData* data = track.getData();

        if (player.getCurrentTick() == 0) {
            // remember last volume slide command parameter
            if (data->getEffectValue() > 0)
                track.storeEffect(4, data->getEffectValue());

            // process fine volume slides only on first tick
            if (track.recallEffectLower(4) == 0x0f) {
                tmp = track.getVolume() + track.recallEffectUpper(4);//                player->channels[channel].volume + (player->channels[channel].effect_last_value[4] >> 4);
                if (tmp > 64)
                    tmp = 64;
                track.setVolume(tmp);
            } else if (track.recallEffectUpper(4) == 0x0f) {
                tmp = static_cast<s16>(track.getVolume()) - static_cast<s16>(track.recallEffectLower(4));
                if (tmp < 0)
                    tmp = 0;
                track.setVolume(tmp);
            }

            // buggy st3.0 volumeslides, also processed on first tick
            //if (!player->module->module_info.flags_s3m.st30volumeslides)
                return;
        }

        // process regular volume slides
        if (track.recallEffectLower(4) == 0x00) {
            tmp = track.getVolume() + track.recallEffectUpper(4);
            if (tmp > 64)
                tmp = 64;
            track.setVolume(tmp);
        } else if (track.recallEffectUpper(4) == 0x00) {
            tmp = static_cast<s16>(track.getVolume()) - static_cast<s16>(track.recallEffectLower(4));
            if (tmp < 0)
                tmp = 0;
            track.setVolume(tmp);
        }    
    }

    void EffectsS3M::ESlideDown(Player& player, Track& track)
    {
        s32 tmp;
        PatternData* data = track.getData();

        if (player.getCurrentTick() == 0) {
            // remember last slide down command parameter. This effect also memorizes the slide up value
            if (data->getEffectValue() > 0) {
                track.storeEffect(5, data->getEffectValue());
                track.storeEffect(6, data->getEffectValue());
            }
            
            //if ((player->channels[channel].effect_last_value[5] & 0xf0) == 0xf0) {
            if (track.recallEffectUpper(5) == 0x0f) {
                // fine portamento
                tmp =  static_cast<s32>(track.getPeriod()) + static_cast<s32>(track.recallEffectLower(5) << 2);
                track.setPeriod(tmp);
                EffectsS3M::setTrackFrequency(player, track, track.getPeriod());
            } else if (track.recallEffectUpper(5) == 0x0e) {
                // extra fine portamento 
                tmp =  static_cast<s32>(track.getPeriod()) + static_cast<s32>(track.recallEffectLower(5));
                track.setPeriod(tmp);
                EffectsS3M::setTrackFrequency(player, track, track.getPeriod());
            }

            return;
        }

        if (track.recallEffect(5) < 0xe0) {
            /* regular portamento */
            tmp = static_cast<s32>(track.getPeriod()) + static_cast<s32>(track.recallEffect(5) << 2);
            track.setPeriod(tmp);
            EffectsS3M::setTrackFrequency(player, track, track.getPeriod());
        }

    }

    void EffectsS3M::FSlideUp(Player& player, Track& track)
    {
        s32 tmp;
        PatternData* data = track.getData();

        if (player.getCurrentTick() == 0) {
            // remember last slide down command parameter. This effect also memorizes the slide up value
            if (data->getEffectValue() > 0) {
                track.storeEffect(5, data->getEffectValue());
                track.storeEffect(6, data->getEffectValue());
            }
            
            //if ((player->channels[channel].effect_last_value[5] & 0xf0) == 0xf0) {
            if (track.recallEffectUpper(6) == 0x0f) {
                // fine portamento
                tmp =  static_cast<s32>(track.getPeriod()) - static_cast<s32>(track.recallEffectLower(6) << 2);
                track.setPeriod(tmp);
                EffectsS3M::setTrackFrequency(player, track, track.getPeriod());
            } else if (track.recallEffectUpper(6) == 0x0e) {
                // extra fine portamento 
                tmp =  static_cast<s32>(track.getPeriod()) - static_cast<s32>(track.recallEffectLower(6));
                track.setPeriod(tmp);
                EffectsS3M::setTrackFrequency(player, track, track.getPeriod());
            }

            return;
        }

        if (track.recallEffect(6) < 0xe0) {
            /* regular portamento */
            tmp = static_cast<s32>(track.getPeriod()) - static_cast<s32>(track.recallEffect(6) << 2);
            track.setPeriod(tmp);
            EffectsS3M::setTrackFrequency(player, track, track.getPeriod());
        }

    }


    void EffectsS3M::GSlideToNote(Player& player, Track& track)
    {
        PatternData* data = track.getData();
        s32 tmp;
        
        if (player.getCurrentTick() == 0) {
            if (data->getEffectValue() > 0) 
                track.storeEffect(7, data->getEffectValue());
            return;        
        }

        if (track.getDestPeriod() == 0)
            return;

        if (track.getPeriod() > track.getDestPeriod()) {
            tmp = static_cast<s32>(track.getPeriod()) - static_cast<s32>(track.recallEffect(7) << 2);
            if (tmp < track.getDestPeriod())
                tmp = track.getDestPeriod();
            track.setPeriod(tmp);
        } else if (track.getPeriod() < track.getDestPeriod()) {
            tmp = static_cast<s32>(track.getPeriod()) + static_cast<s32>(track.recallEffect(7) << 2);
            if (tmp < track.getDestPeriod())
                tmp = track.getDestPeriod();
            track.setPeriod(tmp);
        }
        EffectsS3M::setTrackFrequency(player, track, track.getPeriod());
    }


    void EffectsS3M::HVibrato(Player& player, Track& track)
    {
        PatternData* data = track.getData();
        u8 temp;
        u16 delta;

        if (player.getCurrentTick() == 0) {
            if (data->getEffectValueUpper() != 0x00) 
                track.storeEffectUpper(8, data->getEffectValueUpper());

            if (data->getEffectValueLower() != 0x00) 
                track.storeEffectLower(8, data->getEffectValueLower());

            return;    
        }

        temp = track.getVibratoState() & 0x1f;

        switch (track.getVibratoWaveform() & 3) {
            case 0: 
                delta = DefsMOD::sineTable[temp]; 
                break;

            case 1:
                temp <<= 3;
                if (track.getVibratoState() < 0)
                    temp = 255 - temp;
                delta = temp;
                break;

            case 2:
                delta = 255;
                break;

            case 3:
                delta = DefsMOD::sineTable[temp];
                break;
        }

        delta *= track.recallEffectLower(8);
        delta /= 32; // (128 / 4 = 32) - due to 4 times bigger periods in s3m compared to protracker

        if (track.getVibratoState() >= 0)
            EffectsS3M::setTrackFrequency(player, track, track.getPeriod() + delta);
        else
            EffectsS3M::setTrackFrequency(player, track, track.getPeriod() - delta);

        int tmp = track.getVibratoState() + track.recallEffectUpper(8);
        if (tmp > 31)
            tmp -= 64;
        track.setVibratoState(tmp);
    }


    void EffectsS3M::ITremor(Player& player, Track& track)
    {
        PatternData* data = track.getData();
        int tmp;
        
        if (player.getCurrentTick() == 0) {
            if (data->getEffectValue() > 0) 
                track.storeEffect(9, data->getEffectValue());
        }

        uint8_t tremor_on = track.recallEffectUpper(9) + 1;
        uint8_t tremor_off = track.recallEffectLower(9) + 1;

        if (track.getTremorState() < tremor_on)
            track.setFxVolume(64);
        else
            track.setFxVolume(0);

        tmp = track.getTremorState() + 1;
        if (tmp > (tremor_on + tremor_off))
            tmp = 0;
        track.setTremorState(tmp);
    }


    void EffectsS3M::JArpeggio(Player& player, Track& track)
    {
        PatternData* data = track.getData();

        uint8_t temp = player.getCurrentTick() % 3;
        //protracker_lookup_period_index(channel->period);
        int temp2 = track.getPeriod();

        // arpeggio without note makes no sense (bladswed.mod))
        if (temp2 == 0)
            return;

        switch (temp) {
        case 0:
            //temp2 = protracker_periods[temp2];
            break;
        case 1:
            //temp2 = protracker_periods[temp2 + (channel->current_effect_value >> 4)];
            temp2 *= pow(2.0f, (static_cast<float>(data->getEffectValueUpper()) / -12.0f));
            break;
        case 2:
            //temp2 = protracker_periods[temp2 + (channel->current_effect_value & 0x0f)];
            temp2 *= pow(2.0f, (static_cast<float>(data->getEffectValueLower()) / -12.0f));
            break;
        }
        EffectsS3M::setTrackFrequency(player, track, temp2);
    }


    void EffectsS3M::KVibratoVolumeSlide(Player& player, Track& track) 
    {
        PatternData* data = track.getData();
        int tmp;
        u8 temp;
        u16 delta;

        if (player.getCurrentTick() == 0) {
            if (data->getEffectValue() > 0)
                track.storeEffect(11, data->getEffectValue());

            return;    
        }

        // maintain vibrato
        temp = track.getVibratoState() & 0x1f;
        switch (track.getVibratoWaveform() & 3) {
            case 0: 
                delta = DefsMOD::sineTable[temp]; 
                break;

            case 1:
                temp <<= 3;
                if (track.getVibratoState() < 0)
                    temp = 255 - temp;
                delta = temp;
                break;

            case 2:
                delta = 255;
                break;

            case 3:
                delta = DefsMOD::sineTable[temp];
                break;
        }

        delta *= track.recallEffectLower(8);
        delta /= 32; // (128 / 4 = 32) - due to 4 times bigger periods in s3m compared to protracker

        if (track.getVibratoState() >= 0)
            EffectsS3M::setTrackFrequency(player, track, track.getPeriod() + delta);
        else
            EffectsS3M::setTrackFrequency(player, track, track.getPeriod() - delta);
        
        tmp = track.getVibratoState() + track.recallEffectUpper(8);
        if (tmp > 31)
            tmp -= 64;
        track.setVibratoState(tmp);

        
        // do the volume slide
        if (track.recallEffectLower(11) == 0x00) {
            tmp = static_cast<int>(track.getVolume()) + static_cast<int>(track.recallEffectUpper(11));
            if (tmp > 64)
                tmp = 64;
            track.setVolume(tmp);
        } else if (track.recallEffectUpper(11) == 0x00) {
            tmp = static_cast<int>(track.getVolume()) - static_cast<int>(track.recallEffectUpper(11));
            if (tmp < 0)
                tmp = 0;
            track.setVolume(tmp);
        }      

    }

    void EffectsS3M::LSlideToNoteVolumeSlide(Player& player, Track& track)
    {
        PatternData* data = track.getData();
        int tmp;

        if (player.getCurrentTick() == 0) {
            if (data->getEffectValue() > 0)
                track.storeEffect(12, data->getEffectValue());

            return;    
        }

        // maintain note portamento 
        if (track.getDestPeriod() != 0) {
            if (track.getPeriod() > track.getDestPeriod()) {
                tmp = static_cast<s32>(track.getPeriod()) - static_cast<s32>(track.recallEffect(7) << 2);
                if (tmp < track.getDestPeriod())
                    tmp = track.getDestPeriod();
                track.setPeriod(tmp);
            } else if (track.getPeriod() < track.getDestPeriod()) {
                tmp = static_cast<s32>(track.getPeriod()) + static_cast<s32>(track.recallEffect(7) << 2);
                if (tmp < track.getDestPeriod())
                    tmp = track.getDestPeriod();
                track.setPeriod(tmp);
            }
            EffectsS3M::setTrackFrequency(player, track, track.getPeriod());        
        }
        //=======!!!!!!========

        /* do the volume slide */
        if ((player->channels[channel].effect_last_value[12] & 0x0f) == 0x00) {
            tmp = player->channels[channel].volume + (player->channels[channel].effect_last_value[12] >> 4);
            if (tmp > 64)
                tmp = 64;
            player->channels[channel].volume = tmp;
        } else if ((player->channels[channel].effect_last_value[12] & 0xf0) == 0x00) {
            tmp = (int)player->channels[channel].volume - (int)(player->channels[channel].effect_last_value[12] & 0x0f);
            if (tmp < 0)
                tmp = 0;
            player->channels[channel].volume = tmp;
        } 
    }


    void EffectsS3M::OSampleOffset(Player& player, Track& track)
    {
        if (player->current_tick == 0) {
            if (player->channels[channel].effect_value)
                player->channels[channel].effect_last_value[15] = player->channels[channel].effect_value;

            player->channels[channel].sample_pos = player->channels[channel].effect_last_value[15] << 8;
            if (player->channels[channel].sample_pos > player->module->samples[player->channels[channel].sample_num - 1].header.length - 1) 
                player->channels[channel].sample_pos = player->module->samples[player->channels[channel].sample_num - 1].header.length - 1;
        }
    }


    void EffectsS3M::QRetriggerVolumeSlide(Player& player, Track& track)
    {
        int tmp;

        if (player->current_tick == 0) {
            if (player->channels[channel].effect_value)
                player->channels[channel].effect_last_value[17] = player->channels[channel].effect_value;
            return;
        }

        /* do volume slide */
        tmp = (int)player->channels[channel].volume;

        switch (player->channels[channel].effect_last_value[17] >> 4) {
            case 0: break;
            case 1: tmp -= 1; break;
            case 2: tmp -= 2; break;
            case 3: tmp -= 4; break;
            case 4: tmp -= 8; break;
            case 5: tmp -= 16; break;
            case 6: tmp = (tmp * 2) / 3; break;
            case 7: tmp /= 2; break;
            case 8: break;
            case 9: tmp += 1; break;
            case 10: tmp += 2; break;
            case 11: tmp += 4; break;
            case 12: tmp += 8; break;
            case 13: tmp += 16; break;
            case 14: tmp = (tmp * 3) / 2; break;
            case 15: tmp *= 2; break;
        }


        if (tmp > 64)
            tmp = 64;

        if (tmp < 0)
            tmp = 0;

        player->channels[channel].volume = (int8_t)tmp;

        /* do retrigger */
        if ((player->channels[channel].effect_last_value[17] & 0xf) > 0) {
            if ((player->current_tick % (player->channels[channel].effect_last_value[17] & 0xf)) == 0)
                player->channels[channel].sample_pos = 0;  
        }


    }

    void EffectsS3M::RTremolo(Player& player, Track& track)
    {
        uint8_t temp;
        int temp2;
        uint16_t delta;

        if (player->current_tick == 0) {
            if (player->channels[channel].tremolo_waveform < 4)
                player->channels[channel].tremolo_state = 0;

            if ((player->channels[channel].effect_value >> 4) != 0x00) 
                player->channels[channel].effect_last_value[player->channels[channel].effect_num] = (player->channels[channel].effect_value >> 4);

            if ((player->channels[channel].effect_value & 0xf) != 0x00) 
                player->channels[channel].effect_last_value_y[player->channels[channel].effect_num] = (player->channels[channel].effect_value & 0xf);

            return;    
        }

        temp = player->channels[channel].tremolo_state & 0x1f;

        switch (player->channels[channel].tremolo_waveform & 3) {
            case 0: 
                delta = defs_mod_sine_table[temp]; 
                break;

            case 1:
                temp <<= 3;
                if (player->channels[channel].tremolo_state < 0)
                    temp = 255 - temp;
                delta = temp;
                break;

            case 2:
                delta = 255;
                break;

            case 3:
                delta = defs_mod_sine_table[temp];
                break;
        }


        delta *= player->channels[channel].effect_last_value_y[player->channels[channel].effect_num];
        delta /= 64;

        if (player->channels[channel].tremolo_state >= 0) {
            if (player->channels[channel].volume + delta > 64) 
                delta = 64 - player->channels[channel].volume;
            temp2 = (int)player->channels[channel].volume + delta;
            player->channels[channel].volume_master = (uint8_t)temp2;
        } else {
            if ((int)player->channels[channel].volume - delta < 0) 
                delta = player->channels[channel].volume;

            temp2 = (int)player->channels[channel].volume - delta;
            player->channels[channel].volume_master = (uint8_t)temp2;
        }

        player->channels[channel].tremolo_state += player->channels[channel].effect_last_value[player->channels[channel].effect_num];
        if (player->channels[channel].tremolo_state > 31)
            player->channels[channel].tremolo_state -= 64;
    }



    void EffectsS3M::SSpecial(Player& player, Track& track)
    {
        switch (track.getData()->getEffectValueUpper()) {
            case 0x3: EffectsS3M::S3SetVibratoWaveform(player, track); break;
            case 0x4: EffectsS3M::S4SetTremoloWaveform(player, track); break;
            case 0x8: EffectsS3M::S8Panning(player, track); break;
            case 0xA: EffectsS3M::SAStereoControl(player, track); break;
            case 0xC: EffectsS3M::SCNoteCut(player, track); break;
            case 0xD: EffectsS3M::SDDelaySample(player, track); break;
            default: Effects::unimplementedEffect(player, track); break;
        }
    }

    void EffectsS3M::S3SetVibratoWaveform(Player& player, Track& track)
    {
        if (player->current_tick == 0) 
            player->channels[channel].vibrato_waveform = player->channels[channel].effect_value & 0x0f;

    }

    void EffectsS3M::S4SetTremoloWaveform(Player& player, Track& track)
    {
        if (player->current_tick == 0) 
            player->channels[channel].tremolo_waveform = player->channels[channel].effect_value & 0x0f;

    }

    void EffectsS3M::S8Panning(Player& player, Track& track) 
    {
        if (player->current_tick == 0) {
            uint8_t i = player->channels[channel].effect_value & 0x0f;
            player->channels[channel].panning = (i << 4) | i;//  (i << 4) | ((i << 1) + (i>6?1:0));
        }
    }

    void EffectsS3M::SAStereoControl(Player& player, Track& track)
    {
        if (player->current_tick == 0) {
            uint8_t i = player->channels[channel].effect_value & 0x0f;

            if (i > 7)
                i -= 8;
            else
                i += 8;

            player->channels[channel].panning = (i << 4) | i;
        }
    }

    void EffectsS3M::SCNoteCut(Player& player, Track& track)
    {
        if ((player->channels[channel].effect_value & 0x0f) <= player->current_tick)
            player->channels[channel].volume = 0;
    }

    void EffectsS3M::SDDelaySample(Player& player, Track& track)
    {
        if (player->current_tick == 0)
            player->channels[channel].sample_delay = 0;

        if (player->channels[channel].sample_delay == (player->channels[channel].effect_value & 0xf)) {
            if (player->channels[channel].dest_sample_num > 0) {
                player->channels[channel].sample_num = player->channels[channel].dest_sample_num;
                player->channels[channel].volume = player->module->samples[player->channels[channel].sample_num - 1].header.volume;
            }

            if (player->channels[channel].dest_period > 0) {
                player->channels[channel].period = player->channels[channel].dest_period;
                player->channels[channel].sample_pos = 0;
                player_channel_set_frequency(player, player->channels[channel].period, channel);
            }

            if (player->channels[channel].dest_volume >= 0)
                player->channels[channel].volume = player->channels[channel].dest_volume;
        }

        player->channels[channel].sample_delay++; // = (player->channels[channel].current_effect_value & 0xf);
    }

    void EffectsS3M::TSetBpm(Player& player, Track& track)
    {
        if (player->current_tick == 0) {
             if (player->channels[channel].effect_value >= 0x20) {
                player->bpm = player->channels[channel].effect_value;
                player->tick_duration = player_calc_tick_duration(player->bpm, player->sample_rate);
             }
        }
    }

    // protracker panning
    void EffectsS3M::XPanning(Player& player, Track& track) 
    {
        if (player->current_tick == 0) {
            int tmp = player->channels[channel].effect_value;
            tmp = (tmp * 256) / 80;
            //printf("%i\n", tmp);
            player->channels[channel].panning = tmp;
        }
    }    
    
    
}
/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "EffectsS3M.hpp"
#include "Player.hpp"
#include "DefsMOD.hpp"
#include "DefsS3M.hpp"
#include <cmath>

namespace vmp
{
    
    EffectsS3M::EffectsS3M()
        : Effects(vector<effect_t>({
            Effects::unimplementedEffect,
            EffectsS3M::ASetSpeed,
            EffectsS3M::BPositionJump,
            EffectsS3M::CPatternBreak,
            EffectsS3M::DVolumeSlide,
            EffectsS3M::ESlideDown,
            EffectsS3M::FSlideUp,
            EffectsS3M::GSlideToNote,
            EffectsS3M::HVibrato,
            EffectsS3M::ITremor,
            EffectsS3M::JArpeggio,
            EffectsS3M::KVibratoVolumeSlide,
            EffectsS3M::LSlideToNoteVolumeSlide,
            Effects::unimplementedEffect,           // No "M" Effect
            Effects::unimplementedEffect,           // No "N" Effect
            EffectsS3M::OSampleOffset,
            Effects::unimplementedEffect,           // No "P" Effect
            EffectsS3M::QRetriggerVolumeSlide,
            EffectsS3M::RTremolo,
            EffectsS3M::SSpecial,
            EffectsS3M::TSetBpm,
            Effects::unimplementedEffect,           // No "U" Effect
            Effects::unimplementedEffect,           // No "V" Effect
            Effects::unimplementedEffect,           // No "W" Effect
            EffectsS3M::XPanning
    }))
    {}
    
    EffectsS3M::~EffectsS3M()
    {}
    
    
    void EffectsS3M::newRowAction(Player& player, Track& track)
    {
        PatternData* data = track.getData();

        // special behaviour for sample / note delay, here the effect starts the sound
        if ((data->getEffectCmd() == 19) && (data->getEffectValueUpper() == 0xd)) {
            return;
        }

        // set sample
        if (data->hasInstrument()) {
            track.setInstrument(data->getInstrument());
            track.setVolume(player.getModule()->getSample(data->getInstrument()).getDefaultVolume());
        }

        // set volume
        if (data->hasVolume())
            track.setVolume(data->getVolume());
        

        // set period (note)
        if (data->hasNote()) {
            
            // reset vibrato
            if (track.getVibratoWaveform() < 4)
                track.setVibratoState(0);

            // reset tremolo
            if (track.getTremoloWaveform() < 4)
                track.setTremoloState(0);
            
            //track.setInstrument(data->getInstrument());
            if (data->getNote() == 254) {           // note off
                track.setActive(false);
                return;
            } else {
                // special hack for note portamento... 
                if (data->getEffectCmd() != 0x7) {
                    track.setDestPeriod(0);
                    if (!player.getPatternDelayActive()) {
                        track.setActive(true);
                        track.setPeriod(EffectsS3M::getTunedPeriod(player, track, DefsS3M::periods[data->getNote()]));
                        track.setSamplePos(0);
                    }
                }
            }
        } 

        track.setFxVolume(64);

        // do not set frequency for tone portamento effects
        //if ((data->effect_num != 0x7) && (data->effect_num != 0x5) && (data->effect_num != 0x6) && (data->effect_num != 0x8)) {
        if (data->getEffectCmd() != 0x7) {
            if (data->hasNote())
                EffectsS3M::setTrackFrequency(player, track, track.getPeriod());
        }
    }
    
    
    void EffectsS3M::setTrackFrequency(Player& player, Track& track, u16 period)
    {
        // no period? bail out
        if (!period)
            return;
        
        track.setFrequency(14317056L / period);
        track.setSampleStep(static_cast<precision_t>(track.getFrequency()) / static_cast<precision_t>(player.getSampleRate()));       
    }
    
    
    u16 EffectsS3M::getTunedPeriod(Player& player, Track& track, const u16 basePeriod)
    {
        Sample& sample = player.getModule()->getSample(track.getInstrument());
        return (basePeriod * 8363) / sample.getMiddleCSpeed();
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
            // regular portamento
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
            // regular portamento
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
            if (data->hasNote())
                track.setDestPeriod(EffectsS3M::getTunedPeriod(player, track, DefsS3M::periods[data->getNote()]));
            return;        
        }

        //if (!data->hasNote())
        //    return;
        
        if (track.getPeriod() > track.getDestPeriod()) {
            tmp = static_cast<s32>(track.getPeriod()) - static_cast<s32>(track.recallEffect(7) << 2);
            if (tmp < track.getDestPeriod())
                tmp = track.getDestPeriod();
            track.setPeriod(tmp);
        } else if (track.getPeriod() < track.getDestPeriod()) {
            tmp = static_cast<s32>(track.getPeriod()) + static_cast<s32>(track.recallEffect(7) << 2);
            if (tmp > track.getDestPeriod())
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
            tmp = static_cast<int>(track.getVolume()) - static_cast<int>(track.recallEffectLower(11));
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
                if (tmp > track.getDestPeriod())
                    tmp = track.getDestPeriod();
                track.setPeriod(tmp);
            }
            EffectsS3M::setTrackFrequency(player, track, track.getPeriod());        
        }
        
        // do the volume slide 
        if (track.recallEffectLower(12) == 0x00) {
            tmp = static_cast<int>(track.getVolume()) + static_cast<int>(track.recallEffectUpper(12));
            if (tmp > 64)
                tmp = 64;
            track.setVolume(tmp);
        } else if (track.recallEffectUpper(12) == 0x00) {
            tmp = static_cast<int>(track.getVolume()) - static_cast<int>(track.recallEffectLower(12));
            if (tmp < 0)
                tmp = 0;
            track.setVolume(tmp);
        }      
    }


    void EffectsS3M::OSampleOffset(Player& player, Track& track)
    {
        PatternData* data = track.getData();
        precision_t tmp;
        
        if (player.getCurrentTick() == 0) {
            if (data->getEffectValue() > 0)
                track.storeEffect(15, data->getEffectValue());

            tmp = static_cast<precision_t>(track.recallEffect(15) << 8);
            if (tmp > player.getModule()->getSample(track.getInstrument()).getLength() - 1)
                tmp = player.getModule()->getSample(track.getInstrument()).getLength() - 1;
            track.setSamplePos(tmp);
        }
    }


    void EffectsS3M::QRetriggerVolumeSlide(Player& player, Track& track)
    {
        PatternData* data = track.getData();
        int tmp;

        if (player.getCurrentTick() == 0) {
            if (data->hasEffectValue())
                track.storeEffect(17, data->getEffectValue());
            return;
        }

        // do volume slide
        tmp = static_cast<int>(track.getVolume());

        switch (track.recallEffectUpper(17)) {
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

        track.setVolume(tmp);

        // do retrigger 
        if (track.recallEffectLower(17) > 0) {
            if ((player.getCurrentTick() % track.recallEffectLower(17)) == 0)
                track.setSamplePos(0);
        }
    }

    void EffectsS3M::RTremolo(Player& player, Track& track)
    {
        PatternData* data = track.getData();
        u8 temp;
        int temp2;
        u16 delta;

        if (player.getCurrentTick() == 0) {
            if (track.getTremoloWaveform() < 4)
                track.setTremoloState(0);

            if ((data->getEffectValueUpper()) != 0x00) 
                track.storeEffectUpper(18, data->getEffectValueUpper());

            if ((data->getEffectValueLower()) != 0x00) 
                track.storeEffectUpper(18, data->getEffectValueLower());

            return;    
        }

        temp = track.getTremoloState() & 0x1f;

        switch (track.getTremoloWaveform() & 3) {
            case 0: 
                delta = DefsMOD::sineTable[temp]; 
                break;

            case 1:
                temp <<= 3;
                if (track.getTremoloState() < 0)
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


        delta *= track.recallEffectLower(18);
        delta /= 64;

        if (track.getTremoloState() >= 0) {
            if (track.getVolume() + delta > 64) 
                delta = 64 - track.getVolume();

            temp2 = static_cast<int>(track.getVolume()) + delta;
            track.setFxVolume(temp2);
        } else {
            if ((static_cast<int>(track.getVolume()) - delta) < 0) 
                delta = track.getVolume();

            temp2 = static_cast<int>(track.getVolume()) - delta;
            track.setFxVolume(temp2);
        }

        temp2 = track.getTremoloState() + track.recallEffectUpper(18);
        if (temp2 > 31)
            temp2 -= 64;
        track.setTremoloState(temp2);
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
        if (player.getCurrentTick() == 0) 
            track.setVibratoWaveform(track.getData()->getEffectValueLower());
    }

    void EffectsS3M::S4SetTremoloWaveform(Player& player, Track& track)
    {
        if (player.getCurrentTick() == 0) 
            track.setTremoloWaveform(track.getData()->getEffectValueLower());
    }

    void EffectsS3M::S8Panning(Player& player, Track& track) 
    {
        if (player.getCurrentTick() == 0) {
            u8 i = track.getData()->getEffectValueLower();
            track.setPanning((i << 4) | i);
        }
    }

    void EffectsS3M::SAStereoControl(Player& player, Track& track)
    {
        if (player.getCurrentTick() == 0) {
            u8 i = track.getData()->getEffectValueLower();

            if (i > 7)
                i -= 8;
            else
                i += 8;

            track.setPanning((i << 4) | i);
        }
    }

    void EffectsS3M::SCNoteCut(Player& player, Track& track)
    {
        if (track.getData()->getEffectValueLower() <= player.getCurrentTick())
            track.setVolume(0);
    }

    void EffectsS3M::SDDelaySample(Player& player, Track& track)
    {
        PatternData* data = track.getData();
        
        if (player.getCurrentTick() == 0) 
            track.setSampleDelay(0);

        if (track.getSampleDelay() == data->getEffectValueLower()) {
            if (data->hasInstrument()) {
                track.setInstrument(data->getInstrument());
                track.setVolume(player.getModule()->getSample(data->getInstrument()).getDefaultVolume());
            }

            if (data->hasNote()) {
                track.setPeriod(EffectsS3M::getTunedPeriod(player, track, DefsS3M::periods[data->getNote()]));
                track.setSamplePos(0);
                setTrackFrequency(player, track, track.getPeriod());
            }            
            
            if (data->hasVolume())
                track.setVolume(data->getVolume());
        }

        track.setSampleDelay(track.getSampleDelay() + 1);
    }

    void EffectsS3M::TSetBpm(Player& player, Track& track)
    {
        PatternData* data = track.getData();
        
        if (player.getCurrentTick() == 0) {
             if (data->getEffectValue() >= 0x20) {
                 player.setBpm(data->getEffectValue());
                 player.calcTickDuration();
             }
        }
    }

    // protracker panning
    void EffectsS3M::XPanning(Player& player, Track& track) 
    {
        if (player.getCurrentTick() == 0) {
            int tmp = track.getData()->getEffectValue();
            tmp = (tmp * 256) / 80;
            track.setPanning(tmp);
        }
    }    
    
    
}
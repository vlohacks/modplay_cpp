/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
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

#include "EffectsMOD.hpp"
#include "Effects.hpp"
#include "DefsMOD.hpp"
#include "Player.hpp"
#include <cmath>

namespace vmp
{
    EffectsMOD::EffectsMOD() 
    
    {
        effectMap = vector<effect_t>(NUM_EFFECTS, Effects::unimplementedEffect);
        effectMap[0x0] = EffectsMOD::arpeggio;
        effectMap[0x1] = EffectsMOD::slideUp;
        effectMap[0x2] = EffectsMOD::slideDown;
        effectMap[0x3] = EffectsMOD::slideToNote;
        effectMap[0x4] = EffectsMOD::vibrato;
        effectMap[0x5] = EffectsMOD::slideToNoteVolumeSlide;
        effectMap[0x6] = EffectsMOD::vibratoVolumeSlide;
        effectMap[0x7] = EffectsMOD::tremolo;
        effectMap[0x8] = EffectsMOD::panning;
        effectMap[0x9] = EffectsMOD::sampleOffset;
        effectMap[0xa] = EffectsMOD::volumeSlide;
        effectMap[0xb] = EffectsMOD::positionJump;
        effectMap[0xc] = EffectsMOD::setVolume;
        effectMap[0xd] = EffectsMOD::patternBreak;
        effectMap[0xe] = EffectsMOD::special;
        effectMap[0xf] = EffectsMOD::setSpeed;
    }
    
    
    void EffectsMOD::setTrackFrequency(Player& player, Track& track, u16 period)
    {
        const float x = 1.007246412;
        
        Sample& sample = player.getModule()->getSample(track.getInstrument());
        s8 finetune;

        /* no period? bail out */
        if (!period)
            return;

        // unusual: finetune is a signed nibble
        finetune = (sample.getFinetune() >= 8 
                ? -(16 - sample.getFinetune()) 
                : sample.getFinetune());
        track.setFrequency(DefsMOD::paulaFreq[0] / (period * 2));

        // TODO: will there be a day when we can get rid of libm here?
        if (finetune != 0) {
            track.setFrequency( track.getFrequency() * pow(x, finetune));
        }

        // Performance tuning: do this calculation only once rather than every sample
        track.setSampleStep(static_cast<precision_t>(track.getFrequency()) / static_cast<precision_t>(player.getSampleRate()));
    }
    
    
    void EffectsMOD::newRowAction(Player& player, Track& track)
    {
        PatternData* data = track.getData();
        Module* module = player.getModule();

        // special behaviour for sample / note delay
        if ((data->getEffectCmd() == 0xe) && (data->getEffectValueUpper() == 0xd)) {
            if (data->hasNote())
                track.setDestPeriod(DefsMOD::periods[data->getNote()]);
            
            if (data->hasInstrument())
                track.setDestInstrument(data->getInstrument());

            //player->channels[channel_num].period_index = data->period_index;
            return;
        }

        // set sample
        if (data->hasInstrument()) {
            if (module->getSample(data->getInstrument()).hasData()) {
                //fprintf(stderr, "uiaeiaeiae!!!\n");
                track.setActive(true);
                track.setInstrument(data->getInstrument());
                track.setVolume(module->getSample(data->getInstrument()).getDefaultVolume());
            } else {
                track.setActive(false);
            }
        }

        // set period (note)
        if (data->hasNote()) {
            // reset vibrato
            if (track.getVibratoWaveform() < 4)
                track.setVibratoState(0);    

            // reset tremolo
            if (track.getTremoloWaveform() < 4)
                track.setTremoloState(0);

            // special hack for note portamento... 
            if ((data->getEffectCmd() == 0x3) || (data->getEffectCmd() == 0x5)) {
                track.setDestPeriod(DefsMOD::periods[data->getNote()]);
                //player->channels[channel_num].period_index = data->period_index;
            } else {
                if (!player.getPatternDelayActive()) {
                    track.setPeriod(DefsMOD::periods[data->getNote()]);
                    //player->channels[channel_num].period_index = data->period_index;
                    track.setSamplePos(0);
                    //track.setPendingFrequencyUpdate(true);
                }
            }
        }
        setTrackFrequency(player, track, track.getPeriod());

    }
    
    

    // 0
    void EffectsMOD::arpeggio(Player& player, Track& track)
    {
        u8 temp = player.getCurrentTick() % 3;
        //protracker_lookup_period_index(channel->period);
        int temp2 = track.getPeriod();
        
        PatternData* data = track.getData();

        // arpeggio without note makes no sense (bladswed.mod))
        if (!temp2)
            return;

        switch (temp) {
        case 0:
            //temp2 = protracker_periods[temp2];
            break;
        case 1:
            //temp2 = protracker_periods[temp2 + (channel->current_effect_value >> 4)];
            temp2 *= pow(2.0f, (float)(data->getEffectValueUpper()) / -12.0f);
            break;
        case 2:
            //temp2 = protracker_periods[temp2 + (channel->current_effect_value & 0x0f)];
            temp2 *= pow(2.0f, (float)(data->getEffectValueLower()) / -12.0f);
            break;
        }
        setTrackFrequency(player, track, temp2);
    }

    // 1
    void EffectsMOD::slideUp(Player& player, Track& track) 
    {
        if (player.getCurrentTick() == 0)
            return;
        
        PatternData* data = track.getData();
        u16 period = track.getPeriod();
        period -= data->getEffectValue();
        
        if (period < DefsMOD::periods[DefsMOD::numPeriods - 1])
            period = DefsMOD::periods[DefsMOD::numPeriods - 1];
        track.setPeriod(period);
        setTrackFrequency(player, track, track.getPeriod());
    }

    
    
    // 2
    void EffectsMOD::slideDown(Player& player, Track& track)
    {
        if (player.getCurrentTick() == 0)
            return;

        PatternData* data = track.getData();
        u16 period = track.getPeriod();
        period += data->getEffectValue();
        
        if (period > DefsMOD::periods[0])
            period = DefsMOD::periods[0];
        
        track.setPeriod(period);
        setTrackFrequency(player, track, track.getPeriod());
    }

  
    // 3
    void EffectsMOD::slideToNote(Player& player, Track& track)
    {
        PatternData* data = track.getData();
        
        if (player.getCurrentTick() == 0)
            return;

        if (track.getDestPeriod() == 0)
            return;

        if (data->getEffectValue() != 0x00)
            track.storeEffect(data->getEffectCmd(), data->getEffectValue());

        if (track.getPeriod() > track.getDestPeriod()) {
            int tmp = (int)track.getPeriod() - (int)track.recallEffect(data->getEffectCmd());
            if (tmp < track.getDestPeriod())
                tmp = track.getDestPeriod();
            track.setPeriod(static_cast<u16>(tmp));
        } else if (track.getPeriod() < track.getDestPeriod()) {
            int tmp = (int)track.getPeriod() + (int)track.recallEffect(data->getEffectCmd());
            if (tmp > track.getDestPeriod())
                tmp = track.getDestPeriod();
            track.setPeriod(static_cast<u16>(tmp));
        }
        setTrackFrequency(player, track, track.getPeriod());
    }


    // 4
    void EffectsMOD::vibrato(Player& player, Track& track)
    {
        uint8_t temp;
        uint16_t delta;
        
        PatternData* data = track.getData();

        if (player.getCurrentTick() == 0) {
            if (data->getEffectValueLower() != 0) 
                track.storeEffectLower(data->getEffectCmd(), data->getEffectValueLower());

            if (data->getEffectValueUpper() != 0) 
                track.storeEffectUpper(data->getEffectCmd(), data->getEffectValueUpper());

            return;    
        }

        temp =  track.getVibratoState() & 0x1f;

        switch (track.getVibratoWaveform() & 3) {
            case 0: 
                delta =  DefsMOD::sineTable[temp]; 
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

        delta *= track.recallEffectLower(data->getEffectCmd());
        delta /= 128;

        if (track.getVibratoState() >= 0)
            setTrackFrequency(player, track, track.getPeriod() + delta);
        else
            setTrackFrequency(player, track, track.getPeriod() - delta);
        
        int tmp2 = track.getVibratoState();
        tmp2 += track.recallEffectUpper(data->getEffectCmd());
        if (tmp2 > 31)
            tmp2 -= 64;
        track.setVibratoState(tmp2);
    }
          
    // 5
    void EffectsMOD::slideToNoteVolumeSlide(Player& player, Track& track)
    {
        PatternData* data = track.getData();
        
        if (player.getCurrentTick() == 0)
            return;

        // maintain portamento based on last portamento to note effect value
        if (track.getPeriod() > track.getDestPeriod()) {
            int tmp = (int)track.getPeriod() - (int)track.recallEffect(0x3);
            if (tmp < track.getDestPeriod())
                tmp = track.getDestPeriod();
            track.setPeriod(static_cast<u16>(tmp));
        } else if (track.getPeriod() < track.getDestPeriod()) {
            int tmp = (int)track.getPeriod() + (int)track.recallEffect(0x3);
            if (tmp > track.getDestPeriod())
                tmp = track.getDestPeriod();
            track.setPeriod(static_cast<u16>(tmp));
        }
        setTrackFrequency(player, track, track.getPeriod());

        // do volume slide based on current effect values
        int tmp;
        if (data->getEffectValueUpper() > 0) {
            tmp = track.getVolume() + data->getEffectValueUpper(); 
            if (tmp > 64)
                tmp = 64;
        } else {
            tmp = track.getVolume() - data->getEffectValueLower();
            if (tmp < 0)	
                tmp = 0;
        }
        track.setVolume(tmp);
    }

    // 6
    void EffectsMOD::vibratoVolumeSlide(Player& player, Track& track)
    {

        uint8_t temp;
        uint16_t delta;
        PatternData* data = track.getData();

        if (player.getCurrentTick() == 0)
            return;    

        // maintain vibrato using last vibrato effect params
        temp =  track.getVibratoState() & 0x1f;

        switch (track.getVibratoWaveform() & 3) {
            case 0: 
                delta =  DefsMOD::sineTable[temp]; 
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

        delta *= track.recallEffectLower(0x4);
        delta /= 128;

        if (track.getVibratoState() >= 0)
            setTrackFrequency(player, track, track.getPeriod() + delta);
        else
            setTrackFrequency(player, track, track.getPeriod() - delta);
        
        int tmp2 = track.getVibratoState();
        tmp2 += track.recallEffectUpper(0x4);
        if (tmp2 > 31)
            tmp2 -= 64;
        track.setVibratoState(tmp2); 

        
        // do volume slide based on current effect values
        int tmp;
        if (data->getEffectValueUpper() > 0) {
            tmp = track.getVolume() + data->getEffectValueUpper(); 
            if (tmp > 64)
                tmp = 64;
        } else {
            tmp = track.getVolume() - data->getEffectValueLower();
            if (tmp < 0)	
                tmp = 0;
        }
        track.setVolume(tmp); 
    }
    
    // 7
    void EffectsMOD::tremolo(Player& player, Track& track)
    {
        uint8_t temp;
        int temp2;
        uint16_t delta;
        PatternData* data = track.getData();
        
        if (player.getCurrentTick() == 0) {
            if (data->getEffectValueUpper() != 0)
                track.storeEffectUpper(data->getEffectCmd(), data->getEffectValueUpper());

            if (data->getEffectValueLower() != 0)
                track.storeEffectLower(data->getEffectCmd(), data->getEffectValueUpper());

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

        delta *= track.recallEffectLower(data->getEffectCmd());//   player->channels[channel].effect_last_value_y[player->channels[channel].effect_num];
        delta /= 64;

        if (track.getTremoloState() >= 0) {
            if (track.getVolume() + delta > 64) 
                delta = 64 - track.getVolume();
            temp2 = track.getVolume() + delta;
            track.setVolume(temp2);
        } else {
            if (track.getVolume() - delta < 0) 
                delta = track.getVolume();

            temp2 = track.getVolume() - delta;
            track.setVolume(temp2);
        }

        //fprintf(stderr, "tremolo: s=%i, v=%i\n", player->channels[channel].tremolo_state, player->channels[channel].volume_master);
        temp2 = track.getTremoloState();
        temp2 += track.recallEffectUpper(data->getEffectCmd());
        if (temp2 > 31)
            temp2 -= 64;
        track.setTremoloState(temp2);
    }
    
    // 8
    void EffectsMOD::panning(Player& player, Track& track)
    {
        track.setPanning(track.getData()->getEffectValue());
    }
    
    // 9
    void EffectsMOD::sampleOffset(Player& player, Track& track)
    {
        PatternData* data = track.getData();
        if (player.getCurrentTick() == 0)
            track.setSamplePos( (data->getEffectValueUpper() * 4096) + (data->getEffectValueLower() * 256) );
    }

    // a
    void EffectsMOD::volumeSlide(Player& player, Track& track)
    {
        if (player.getCurrentTick() == 0)
            return;
        
        PatternData* data = track.getData();
        int tmp;

        if (data->getEffectValueUpper() > 0) {
            tmp = track.getVolume() + data->getEffectValueUpper(); 
            if (tmp > 64)
                tmp = 64;
        } else {
            tmp = track.getVolume() - data->getEffectValueLower();
            if (tmp < 0)	
                tmp = 0;
        }
        track.setVolume(tmp);
        //fprintf(stderr, "volsld %d\n", tmp);
        
    }
    
    
    void EffectsMOD::positionJump(Player& player, Track& track)
    {
        PatternData* data = track.getData();
        // prevent egoist-mods from looping back if no single-mod-loop is enabled
        if (data->getEffectValue() <=  player.getCurrentOrder()) {
            if (!player.getLoop())
                return;
        }

        if (player.getCurrentTick() == 0) {
            player.setNextOrder(data->getEffectValue());
            player.setNextRow(0);
            player.setDoBreak();
        }
    }
    
    // c
    void EffectsMOD::setVolume(Player& player, Track& track)
    {
        if (player.getCurrentTick() == 0)
            track.setVolume(track.getData()->getEffectValue());
    }

    // d
    void EffectsMOD::patternBreak(Player& player, Track& track)
    {
        PatternData* data = track.getData();
        if (player.getCurrentTick() == 0) {
            player.setNextRow(data->getEffectValueUpper() * 10 + data->getEffectValueLower());
            player.setDoBreak();
        }    
    }
          
    void EffectsMOD::special(Player& player, Track& track)
    {
        // dispatch special commands
        switch (track.getData()->getEffectValueUpper()) {
            case 0x1: fineSlideUp(player, track); break;
            case 0x2: fineSlideDown(player, track); break;
            case 0x4: setVibratoWaveform(player, track); break;
            case 0x6: patternLoop(player, track); break;
            case 0x7: setTremoloWaveform(player, track); break;
            case 0x8: ePanning(player, track); break;
            case 0x9: retriggerSample(player, track); break;
            case 0xa: fineVolumeUp(player, track); break;
            case 0xb: fineVolumeDown(player, track); break;
            case 0xc: noteCut(player, track); break;
            case 0xd: delaySample(player, track); break;
            case 0xe: patternDelay(player, track); break;
            default:  Effects::unimplementedEffect(player, track); break;
        }
    }


    void EffectsMOD::fineSlideUp(Player& player, Track& track)
    {
        PatternData* data = track.getData();
        if (player.getCurrentTick() == 0) {
            int tmp = track.getPeriod() - data->getEffectValueLower();
            if (tmp < DefsMOD::periods[DefsMOD::numPeriods - 1])
                tmp = DefsMOD::periods[DefsMOD::numPeriods - 1];
            track.setPeriod(tmp);
            setTrackFrequency(player, track, track.getPeriod());
        }
    }

    void EffectsMOD::fineSlideDown(Player& player, Track& track)
    {
        PatternData* data = track.getData();
        if (player.getCurrentTick() == 0) {
            int tmp = track.getPeriod() + data->getEffectValueLower();
            if (tmp > DefsMOD::periods[0])
                tmp = DefsMOD::periods[0];
            track.setPeriod(tmp);
            setTrackFrequency(player, track, track.getPeriod());
        }
    }

    void EffectsMOD::setVibratoWaveform(Player& player, Track& track)
    {
        if (player.getCurrentTick() == 0) 
            track.setVibratoWaveform(track.getData()->getEffectValueLower());
    }



    void EffectsMOD::patternLoop(Player& player, Track& track)
    {
        PatternData* data = track.getData();
        if (player.getCurrentTick() == 0) {
            if (data->getEffectValueLower() == 0) {
                track.setPatternLoopPosition(player.getCurrentRow());
            } else {
                if (track.getPatternLoopCount() < data->getEffectValueLower()) {
                    player.setNextRow(track.getPatternLoopPosition());
                    track.setPatternLoopCount(track.getPatternLoopCount() + 1);
                } else {
                    track.setPatternLoopCount(0);
                }
            }
        }    
    }

    void EffectsMOD::setTremoloWaveform(Player& player, Track& track)
    {
        if (player.getCurrentTick() == 0) 
            track.setTremoloWaveform(track.getData()->getEffectValueLower());
    }



    void EffectsMOD::ePanning(Player& player, Track& track)
    {
        track.setPanning(track.getData()->getEffectValueLower() + 0x10);
    }

    void EffectsMOD::retriggerSample(Player& player, Track& track)
    {
        PatternData* data = track.getData();
        if (data->getEffectValueLower() == 0)
            return;

        if ((player.getCurrentTick() % data->getEffectValueLower()) == 0)
            track.setSamplePos(0);
    }

    void EffectsMOD::fineVolumeUp(Player& player, Track& track)
    {
        PatternData* data = track.getData();
        int tmp; 
        if (player.getCurrentTick() == 0) {
            tmp = track.getVolume() + data->getEffectValueLower();
            if (tmp > 64)
                tmp = 64;
            track.setVolume(tmp);
        }
    }

    void EffectsMOD::fineVolumeDown(Player& player, Track& track)
    {
        PatternData* data = track.getData();
        int tmp; 
        if (player.getCurrentTick() == 0) {
            tmp = track.getVolume() - data->getEffectValueLower();
            if (tmp < 0)
                tmp = 0;
            track.setVolume(tmp);
        }
    }

    void EffectsMOD::noteCut(Player& player, Track& track)
    {
        if (track.getData()->getEffectValueLower() <= player.getCurrentTick())
            track.setVolume(0);
    }

    void EffectsMOD::delaySample(Player& player, Track& track)
    {
        PatternData* data = track.getData();
        
        if (player.getCurrentTick() == 0)
            track.setSampleDelay(0);

        if (track.getSampleDelay() == data->getEffectValueLower()) {
            if (data->hasInstrument()) {
                track.setInstrument(track.getDestInstrument());
                track.setVolume(player.getModule()->getSample(track.getInstrument()).getDefaultVolume());
            }

            if (data->hasNote()) {
                track.setPeriod(track.getDestPeriod());
                track.setSamplePos(0);
                setTrackFrequency(player, track, track.getPeriod());
            }            
        }

        track.setSampleDelay(track.getSampleDelay() + 1);
    }

    void EffectsMOD::patternDelay(Player& player, Track& track)
    {
        if (!player.getPatternDelayActive())
            player.setPatternDelay(track.getData()->getEffectValueLower());
    }
  
    
    void EffectsMOD::setSpeed(Player& player, Track& track)
    {
        // normally f00 stops module, but we might want to play other mods
        // so we ignore f00
        PatternData* data = track.getData();
        if (data->getEffectValue() == 0)
            return;

        if (player.getCurrentTick() == 0) {
            if (data->getEffectValue() <= 32) {
                player.setSpeed(data->getEffectValue());
            } else {
                player.setBpm(data->getEffectValue());
                player.calcTickDuration();// -> tick_duration = player_calc_tick_duration(player->bpm, player->sample_rate);
            }
        }    
    }

    
}
 
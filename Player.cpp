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

#include "Player.hpp"
#include "Track.hpp"
#include "EffectsMOD.hpp"
#include "EffectsS3M.hpp"
#include "DefsMOD.hpp"
#include "ModuleUtils.hpp"
#include <cmath>

namespace vmp
{
    Player::Player(Module* m, u32 sample_rate)
        : sampleRate(sample_rate)
        , resampling(RESAMPLING_NONE)
    {
        setModule(m);
    }
    
    Player::Player(u32 sample_rate)
        : sampleRate(sample_rate)
        , resampling(RESAMPLING_NONE)
    {}
    
    Player::~Player()
    {}
    
    void Player::setBpm(u8 val) 
    {
        bpm = val;
        calcTickDuration();
    }
    
    void Player::setSpeed(u8 val)
    {
        speed = val;
    }
    
    void Player::readPcm(sample_t& mix_l, sample_t& mix_r) 
    {
        int i;
        sample_mac_t mix_tmp_l, mix_tmp_r;
        
        if (tickPos <= 0) {

            // last tick reached, advance to next row
            if (currentTick == speed) {
                // if there is a pattern delay, don't advance to the next row
                if (patternDelay > 0) {
                    patternDelay--;
                    patternDelayActive = true;
                } else {
                    currentRow = nextRow;
                    nextRow++;
                    patternDelayActive = false;
                }

                currentTick = 0;

                // advance to next order if last row played or break upcoming
                if ((currentRow >= module->getPattern(currentPattern).getNumRows() || doBreak)) {

                    currentOrder = nextOrder;
                    nextOrder++;
                    doBreak = false;
                    patternDelayActive = false;

                    // only if regular pattern end (no break)
                    if (currentRow >= module->getPattern(currentPattern).getNumRows()) {
                        currentRow = 0;
                        nextRow = 1;
                    }

                    // end of song reached...
                    if (nextOrder >= module->getNumOrders()) {
                        // loop if looping enabled 
                        if (loopModule) {
                            nextOrder = 0;
                        } else {
                            haveData = false;
                        }
                    }

                    currentPattern = module->getPatternForOrder(currentOrder);

                    // TODO reimplement callback stuff
                    //if (player->order_callback)
                        //(player->order_callback)(player, player->current_order, player->current_pattern);
                    //    (player->order_callback)(player, player->callback_user_ptr);

                }

                // TODO reimplement callback stuff
                // maintain row callback
                //if (player->row_callback)
                    //(player->row_callback)(player, player->current_order, player->current_pattern, player->current_row);
                //    (player->row_callback)(player, player->callback_user_ptr);

                // fetch new pattern data from module
                fprintf(stderr, "%02d:%02d:%02d|", currentOrder, currentPattern, currentRow);
                for (i = 0; i < module->getNumTracks(); i++) {
                    tracks[i].setData(&module->getData(currentPattern, currentRow, i));
                    
                    //if (i > 5)
                    ModuleUtils::dumpData(module->getData(currentPattern, currentRow, i));
                    //player->channels[k].effect_num = current_data->effect_num;
                    //player->channels[k].effect_value = current_data->effect_value;

//                    player->newrow_action(player, current_data, k);
                    effects->newRowAction(*this, tracks[i]);
                    //if (i > 5)
                    //fprintf(stderr,  "==%02d==", tracks[i].getInstrument());
                                        
                }
                fprintf(stderr, "\n");   
                
                

            }
            
            for (i = 0; i < module->getNumTracks(); i++) {
                if (tracks[i].getData()->hasEffectCmd())
                    effects->doEffect(tracks[i].getData()->getEffectCmd(), *this, tracks[i]);
            }
            
            // go for next tick
            currentTick++;
            tickPos = tickDuration;
            // maintain tick callback
            //if (player->tick_callback)
                //(player->tick_callback)(player, player->current_order, player->current_pattern, player->current_row, player->current_tick, player->channels);
                //(player->tick_callback)(player, player->callback_user_ptr);
        }
  
        mix_tmp_l = 0;
        mix_tmp_r = 0;
        
        for (Track& track : tracks) {

            if (!track.isActive())
                continue;
            
            sample_mac_t s = fetchSample(track);

            // Performance Tuning: no need to do anything with 0-samples
            if (s != 0) {
                s *= track.getVolume();
                s *= track.getFxVolume();
                s /= (64 * 64); 

                sample_mac_t cr = (s * track.getPanning()) / 256;
                sample_mac_t cl = (s * (255 - track.getPanning())) / 256;

                mix_tmp_l += cl;
                mix_tmp_r += cr;
            }

        }

          // TODO reimplement callback
          //if (player->channel_sample_callback) 
          //    (player->channel_sample_callback)(player, player->callback_user_ptr);

        mix_tmp_l /= static_cast<sample_mac_t>(module->getNumTracks());
        mix_tmp_r /= static_cast<sample_mac_t>(module->getNumTracks());

        mix_l = static_cast<sample_t>(mix_tmp_l);
        mix_r = static_cast<sample_t>(mix_tmp_r);

        tickPos--;
        
    }
    
    void Player::reset()
    {
        speed = module->getInitialSpeed();
        setBpm(module->getInitialBpm());
        
        currentOrder = 0;
        nextOrder = 0;
        currentRow = 0;
        nextRow = currentRow;
        currentPattern = module->getPatternForOrder(currentOrder);
        tickPos = 0;
        //tickDuration = calcTickDuration(bpm, sampleRate);
        doBreak = 1;       // to initialize state 
        currentTick = speed;
        
        patternDelay = 0;
        patternDelayActive = false;
        haveData = true;
    }
    
    void Player::setModule(Module* m) 
    {
        int i;
        
        module = m;
        tracks = vector<Track>(module->getNumTracks());
        
        reset();

        switch (module->getModuleType()) {
            case Module::MODULE_TYPE_MOD:
                effects = std::shared_ptr<Effects>(new EffectsMOD());
                break;
            case Module::MODULE_TYPE_S3M:
                effects = std::shared_ptr<Effects>(new EffectsS3M());
                break;
        }
        
        for(i = 0; i < module->getNumTracks(); i++) 
            tracks[i].setPanning(module->getInitialPanning(i));
    }
    
    sample_mac_t Player::fetchSample(Track& track)
    {
        sample_mac_t s, s2;
        precision_t sample_pos = track.getSamplePos();
        
        Sample& sample = module->getSample(track.getInstrument());

        //precision_t saved_sample_pos = track.getSamplePos();
        // maintain looping
        if (sample.getLoopEnabled()) {
            while (static_cast<u32>(sample_pos) > sample.getLoopEnd())
                sample_pos -= static_cast<precision_t>(sample.getLoopLength());
        } else {
            if (sample_pos >= static_cast<precision_t>(sample.getLength())) {
                return SAMPLE_T_ZERO;
                track.setActive(false);
            }
        }

        // fetch sample 
        s = sample.getData(static_cast<u32>(sample_pos));

        if (resampling == RESAMPLING_LINEAR) {
            // do linear interpolation
            if (sample.getLoopEnabled()) {
                // looping sample will interpolate to loop start
                if (sample_pos >= static_cast<precision_t>(sample.getLoopEnd())) 
                    s2 = sample.getData(sample.getLoopStart());
                else 
                    s2 = sample.getData(static_cast<u32>(sample_pos) + 1);
            } else {
                if (sample_pos < (sample.getLength() - 1)) 
                    s2 = sample.getData(static_cast<u32>(sample_pos) + 1);
                else
                    s2 = s;
            }
            // subtract int from float will leave the fractional offset of sample position which is the "in between" factor 
            // for the interpolation calculation
            s += (s2 - s) * (sample_pos - static_cast<precision_t>(static_cast<u32>(sample_pos)));  
        }

        // advance sample position
        sample_pos += track.getSampleStep();
        track.setSamplePos(sample_pos);

        return s;
    }
    
    void Player::calcTickDuration()
    {
        tickDuration = (((static_cast<precision_t>(sampleRate) / (static_cast<precision_t>(bpm) / 60.0f)) / 4.0f) / 6.0f);
    }
    
    
}
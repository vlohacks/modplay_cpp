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
#include "DefsMOD.hpp"
#include "ModuleUtils.hpp"
#include <cmath>

namespace vmp
{
    Player::Player(Module* m, u32 sample_rate)
        : sampleRate(sample_rate)
        , resampling(RESAMPLING_LINEAR)
        , effects(0)
    {
        setModule(m);
    }
    
    Player::Player(u32 sample_rate)
        : sampleRate(sample_rate)
        , resampling(RESAMPLING_LINEAR)
        , effects(0)
    {}
    
    Player::~Player()
    {}
    
    bool Player::hasData() {
        return haveData;
    }
    
    u8 Player::getCurrentTick()
    {
        return currentTick;
    }
    
    u8 Player::getCurrentOrder()
    {
        return currentOrder;
    }
    
    u8 Player::getCurrentRow()
    {
        return currentRow;
    }
    
    bool Player::getPatternDelayActive() 
    {
        return patternDelayActive;
    }
    
    u32 Player::getSampleRate()
    {
        return sampleRate;
    }
    
    void Player::setBpm(u8 val) 
    {
        bpm = val;
    }
    u8 Player::getBpm()
    {
        return bpm;
    }
    
    void Player::setSpeed(u8 val)
    {
        speed = val;
    }
    u8 Player::getSpeed()
    {
        return speed;
    }
    
    void Player::setDoBreak()
    {
        doBreak = true;
    }
    
    void Player::setNextRow(u8 row)
    {
        nextRow = row;
    }
    
    void Player::setNextOrder(u8 order)
    {
        nextOrder = order;
    }
    
    bool Player::getLoop() {
        return loopModule;
    }
    void Player::setLoop(bool val) {
        loopModule = val;
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
                    if (nextOrder > module->getNumOrders()) {
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
                fprintf(stderr, "%02x:%02x|", currentPattern, currentRow);
                for (i = 0; i < module->getNumTracks(); i++) {
                    tracks[i].setData(&module->getData(currentPattern, currentRow, i));
                    
                    ModuleUtils::dumpData(module->getData(currentPattern, currentRow, i));
                    //player->channels[k].effect_num = current_data->effect_num;
                    //player->channels[k].effect_value = current_data->effect_value;

//                    player->newrow_action(player, current_data, k);
                    effects->newRowAction(*this, tracks[i]);
                                        
                }
                fprintf(stderr, "\n");
                
                

            }
            
            for (Track& track : tracks) {
                if (track.getData()->hasEffectCmd())
                    effects->doEffect(track.getData()->getEffectCmd(), *this, track);
            }
            

/*
            // maintain effects
            for (k=0; k < player->module->num_channels; k++) {
                if ((player->effect_map)[player->channels[k].effect_num])
                    (player->effect_map)[player->channels[k].effect_num](player, k);
            }
            */
            // go for next tick
            //printf("%d %d", currentTick, tickPos);
            currentTick++;
            tickPos = tickDuration;
            //player->current_tick++;
            //player->tick_pos = player->tick_duration;

            // maintain tick callback
            //if (player->tick_callback)
                //(player->tick_callback)(player, player->current_order, player->current_pattern, player->current_row, player->current_tick, player->channels);
                //(player->tick_callback)(player, player->callback_user_ptr);


        }
  
        mix_tmp_l = 0;
        mix_tmp_r = 0;
        
        for (Track& track : tracks) {
              //if ((player->solo_channel >= 0) && (k != player->solo_channel))
              //    continue;
            if (!track.isActive())
                continue;
            
            sample_mac_t s = fetchSample(track);//  player_channel_fetch_sample(player, k);

            // Performance Tuning: no need to do anything with 0-samples
            if (s != 0) {
                s *= track.getVolume();
                s /= 64; 

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
    
    void Player::setModule(Module* m) 
    {
        int i;
        
        module = m;
        tracks = vector<Track>(module->getNumTracks());
        
        speed = module->getInitialSpeed();
        bpm = module->getInitialBpm();
        calcTickDuration();
        
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
                
        if (effects) {
            delete effects;
        }
        
        switch (module->getModuleType()) {
            case Module::MODULE_TYPE_MOD:
                effects = new EffectsMOD();
                break;
        }
        
        haveData = true;
        i = 0;
        for(Track& track: tracks) {
            track.setPanning(module->getInitialPanning(i));
            i++;
        }
    }
    
    Module* Player::getModule()
    {
        return module;
    }
    
    /* moved to effects section
    void Player::setTrackFrequency(Track& track)
    {
        const float x = 1.007246412;
        
        Sample& sample = module->getSample(track.getInstrument());
        int8_t finetune;
        uint16_t period = track.getPeriod();

        // no period? bail out 
        if (!period)
            return;

        switch (module->getModuleType()) {
            case Module::MODULE_TYPE_MOD:
                // unusual: finetune is a signed nibble
                finetune = (sample.getFinetune() >= 8 
                        ? -(16 - sample.getFinetune()) 
                        : sample.getFinetune());
                track.setFrequency(DefsMOD::paulaFreq[0] / (period * 2));
                
                // TODO: will there be a day when we can get rid of libm here?
                if (finetune != 0)
                    track.setFrequency( track.getFrequency() * pow(x, finetune));

                break;

            //case module_type_stm:
            case Module::MODULE_TYPE_S3M:
                track.setFrequency(14317056L / period);
                break;
        }

        // Performance tuning: do this calculation only once rather than every sample
        track.setSampleStep(static_cast<precision_t>(track.getFrequency()) / static_cast<precision_t>(sampleRate));
    }
    */
    
    sample_mac_t Player::fetchSample(Track& track)
    {
        sample_mac_t s, s2;
        
        Sample& sample = module->getSample(track.getInstrument());

        // maintain looping
        if (sample.getLoopEnabled()) {
            while (track.getSamplePos() >= static_cast<precision_t>(sample.getLoopEnd())) {
                track.setSamplePos(track.getSamplePos() - static_cast<precision_t>(sample.getLoopLength()));
            }
        } else {
            if (track.getSamplePos() >= static_cast<precision_t>(sample.getLength()))
                return SAMPLE_T_ZERO;
        }

        // fetch sample 
        s = sample.getData(track.getSamplePos());

        if (resampling == RESAMPLING_LINEAR) {
            // do linear interpolation
            if (sample.getLoopEnabled()) {
                // looping sample will interpolate to loop start
                if (track.getSamplePos() >= static_cast<precision_t>(sample.getLoopEnd())) 
                    s2 = sample.getData(sample.getLoopStart());
                else 
                    s2 = sample.getData(track.getSamplePos() + 1);
            } else {
                if (track.getSamplePos() < (sample.getLength() - 1)) 
                    s2 = sample.getData(static_cast<u32>(track.getSamplePos()) + 1);
                else
                    s2 = s;
            }
            // conversion to int will remove the fractional part of the sample_pos
            s += (s2 - s) * (track.getSamplePos() - static_cast<precision_t>(static_cast<int>(track.getSamplePos())));  
        }

        // advance sample position
        track.setSamplePos(track.getSamplePos() + track.getSampleStep());
        //channel->sample_pos += ((float)channel->frequency / (float)player->sample_rate);

        return s;
    }
    
    void Player::calcTickDuration()
    {
        tickDuration = ((((precision_t)sampleRate / ((precision_t)bpm / 60.0f)) / 4.0f) / 6.0f);
    }
    
    
}
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

/* 
 * File:   Player.hpp
 * Author: vlo
 *
 * Created on 8. September 2015, 17:39
 */

#ifndef PLAYER_HPP
#define	PLAYER_HPP

#include "Module.hpp"
#include "Track.hpp"
#include "SampleConv.hpp"
#include "Effects.hpp"
//#include "EffectsMOD.hpp"

namespace vmp 
{
    class Player 
    {
    public:
        Player(Module* m, u32 sample_rate);
        Player(u32 sample_rate);
        ~Player();
        
        void setModule(Module* m);
        Module* getModule();
        
        void readPcm(sample_t& mix_l, sample_t& mix_r);
        bool hasData();
        
        u8 getCurrentTick();
        u8 getCurrentOrder();
        u8 getCurrentRow();
        
        bool getLoop();
        void setLoop(bool val);
        
        bool getPatternDelayActive();
        u32 getSampleRate();
        
        typedef enum {
            RESAMPLING_NONE,
            RESAMPLING_LINEAR
        } resampling_t;
        
        void setBpm(u8 val);
        u8 getBpm();
        
        void setSpeed(u8 val);
        u8 getSpeed();
        
        void calcTickDuration();
        
        void setDoBreak();
        void setNextRow(u8 row);
        void setNextOrder(u8 order);
        
    private:
        Module* module;
        Effects* effects;
        vector<Track> tracks;
        u32 sampleRate;
        resampling_t resampling;
        
        // internal status variables
        u8 speed;
        u8 bpm;
        u8 currentTick;
        u8 currentRow;
        u8 currentPattern;
        u8 currentOrder;
        u8 nextOrder;
        u8 nextRow;
        
        precision_t tickPos;                                     // current position in tick (internal)
        precision_t tickDuration;                                // duration of one tick (gets calculated when set speed effect occurs)  
        int patternDelay;
        bool patternDelayActive;
        bool doBreak;
        bool haveData;
        bool loopModule;
        
        
        precision_t calcTickDuration(const uint16_t bpm, const uint32_t sample_rate);
        
        //void setTrackFrequency(Track& track);
        
        sample_mac_t fetchSample(Track& track);
        
        
        
    };
}

#endif	/* PLAYER_HPP */


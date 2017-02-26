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
 * 
 * Track.hpp
 * This represents the Track state in the player, not to be confused with an 
 * audio channel. Since IT format supports "Virtual Channels" per track, this
 * got separated. Old formats just alloc one channel per track and use this.
 * 
 */


#ifndef TRACK_HPP
#define TRACK_HPP

#include "Types.hpp"
#include "PatternData.hpp"

namespace vmp
{
    class Track
    {
    private:
        
        typedef union {
            struct {
                u8 lower : 4;
                u8 upper : 4;
            };
            u8 all;
        } effect_storage_t;        
        
        PatternData* patternData;
        
        bool pendingFrequencyUpdate;
        u16 period;
        u32 frequency;
        u8 instrument;
        u8 volume;
        
        int vibratoState;
        u8 vibratoWaveform;
        
        int tremoloState;
        u8 tremoloWaveform;
        
        u16 destPeriod;
        u8 destInstrument;
        u8 patternLoopPosition;
        u8 patternLoopCount;
        
        precision_t samplePos;
        precision_t sampleStep;
        
        u8 panning;
        bool active;
        
        vector<effect_storage_t> effectStorage; 
        
        
        
    public:
        
        Track();
        
        void setData(PatternData* data);
        PatternData* getData();
        
        u16 getPeriod();
        void setPeriod(u16 p);
        u32 getFrequency();
        void setFrequency(u32 f);
        
        u8 getInstrument();
        void setInstrument(u8 instr);
        u8 getVolume();
        void setVolume(u8 vol);
        
        int getVibratoState();
        void setVibratoState(int vibrato_state);
        
        u8 getVibratoWaveform();
        void setVibratoWaveform(u8 vibrato_waveform);

        int getTremoloState();
        void setTremoloState(int tremolo_state);
        
        u8 getTremoloWaveform();
        void setTremoloWaveform(u8 tremolo_waveform);

        
        
        u16 getDestPeriod();
        void setDestPeriod(u16 dest_period);
        u8 getDestInstrument();
        void setDestInstrument(u8 dest_instrument);
        u8 getPatternLoopPosition();
        void setPatternLoopPosition(u8 loop_position);
        u8 getPatternLoopCount();
        void setPatternLoopCount(u8 loop_count);
        
        bool getPendingFrequencyUpdate();
        void setPendingFrequencyUpdate(bool v);
        
        precision_t getSamplePos();
        void setSamplePos(precision_t sample_pos);
        
        precision_t getSampleStep();
        void setSampleStep(precision_t sample_step);
        
        u8 getPanning();
        void setPanning(u8 pan);
        
        bool isActive();
        void setActive(bool state);
        
        u8 recallEffect(int effect_no);
        u8 recallEffectLower(int effect_no);
        u8 recallEffectUpper(int effect_no);
        
        void storeEffect(int effect_no, u8 value);
        void storeEffectLower(int effect_no, u8 value);
        void storeEffectUpper(int effect_no, u8 value);
        
    };
}

#endif /* TRACK_HPP */


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
        u8 fxVolume;        // currently only for tremolo effect :-(
        
        int vibratoState;
        u8 vibratoWaveform;
        
        int tremoloState;
        u8 tremoloWaveform;
        
        int tremorState;
        
        u16 destPeriod;
        u8 destInstrument;
        u8 destVolume;
        
        u8 patternLoopPosition;
        u8 patternLoopCount;
        u8 sampleDelay;
        
        precision_t samplePos;
        precision_t sampleStep;
        
        u8 panning;
        bool active;
        
        vector<effect_storage_t> effectStorage; 
        
        
        
    public:
        
        Track();
        
    void setData(PatternData* data)          { patternData = data; }
    PatternData* getData() const             { return patternData; }
    
    u16 getPeriod() const                    { return period; }
    void setPeriod(u16 p)                    { period = p; }
    
    u32 getFrequency() const                 { return frequency; }
    void setFrequency(u32 f)                 { frequency = f; }
    
    u8 getInstrument() const                 { return instrument; }
    void setInstrument(u8 instr)             { instrument = instr; }
    
    u8 getVolume() const                     { return volume; }
    void setVolume(u8 vol)                   { volume = vol; }

    u8 getFxVolume() const                   { return fxVolume; }
    void setFxVolume(u8 vol)                 { fxVolume = vol; }
    
    int getVibratoState() const              { return vibratoState; }
    void setVibratoState(int vibrato_state)  { vibratoState = vibrato_state; }
    
    u8 getVibratoWaveform() const                { return vibratoWaveform; }
    void setVibratoWaveform(u8 vibrato_waveform) { vibratoWaveform = vibrato_waveform; }

    int getTremoloState() const              { return tremoloState; }
    void setTremoloState(int tremolo_state)  { tremoloState = tremolo_state; }
    
    u8 getTremoloWaveform() const            { return tremoloWaveform; }
    void setTremoloWaveform(u8 tremolo_waveform) { tremoloWaveform = tremolo_waveform; }
    
    int getTremorState() const              { return tremorState; }
    void setTremorState(int tremor_state)   { tremorState = tremor_state; }
    
    
    u16 getDestPeriod() const                { return destPeriod; }
    void setDestPeriod(u16 dest_period)      { destPeriod = dest_period; }
    
    u8 getDestInstrument() const             { return destInstrument; }
    void setDestInstrument(u8 dest_instrument) { destInstrument = dest_instrument; }

    u8 getDestVolume() const                 { return destVolume; }
    void setDestVolume(u8 vol)               { destVolume = vol; }

    
    u8 getPatternLoopPosition() const        { return patternLoopPosition; }
    void setPatternLoopPosition(u8 loop_position) { patternLoopPosition = loop_position; }

    u8 getPatternLoopCount() const           { return patternLoopCount; }
    void setPatternLoopCount(u8 loop_count)  { patternLoopCount = loop_count; }
    
    u8 getSampleDelay() const                { return sampleDelay; }
    void setSampleDelay(u8 delay)            { sampleDelay = delay; }
    
    precision_t getSamplePos() const         { return samplePos; }
    void setSamplePos(precision_t sample_pos) { samplePos = sample_pos; }
    
    precision_t getSampleStep() const        { return sampleStep; }
    void setSampleStep(precision_t sample_step) { sampleStep = sample_step; }

    u8 getPanning() const { return panning; }
    void setPanning(u8 pan) { panning = pan; }
    
    bool isActive()  const { return active; }
    void setActive(bool state) { active = state; }
    
    u8 recallEffect(int effect_no) const { return effectStorage[effect_no].all; }
    u8 recallEffectLower(int effect_no) const { return effectStorage[effect_no].lower; }
    u8 recallEffectUpper(int effect_no) const { return effectStorage[effect_no].upper; }
    
    void storeEffect(int effect_no, u8 value) { effectStorage[effect_no].all = value; }
    void storeEffectLower(int effect_no, u8 value) { effectStorage[effect_no].lower = value; }
    void storeEffectUpper(int effect_no, u8 value) { effectStorage[effect_no].upper = value; }
        
    };
}

#endif /* TRACK_HPP */


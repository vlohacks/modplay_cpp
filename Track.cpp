/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include <stdbool.h>

#include "Track.hpp"

namespace vmp
{
    Track::Track() 
        : active(false)
//        , pendingFrequencyUpdate(false)
        , period(0)
        , frequency(0)
        , vibratoState(0)
        , vibratoWaveform(0)
        , tremoloState(0)
        , tremoloWaveform(0)
        , samplePos(0)
        , sampleStep(0)
        , effectStorage(128)
        , patternLoopPosition(0)
    {}
    
    void Track::setData(PatternData* data) { patternData = data; }
    PatternData* Track::getData() { return patternData; }
    
    u16 Track::getPeriod() { return period; }
    void Track::setPeriod(u16 p) { period = p; }
    
    u32 Track::getFrequency() { return frequency; }
    void Track::setFrequency(u32 f) { frequency = f; }
    /*
    bool Track::getPendingFrequencyUpdate() { return pendingFrequencyUpdate; }
    void Track::setPendingFrequencyUpdate(bool v)  { pendingFrequencyUpdate = v; }
    */
    
    u8 Track::getInstrument() { return instrument; }
    void Track::setInstrument(u8 instr) { instrument = instr; }
    
    u8 Track::getVolume() { return volume; }
    void Track::setVolume(u8 vol) { volume = vol; }
    
    int Track::getVibratoState() { return vibratoState; }
    void Track::setVibratoState(int vibrato_state) { vibratoState = vibrato_state; }
    
    u8 Track::getVibratoWaveform() { return vibratoWaveform; }
    void Track::setVibratoWaveform(u8 vibrato_waveform) { vibratoWaveform = vibrato_waveform; }

    int Track::getTremoloState() { return tremoloState; }
    void Track::setTremoloState(int tremolo_state) { tremoloState = tremolo_state; }
    
    u8 Track::getTremoloWaveform() { return tremoloWaveform; }
    void Track::setTremoloWaveform(u8 tremolo_waveform) { tremoloWaveform = tremolo_waveform; }
    
    
    u16 Track::getDestPeriod() { return destPeriod; }
    void Track::setDestPeriod(u16 dest_period) { destPeriod = dest_period; }
    
    u8 Track::getDestInstrument() { return destInstrument; }
    void Track::setDestInstrument(u8 dest_instrument) { destInstrument = dest_instrument; }
    
    u8 Track::getPatternLoopPosition() { return patternLoopPosition; }
    void Track::setPatternLoopPosition(u8 loop_position) { patternLoopPosition = loop_position; }

    u8 Track::getPatternLoopCount() { return patternLoopCount; }
    void Track::setPatternLoopCount(u8 loop_count) { patternLoopCount = loop_count; }
    
    precision_t Track::getSamplePos() { return samplePos; }
    void Track::setSamplePos(precision_t sample_pos) { samplePos = sample_pos; }
    
    precision_t Track::getSampleStep() { return sampleStep; }
    void Track::setSampleStep(precision_t sample_step) { sampleStep = sample_step; }

    u8 Track::getPanning() { return panning; }
    void Track::setPanning(u8 pan) { panning = pan; }
    
    bool Track::isActive() { return active; }
    void Track::setActive(bool state) { active = state; }
    
    u8 Track::recallEffect(int effect_no) { return effectStorage[effect_no].all; }
    u8 Track::recallEffectLower(int effect_no) { return effectStorage[effect_no].lower; }
    u8 Track::recallEffectUpper(int effect_no) { return effectStorage[effect_no].upper; }
    
    void Track::storeEffect(int effect_no, u8 value) { effectStorage[effect_no].all = value; }
    void Track::storeEffectLower(int effect_no, u8 value) { effectStorage[effect_no].lower = value; }
    void Track::storeEffectUpper(int effect_no, u8 value) { effectStorage[effect_no].upper = value; }
    
}
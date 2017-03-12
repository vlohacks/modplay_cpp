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
        , volume(64)
        , fxVolume(64)
    {}
    
    void Track::setData(PatternData* data) { patternData = data; }
    PatternData* Track::getData() const { return patternData; }
    
    u16 Track::getPeriod() const { return period; }
    void Track::setPeriod(u16 p) { period = p; }
    
    u32 Track::getFrequency() const { return frequency; }
    void Track::setFrequency(u32 f) { frequency = f; }
    /*
    bool Track::getPendingFrequencyUpdate() { return pendingFrequencyUpdate; }
    void Track::setPendingFrequencyUpdate(bool v)  { pendingFrequencyUpdate = v; }
    */
    
    u8 Track::getInstrument() const { return instrument; }
    void Track::setInstrument(u8 instr) { instrument = instr; }
    
    u8 Track::getVolume() const { return volume; }
    void Track::setVolume(u8 vol) { volume = vol; }

    u8 Track::getFxVolume() const { return fxVolume; }
    void Track::setFxVolume(u8 vol) { fxVolume = vol; }

    
    int Track::getVibratoState() const { return vibratoState; }
    void Track::setVibratoState(int vibrato_state) { vibratoState = vibrato_state; }
    
    u8 Track::getVibratoWaveform() const{ return vibratoWaveform; }
    void Track::setVibratoWaveform(u8 vibrato_waveform) { vibratoWaveform = vibrato_waveform; }

    int Track::getTremoloState() const { return tremoloState; }
    void Track::setTremoloState(int tremolo_state) { tremoloState = tremolo_state; }
    
    u8 Track::getTremoloWaveform() const { return tremoloWaveform; }
    void Track::setTremoloWaveform(u8 tremolo_waveform) { tremoloWaveform = tremolo_waveform; }
    
    
    u16 Track::getDestPeriod() const { return destPeriod; }
    void Track::setDestPeriod(u16 dest_period) { destPeriod = dest_period; }
    
    u8 Track::getDestInstrument() const { return destInstrument; }
    void Track::setDestInstrument(u8 dest_instrument) { destInstrument = dest_instrument; }
    
    u8 Track::getPatternLoopPosition() const { return patternLoopPosition; }
    void Track::setPatternLoopPosition(u8 loop_position) { patternLoopPosition = loop_position; }

    u8 Track::getPatternLoopCount() const { return patternLoopCount; }
    void Track::setPatternLoopCount(u8 loop_count) { patternLoopCount = loop_count; }
    
    u8 Track::getSampleDelay() const { return sampleDelay; }
    void Track::setSampleDelay(u8 delay) { sampleDelay = delay; }
    
    precision_t Track::getSamplePos() const { return samplePos; }
    void Track::setSamplePos(precision_t sample_pos) { samplePos = sample_pos; }
    
    precision_t Track::getSampleStep() const { return sampleStep; }
    void Track::setSampleStep(precision_t sample_step) { sampleStep = sample_step; }

    u8 Track::getPanning() const { return panning; }
    void Track::setPanning(u8 pan) { panning = pan; }
    
    bool Track::isActive()  const { return active; }
    void Track::setActive(bool state) { active = state; }
    
    u8 Track::recallEffect(int effect_no) const { return effectStorage[effect_no].all; }
    u8 Track::recallEffectLower(int effect_no) const { return effectStorage[effect_no].lower; }
    u8 Track::recallEffectUpper(int effect_no) const { return effectStorage[effect_no].upper; }
    
    void Track::storeEffect(int effect_no, u8 value) { effectStorage[effect_no].all = value; }
    void Track::storeEffectLower(int effect_no, u8 value) { effectStorage[effect_no].lower = value; }
    void Track::storeEffectUpper(int effect_no, u8 value) { effectStorage[effect_no].upper = value; }
}
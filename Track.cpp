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
        , tremorState(0)
        , samplePos(0)
        , sampleStep(0)
        , effectStorage(128)
        , patternLoopPosition(0)
        , patternLoopCount(0)
        , volume(64)
        , fxVolume(64)
    {}
    

}
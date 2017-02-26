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

/* 
 * File:   EffectsMOD.hpp
 * Author: vlo
 *
 * Created on 14. Juli 2016, 21:46
 */

#ifndef EFFECTSMOD_HPP
#define EFFECTSMOD_HPP

#include "Effects.hpp"

namespace vmp 
{
    class EffectsMOD : public Effects
    {
    private:
        const int NUM_EFFECTS = 16;
        
    public:
        EffectsMOD();
        ~EffectsMOD();
        
        
        void newRowAction(Player& player, Track& track) override;
        static void setTrackFrequency(Player& player, Track& track, u16 period);
        
        
        static void arpeggio(Player& player, Track& track);
        static void slideUp(Player& player, Track& track);
        static void slideDown(Player& player, Track& track);
        static void slideToNote(Player& player, Track& track);
        static void vibrato(Player& player, Track& track);
        static void slideToNoteVolumeSlide(Player& player, Track& track);
        static void vibratoVolumeSlide(Player& player, Track& track);
        static void tremolo(Player& player, Track& track);
        static void panning(Player& player, Track& track);
        static void sampleOffset(Player& player, Track& track);
        static void volumeSlide(Player& player, Track& track);
        static void positionJump(Player& player, Track& track);
        static void setVolume(Player& player, Track& track);
        static void patternBreak(Player& player, Track& track);
        static void special(Player& player, Track& track);
        static void setSpeed(Player& player, Track& track);
        
        static void fineSlideUp(Player& player, Track& track);
        static void fineSlideDown(Player& player, Track& track);
        static void setVibratoWaveform(Player& player, Track& track);
        static void patternLoop(Player& player, Track& track);
        static void setTremoloWaveform(Player& player, Track& track);
        static void ePanning(Player& player, Track& track);
        static void retriggerSample(Player& player, Track& track);
        static void fineVolumeUp(Player& player, Track& track);
        static void fineVolumeDown(Player& player, Track& track);
        static void noteCut(Player& player, Track& track);
        static void delaySample(Player& player, Track& track);
        static void patternDelay(Player& player, Track& track);
    };
}

#endif /* EFFECTSMOD_HPP */


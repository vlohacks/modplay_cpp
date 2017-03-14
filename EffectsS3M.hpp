/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   EffectsS3M.hpp
 * Author: vlo
 *
 * Created on 13. März 2017, 22:30
 */

#ifndef EFFECTSS3M_HPP
#define EFFECTSS3M_HPP

#include "Effects.hpp"

namespace vmp
{
    class EffectsS3M : public Effects
    {
    private:
        static u16 getTunedPeriod(Player& player, Track& track, const u16 basePeriod);
    public:
        EffectsS3M();
        ~EffectsS3M();
        
        
        void newRowAction(Player& player, Track& track) override;
        static void setTrackFrequency(Player& player, Track& track, u16 period);
        
        static void ASetSpeed(Player& player, Track& track);
        static void BPositionJump(Player& player, Track& track);
        static void CPatternBreak(Player& player, Track& track);
        static void DVolumeSlide(Player& player, Track& track);
        static void ESlideDown(Player& player, Track& track);
        static void FSlideUp(Player& player, Track& track);
        static void GSlideToNote(Player& player, Track& track);
        static void HVibrato(Player& player, Track& track);
        static void ITremor(Player& player, Track& track);
        static void JArpeggio(Player& player, Track& track);
        static void KVibratoVolumeSlide(Player& player, Track& track);
        static void LSlideToNoteVolumeSlide(Player& player, Track& track);
        static void OSampleOffset(Player& player, Track& track);
        static void QRetriggerVolumeSlide(Player& player, Track& track);
        static void RTremolo(Player& player, Track& track);
        static void SSpecial(Player& player, Track& track);
        static void S3SetVibratoWaveform(Player& player, Track& track);
        static void S4SetTremoloWaveform(Player& player, Track& track);
        static void S8Panning(Player& player, Track& track);
        static void SAStereoControl(Player& player, Track& track);
        static void SCNoteCut(Player& player, Track& track);
        static void SDDelaySample(Player& player, Track& track);
        static void TSetBpm(Player& player, Track& track);
        static void XPanning(Player& player, Track& track);
        
    };
}

#endif /* EFFECTSS3M_HPP */


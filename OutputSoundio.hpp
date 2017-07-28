/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   OutputSoundio.hpp
 * Author: vlo
 *
 * Created on 27. Juli 2017, 17:20
 */

#ifndef OUTPUTSOUNDIO_HPP
#define OUTPUTSOUNDIO_HPP

#include "Output.hpp"
#include "Types.hpp"
#include <soundio/soundio.h>

namespace vmp
{
    class OutputSoundio : public Output
    {
    private:
        static void soundioCallback(struct SoundIoOutStream* stream, int frame_count_min, int frame_count_max);
        struct SoundIo* soundio;
        struct SoundIoDevice* device;
        struct SoundIoOutStream* outstream; 
        
    public:
        OutputSoundio(Player* p);
        ~OutputSoundio();
        
        void start() override;
        void stop() override;
    };
}

#endif /* OUTPUTSOUNDIO_HPP */


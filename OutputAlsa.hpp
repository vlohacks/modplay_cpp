/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   OutputAlsa.hpp
 * Author: vlo
 *
 * Created on 25. Februar 2017, 21:45
 */

#ifndef OUTPUTALSA_HPP
#define OUTPUTALSA_HPP

#include "Output.hpp"
#include "Types.hpp"
#include <thread>
#include <cstdio>
#include <alsa/asoundlib.h>

namespace vmp
{

    class OutputOptionsAlsa : public OutputOptions
    {
    public:
        OutputOptionsAlsa();
        ~OutputOptionsAlsa();
        string device;
        u32 outputBufferSize;
        u32 sampleRate;
    };

    class OutputAlsa : public Output
    {
    private:
        vector<s16> outputBuffer;
        int outputBufferPos;
        std::thread* workerThread;
        volatile bool running;
        void worker();
        const OutputOptionsAlsa outputOptions;
        snd_pcm_t* playback_handle;
        
    public:
        OutputAlsa(const OutputOptionsAlsa& options, Player* p);
        ~OutputAlsa();
        
        void start() override;
        void stop() override;
    };
    
    
    

}

#endif /* OUTPUTALSA_HPP */


/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "OutputAlsa.hpp"
namespace vmp
{
    OutputOptionsAlsa::OutputOptionsAlsa()
        : outputBufferSize(1024)
        , device("default")
        , sampleRate(44100)
    {}
    
    OutputOptionsAlsa::~OutputOptionsAlsa()
    {}
    
    
    
    
    OutputAlsa::OutputAlsa(const OutputOptionsAlsa& options, Player* p)
        : outputOptions(options)
        , Output(p)
    {
        //player = p;
    }
    
    
    OutputAlsa::~OutputAlsa()
    {}
    
    void OutputAlsa::start()
    {
        int err;

        outputBuffer = vector<s16>(outputOptions.outputBufferSize);
        outputBufferPos = 0;

        snd_pcm_hw_params_t * hw_params;    

        if ((err = snd_pcm_open(&playback_handle, outputOptions.device.c_str(), SND_PCM_STREAM_PLAYBACK, 0)) < 0) {
            fprintf(stderr, "cannot open audio device %s (%s)\n",
                    outputOptions.device.c_str(),
                    snd_strerror(err));
            exit(1);
        }


        snd_pcm_hw_params_alloca(&hw_params);

        if ((err = snd_pcm_hw_params_any(playback_handle, hw_params)) < 0) {
            fprintf(stderr, "cannot initialize hardware parameter structure (%s)\n",
                    snd_strerror(err));
            exit(1);
        }

        if ((err = snd_pcm_hw_params_set_access(playback_handle, hw_params, SND_PCM_ACCESS_RW_INTERLEAVED)) < 0) {
            fprintf(stderr, "cannot set access type (%s)\n",
                    snd_strerror(err));
            exit(1);
        }

        if ((err = snd_pcm_hw_params_set_format(playback_handle, hw_params, SND_PCM_FORMAT_S16_LE)) < 0) {
            fprintf(stderr, "cannot set sample format (%s)\n",
                    snd_strerror(err));
            exit(1);
        }


        unsigned int rate = 44100;
        int dir = 0;
        unsigned int periods = 2;
        //snd_pcm_uframes_t periodsize = 8192;

        if ((err = snd_pcm_hw_params_set_rate_near(playback_handle, hw_params, &rate, 0)) < 0) {
            fprintf(stderr, "cannot set sample rate (%s)\n",
                    snd_strerror(err));
            exit(1);
        }
        //fprintf(stderr, "rate = %i\n", rate);

        if ((err = snd_pcm_hw_params_set_channels(playback_handle, hw_params, 2)) < 0) {
            fprintf(stderr, "cannot set channel count (%s)\n",
                    snd_strerror(err));
            exit(1);
        }


        if ((err = snd_pcm_hw_params_set_periods_near(playback_handle, hw_params, &periods, 0)) < 0) {
            fprintf(stderr, "Error setting periods (%s)\n",
                    snd_strerror(err));

            exit(1);
        }    
        //fprintf(stderr, "periods = %i\n", periods);

        if (snd_pcm_hw_params_set_buffer_size(playback_handle, hw_params, (outputOptions.outputBufferSize * periods) >> 2) < 0) {
          fprintf(stderr, "Error setting buffersize.\n");
          exit(1);
        }


        if ((err = snd_pcm_hw_params(playback_handle, hw_params)) < 0) {
            fprintf(stderr, "cannot set parameters (%s)\n",
                    snd_strerror(err));
            exit(1);
        }

        //snd_pcm_hw_params_free(hw_params);

        if ((err = snd_pcm_prepare(playback_handle)) < 0) {
            fprintf(stderr, "cannot prepare audio interface for use (%s)\n",
                    snd_strerror(err));
            exit(1);
        }        
        
        running = true;
        workerThread = new std::thread(&OutputAlsa::worker, this);
        
    }
    
    void OutputAlsa::stop()
    {
        running = false;
        workerThread->join();
        snd_pcm_close(playback_handle);
    }
    
    
    void OutputAlsa::worker() 
    {
        sample_t ls, rs;
        int err;
       
        while(running) {
            if (player->hasData()) {
                player->readPcm(ls, rs);
                outputBuffer[outputBufferPos++] = sample_to_s16(ls);
                outputBuffer[outputBufferPos++] = sample_to_s16(rs);
                if (outputBufferPos >= outputOptions.outputBufferSize) {
                    if ((err = snd_pcm_writei(playback_handle, outputBuffer.data(), outputOptions.outputBufferSize / 2)) != (outputOptions.outputBufferSize / 2)) {
                        fprintf (stderr, "write to audio interface failed (%s)\n", snd_strerror (err));
                            exit (1);
                        }
                        //snd_pcm_prepare(playback_handle);
                        outputBufferPos = 0;
                    }                
            }
        }
    }
    
    
}
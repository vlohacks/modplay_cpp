/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "OutputSoundio.hpp"
#include "Config.hpp"
#include "Player.hpp"
#include "Output.hpp"

namespace vmp 
{

    OutputSoundio::OutputSoundio(Player* p)
        : Output(p)
    {}
    
    OutputSoundio::~OutputSoundio()
    {}
    
    void OutputSoundio::soundioCallback(SoundIoOutStream* stream, int frame_count_min, int frame_count_max)
    {
        OutputSoundio* self = static_cast<OutputSoundio*>(stream->userdata);
        Player* player = self->getPlayer();
        
        const struct SoundIoChannelLayout *layout = &stream->layout;
        struct SoundIoChannelArea *areas;
        int frames_left = frame_count_max;
        sample_t s[2];
        int err;
        
        
        
        
        while (frames_left > 0) {
            int frame_count = frames_left;

            if ((err = soundio_outstream_begin_write(stream, &areas, &frame_count))) {
                fprintf(stderr, "%s\n", soundio_strerror(err));
                exit(1);
            }

            if (!frame_count)
                break;

            for (int frame = 0; frame < frame_count; frame += 1) {

                player->readPcm(s[0], s[1]);


                for (int channel = 0; channel < layout->channel_count; channel += 1) {
                    sample_t *ptr = (sample_t*)(areas[channel].ptr + areas[channel].step * frame);
                    *ptr = s[channel & 1];
                }
            }

            if ((err = soundio_outstream_end_write(stream))) {
                fprintf(stderr, "%s\n", soundio_strerror(err));
                exit(1);
            }

            frames_left -= frame_count;
        }

    }
    
    void OutputSoundio::start()
    {
        int err;
        int defaultDevice;
        
        soundio = soundio_create();
        if (soundio == 0) {
            fprintf(stderr, "could not create soundio stuct\n");
            exit(1);
        }
        
        if ((err = soundio_connect(soundio)) != 0) {
            fprintf(stderr, "error connecting: %s\n", soundio_strerror(err));
            exit(1);
        }
        
        soundio_flush_events(soundio);
        
        defaultDevice = soundio_default_output_device_index(soundio);
        if (defaultDevice < 0) {
            fprintf(stderr, "no output device found\n");
            exit(1);
        }
        
        device = soundio_get_output_device(soundio, defaultDevice);
        if (device == 0) {
            fprintf(stderr, "out of memory\n");
            exit(1);
        }
        
        outstream = soundio_outstream_create(device);
        if (outstream == 0) {
            fprintf(stderr, "out of memory\n");
            exit(1);
        }


#ifdef MIXING_FLOAT
#ifdef VMP_LITTLE_ENDIAN
        outstream->format = SoundIoFormatFloat32LE;
#else
        outstream->format = SoundIoFormatFloat32BE;
#endif
#endif

        
#ifdef MIXING_S16
#ifdef VMP_LITTLE_ENDIAN
        outstream->format = SoundIoFormatS16LE;
#else
        outstream->format = SoundIoFormatS16BE;
#endif
#endif
        
#ifdef MIXING_S8
        outstream->format = SoundIoFormatS8;
#endif
       
		outstream->
        
        outstream->userdata = this;
        outstream->write_callback = OutputSoundio::soundioCallback;
        outstream->sample_rate = player->getSampleRate();
        outstream->name = "ModPlayer";
        
        if ((err = soundio_outstream_open(outstream))) {
            fprintf(stderr, "unable to open device: %s", soundio_strerror(err));
            exit(1);
        }

        if (outstream->layout_error)
            fprintf(stderr, "unable to set channel layout: %s\n", soundio_strerror(outstream->layout_error));

        if ((err = soundio_outstream_start(outstream))) {
            fprintf(stderr, "unable to start device: %s\n", soundio_strerror(err));
            exit(1);
        }
        
        running = true;
    }
    
    void OutputSoundio::stop()
    {
        soundio_outstream_destroy(outstream);
        soundio_device_unref(device);
        soundio_destroy(soundio);      
    }
}

/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "OutputBenchmark.hpp"
#include "time.h"
#include "Player.hpp"

namespace vmp
{
    
    OutputBenchmark::OutputBenchmark(Player* p)
        : Output(p)
    {}
        
    OutputBenchmark::~OutputBenchmark()
    {}    
    
    void OutputBenchmark::worker()
    {
        sample_t mix_l, mix_r;
        time_t current_time, last_time;
        u32 sample_count;
        current_time = time(0);
        last_time = current_time;
        
        while (running) {
            current_time = time(0);
            player->readPcm(mix_l, mix_r);

            sample_count++;
            
            if (current_time != last_time) {
                printf("%lu samples/sec\n", sample_count);
                sample_count = 0;
                last_time = current_time;
            }
        }
    }
    
    void OutputBenchmark::start()
    {
        running = true;
        workerThread = new std::thread(&OutputBenchmark::worker, this);        
    }
    
    void OutputBenchmark::stop()
    {
        running = false;
        workerThread->join();
    }    
    
    
}
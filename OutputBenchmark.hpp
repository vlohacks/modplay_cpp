/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   OutputBenchmark.hpp
 * Author: vlo
 *
 * Created on 11. März 2017, 16:46
 */

#ifndef OUTPUTBENCHMARK_HPP
#define OUTPUTBENCHMARK_HPP

#include "Output.hpp"
#include "Types.hpp"
#include <thread>
#include <cstdio>

namespace vmp
{
    class OutputBenchmark : public Output
    {
    private:
        std::thread* workerThread;
        void worker();
        
    public:
        OutputBenchmark(Player* p);
        ~OutputBenchmark();
        
        void start() override;
        void stop() override;
    };
}


#endif /* OUTPUTBENCHMARK_HPP */


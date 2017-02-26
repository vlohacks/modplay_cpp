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


#include <math.h>

#include "OutputRaw.hpp"
#include "SampleConv.hpp"

namespace vmp
{
    
    void OutputRaw::worker() 
    {
        sample_t ls, rs;
        s16 s[2];
       
        while(running) {
            if (player->hasData()) {
                player->readPcm(ls, rs);
                s[0] = sample_to_s16(ls);
                s[1] = sample_to_s16(rs);
                fwrite(s, sizeof(s), 1, outputFile);
            }
        }
    }
   
    OutputRaw::OutputRaw(Player* p)
        : Output(p)
    {
        //player = p;
    }
    
    OutputRaw::~OutputRaw()
    {
    }
    
    void OutputRaw::setFilename(string fn)
    {
        filename = fn;
    }
    
    void OutputRaw::start()
    {
        if (filename == "-")
            outputFile = stdout;
        else
            outputFile = fopen(filename.c_str(), "wb");
        
        if (outputFile == 0)
            return;//throw new exception("error opening file");
        
        running = true;
        workerThread = new std::thread(&OutputRaw::worker, this);
    }
    
    void OutputRaw::stop()
    {
        running = false;
        workerThread->join();
    }
    
    
}
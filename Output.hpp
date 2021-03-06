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
 * File:   Output.hpp
 * Author: vlo
 *
 * Created on 7. April 2016, 01:01
 */

#ifndef OUTPUT_HPP
#define OUTPUT_HPP

#include "Player.hpp"
#include "SampleConv.hpp"

namespace vmp
{
    class Output
    {
    protected:
        Player* player;
        volatile bool running;
        
    public:
        Output();
        Output(Player* p);
        virtual ~Output() {};
                
        void setPlayer(Player* p);
        Player* getPlayer();
        virtual void start() = 0;
        virtual void stop() = 0;
        bool isRunning();
    };
    
    class OutputOptions
    {
    public:
        virtual ~OutputOptions() {};
    };
}

#endif /* OUTPUT_HPP */


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
 * File:   DefsMOD.hpp
 * Author: vlo
 *
 * Created on 12. März 2016, 21:11
 */

#ifndef DEFSMOD_HPP
#define DEFSMOD_HPP

#include "Types.hpp"

namespace vmp 
{
    class DefsMOD
    {
    public:
        
        static const int numPeriods = 60;
        static constexpr u16 periods[numPeriods] = {
            1712,1616,1525,1440,1357,1281,1209,1141,1077,1017, 961, 907,
             856, 808, 762, 720, 678, 640, 604, 570, 538, 508, 480, 453,
             428, 404, 381, 360, 339, 320, 302, 285, 269, 254, 240, 226,
             214, 202, 190, 180, 170, 160, 151, 143, 135, 127, 120, 113,
             107, 101,  95,  90,  85,  80,  76,  71,  67,  64,  60,  57
        };
        
        static constexpr u8 sineTable[32] = {
            0x00, 0x18, 0x31, 0x4A, 0x61, 0x78, 0x8D, 0xA1,
            0xB4, 0xC5, 0xD4, 0xE0, 0xEB, 0xF4, 0xFA, 0xFD,
            0xFF, 0xFD, 0xFA, 0xF4, 0xEB, 0xE0, 0xD4, 0xC5,
            0xB4, 0xA1, 0x8D, 0x78, 0x61, 0x4A, 0x31, 0x18
        };
        
        static constexpr float paulaFreq[2] = { 
            7093789.2,          // PAL
            7159090.5           // NTSC
        };
        
        typedef enum {
            PAL  = 0,
            NTSC = 1
        } paulafreq_index_t;
        
    };
}

#endif /* DEFSMOD_HPP */


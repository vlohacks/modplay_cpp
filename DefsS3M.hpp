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
 * File:   DefsS3M.hpp
 * Author: vlo
 *
 * Created on 12. März 2016, 21:11
 */

#ifndef DEFSS3M_HPP
#define DEFSS3M_HPP

#include "Types.hpp"

namespace vmp 
{
    class DefsS3M
    {
    public:
        
        static const int numPeriods = 108;
        static constexpr u16 periods[numPeriods] = {
            27392, 25856, 24384, 23040, 21696, 20480, 19328, 18240, 17216, 16256, 15360, 14496,
            13696, 12928, 12192, 11520, 10848, 10240,  9664,  9120,  8608,  8128,  7680,  7248,
             6848,  6464,  6096,  5760,  5424,  5120,  4832,  4560,  4304,  4064,  3840,  3624,
             3424,  3232,  3048,  2880,  2712,  2560,  2416,  2280,  2152,  2032,  1920,  1812,
             1712,  1616,  1524,  1440,  1356,  1280,  1208,  1140,  1076,  1016,   960,   906,
              856,   808,   762,   720,   678,   640,   604,   570,   538,   508,   480,   453,
              428,   404,   381,   360,   339,   320,   302,   285,   269,   254,   240,   226,
              214,   202,   190,   180,   170,   160,   151,   143,   135,   127,   120,   113,
              107,   101,    95,    90,    85,    80,    75,    71,    67,    63,    60,    56 
        };
    };
}

#endif /* DEFSS3M_HPP */


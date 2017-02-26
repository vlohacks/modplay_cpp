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
 * File:   ByteSex.hpp
 * Author: vlo
 * 
 * Utility class for swapping endianess of integers. Many formats 
 * use big endian byteorder - since the MOD format stems from Amiga which
 * is a big endian machine
 *
 * Created on 20. Februar 2016, 15:17
 */

#ifndef BYTESEX_HPP
#define BYTESEX_HPP

#include "Types.hpp"
#include "Config.hpp"

namespace vmp
{
    

    class ByteSex 
    {
    public:
        static u16 swapEndianU16(u16 val);
        static u32 swapEndianU32(u32 val);
        
        static u16 fromU16le(u16 val);
        static u16 fromU16be(u16 val);
        static u32 fromU32le(u32 val);
        static u32 fromU32be(u32 val);
        
        static u16 toU16le(u16 val);
        static u16 toU16be(u16 val);
        static u32 toU32le(u32 val);
        static u32 toU32be(u32 val);

        
    };
}

#endif /* BYTESEX_HPP */


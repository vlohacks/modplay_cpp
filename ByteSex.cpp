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

#include "ByteSex.hpp"

namespace vmp 
{
        u16 ByteSex::swapEndianU16(u16 val)
        {
             return ((val >> 8) | (val << 8));
        }
        
        u32 ByteSex::swapEndianU32(u32 val) 
        {
            return ((val>>24)&0xff) | ((val<<8)&0xff0000) | ((val>>8)&0xff00) | ((val<<24)&0xff000000);
        }



        /* convert endianess dependent on target machine architecture */
        u16 ByteSex::fromU16be(u16 val) 
        {
#ifdef VMP_LITTLE_ENDIAN
            return swapEndianU16(val);
#else
            return val;
#endif
        }

        u16 ByteSex::fromU16le(u16 val) 
        {
#ifdef VMP_LITTLE_ENDIAN
            return val;
#else
            return swapEndianU16(val);
#endif
        }
        
        
        u32 ByteSex::fromU32be(u32 val) 
        {
#ifdef VMP_LITTLE_ENDIAN
            return swapEndianU32(val);
#else
            return val;
#endif
        }

        u32 ByteSex::fromU32le(u32 val) 
        {
#ifdef VMP_LITTLE_ENDIAN
            return val;
#else
            return swapEndianU32(val);
#endif
        }


        
        
        
        
        
        u16 ByteSex::toU16be(u16 val) 
        {
#ifdef VMP_LITTLE_ENDIAN
            return swapEndianU16(val);
#else
            return val;
#endif
        }

        u16 ByteSex::toU16le(u16 val) 
        {
#ifdef VMP_LITTLE_ENDIAN
            return val;
#else
            return swapEndianU16(val);
#endif
        }
        
        
        u32 ByteSex::toU32be(u32 val) 
        {
#ifdef VMP_LITTLE_ENDIAN
            return swapEndianU32(val);
#else
            return val;
#endif
        }

        u32 ByteSex::toU32le(u32 val) 
        {
#ifdef VMP_LITTLE_ENDIAN
            return val;
#else
            return swapEndianU32(val);
#endif
        }



        
}
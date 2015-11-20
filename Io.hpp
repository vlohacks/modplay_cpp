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
 * File:   Io.hpp
 * Author: vlo
 * 
 * I/O abstraction layer
 * low level functions seek, read, write, tell and eof are implemented in 
 * subclass for specific access type (for example file or memory or obscure
 * something cloudy...
 * 
 * The class itself implements helper methods for easily reading / writing
 * basic types.
 *
 * Created on 8. September 2015, 19:52
 */

#ifndef IO_HPP
#define	IO_HPP

#include "Types.hpp"

namespace vmp 
{
    class Io
    {
    public:
        typedef enum {
            IO_SEEK_SET,
            IO_SEEK_CUR,
            IO_SEEK_END
        } whence_t;

        /* These functions are specific for the i/o type and implemented in
         * subclasses
         */
        virtual ~Io() {}
        virtual void seek(size_t n, whence_t whence) = 0;
        virtual void read(void * ptr, size_t size, size_t n) = 0;
        virtual void write(void * ptr, size_t size, size_t n) = 0;
        virtual size_t tell() = 0;
        virtual bool eof() = 0;
        
        /* These "high level" functions are implemented in the Io class itself
         */
        u8 readU8();
        u16 readU16();
        u32 readU32();

        s8 readS8();
        s16 readS16();
        s32 readS32();
        
        void writeU8(u8 val);
        void writeU16(u16 val);
        void writeU32(u32 val);

        void writeS8(s8 val);
        void writeS16(s16 val);
        void writeS32(s32 val);
        
    };
}

#endif	/* IO_HPP */


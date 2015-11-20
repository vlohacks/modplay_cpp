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

#include "Io.hpp"

namespace vmp 
{

    u8 Io::readU8() {
        u8 v;
        this->read(&v, sizeof(u8), 1);
        return v;
    }

    u16 Io::readU16() {
        u16 v;
        this->read(&v, sizeof(u16), 1);
        return v;
    }

    u32 Io::readU32() {
        u32 v;
        this->read(&v, sizeof(u32), 1);
        return v;
    }
    
    s8 Io::readS8() {
        s8 v;
        this->read(&v, sizeof(s8), 1);
        return v;
    }

    s16 Io::readS16() {
        s16 v;
        this->read(&v, sizeof(s16), 1);
        return v;
    }

    s32 Io::readS32() {
        s32 v;
        this->read(&v, sizeof(s32), 1);
        return v;
    }    
    
    void Io::writeU8(u8 val)    { this->write(&val, sizeof(u8), 1); }
    void Io::writeU16(u16 val)  { this->write(&val, sizeof(u16), 1); }
    void Io::writeU32(u32 val)  { this->write(&val, sizeof(u32), 1); }

    void Io::writeS8(s8 val)    { this->write(&val, sizeof(s8), 1); }
    void Io::writeS16(s16 val)  { this->write(&val, sizeof(s16), 1); }
    void Io::writeS32(s32 val)  { this->write(&val, sizeof(s32), 1); }
}

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
#include "ByteSex.hpp"

namespace vmp 
{

    /* universal (unsigned) implementation - sign should not matter for pure I/O
     * without arithmetic and I have koen bock, implementing the stuff for all
     * types
     */
    u8 Io::read8() {
        u8 v;
        this->read(&v, sizeof(u8), 1);
        return v;
    }

    u16 Io::read16le() {
        u16 v;
        this->read(&v, sizeof(u16), 1);
        return ByteSex::fromU16le(v);
    }

    u16 Io::read16be() {
        u16 v;
        this->read(&v, sizeof(u16), 1);
        return ByteSex::fromU16be(v);
    }

    
    u32 Io::read32le() {
        u32 v;
        this->read(&v, sizeof(u32), 1);
        return ByteSex::fromU32le(v);
    }

    u32 Io::read32be() {
        u32 v;
        this->read(&v, sizeof(u32), 1);
        return ByteSex::fromU32be(v);
    }

    void Io::write8(u8 val)    { this->write(&val, sizeof(u8), 1); }
    void Io::write16le(u16 val)  { this->write(&val, sizeof(u16), 1); }
    void Io::write16be(u16 val)  { this->write(&val, sizeof(u16), 1); }    
    void Io::write32le(u32 val)  { this->write(&val, sizeof(u32), 1); }
    void Io::write32be(u32 val)  { this->write(&val, sizeof(u32), 1); }

    
    
    /* typesafe interface */
    s8 Io::readS8() { return static_cast<s8>(read8()); }
    u8 Io::readU8() { return static_cast<u8>(read8()); }

    s16 Io::readS16le() { return static_cast<s16>(read16le()); }
    s16 Io::readS16be() { return static_cast<s16>(read16be()); }
    u16 Io::readU16le() { return static_cast<u16>(read16le()); }
    u16 Io::readU16be() { return static_cast<u16>(read16be()); }

    s32 Io::readS32le() { return static_cast<s32>(read32le()); }
    s32 Io::readS32be() { return static_cast<s32>(read32be()); }
    u32 Io::readU32le() { return static_cast<u32>(read32le()); }
    u32 Io::readU32be() { return static_cast<u32>(read32be()); }
    
	template<typename T>
	T Io::readT()
	{
		T tmp;
		this->read(static_cast<void*>(tmp), sizeof(T), 1);
		return tmp;
	}
	
	template<typename T>
	void Io::writeT(T val) 
	{
		this->write(static_cast<void*>(val), sizeof(val), 1);
	}

    void Io::writeS8(s8 val) { write8(static_cast<s8>(val)); }
    void Io::writeU8(u8 val) { write8(static_cast<u8>(val)); }
    
    void Io::writeS16le(s16 val) { write16le(static_cast<u16>(val)); }
    void Io::writeS16be(s16 val) { write16be(static_cast<u16>(val)); }
    void Io::writeU16le(u16 val) { write16le(static_cast<u16>(val)); }
    void Io::writeU16be(u16 val) { write16be(static_cast<u16>(val)); }
    
    void Io::writeS32le(s32 val) { write32le(static_cast<u32>(val)); }
    void Io::writeS32be(s32 val) { write32be(static_cast<u32>(val)); }
    void Io::writeU32le(u32 val) { write32le(static_cast<u32>(val)); }
    void Io::writeU32be(u32 val) { write32be(static_cast<u32>(val)); }
    
}

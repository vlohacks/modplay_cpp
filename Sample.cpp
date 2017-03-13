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

#include "Sample.hpp"
#include "ByteSex.hpp"

namespace vmp {
    Sample::Sample() 
        : length(0)
        , haveData(false)
    {}
    
    Sample::Sample(const void* src_data, sample_data_t src_format, u32 count) 
    {
        loadPtr(src_data, src_format, count);
    }
    
    Sample::Sample(Io& io, sample_data_t src_format, u32 count) 
    {
        loadIo(io, src_format, count);
    }
    
    Sample::~Sample() {}
    
    void Sample::loadIo(Io& io, const sample_data_t src_format, u32 count)
    {
        length = count;
        while(count--) {
            switch (src_format) {
                case PCM_S8:        data.push_back(sample_from_s8(io.readS8())); break;
                case PCM_U8:        data.push_back(sample_from_u8(io.readS8())); break;
                case PCM_S16_LE:    data.push_back(sample_from_s16(io.readS16le())); break;
                case PCM_S16_BE:    data.push_back(sample_from_s16(io.readS16be())); break;
                case PCM_U16_LE:    data.push_back(sample_from_u16(io.readU16le())); break;
                case PCM_U16_BE:    data.push_back(sample_from_u16(io.readU16be())); break;
            }
        }
        haveData = true;
    }
    
    void Sample::loadPtr(const void* src_data, const sample_data_t src_format, u32 count)
    {
        length = count;
        int i = 0;
        while(count--) {
            switch (src_format) {
                case PCM_S8:        data.push_back(sample_from_s8(static_cast<const s8*>(src_data)[i])); break;
                case PCM_U8:        data.push_back(sample_from_u8(static_cast<const u8*>(src_data)[i])); break;
                case PCM_S16_LE:    data.push_back(sample_from_s16(ByteSex::fromU16le(static_cast<const s16*>(src_data)[i]))); break;
                case PCM_S16_BE:    data.push_back(sample_from_s16(ByteSex::fromU16be(static_cast<const s16*>(src_data)[i]))); break;
                case PCM_U16_LE:    data.push_back(sample_from_u16(ByteSex::fromU16le(static_cast<const u16*>(src_data)[i]))); break;
                case PCM_U16_BE:    data.push_back(sample_from_u16(ByteSex::fromU16be(static_cast<const u16*>(src_data)[i]))); break;
            }
            i++;
        }
        haveData = true;
    }
    
    void Sample::loadIo(Io& io, const sample_data_t src_format) { loadIo(io, src_format, length); }
    void Sample::loadPtr(const void* src_data, const sample_data_t src_format) { loadPtr(src_data, src_format, length); }

    void Sample::setLoop(bool enabled, u32 start, u32 end) 
    {
        loopEnabled = enabled;
        loopStart = start;
        loopEnd = end;
    }
    
    
}
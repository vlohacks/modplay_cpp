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

/* 
 * File:   Sample.hpp
 * Author: vlo
 * 
 * Class for holding sample PCM data
 *
 * Created on 12. November 2015, 23:09
 */

#ifndef SAMPLE_HPP
#define SAMPLE_HPP

#include "Types.hpp"
#include "SampleConv.hpp"
#include "Io.hpp"

namespace vmp
{
    class Sample
    {
    private:
        u32     length;
        bool    loopEnabled;
        u32     loopStart;
        u32     loopEnd;
        u8      defaultVolume;
        u32     middleCSpeed;
        s8      finetune;
        string  name;
        bool    haveData;
        
        vector<sample_t>    data;
        
    public:
        typedef enum {
            PCM_U8,
            PCM_S8,
            PCM_U16_LE,
            PCM_S16_LE,
            PCM_U16_BE,
            PCM_S16_BE
        } sample_data_t;
        
        Sample();
        Sample(const void* src_data, sample_data_t src_format, u32 count);
        Sample(Io& io, sample_data_t src_format, u32 count);
        ~Sample();
        
        void    setName(string name);
        void    setLoop(bool enabled, u32 start, u32 end);
        void    setMiddleCSpeed(u32 val);
        void    setFinetune(s8 val);
        void    setLength(u32 val);
        void    setDefaultVolume(u8 val);
        

        void    loadPtr(const void* src_data, sample_data_t src_format, u32 count);
        void    loadIo (Io& io, sample_data_t src_format, u32 count);

        void    loadPtr(const void* src_data, sample_data_t src_format);
        void    loadIo (Io& io, sample_data_t src_format);

        
        bool    getLoopEnabled();
        u32     getLoopStart();
        u32     getLoopEnd();
        u32     getLoopLength();
        u32     getLength();
        u8      getDefaultVolume();
        u32     getMiddleCSpeed();
        s8      getFinetune();
        const string& getName();
        sample_t getData(u32 index);
        bool    hasData();
    };
}

#endif /* SAMPLE_HPP */


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

namespace vmp
{
    class Sample
    {
    private:
        u16     length;
        bool    loopEnabed;
        u16     loopStart;
        u16     loopEnd;
        u32     middleCSpeed;
        char*   name;
        
    public:
        Sample();
        ~Sample();
        
        void    setLoop(bool enabled, u16 start, u16 end);
        void    setName(char* name);
        
        void    loadPcmU8(u8* data);
        void    loadPcmS8(s8* data);
        void    loadPcmU16(u16* data);
        void    loadPcmS16(s16* data);
        
        u16     getLoopStart();
        u16     getLoopEnd();
        u32     getMiddleCSpeed();
        char*   getName();
    };
}

#endif /* SAMPLE_HPP */


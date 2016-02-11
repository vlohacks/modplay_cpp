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
 * File:   Module.hpp
 * Author: vlo
 * 
 * Represents a Module - ie the module data of an MOD / S3M / ... 
 *
 * Created on 8. September 2015, 17:44
 */

#ifndef MODULE_HPP
#define	MODULE_HPP

#include "Types.hpp"
#include "PatternData.hpp"

#include "Io.hpp"

namespace vmp 
{
    class Module 
    {
    private:
        u8 numTracks;
        u8 numPatterns;
        u8 numOrders;
        u8 numSamples;
        
        u8 initialSpeed;
        u8 initialBpm;
        
        pattern_t* patterns = 0;
        u8* orders = 0;
        char* songTitle = 0;
        u8* initialPanning = 0;
        
    protected:
        void allocData();
        
    public:
        typedef struct {
            PatternData* data;
        } pattern_row_t;
        
        typedef struct {
            u8 num_rows;
            pattern_row_t* rows;
        } pattern_t;
        
        Module();
        virtual ~Module() {};
        
        virtual bool loadCheck(Io* io) = 0;
        virtual void load(Io* io) = 0;
        
        pattern_t* getPattern(u8 pattern_no);
        PatternData* getData(u8 pattern_no, u8 row_no, u8 track_no);
    };
}

#endif	/* MODULE_HPP */


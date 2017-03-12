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
#include "Pattern.hpp"
#include "PatternData.hpp"
#include "Sample.hpp"

#include "Io.hpp"

namespace vmp 
{
    class Module 
    {
    protected:
        u8 numTracks;
        u8 numPatterns;
        u8 numOrders;
        u8 numSamples;
        u8 numInstruments;
        
        u8 initialSpeed;
        u8 initialBpm;
        
        string songTitle;
        vector<Pattern> patterns;
        vector<Sample> samples;
        vector<u8> orders;
        vector<u8> initialPanning;
        
   
    public:
        typedef enum  {
            MODULE_TYPE_MOD,
            MODULE_TYPE_S3M
        } module_type_t;
        
        Module(const module_type_t module_type);
        ~Module();
        
        virtual bool loadCheck(Io& io) = 0;
        virtual void load(Io& io) = 0;
        
        Pattern& getPattern(u8 pattern_no);
        PatternData& getData(u8 pattern_no, u8 row_no, u8 track_no);
        Sample& getSample(u8 sample_no);
        
        u8 getNumPatterns() const;
        u8 getNumOrders() const;
        u8 getNumTracks() const;
        u8 getNumSamples() const;
        u8 getPatternForOrder(u8 order_no) const;
        module_type_t getModuleType() const;
        
        u8 getInitialSpeed() const;
        u8 getInitialBpm() const;
        u8 getInitialPanning(u8 track_no) const;

    protected:
        const module_type_t moduleType;
        
    };
}

#endif	/* MODULE_HPP */


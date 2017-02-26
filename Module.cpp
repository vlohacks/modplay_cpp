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

#include "Module.hpp"
#include "IoFile.hpp"

namespace vmp 
{
    Module::Module() {}    
    Module::~Module() {}
      
    Pattern& Module::getPattern(u8 pattern_no) 
    {
        return patterns[pattern_no];
    }
    
    PatternData& Module::getData(u8 pattern_no, u8 row_no, u8 track_no)
    {
        return patterns[pattern_no].getRow(row_no)[track_no];
    }
    
    u8 Module::getNumPatterns() { return numPatterns; }
    u8 Module::getNumTracks()   { return numTracks; }
    u8 Module::getNumOrders()   { return numOrders; }
    u8 Module::getNumSamples()  { return numSamples; }
    Sample& Module::getSample(u8 sample_no) { return samples[sample_no]; }
    
    u8 Module::getPatternForOrder(u8 order_no)  
    {
        return orders[order_no];
    }
    
    Module::module_type_t Module::getModuleType() {
        return moduleType;
    }
    
    u8 Module::getInitialSpeed() { return initialSpeed; }
    u8 Module::getInitialBpm() { return initialBpm; }
    u8 Module::getInitialPanning(u8 track_no) { return initialPanning[track_no]; }
}
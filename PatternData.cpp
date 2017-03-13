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

#include "PatternData.hpp"


namespace vmp
{
    
    PatternData::PatternData()
        : flags(0)
    {}
    
    PatternData::~PatternData() {}
    
    void PatternData::setNote(u8 val) 
    { 
        note = val; 
        flags |= FLAG_NOTE;
    }
    
    void PatternData::setInstrument(u8 val) 
    { 
        instrument = val; 
        flags |= FLAG_INSTRUMENT;
    }
    
    void PatternData::setVolume(u8 val) 
    { 
        volume = val; 
        flags |= FLAG_VOLUME;
    }
    
    void PatternData::setEffectCmd(u8 val) 
    { 
        effectCmd = val; 
        flags |= FLAG_EFFECT_CMD;
    }
    
    void PatternData::setEffectValue(u8 val) 
    { 
        effectValue = val;
        flags |= FLAG_EFFECT_VALUE;
    }
    
    void PatternData::clearNote()           { flags &= ~FLAG_NOTE; }
    void PatternData::clearInstrument()     { flags &= ~FLAG_INSTRUMENT; }
    void PatternData::clearVolume()         { flags &= ~FLAG_VOLUME; }
    void PatternData::clearEffectCmd()      { flags &= ~FLAG_EFFECT_CMD; }
    void PatternData::clearEffectValue()    { flags &= ~FLAG_EFFECT_VALUE; }
}
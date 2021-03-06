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
 * File:   PatternData.hpp
 * Author: vlo
 *
 * Well, this looks bloated but it's 
 * 
 * Created on 12. November 2015, 20:51
 */

#ifndef PATTERNDATA_HPP
#define PATTERNDATA_HPP

#include "Types.hpp"

namespace vmp 
{
    class PatternData
    {
    private:
        u8 flags;
        u8 note;
        u8 instrument;
        u8 volume;
        u8 effectCmd;
        u8 effectValue;
        
        const u8 FLAG_NOTE          = (1 << 0);
        const u8 FLAG_INSTRUMENT    = (1 << 1);
        const u8 FLAG_VOLUME        = (1 << 2);
        const u8 FLAG_EFFECT_CMD    = (1 << 3);
        const u8 FLAG_EFFECT_VALUE  = (1 << 4);
        
    public:
        PatternData();
        ~PatternData();
        
        bool    hasNote()               { return flags & FLAG_NOTE; }
        bool    hasInstrument()         { return flags & FLAG_INSTRUMENT; }
        bool    hasVolume()             { return flags & FLAG_VOLUME; }
        bool    hasEffectCmd()          { return flags & FLAG_EFFECT_CMD; }
        bool    hasEffectValue()        { return flags & FLAG_EFFECT_VALUE; }

        u8      getNote()               { return note; }
        u8      getInstrument()         { return instrument; }
        u8      getVolume()             { return volume; }
        u8      getEffectCmd()          { return effectCmd; }
        u8      getEffectValue()        { return effectValue; }
        u8      getEffectValueLower()   { return (effectValue & 0x0f); }
        u8      getEffectValueUpper()   { return (effectValue >> 4); }
        
        void    setNote(u8 val);
        void    setInstrument(u8 val);
        void    setVolume(u8 val);
        void    setEffectCmd(u8 val);
        void    setEffectValue(u8 val);
        
        void    clearNote();
        void    clearInstrument();
        void    clearVolume();
        void    clearEffectCmd();
        void    clearEffectValue();
        void    clearAll();
    };
}

#endif /* PATTERNDATA_HPP */


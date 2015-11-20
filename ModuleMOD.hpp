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
 * File:   ModuleMOD.hpp
 * Author: vlo
 * 
 * MOD file (*.mod)
 *
 * Created on 12. November 2015, 23:20
 */

#ifndef MODULEMOD_HPP
#define MODULEMOD_HPP

#include "Module.hpp"

namespace vmp 
{
    class ModuleMOD : public Module
    {
    private:
        
        typedef struct {
            u32 signature;
            u8 numTracks;
            char * description;
        } modtype_t;
        
        const int numModTypes = 8;
        const modtype_t modTypes[] = {
            { 0x2e4b2e4d, 4, "Protracker module" },                                // M.K.
            { 0x214b214d, 4, "Protracker module with more than 64 patterns" },     // M!K!
            { 0x34544c46, 4, "Startrekker, 4 channels" },                          // FLT4
            { 0x38544c46, 8, "Startrekker, 8 channels" },                          // FLT8
            { 0x4e484332, 2, "Extended protracker, 2 channels" },                  // 2CHN
            { 0x4e484334, 4, "Extended protracker, 4 channels" },                  // 4CHN
            { 0x4e484336, 6, "Extended protracker, 6 channels" },                  // 6CHN
            { 0x4e484338, 8, "Extended protracker, 8 channels" }                   // 8CHN
        };        
        
        
    public:
        ModuleMOD();
        ~ModuleMOD();
        
        bool loadCheck(Io * io);
        void load(Io * io);       
    };
}

#endif /* MODULEMOD_HPP */


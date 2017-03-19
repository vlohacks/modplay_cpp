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

#include "ModuleUtils.hpp"
#include "Pattern.hpp"
#include "PatternData.hpp"

namespace vmp 
{
    void ModuleUtils::dumpModule(Module& mod) {
        int i, j, k;
        for (i = 0; i < mod.getNumPatterns(); i++) {
            Pattern& p = mod.getPattern(i);
            fprintf(stderr, "=== Pattern %i ===\n", i);
            for (j = 0; j < p.getNumRows(); j++) {
                fprintf(stderr, "%02x|", j);
                for (k = 0; k < mod.getNumTracks(); k++) {
                    PatternData& d = p.getRow(j)[k];
                    ModuleUtils::dumpData(d);
                }
                fprintf(stderr, "\n");
            }
        }
    }
    
    void ModuleUtils::dumpData(PatternData& d)
    {
                    if (d.hasNote())
                        fprintf(stderr, "%03x ", d.getNote());
                    else
                        fprintf(stderr, "... ");
                                
                    if (d.hasInstrument())
                        fprintf(stderr, "%02d ", d.getInstrument());
                    else
                        fprintf(stderr, ".. ");
                    
                    if (d.hasVolume())
                        fprintf(stderr, "%02d ", d.getVolume());
                    else
                        fprintf(stderr, ".. ");
                    
                    if (d.hasEffectCmd())
                        fprintf(stderr, "%02x", d.getEffectCmd());
                    else
                        fprintf(stderr, "..");
                    
                    if (d.hasEffectCmd())
                        fprintf(stderr, "%02x|", d.getEffectValue());
                    else
                        fprintf(stderr, "..|");
    }
        
}
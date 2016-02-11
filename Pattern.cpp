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

#include "Pattern.hpp"
#include "PatternRow.hpp"

namespace vmp
{
    Pattern::Pattern(const u8 num_rows, const u8 num_tracks)
    {
        int i;
        numRows = num_rows;
        rows = new PatternRow[numRows];
        for (i = 0; i < numRows; i++)
            rows[i].init(num_tracks);
    }
    
    Pattern::~Pattern()
    {
        delete[] rows;
    }
    
    PatternRow& Pattern::getRow(const u8 row_no)
    {
        return rows[row_no];
    }
    
    u8 Pattern::getNumRows()
    {
        return numRows;
    }
}
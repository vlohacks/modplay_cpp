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
#include "PatternRow.hpp"

namespace vmp {
    PatternRow::PatternRow(const u8 num_tracks)
    {
        init(num_tracks);
    }
    
    PatternRow::PatternRow() {}

    void PatternRow::init(const u8 num_tracks)
    {
        data = new PatternData[num_tracks];
    }

    
    PatternRow::~PatternRow()
    {
        delete[] data;
    }
    
    PatternData& PatternRow::getData(const u8 track_no)
    {
        return data[track_no];
    }
}

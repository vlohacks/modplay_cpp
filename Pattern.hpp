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
 * File:   Pattern.hpp
 * Author: vlo
 *
 * Created on 8. Februar 2016, 19:59
 */

#ifndef PATTERN_HPP
#define PATTERN_HPP

#include "PatternData.hpp"
#include "Types.hpp"

namespace vmp 
{
    class Pattern 
    {
    private:
        u8 numRows;
        //PatternRow* rows;
        vector<vector<PatternData>> patternData;
        
    public:
        Pattern(const u8 num_rows, const u8 num_tracks);
        Pattern();
        ~Pattern();
        
        std::vector<PatternData>&   getRow(const u8 row_no) { return patternData[row_no]; }
        u8                          getNumRows()            { return patternData.size(); }        
        
    };
}

#endif /* PATTERN_HPP */


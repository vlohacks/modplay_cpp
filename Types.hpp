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
 * File:   Types.hpp
 * Author: vlo
 * 
 * Basic type declarations
 *
 * Created on 12. November 2015, 20:29
 */

#ifndef TYPES_HPP
#define TYPES_HPP

#include <cstdint>
#include <vector>
#include <string>
#include "Config.hpp"

namespace vmp {
    
    /* integer scalars */
    typedef std::size_t size_t;
    typedef uint32_t    u32;
    typedef uint16_t    u16;
    typedef uint8_t     u8;
    typedef int32_t     s32;
    typedef int16_t     s16;
    typedef int8_t      s8;
    
    /* containers */
    template<typename T> using vector = std::vector<T>;
    //template using string = std::string;
    typedef std::string string;
    
#ifdef PRECISION_FLOAT
    typedef float       precision_t;
#endif
    
}


#endif /* TYPES_HPP */


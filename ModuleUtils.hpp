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
 * File:   ModuleUtils.hpp
 * Author: vlo
 *
 * Created on 6. April 2016, 23:58
 */

#ifndef MODULEUTILS_HPP
#define MODULEUTILS_HPP

#include "Module.hpp"

namespace vmp
{
    class ModuleUtils
    {
    public:
        static void dumpModule(Module& mod);
        static void dumpData(PatternData& d);
    };
}

#endif /* MODULEUTILS_HPP */


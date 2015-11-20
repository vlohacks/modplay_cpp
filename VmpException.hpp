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
 * File:   VMPException.hpp
 * Author: vlo
 *
 * Created on 12. November 2015, 18:51
 */

#ifndef VMPEXCEPTION_HPP
#define VMPEXCEPTION_HPP

#include <exception>

namespace vmp 
{
    class VmpException : public std::exception
    {
        virtual const char* what() const throw() {
            return "fuck";
        }
    };
}

#endif /* VMPEXCEPTION_HPP */


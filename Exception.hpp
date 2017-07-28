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
 * File:   Exception.hpp
 * Author: vlo
 *
 * Created on 12. November 2015, 18:51
 */

#ifndef EXCEPTION_HPP
#define EXCEPTION_HPP

namespace vmp 
{
    class Exception 
    {
        virtual const char* what() const = 0;
        virtual void outputSummary() const = 0;
    };
    /*
    class IoException 
    {
        IoException(const char* zeuch) : filename(zeuch) {};
        const char* filename;
        virtual const char* what() const throw() {
            return filename;
        }
    };
    */
    
}

#endif /* EXCEPTION_HPP */


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
 * File:   IoFile.hpp
 * Author: vlo
 * 
 * file input/output 
 *
 * Created on 12. November 2015, 18:44
 */

#ifndef IOFILE_HPP
#define IOFILE_HPP

#include "Io.hpp"
#include <cstdio>

namespace vmp 
{
    class IoFile : public Io
    {
    private:
        FILE* f;
        
    public:
        IoFile(const char* filename, const char* mode);
        ~IoFile();
        
        void seek(size_t n, whence_t whence) override;
        void read(void* ptr, size_t size, size_t n) override;
        void write(void* ptr, size_t size, size_t n) override;
        size_t tell() override;
        bool eof() override;             
    };
    
}

#endif /* IOFILE_HPP */


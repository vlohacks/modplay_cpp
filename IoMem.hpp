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
 * File:   IoMem.hpp
 * Author: vlo
 * 
 * Memory input/output 
 *
 * Created on 12. November 2015, 18:44
 */

#ifndef IOMEM_HPP
#define IOMEM_HPP

#include "Io.hpp"

namespace vmp 
{
    class IoMem : public Io
    {
    private:
        void* memPtr;
        size_t memSize;
        size_t position;
        
    public:
        
        IoMem(void* ptr, size_t size);
        ~IoMem();
        
        void seek(size_t n, whence_t whence) override;
        int read(void* ptr, size_t size, size_t n) override;
        int write(void* ptr, size_t size, size_t n) override;
        size_t tell() override;
        bool eof() override;
    };
}

#endif /* IO_MEM_HPP */


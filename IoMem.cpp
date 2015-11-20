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
#include "IoMem.hpp"
#include "VmpException.hpp";

namespace vmp
{
    IoMem::IoMem(void* ptr, size_t size) 
    {
        memPtr = ptr;
        memSize = size;
        position = 0;
    }
    
    IoFile::~IoFile() 
    {
    }
    
    void IoMem::read(void* ptr, size_t size, size_t n)
    {
        int i;
        for (i = 0; i < n; i++) {
            if (position + size < memSize) {
                memcpy(ptr, (void *)(memPtr + position), size);
                position += size;
                ptr += size;
            } else {
                throw VmpException();
            }
        }
        return i;
    }

    void IoMem::write(const void * ptr, size_t size, size_t n)
    {
        int i;
        for (i = 0; i < n; i++) {
            if (position + size < memSize) {
                memcpy((void *)(memPtr + position), ptr, size);
                position += size;
                ptr += size;
            } else {
                throw VmpException();
            }
        }
        return i;
    }

    size_t IoMem::tell()
    {
        return position;
    }

    void IoMem::seek(size_t n, whence_t whence)
    {
        switch (whence) {
            case IO_SEEK_CUR: 
                position += n;
                break;

            case IO_SEEK_SET:
                position = n;
                break;

            case IO_SEEK_END: 
                position = memSize - n;
                break;
        }

        if (position > memSize)
            position = memSize;

    }

    bool IoMem::eof() 
    {
        return (position >= memSize);
    }   
}
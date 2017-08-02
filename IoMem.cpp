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
#include "Exception.hpp"
#include <cstring>

namespace vmp
{
    IoMem::IoMem(void* ptr, size_t size) 
		: memPtr(ptr)
		, memSize(size)
		, position(0)
    {}
    
    IoMem::~IoMem() 
    {}
    
    void IoMem::read(void* sptr, size_t size, size_t n)
    {
        int i;
        char* ptr = static_cast<char*>(sptr);
        for (i = 0; i < n; i++) {
            if (position + size <= memSize) {
                memcpy(ptr, static_cast<void*>(static_cast<char*>(memPtr) + position), size);
                position += size;
                ptr += size;
            } else {
                throw IoException("IO: read beyond bounds", 0, memPtr);
            }
        }
    }

    void IoMem::write(void* sptr, size_t size, size_t n)
    {
        int i;
        char* ptr = static_cast<char*>(sptr);
        for (i = 0; i < n; i++) {
            if (position + size <= memSize) {
                memcpy(static_cast<void*>(static_cast<char*>(memPtr) + position), ptr, size);
                position += size;
                ptr += size;
            } else {
                throw IoException("IO: write beyond bounds", 0, memPtr);
            }
        }
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
            throw IoException("IO: seek beyond bounds", 0, memPtr);

    }

    bool IoMem::eof() 
    {
        return (position >= memSize);
    }   
}

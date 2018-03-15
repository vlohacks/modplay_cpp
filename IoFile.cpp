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

#include "IoFile.hpp"
#include "Exception.hpp"

namespace vmp 
{
    IoFile::IoFile(const char* filename, const char* mode) 
    {
        f = fopen(filename, mode);
        if (f == 0) {
            throw Exception("Error opening file");
        }
    }
    
    IoFile::~IoFile() 
    {
        if (f) {
            fclose(f);
        }
    }
    
    void IoFile::seek(std::size_t n, whence_t whence)
    {
        switch (whence) {
            case IO_SEEK_CUR: fseek(f, n, SEEK_CUR); break;
            case IO_SEEK_SET: fseek(f, n, SEEK_SET); break;
            case IO_SEEK_END: fseek(f, n, SEEK_END); break;
        }
    }
    
    int IoFile::read(void* ptr, std::size_t size, std::size_t n)
    {
        return fread(ptr, size, n, f);
    }
    
    int IoFile::write(void* ptr, size_t size, size_t n)
    {
        return fwrite(ptr, size, n, f);
    }
    
    size_t IoFile::tell() {
        return ftell(f);
    }
    
    bool IoFile::eof() {
        return (feof(f) != 0);
    }
    
}

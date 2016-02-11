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

#include "Io.hpp"
#include "IoFile.hpp"

#include "Pattern.hpp"

int main(int argc, char** argv) 
{

	vmp::Io* f = new vmp::IoFile("test22", "rb");

	for (int i = 0; i < 10; i++) {
		printf("%x\n", f->readU8());
	}   
        
        vmp::Pattern p(64, 4);
        
        p.getRow(0).getData(0).setVolume(32);
        p.getRow(0).getData(0).setEffectCmd(0x01);
        
        for (int i = 0; i < p.getNumRows(); i++) {
            if (p.getRow(i).getData(0).hasVolume())
                printf ("%i vol=%i\n", i, p.getRow(i).getData(0).getVolume());
            
            if (p.getRow(i).getData(0).hasVolume())
                printf ("%i fx=%i\n", i, p.getRow(i).getData(0).getEffectCmd());        
        }
        
        delete f;
        
	return 0;
}

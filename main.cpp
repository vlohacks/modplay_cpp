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

#include <thread>
#include <chrono>

#include "Pattern.hpp"
#include "ModuleFactory.hpp"
#include "ModuleUtils.hpp"
#include "Player.hpp"
#include "OutputRaw.hpp"
#include "OutputAlsa.hpp"
#include "OutputBenchmark.hpp"
#include "OutputSoundio.hpp"
#include "IoMem.hpp"
#include <iostream>
#include "dreamit.h"

int main(int argc, char** argv) 
{
    
    //vmp::ModuleS3M mod;
    
    vmp::Module* mod;
    
    try {
        //vmp::IoMem f(dreamit, sizeof(dreamit));
		vmp::IoFile f(argv[1], "rb");
        f.seek(0, vmp::Io::IO_SEEK_SET);
        mod = vmp::ModuleFactory::byHeader(f);
    } catch (vmp::IoException e) {
        return 1;
    } 
    
	vmp::Player player(44100);
	player.setModule(mod);
        
	vmp::OutputOptionsAlsa oo;
        
        
	//vmp::OutputRaw o(&player);
	//vmp::OutputAlsa o(oo, &player);
	vmp::OutputSoundio o(&player);
	//vmp::OutputBenchmark o(&player);
        
	player.setLoop(true);
	
	o.start();
	
	while (o.isRunning()) {
		sleep(0);
	}

	int r = 0;
	//int r =  app.exec();
        //o.setFilename("-");
	if (o.isRunning())
		o.stop();
        
	return r;
}



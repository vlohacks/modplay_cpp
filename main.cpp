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
#include "ModuleMOD.hpp"
#include "ModuleUtils.hpp"
#include "Player.hpp"
#include "OutputRaw.hpp"
#include "OutputAlsa.hpp"
#include "IoMem.hpp"
#include <iostream>

#include <QApplication>
#include "dreamit.h"
#include "Dos.hpp"
#include "OutputBenchmark.hpp"

int main(int argc, char** argv) 
{
        //QApplication app(argc, argv);
	vmp::IoFile f(argv[1], "rb");
        //vmp::IoMem f(dreamit, sizeof(dreamit));
      
        f.seek(0, vmp::Io::IO_SEEK_SET);
        
        vmp::ModuleMOD mod;
        mod.load(f);
        //vmp::ModuleUtils::dumpModule(mod);
        
        for (int i = 0; i < mod.getNumSamples(); i++) {
            vmp::Sample& s = mod.getSample(i);
            printf("%-32s l: %06d ll: %06d, ls: %06d le: %06d\n", s.getName().c_str(), s.getLength(), s.getLoopLength(), s.getLoopStart(), s.getLoopEnd());
        }
        
        vmp::Player player(44100);
        player.setModule(&mod);
        
        vmp::OutputOptionsAlsa oo;
        
        //vmp::OutputRaw o(&player);
        vmp::OutputAlsa o(oo, &player);
        //vmp::OutputBenchmark o(&player);
        
        
        player.setLoop(true);
        
        o.start();
        
        /*
        vmp::Dos window;
        window.setOutput(&o);
        window.setWindowTitle("DREAMIT Operation System PRO");
        window.resize(640, 64);
        
	window.show();
	*/
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



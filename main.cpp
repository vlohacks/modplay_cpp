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
#include <iostream>

int main(int argc, char** argv) 
{

	vmp::IoFile f(argv[1], "rb");
      
        f.seek(0, vmp::Io::IO_SEEK_SET);
        
        vmp::ModuleMOD mod;
        mod.load(f);
        //vmp::ModuleUtils::dumpModule(mod);
        
        vmp::Player player(44100);
        player.setModule(&mod);
        
        vmp::OutputOptionsAlsa oo;
        
        //vmp::OutputRaw o(&player);
        vmp::OutputAlsa o(oo, &player);
        
        for (int i = 0; i < mod.getNumSamples(); i++)
            printf("%d FT: %d\n", i, mod.getSample(i).getFinetune());
        
        //o.setFilename("-");
        o.start();
        while (player.hasData()) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
        o.stop();
        
	return 0;
}

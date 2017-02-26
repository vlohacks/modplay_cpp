/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
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
#include "Effects.hpp"

namespace vmp 
{
    
    void Effects::doEffect(int effect_num, Player& player, Track& track)
    {
        effectMap[effect_num](player, track);
    }
    
    
    void Effects::unimplementedEffect(Player& player, Track& track)
    {
        // todo... cry
        fprintf(stderr, "FUCKYOU!! UNIMPLEMENTED EFFECT!!!!! %01x\n",track.getData()->getEffectCmd());
    }
}
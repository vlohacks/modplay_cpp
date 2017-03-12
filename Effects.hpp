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

/* 
 * File:   Effects.hpp
 * Author: vlo
 *
 * Created on 14. Juli 2016, 21:29
 */

#ifndef EFFECTS_HPP
#define EFFECTS_HPP

#include "Types.hpp"
#include "Track.hpp"
#include "Module.hpp"

namespace vmp
{
    class Player;
    class Effects 
    {
    protected:
        typedef void (*effect_t)(Player&, Track&);
        const vector<effect_t> effectMap;
        
        static void unimplementedEffect(Player&, Track&);
        
        
    public:
        Effects(vector<effect_t> map);
        void doEffect(int effect_num, Player& player, Track& track);
        virtual void newRowAction(Player& player, Track& track) = 0;
    };
}

#endif /* EFFECTS_HPP */


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
 * File:   Config.hpp
 * Author: vlo
 *
 * Created on 19. November 2015, 00:08
 */

#ifndef CONFIG_HPP
#define CONFIG_HPP


/* this defines what resolution internally is used for sample / mixing ops 
 * Possible options:
 * MIXING_FLOAT: for systems capable of doing fast floating point math
 * MIXING_S16: uses 16 bit signed int for sampling and 32 bit signed int for
 *      multiply/accumulate operations
 * MIXING_S8: uses 8 bit signed int for sampling and 16 bit signed int for
 *      multiply/accumulate operations
 */
#define MIXING_S16

/* this defines how inter-sample calculations are done internally
 * (TODO not yet implemented)
 * SAMPLING_FLOAT: uses float
 * SAMPLING_U24_8: uses 24:8 bit fixed point math (for systems which love 
 *      integer, but hate float
 */
#define SAMPLING_FLOAT


#endif /* CONFIG_HPP */


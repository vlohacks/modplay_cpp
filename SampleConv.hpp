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


#ifndef SAMPLECONV_HPP
#define	SAMPLECONV_HPP

#include "Config.hpp"
#include "Types.hpp"

namespace vmp 
{

#ifdef MIXING_FLOAT
typedef float sample_t;
typedef float sample_mac_t;
static const sample_t SAMPLE_T_MAX = 1.0f;
static const sample_t SAMPLE_T_MIN = -1.0f;
static const sample_t SAMPLE_T_ZERO = 0.0f;

static inline sample_t sample_from_float(float s)   { return static_cast<sample_t>(s); }
static inline sample_t sample_from_double(double s) { return static_cast<sample_t>(s); }
static inline sample_t sample_from_s16(s16 s)       { return static_cast<sample_t>(s) / 32768.0f; }
static inline sample_t sample_from_s8(s8 s)         { return static_cast<sample_t>(s) / 128.0f; }
static inline sample_t sample_from_u16(u16 s)       { return static_cast<sample_t>(s ^ 32768) / 32768.0f; }
static inline sample_t sample_from_u8(u8 s)         { return static_cast<sample_t>(s ^ 128) / 128.0f; }


static inline s16 sample_to_s16(sample_t s)         { return static_cast<s16>(s * 32768.0f); }
static inline s8 sample_to_s8(sample_t s)           { return static_cast<s8>(s * 128.0f); }
#endif /* MIXING_FLOAT */


#ifdef MIXING_S16
typedef s16 sample_t;
typedef s32 sample_mac_t;

static const sample_t SAMPLE_T_MAX = 32767;
static const sample_t SAMPLE_T_MIN = -32768;
static const sample_t SAMPLE_T_ZERO = 0;

static inline sample_t sample_from_float(float s)   { return static_cast<sample_t>(s * 32768); }
static inline sample_t sample_from_double(double s) { return static_cast<sample_t>(s * 32768); }
static inline sample_t sample_from_s16(s16 s)       { return static_cast<sample_t>(s); }
static inline sample_t sample_from_s8(s8 s)         { return static_cast<sample_t>(s) << 8; }
static inline sample_t sample_from_u16(u16 s)       { return static_cast<sample_t>(s ^ 32768); }
static inline sample_t sample_from_u8(u8 s)         { return static_cast<sample_t>(s ^ 128) << 8; }


static inline s16 sample_to_s16(sample_t s)         { return s; }
static inline s8 sample_to_s8(sample_t s)           { return static_cast<s8>(s >> 8); }
#endif /* MIXING_S16 */

#ifdef MIXING_S8
typedef s8 sample_t;
typedef s16 sample_mac_t;
static const sample_t SAMPLE_T_MAX = 127;
static const sample_t SAMPLE_T_MIN = -128;
static const sample_t SAMPLE_T_ZERO = 0;

static inline sample_t sample_from_float(float s)   { return static_cast<sample_t>(s * 128); }
static inline sample_t sample_from_double(double s) { return static_cast<sample_t>(s * 128); }
static inline sample_t sample_from_s16(s16 s)       { return static_cast<sample_t>(s >> 8); }
static inline sample_t sample_from_s8(s8 s)         { return static_cast<sample_t>(s); }
static inline sample_t sample_from_u16(s16 s)       { return static_cast<sample_t>((s ^ 32758) >> 8); }
static inline sample_t sample_from_u8(s8 s)         { return static_cast<sample_t>(s ^ 128); }

static inline s16 sample_to_s16(sample_t s)         { return (static_cast<s16>(s) << 8); }
static inline s8 sample_to_s8(sample_t s)           { return static_cast<s8>(s); }

#endif /* MIXING_S8 */
}

#endif	/* MIXING_HPP */


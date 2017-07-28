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
 * File:   Module.hpp
 * Author: vlo
 * 
 * Represents a Module - ie the module data of an MOD / S3M / ... 
 *
 * Created on 8. September 2015, 17:44
 */

#ifndef MODULE_HPP
#define	MODULE_HPP

#include "Types.hpp"
#include "Pattern.hpp"
#include "PatternData.hpp"
#include "Sample.hpp"
#include "Io.hpp"
#include "Exception.hpp"

namespace vmp 
{
    class Module 
    {
    protected:
        u8 numTracks;
        u8 numPatterns;
        u8 numOrders;
        u8 numSamples;
        u8 numInstruments;
        
        u8 initialSpeed;
        u8 initialBpm;
        
        u32 flags;
        
        string songTitle;
        vector<Pattern> patterns;
        vector<Sample> samples;
        vector<u8> orders;
        vector<u8> initialPanning;
        
        static const int numModuleTypes = 2;
        
   
    public:
        typedef enum  {
            MODULE_TYPE_MOD,
            MODULE_TYPE_S3M
        } module_type_t;
        
        typedef struct {
            const char*   defaultFileExt;
            const char*   description;
        } module_type_details_t;
        
        static constexpr module_type_details_t moduleTypeDetails[numModuleTypes] = {
            { "mod", "Protracker / FastTracker Module" },
            { "s3m", "Scream Tracker Module" }
        };
        
        Module(const module_type_t module_type);
        ~Module();
        
        virtual bool loadCheck(Io& io) = 0;
        virtual void load(Io& io) = 0;
        
        Pattern&        getPattern(u8 pattern_no)                       { return patterns[pattern_no]; }
        PatternData&    getData(u8 pattern_no, u8 row_no, u8 track_no)  { return patterns[pattern_no].getRow(row_no)[track_no];  }
        u8              getNumPatterns() const                          { return numPatterns; }
        u8              getNumTracks() const                            { return numTracks; }
        u8              getNumOrders() const                            { return numOrders; }
        u8              getNumSamples() const                           { return numSamples; }
        Sample&         getSample(u8 sample_no)                         { return samples[sample_no]; }
        u8              getPatternForOrder(u8 order_no) const           { return orders[order_no]; }
        module_type_t   getModuleType() const                           { return moduleType; }
        u8              getInitialSpeed() const                         { return initialSpeed; }
        u8              getInitialBpm() const                           { return initialBpm; }
        u8              getInitialPanning(u8 track_no) const            { return initialPanning[track_no]; }
        
        bool            getFlag(const u32 flag) const                   { return flags & flag; }
        void            setFlag(const u32 flag)                         { flags |= flag; }
        void            clearFlag(const u32 flag)                       { flags &= ~flag; }
        
        class ModuleFormatException : public Exception 
        {
        private:
            const module_type_t moduleType;


        public:
            ModuleFormatException(const module_type_t type)
                : moduleType(type)
            {}

            const char* what() const override {
                return "Invalid module format";
            }

            void outputSummary() const override
            {
                printf("Data is not a valid %s (.%s)\n", moduleTypeDetails[moduleType].description, moduleTypeDetails[moduleType].defaultFileExt);
            }

        };        
        
    
    protected:
        const module_type_t moduleType;
        
    };
    

    
}

#endif	/* MODULE_HPP */


/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ModuleS3M.hpp
 * Author: vlo
 *
 * Created on 14. März 2017, 19:37
 */

#ifndef MODULES3M_HPP
#define MODULES3M_HPP

#include "Module.hpp"

namespace vmp
{
    class ModuleS3M : public Module
    {
    private:
        u16 version;
        u8  initialMasterVolume;
        const u32       s3mSignature = 0x4d524353;    // "SCRM"
        const size_t    s3mSignatureOffset = 0x2c;
    public:
        
        ModuleS3M();
        
        bool loadCheck(Io& io);
        void load(Io& io);
        
        enum S3MFlags {
            ST2VIBRATO         = (1<<0),
            ST2TEMPO           = (1<<1),
            AMIGASLIDES        = (1<<2),
            AMIGALIMITS        = (1<<4),
            ST30VOLUMESLIDES   = (1<<6),
            MONO               = (1<<7)
        };
    };
}

#endif /* MODULES3M_HPP */


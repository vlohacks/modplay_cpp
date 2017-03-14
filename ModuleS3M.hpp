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
        u32 flags;
        
    public:
        
        ModuleS3M();
        
        bool loadCheck(Io& io);
        void load(Io& io);   
    };
}

#endif /* MODULES3M_HPP */


/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Dos.hpp
 * Author: vlo
 *
 * Created on 26. Februar 2017, 12:28
 */

#ifndef DOS_HPP
#define DOS_HPP

#include <QLabel>
#include <QWidget>
#include <QGridLayout>
#include <QPushButton>

#include "Output.hpp"
#include "Player.hpp"

namespace vmp 
{
    class Dos : public QWidget 
    {
        //Q_OBJECT
        public:
            Dos(QWidget* parent = 0);
            void setOutput(Output* o);

        private slots:
            void RunDreamIt();
            void choice1();
            void choice2();
            void choice3();
            Output* output;
            QLabel* lbl;

        private:

    };
}

#endif /* DOS_HPP */


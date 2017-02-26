/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "Dos.hpp"

namespace vmp 
{
    Dos::Dos(QWidget* parent) 
        : QWidget(parent)
    {
        QPushButton* jaBtn = new QPushButton("Ja", this);
        QPushButton* neinBtn = new QPushButton("nein", this);
        QPushButton* exitBtn = new QPushButton("Exit", this);
        lbl = new QLabel("Willst du DREAMIT hören??? (Echt gutes Lied!)");

        QGridLayout* grid = new QGridLayout(this);

        grid->addWidget(lbl, 0, 0 , 1, 3);
        grid->addWidget(jaBtn, 1, 0);
        grid->addWidget(neinBtn, 1, 1);
        grid->addWidget(exitBtn, 1, 2);

        setLayout(grid);

        connect(jaBtn, &QPushButton::clicked, this, &Dos::choice1);
        connect(neinBtn, &QPushButton::clicked, this, &Dos::choice2);
        connect(exitBtn, &QPushButton::clicked, this, &Dos::choice3);

    }

    void Dos::setOutput(Output* o)
    {
        output = o;
    }
    
    void Dos::choice1() {
        lbl->setText("Das machen wir doch glatt...!");
        RunDreamIt();
    }

    void Dos::choice2() {
        lbl->setText("Ich starte es aber trotzdem rotzdem.. hahahaa!");
        RunDreamIt();
    }


    void Dos::choice3() {
        lbl->setText("Wieso beenden? Da spielen wir doch lieber DREAMIT. Du stimmst mir doch zu, oder?");
        RunDreamIt();
    }
    
    
    void Dos::RunDreamIt()
    {
        if (output->isRunning())
            output->stop();

        output->getPlayer()->reset();

        output->start();
    }
}
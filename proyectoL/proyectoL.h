#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_proyectoL.h"

class proyectoL : public QMainWindow
{
    Q_OBJECT

public:
    proyectoL(QWidget *parent = nullptr);
    ~proyectoL();

private:
    Ui::proyectoLClass ui;
};


#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_proyectoL.h"
#include <qpushbutton>
#include <qthread>

#include <controllers/controller.h>

class proyectoL : public QMainWindow
{
    Q_OBJECT

public:
    proyectoL(QWidget *parent = nullptr);
    ~proyectoL();

    void on_button_clicked();

private:
    Ui::proyectoLClass ui;
};


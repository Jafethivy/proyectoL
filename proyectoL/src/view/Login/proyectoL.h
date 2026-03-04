#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_proyectoL.h"

class proyectoL : public QMainWindow
{
    Q_OBJECT

public:
    proyectoL(QWidget *parent = nullptr);
    ~proyectoL();

public slots:
	void on_login_b_clicked();

signals:
	void LoginAttempt(const QString& username, const QString& password);

private:
    Ui::proyectoLClass ui;
};


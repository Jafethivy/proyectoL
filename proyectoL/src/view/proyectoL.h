#pragma once

#include <QtWidgets/QMainWindow>
#include <QDialog>
#include <QCloseEvent>
#include <QTimer>
#include "ui_proyectoL.h"

class login;
class reception;

class proyectoL : public QMainWindow
{
    Q_OBJECT

public:
    proyectoL(QWidget *parent = nullptr);
    ~proyectoL();

    //Helpers
    login* loginWidget() const;
	reception* receptionWidget() const;
    
    void screen_area();

    //StackedWidget
    void set_login();
    void set_area(const QString& area);

signals:
	void closingRequested();
	void closeApproved();

public slots:
    void onCloseApproved();
protected:
	void closeEvent(QCloseEvent* event) override;

private:

    Ui::proyectoLClass ui;

    bool m_closingPending = false;
    QCloseEvent* m_pendingCloseEvent = nullptr;

	login* loginWindow = nullptr;
	reception* receptionWindow = nullptr;
private:
    int width_screen;
    int height_screen;

};


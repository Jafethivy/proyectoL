#pragma once

#include <QtWidgets/QMainWindow>
#include <QDialog>
#include <QCloseEvent>
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

    void set_login();

    //Setter
    void set_area(const QString& area);

signals:
	void closingRequested();
	void closeApproved();

public slots:
    void onCloseApproved();
protected:
	void closeEvent(QCloseEvent* event) override;

private:
    void setResizableWindowSize(int w, int h);

    Ui::proyectoLClass ui;

    bool m_closingPending = false;
    QCloseEvent* m_pendingCloseEvent = nullptr;

	login* loginWindow = nullptr;
	reception* receptionWindow = nullptr;

};


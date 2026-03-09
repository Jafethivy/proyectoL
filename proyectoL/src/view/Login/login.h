#pragma once

#include <QWidget>
#include <QMainWindow>
#include <QDialog>

#include "login/ui_login.h"


class login : public QWidget
{
	Q_OBJECT

public:
	login(QWidget *parent = nullptr);
	~login();

	void setMainWindow(QMainWindow* mainWindow);

	void Status(const QString& area, const bool& status);
	void ShowLoginError();

public slots:
	void on_login_b_clicked();

signals:
	void LoginAttempt(const QString& username, const QString& password);
	void LoginSuccess(const QString& area);


private:
	Ui::loginClass ui;

	QMainWindow* m_mainWindow = nullptr;
};


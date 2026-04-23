#pragma once

#include <QObject>
#include <QDebug>
#include <QString>

#include "view/Login/login.h"

class DB;

class Controller : public QObject
{
	Q_OBJECT
public:
	explicit Controller(login* window, QObject* parent = nullptr);

public slots:
	void on_login_attempt(const QString& username, const QString& password);
	void on_login_status(const int& area, const bool& status);
	
	void on_endSession();
	void on_endSession_success();

signals:
	void LoginAttempt(const QString& username, const QString& password);
	void LoginStatus(const int& area, const bool& status);
	
	void endSession(const int& status);
	void endSession_success();

private:
	login* m_window = nullptr;
};
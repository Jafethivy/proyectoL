#pragma once

#include <QObject>
#include <QDebug>
#include <QString>
#include "view/Login/proyectoL.h"

class DB;

class Controller : public QObject
{
	Q_OBJECT
public:
	explicit Controller(proyectoL* window, QObject* parent = nullptr);

public slots:
	void on_login_attempt(const QString& username, const QString& password);

signals:
	void LoginAttempt(const QString& username, const QString& password);

private:
	proyectoL* m_window = nullptr;
};
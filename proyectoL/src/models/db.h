#pragma once

#include <QObject>
#include <QThread>
#include <QDebug>

#include <mysqlx/xdevapi.h>

using namespace mysqlx;

class DB : public QObject
{
	Q_OBJECT
public:
	explicit DB(QObject* parent = nullptr);

public slots:
	void connectToDatabase();
	void Login(const QString& user_id, const QString& password);

signals:

};
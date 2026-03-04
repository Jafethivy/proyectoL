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

	void debug();

public slots:
	Session* connectToDatabase();

signals:

private:
	Session* sess = nullptr;
};
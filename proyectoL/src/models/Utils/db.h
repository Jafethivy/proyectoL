#pragma once

#include <QObject>
#include <QThread>
#include <QDebug>
#include <string>

#include <mysqlx/xdevapi.h>

using namespace mysqlx;

class DB : public QObject
{
	Q_OBJECT
public:
	explicit DB(QObject* parent = nullptr);
	void debug();

	QVector<std::string> get_user_info(std::string& username);
	void updateStatusDb(const std::string& username, const int& status);

public slots:
	Session* connectToDatabase();

protected:
	bool status;
private:
	Session* sess = nullptr;
};
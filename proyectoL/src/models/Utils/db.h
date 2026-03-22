#pragma once

#include <QObject>
#include <QThread>
#include <QDebug>
#include <QVariant>
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

	//reception
	void createReservation(QVariantMap m_data);

public slots:
	Session* connectToDatabase();

signals:
	void n_ReservationCreated(QVariantMap n_data);
protected:
	bool status;
private:
	Session* sess = nullptr;
};
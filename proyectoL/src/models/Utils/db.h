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

	QVariantMap get_user_info(std::string& username);
	void updateStatusDb(const std::string& username, const int& status);

	//reception
	void initReservations();

	void createReservation(QVariantMap m_data);
	void editReservation(QVariantMap m_data);
	void removeReservation(QVariant index);

	void advancedQuery(QVariantMap n_data);
public slots:
	Session* connectToDatabase();

signals:
	void reservationsGetter(QVariantList reservations);

	void n_ReservationCreated(QVariantMap n_data);
	void n_ReservationEdited(QVariantMap n_data);

	void reservationAdvanced(QVariantList reservations);

protected:
	bool status;
private:
	Session* sess = nullptr;
};
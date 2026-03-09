#pragma once

#include <QObject>
#include <QJsonArray>

class Reservations  : public QObject
{
	Q_OBJECT

public:
	explicit Reservations(QObject *parent = nullptr);
	~Reservations();

public slots:
	void addReservation(const QJsonObject& reservation);
	void removeReservation(int index);
	void updateReservation(int index, const QJsonObject& data);

signals:

};


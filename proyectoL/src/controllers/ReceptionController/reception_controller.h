#pragma once

#include <QObject>
#include "view/reception/reception.h"


class reception_controller  : public QObject
{
	Q_OBJECT

public:
	reception_controller(reception* w_reception, QObject *parent = nullptr);
	~reception_controller();

	void on_end_session();
	void createdReservationQml(QVariantMap n_data);

signals:
	void updateSession(const int& status);
	void endSession();

	void c_reservationCreated(QVariantMap m_data);
	void c_createdReservationQml(QVariantMap n_data);

private:
	reception* m_receptionWidget = nullptr;
};


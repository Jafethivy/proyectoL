#pragma once

#include <QObject>
#include "view/reception/reception.h"


class reception_controller  : public QObject
{
	Q_OBJECT

public:
	reception_controller(reception* w_reception = nullptr, QObject *parent = nullptr);
	~reception_controller();

	void on_end_session();
	void createdReservationQml(QVariantMap n_data);
	void editedReservationQml(QVariantMap n_data);

signals:

	void updateSession(const int& status);
	void endSession();

	void c_reservationInit();
	void c_getReservations(QVariantList reservations);
	void c_reservationCreated(QVariantMap m_data);
	void c_reservationEdited(QVariantMap m_data);
	void c_reservationRemoved(QVariant index);

	void c_createdReservationQml(QVariantMap n_data);
	void c_editedReservationQml(QVariantMap n_data);

	void create_qml();
	void c_advancedQuery(QVariantMap n_data);
	void s_reservationsAdvanced(QVariantList reservations);

private:
	reception* m_receptionWidget = nullptr;
};


#include "reception_controller.h"

reception_controller::reception_controller(reception* w_reception, QObject *parent)
	: QObject(parent), m_receptionWidget(w_reception){

	connect(m_receptionWidget, &reception::endSession,
		this, &reception_controller::on_end_session,
		Qt::AutoConnection);
	connect(m_receptionWidget, &reception::signalReservationCreated,
		this, &reception_controller::c_reservationCreated,
		Qt::AutoConnection);
	
	connect(this, &reception_controller::c_createdReservationQml,
		m_receptionWidget, &reception::reservationCreatedQml);
}

reception_controller::~reception_controller()
{}

void reception_controller::on_end_session() {
	emit updateSession(0);
	emit endSession();
}

void reception_controller::createdReservationQml(QVariantMap n_data) {
	emit c_createdReservationQml(n_data);
}

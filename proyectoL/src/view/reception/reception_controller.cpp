#include "reception_controller.h"

reception_controller::reception_controller(reception* w_reception, QObject *parent)
	: QObject(parent), m_receptionWidget(w_reception){

	connect(m_receptionWidget, &reception::endSession,
		this, &reception_controller::on_end_session);

	connect(m_receptionWidget, &reception::signalReservationInit,
		this, &reception_controller::c_reservationInit);
	connect(m_receptionWidget, &reception::signalReservationCreated,
		this, &reception_controller::c_reservationCreated);
	connect(m_receptionWidget, &reception::signalReservationEdited,
		this, &reception_controller::c_reservationEdited);
	connect(m_receptionWidget, &reception::signalReservationRemoved,
		this, &reception_controller::c_reservationRemoved);
	connect(m_receptionWidget, &reception::signalAdvancedQuery,
		this, &reception_controller::c_advancedQuery);

	connect(this, &reception_controller::c_getReservations,
		m_receptionWidget, &reception::init_reservations);
	connect(this, &reception_controller::c_createdReservationQml,
		m_receptionWidget, &reception::reservationCreatedQml);
	connect(this, &reception_controller::c_editedReservationQml,
		m_receptionWidget, &reception::reservationEditedQml);
	connect(this, &reception_controller::s_reservationsAdvanced,
		m_receptionWidget, &reception::reservationAdvanced);

	connect(this, &reception_controller::create_qml,
		m_receptionWidget, &reception::create_qml);
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
void reception_controller::editedReservationQml(QVariantMap n_data) {
	emit c_editedReservationQml(n_data);
}

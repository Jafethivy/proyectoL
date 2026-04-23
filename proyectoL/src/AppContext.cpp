#include "AppContext.h"
#include <QThread>

#include "view/proyectoL.h"
#include "view/reception/reception.h"

#include "view/login/controller.h"
#include "view/reception/reception_controller.h"

#include "models/Login/auth.h"


AppContext::AppContext(QObject* parent) : QObject(parent) {
}

AppContext::~AppContext() {
    cleanup();
}

void AppContext::createObjects() {
	m_window = new proyectoL();

	m_db = new DB();
    m_auth = new Auth(m_db);

    m_loginWidget = m_window->loginWidget();

	m_controller = new Controller(m_loginWidget);

}

void AppContext::setupThreads() {
	m_db->moveToThread(&WorkerThread);
    m_auth->moveToThread(&WorkerThread);

    WorkerThread.start();
}

void AppContext::setupConnections() {

	//Close Event Connections
    QObject::connect(m_window, &proyectoL::closingRequested,
        m_controller, &Controller::on_endSession,
        Qt::AutoConnection);
    QObject::connect(m_controller, &Controller::endSession,
        m_auth, &Auth::updateStatusDb,
        Qt::AutoConnection);

    QObject::connect(m_auth, &Auth::endSession_success,
        m_controller, &Controller::on_endSession_success,
        Qt::AutoConnection);
    QObject::connect(m_controller, &Controller::on_endSession_success,
        m_window, &proyectoL::onCloseApproved);

    //MainWindow -> this (Emit a signal telling to AppContext an area will be used)
    QObject::connect(m_window, &proyectoL::exist,
        this, &AppContext::Demand,
        Qt::AutoConnection);


    //Login Controller -> Auth
    QObject::connect(m_controller, &Controller::LoginAttempt,
        m_auth, &Auth::on_login_attempt,
        Qt::AutoConnection);
    // Auth -> Login Controller
    QObject::connect(m_auth, &Auth::LoginStatus,
        m_controller, &Controller::on_login_status,
        Qt::AutoConnection);
    // Login Controller -> Login Window
    QObject::connect(m_controller, &Controller::LoginStatus,
        m_loginWidget, &login::Status);
    // Login Window -> Main Window
    QObject::connect(m_loginWidget, &login::LoginSuccess,
        m_window, &proyectoL::set_area);

}

void AppContext::cleanup() {
    if (WorkerThread.isRunning()) {
        WorkerThread.quit();
        WorkerThread.wait();
    }

    // deleteLater es seguro incluso si el hilo ya termino
    if (m_auth) m_auth->deleteLater();
    if (m_controller) m_controller->deleteLater();
	if (m_db) m_db->deleteLater();
}

void AppContext::debugConnections() {
    qDebug() << "Debugging connections...";

}

proyectoL* AppContext::initialize() {
    createObjects();
    setupThreads();
    setupConnections();
    return m_window;
}

//on demand (this section will build the areas on demand)
void AppContext::Demand(int area) {
    switch (area) {
    case 1:
        demandReception();
        break;
    case 2:
        break;
    case 3:
        break;
    case 4:
        break;
    }
}

void AppContext::demandReception() {
    if (!m_window->receptionWidget()) return;

    m_receptionWidget = m_window->receptionWidget();
    m_receptionController = new reception_controller(m_receptionWidget);

    //Setup reception connecetions
    
    // Reception Controller -> Auth 
    QObject::connect(m_receptionController, &reception_controller::updateSession,
        m_auth, &Auth::updateStatusDb,
        Qt::AutoConnection);
    // Reception Controller -> Main Window
    QObject::connect(m_receptionController, &reception_controller::endSession,
        m_window, &proyectoL::logout,
        Qt::AutoConnection);
    //Main Window -> Reception Controller
    QObject::connect(m_window, &proyectoL::create_qml,
        m_receptionController, &reception_controller::create_qml,
        Qt::AutoConnection);

    //Reception Controller -> DB [init reservations]
    QObject::connect(m_receptionController, &reception_controller::c_reservationInit,
        m_db, &DB::initReservations,
        Qt::AutoConnection);
    //Reception Controller -> DB [create reservation]
    QObject::connect(m_receptionController, &reception_controller::c_reservationCreated,
        m_db, &DB::createReservation,
        Qt::AutoConnection);
    //Reception Controller -> DB [edit reservation]
    QObject::connect(m_receptionController, &reception_controller::c_reservationEdited,
        m_db, &DB::editReservation,
        Qt::AutoConnection);
    //Reception Controller -> DB [remove reservation]
    QObject::connect(m_receptionController, &reception_controller::c_reservationRemoved,
        m_db, &DB::removeReservation,
        Qt::AutoConnection);
	//Reception Controller -> DB [advanced query]
    QObject::connect(m_receptionController, &reception_controller::c_advancedQuery,
        m_db, &DB::advancedQuery,
		Qt::AutoConnection);

    //DB -> Reception Controller [get reservations]
    QObject::connect(m_db, &DB::reservationsGetter,
        m_receptionController, &reception_controller::c_getReservations,
        Qt::AutoConnection);
    //DB -> Reception Controller [created reservation]
    QObject::connect(m_db, &DB::n_ReservationCreated,
        m_receptionController, &reception_controller::createdReservationQml,
        Qt::AutoConnection);
    //DB -> Reception Controller [edited reservation]
    QObject::connect(m_db, &DB::n_ReservationEdited,
        m_receptionController, &reception_controller::editedReservationQml,
        Qt::AutoConnection);
    //DB -> Reception Controller [advanced query]
    QObject::connect(m_db, &DB::reservationAdvanced,
        m_receptionController, &reception_controller::s_reservationsAdvanced,
        Qt::AutoConnection);
}
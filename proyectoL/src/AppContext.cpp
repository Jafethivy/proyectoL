#include "AppContext.h"
#include <QThread>

#include "view/proyectoL.h"
#include "view/reception/reception.h"

#include "controllers/LoginController/controller.h"
#include "controllers/ReceptionController/reception_controller.h"

#include "models/Login/auth.h"
#include "models/modelsfactory.h"


AppContext::AppContext(QObject* parent) : QObject(parent) {
}

AppContext::~AppContext() {
    cleanup();
}

void AppContext::createObjects() {
	m_window = new proyectoL();

	m_loginWidget = m_window->loginWidget();
	m_receptionWidget = m_window->receptionWidget();

	m_db = new DB();
    m_auth = new Auth(m_db);

	m_controller = new Controller(m_loginWidget);
    m_receptionController = new reception_controller(m_receptionWidget);

	m_modelFactory = new ModelFactory(m_db);

}

void AppContext::setupThreads() {
	m_db->moveToThread(&WorkerThread);
    m_auth->moveToThread(&WorkerThread);
	m_modelFactory->moveToThread(&WorkerThread);

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


	// Reception Window -> Reception Controller
    QObject::connect(m_receptionWidget, &reception::endSession,
        m_receptionController, &reception_controller::on_end_session,
		Qt::AutoConnection);
	// Reception Controller -> Auth 
    QObject::connect(m_receptionController, &reception_controller::updateSession,
        m_auth, &Auth::updateStatusDb,
		Qt::AutoConnection);
	// Reception Controller -> Main Window
    QObject::connect(m_receptionController, &reception_controller::endSession,
        m_window, &proyectoL::set_login,
		Qt::AutoConnection);


}

void AppContext::cleanup() {
    if (WorkerThread.isRunning()) {
        WorkerThread.quit();
        WorkerThread.wait();
    }

    // deleteLater es seguro incluso si el hilo ya terminó
    if (m_auth) m_auth->deleteLater();
    if (m_controller) m_controller->deleteLater();
	if (m_db) m_db->deleteLater();
	if (m_modelFactory) m_modelFactory->deleteLater();
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
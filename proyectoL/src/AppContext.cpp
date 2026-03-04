#include "AppContext.h"
#include <QThread>

#include "view/Login/proyectoL.h"
#include "models/Login/auth.h"
#include "controllers/LoginController/controller.h"
#include "models/modelsfactory.h"

AppContext::AppContext(QObject* parent) : QObject(parent) {
}

AppContext::~AppContext() {
    cleanup();
}

void AppContext::createObjects() {
	m_window = new proyectoL();

	m_db = new DB();
    m_auth = new Auth(m_db);
	m_controller = new Controller(m_window);
	m_modelFactory = new ModelFactory(m_db);

}

void AppContext::setupThreads() {
	m_db->moveToThread(&WorkerThread);
    m_auth->moveToThread(&WorkerThread);
	m_modelFactory->moveToThread(&WorkerThread);

    WorkerThread.start();
}

void AppContext::setupConnections() {
    QObject::connect(m_controller, &Controller::LoginAttempt,
        m_auth, &Auth::on_login_attempt,
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

proyectoL* AppContext::initialize() {
    createObjects();
    setupThreads();
    setupConnections();
    return m_window;
}
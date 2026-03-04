#include "controller.h"


Controller::Controller(proyectoL* window,QObject* parent) 
	: QObject(parent), m_window(window){

	connect(window, &proyectoL::LoginAttempt, this, &Controller::on_login_attempt);

}

void Controller::on_login_attempt(const QString& username, const QString& password) {
	qDebug() << "Controller received login attempt: " << username << " with password: " << password;
	emit LoginAttempt(username, password);
}
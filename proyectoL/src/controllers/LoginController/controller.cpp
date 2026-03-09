#include "controller.h"


Controller::Controller(login* window,QObject* parent) 
	: QObject(parent), m_window(window){

	connect(window, &login::LoginAttempt, this, &Controller::on_login_attempt);

}

void Controller::on_login_attempt(const QString& username, const QString& password) {
	emit LoginAttempt(username, password);
}

void Controller::on_login_status(const QString& area, const bool& status) {
	emit LoginStatus(area, status);
}

void Controller::on_endSession() {
	emit endSession(0);
}
void Controller::on_endSession_success() {
	emit endSession_success();
}
#include "login.h"
#include "utils/ui_error.h"

login::login(QWidget *parent)
	: QWidget(parent){
	ui.setupUi(this);

}

login::~login()
{}

void login::on_login_b_clicked() {
	QString username = ui.user_in->text();
	QString password = ui.pwd_in->text();

	emit LoginAttempt(username, password);
}

void login::Status(const QString& area, const bool& status) {
	if (status) {
		emit LoginSuccess(area);
	}
	else {
		ShowLoginError();
	}
}

void login::setMainWindow(QMainWindow* mainWindow) {
	m_mainWindow = mainWindow;
}

void login::ShowLoginError() {
	QDialog errorDialog(this);
	Ui::Form ui;
	ui.setupUi(&errorDialog);

	errorDialog.setWindowTitle("Error de Login");
	errorDialog.setFixedSize(381, 121);

	QPoint center;
	if (m_mainWindow) {
		center = m_mainWindow->mapToGlobal(m_mainWindow->rect().center());
	}
	else {
		center = this->mapToGlobal(this->rect().center());
	}

	QRect dialogGeo = errorDialog.frameGeometry();
	dialogGeo.moveCenter(center);
	errorDialog.move(dialogGeo.topLeft());

	errorDialog.exec();
}
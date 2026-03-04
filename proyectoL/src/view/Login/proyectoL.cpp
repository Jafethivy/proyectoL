#include "proyectoL.h"

proyectoL::proyectoL(QWidget* parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	setFixedSize(421, 481);

}

proyectoL::~proyectoL()
{}

void proyectoL::on_login_b_clicked() {
	QString username = ui.user_in->text();
	QString password = ui.pwd_in->text();

	emit LoginAttempt(username, password);
}

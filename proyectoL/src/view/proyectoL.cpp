#include "proyectoL.h"

#include "view/reception/reception.h"
#include "view/Login/login.h"

proyectoL::proyectoL(QWidget* parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);


	loginWindow = new login(this);
	loginWindow->setMainWindow(this);

	receptionWindow = new reception(this);

	ui.stackedWidget->addWidget(loginWindow);
	ui.stackedWidget->addWidget(receptionWindow);

	set_login();

}

proyectoL::~proyectoL()
{}

login* proyectoL::loginWidget() const {
	return loginWindow;
}
reception* proyectoL::receptionWidget() const {
	return receptionWindow;
}

void proyectoL::setResizableWindowSize(int w, int h) {
	setMinimumSize(w, h);
	resize(w, h);
}

void proyectoL::set_login() {
	setResizableWindowSize(421, 481);
	ui.stackedWidget->setCurrentWidget(loginWindow);
}

void proyectoL::set_area(const QString& area) {
	setResizableWindowSize(781, 421);
	ui.stackedWidget->setCurrentWidget(receptionWindow);
}

void proyectoL::closeEvent(QCloseEvent* event) {
	if (m_closingPending) {
		event->accept();
		return;
	}

	m_closingPending = true;
	m_pendingCloseEvent = event;

	emit closingRequested();

	event->ignore();
}

void proyectoL::onCloseApproved() {
	m_closingPending = false;
	close();
}
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

void proyectoL::screen_area() {
	QScreen* screen = QGuiApplication::primaryScreen();
	QRect screenGeometry = screen->availableGeometry();
	width_screen = (screenGeometry.width() - this->width()) / 2;
	height_screen = (screenGeometry.height() - this->height()) / 2;
	move(screenGeometry.x() + width_screen, screenGeometry.y() + height_screen);
}

void proyectoL::set_login() {
	showNormal();
	resize(421, 481);
	screen_area();
	ui.stackedWidget->setCurrentIndex(0);
}

void proyectoL::set_area(const QString& area) {
	setUpdatesEnabled(false);
	ui.stackedWidget->setCurrentIndex(1);
	setUpdatesEnabled(true);
	QTimer::singleShot(5, this, [this]() {
		showMaximized();
		});
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
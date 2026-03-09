#include "reception.h"


reception::reception(QWidget *parent)
	: QWidget(parent), m_reservations(nullptr), m_overlay(nullptr), m_tables(nullptr)
	, m_overlay_config(nullptr), m_reception(nullptr), m_config(nullptr) {
	ui.setupUi(this);

	create_qml_reservations();
	create_qml_tables();
	create_qml_reception(parent);
	create_qml_config(parent);
}

reception::~reception(){
}

//Reception Menu
void reception::showMenu() {
	QWidget* mainWindow = window();
	if (!mainWindow) return;

	m_overlay->setGeometry(mainWindow->rect());
	m_overlay->show();
	m_overlay->raise();

	m_reception->adjustSize();
	int menuHeight = m_reception->height();

	QPoint globalPos = ui.reception_button->mapToGlobal(
		QPoint(-(ui.reception_button->width()-8), -menuHeight)
	);
	m_reception->move(globalPos);
	m_reception->show();
	m_reception->raise();

	m_reception->setWindowOpacity(0);
	auto* anim = new QPropertyAnimation(m_reception, "windowOpacity");
	anim->setDuration(150);
	anim->setStartValue(0.0);
	anim->setEndValue(1.0);
	anim->start(QAbstractAnimation::DeleteWhenStopped);

	m_menuVisible = true;
}

void reception::closeMenu() {
	if (!m_menuVisible) return;

	auto* anim = new QPropertyAnimation(m_reception, "windowOpacity");
	anim->setDuration(150);
	anim->setStartValue(1.0);
	anim->setEndValue(0.0);

	connect(anim, &QPropertyAnimation::finished, [this]() {
		m_reception->hide();
		m_overlay->hide();
		m_menuVisible = false;
		});

	anim->start(QAbstractAnimation::DeleteWhenStopped);
}


//Config Menu
void reception::showMenu_config() {
	QWidget* mainWindow = window();
	if (!mainWindow) return;

	m_overlay_config->setGeometry(mainWindow->rect());
	m_overlay_config->show();
	m_overlay_config->raise();

	m_config->adjustSize();
	int menuHeight = m_config->height();

	QPoint globalPos = ui.config_button->mapToGlobal(
		QPoint(1, -menuHeight)
	);
	m_config->move(globalPos);
	m_config->show();
	m_config->raise();

	m_config->setWindowOpacity(0);
	auto* anim = new QPropertyAnimation(m_config, "windowOpacity");
	anim->setDuration(150);
	anim->setStartValue(0.0);
	anim->setEndValue(1.0);
	anim->start(QAbstractAnimation::DeleteWhenStopped);

	m_configVisible = true;
}

void reception::closeMenu_config() {
	if (!m_configVisible) return;

	auto* anim = new QPropertyAnimation(m_config, "windowOpacity");
	anim->setDuration(150);
	anim->setStartValue(1.0);
	anim->setEndValue(0.0);

	connect(anim, &QPropertyAnimation::finished, [this]() {
		m_config->hide();
		m_overlay_config->hide();
		m_configVisible = false;
		});

	anim->start(QAbstractAnimation::DeleteWhenStopped);
}

bool reception::eventFilter(QObject* watched, QEvent* event) {
	if (event->type() != QEvent::MouseButtonPress) {
		return QWidget::eventFilter(watched, event);
	}

	QWidget* targetMenu = nullptr;
	bool isVisible = false;

	if (watched == m_overlay && m_menuVisible) {
		targetMenu = m_reception;
		isVisible = true;
	}
	else if (watched == m_overlay_config && m_configVisible) {
		targetMenu = m_config;
		isVisible = true;
	}

	if (!isVisible) {
		return QWidget::eventFilter(watched, event);
	}

	QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
	QRect menuRect = targetMenu->geometry();
	QPoint globalClick = mouseEvent->globalPosition().toPoint();

	if (!menuRect.contains(globalClick)) {
		if (targetMenu == m_reception) {
			closeMenu();
		}
		else if (targetMenu == m_config) {
			closeMenu_config();
		}
		return true;
	}

	return QWidget::eventFilter(watched, event);
}

//Slots
void reception::on_end_session_clicked() {
	emit endSession();
}

void reception::on_reception_button_clicked() {
	if (!m_menuVisible) {
		showMenu();
	}
	else {
		closeMenu();
	}
}

void reception::on_config_button_clicked() {
	if (!m_configVisible) {
		showMenu_config();
	}
	else {
		closeMenu_config();
	}
}

// Widget Helpers
void reception::update_size() {
	int reception_island_width = static_cast<int>(width() * 0.3);
	ui.reception_island->setFixedWidth(reception_island_width);
}

void reception::resizeEvent(QResizeEvent* event) {
	update_size();
	QWidget::resizeEvent(event);
}

// QML
void reception::createQmlWidget(QQuickWidget*& member,
	const QString& qmlPath,
	QWidget* container){

	member = new QQuickWidget(this);
	member->setResizeMode(QQuickWidget::SizeRootObjectToView);
	member->setSource(QUrl(qmlPath));

	QLayout* layout = container->layout();
	if (!layout) {
		layout = new QVBoxLayout(container);
		layout->setContentsMargins(0, 0, 0, 0);
	}
	layout->addWidget(member);
}

void reception::create_qml_reservations() {
	createQmlWidget(m_reservations,
		QStringLiteral("qrc:/qml/reception/reservations.qml"),
		ui.reception_widget);
}

void reception::create_qml_tables() {
	createQmlWidget(m_tables,
		QStringLiteral("qrc:/qml/reception/tables.qml"),
		ui.tables_widget);
}

void reception::create_qml_reception(QWidget* parent) {
	m_overlay = new QWidget(parent);
	m_overlay->setStyleSheet("background-color: transparent;");
	m_overlay->hide();

	m_reception = new QQuickWidget(nullptr);
	m_reception->setResizeMode(QQuickWidget::SizeRootObjectToView);
	m_reception->setSource(QUrl("qrc:/qml/reception/reception.qml"));
	m_reception->setFixedSize(200, 300);
	m_reception->setWindowFlags(Qt::Tool | Qt::FramelessWindowHint);
	m_reception->setAttribute(Qt::WA_TranslucentBackground);

	m_overlay->installEventFilter(this);
}

void reception::create_qml_config(QWidget* parent) {
	m_overlay_config = new QWidget(parent);
	m_overlay_config->setStyleSheet("background-color: transparent;");
	m_overlay_config->hide();

	m_config = new QQuickWidget(nullptr);
	m_config->setResizeMode(QQuickWidget::SizeRootObjectToView);
	m_config->setSource(QUrl("qrc:/qml/reception/config.qml"));
	m_config->setFixedSize(200, 300);
	m_config->setWindowFlags(Qt::Tool | Qt::FramelessWindowHint);
	m_config->setAttribute(Qt::WA_TranslucentBackground);

	m_overlay_config->installEventFilter(this);
}
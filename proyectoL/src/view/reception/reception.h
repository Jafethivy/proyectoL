#pragma once

#include <QWidget>
#include <QSplitter>
#include <QQuickWidget>
#include <QVBoxLayout>
#include <QPropertyAnimation>
#include <QGraphicsDropShadowEffect>
#include <QQuickItem>
#include <QDebug>
#include <QEvent>
#include <QMouseEvent>
#include <QQmlContext>

#include "reception/ui_reception.h"

class reception : public QWidget
{
	Q_OBJECT

public:
	reception(QWidget *parent = nullptr);
	~reception();

	void reservationCreatedQml(QVariantMap n_data);

public slots:
	void on_end_session_clicked();
	void on_reception_button_clicked();
	void on_config_button_clicked();

	void debug_a();
	void onReservationCreated(QVariant data);

signals:
	void endSession();
	void signalReservationCreated(QVariantMap m_data);
	
private:
	void resizeEvent(QResizeEvent* event) override;

	void createQmlWidget(QQuickWidget*& member,
		const QString& qmlPath,QWidget* container);
	void create_qml_tables();

	//reception
	void create_qml_reception(QWidget* parent);
	void r_create_connections();

	//reservations
	void create_qml_reservations();

	//config
	void create_qml_config(QWidget* parent);

	void showMenu();
	void closeMenu();

	void showMenu_config();
	void closeMenu_config();

protected:
	bool eventFilter(QObject* watched, QEvent* event) override;

private:
	Ui::reception ui;

	QQuickWidget* m_reservations;
	QQuickWidget* m_tables;

	QQuickWidget* m_reception;
	QQuickWidget* m_config;

	QWidget* m_overlay;
	QWidget* m_overlay_config;

	bool m_menuVisible;
	bool m_configVisible;

};


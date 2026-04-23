#pragma once

#include <QWidget>
#include <QSplitter>
#include <QQuickWidget>
#include <QVBoxLayout>
#include <QPropertyAnimation>
#include <QGraphicsDropShadowEffect>
#include <QParallelAnimationGroup>
#include <QQuickItem>
#include <QDebug>
#include <QEvent>
#include <QMouseEvent>
#include <QQmlContext>
#include <QPainter>
#include <QTimer>

#include "reception/ui_reception.h"

class reception : public QWidget
{
	Q_OBJECT
	Q_PROPERTY(int panel_width READ panel_width CONSTANT);

public:
	reception(QWidget *parent = nullptr);
	~reception();

	int panel_width() const { return w_width; }

	//reservations public
	void init_reservations(QVariantList reservations);
	void reservationCreatedQml(QVariantMap n_data);
	void reservationEditedQml(QVariantMap n_data);
	void reservationRemoveQml();

	void reservationAdvanced(QVariantList reservations);

public slots:
	void on_end_session_clicked();
	void on_reception_button_clicked();
	void on_config_button_clicked();
	void on_help_button_clicked();
	void on_advanced_button_clicked();

	void onReservationCreated(QVariant data);
	void onReservationEdited(QVariant data);
	void onReservationRemoved(QVariant index);

	void onAdvancedQuery(QVariant r_data);

	void create_qml();
	void closeAdvanced();
	void closeReception();
	void closeConfig();
	void closeMenuQml(QWidget* overlay, QQuickWidget* widget, bool& flag);

signals:
	void endSession();

	void signalReservationInit();
	void signalReservationCreated(QVariantMap m_data);
	void signalReservationEdited(QVariantMap m_data);
	void signalReservationRemoved(QVariant index);

	void signalAdvancedQuery(QVariantMap r_data);
private:
	void resizeEvent(QResizeEvent* event) override;

	//tables
	void create_qml_tables();

	//reservations
	void create_qml_reservations();
	void resv_create_connections();

	//advanced search
	void createQmlAdvanced();
	void a_create_connections();
	void showAdvanced();
	//reception
	void create_qml_reception();
	void r_create_connections();
	void showMenu();

	//config
	void create_qml_config();
	void showConfig();

	

protected:
	bool eventFilter(QObject* watched, QEvent* event) override;
private:
	Ui::reception ui;

	QQuickWidget* m_reservations = nullptr;
	QQuickWidget* m_tables = nullptr;

	QQuickWidget* m_reception = nullptr;
	QQuickWidget* m_config = nullptr;
	QQuickWidget* m_advanced = nullptr;

	QWidget* m_overlay = nullptr;
	QWidget* m_overlayConfig = nullptr;
	QWidget* m_overlayAdvanced = nullptr;

	bool m_recepVisible;
	bool m_configVisible;
	bool m_advancedVisible;
	bool qml_exist = false;

	int w_width;
};


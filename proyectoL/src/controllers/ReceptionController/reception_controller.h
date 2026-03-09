#pragma once

#include <QObject>
#include "view/reception/reception.h"


class reception_controller  : public QObject
{
	Q_OBJECT

public:
	reception_controller(reception* w_reception, QObject *parent = nullptr);
	~reception_controller();

	void on_end_session();

signals:
	void updateSession(const int& status);
	void endSession();

private:
	reception* m_receptionWidget = nullptr;
};


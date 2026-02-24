#pragma once

#include <QObject>
#include <QThread>
#include <QDebug>


class Controller : public QObject
{
	Q_OBJECT
public:
	explicit Controller(QObject* parent = nullptr);

public slots:

private:
};
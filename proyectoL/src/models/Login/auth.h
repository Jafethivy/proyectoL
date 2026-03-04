#pragma once

#include <QObject>
#include <QDebug>
#include <QString>
#include <string>
#include "models/utils/argon2id.h"
#include "models/utils/db.h"

class DB;

class Auth : public QObject
{
	Q_OBJECT
public:
	Auth(DB*model_db, QObject* parent = nullptr);

	QString get_db_info();
	std::string hashPassword(const std::string& password);
	bool verifyPassword(const QString& password, const QString& hash);

	void debug();
	

public slots:
	void on_login_attempt(const QString& username, const QString& password);

private:
	DB* m_db = nullptr;
};
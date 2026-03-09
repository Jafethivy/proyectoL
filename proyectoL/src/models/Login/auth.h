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

	void get_db_info(std::string& username);

	bool verifyPasswordAuth(const std::string& pwd_hash_local, const std::string& pwd_hash);

	void updateStatusDb(const int& status);
	void updateStatusDb_close(const int& status);

public slots:
	void on_login_attempt(const QString& username, const QString& password);

signals:
	void LoginStatus(const QString& area, const int& status);
	void endSession_success();

private:
	DB* m_db = nullptr;
	Argon2id* m_argon = nullptr;

	std::string pwd_hash_db;

	std::string username_local;
	QString area;
	bool status = false;
};
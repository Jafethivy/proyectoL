#include "auth.h"

Auth::Auth(DB* model_db, QObject* parent) : QObject(parent), m_db(model_db) {
	m_db->debug();
}

void Auth::on_login_attempt(const QString& username, const QString& password) {
	qDebug() << "Login attempt: " << username << " with password: " << password;
}

QString Auth::get_db_info() {
		
	return QString("Database info");
}

std::string Auth::hashPassword(const std::string& password) {
	return password;
}

bool Auth::verifyPassword(const QString& password, const QString& hash) {
	return password == hash;
}

void Auth::debug() {
	qDebug() << "Auth model debug";
}
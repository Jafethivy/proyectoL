#include "auth.h"

Auth::Auth(DB* model_db, QObject* parent) : QObject(parent), m_db(model_db) {
	m_db->debug();
	m_argon = new Argon2id();
}

void Auth::on_login_attempt(const QString& username, const QString& password) {
	username_local = username.toStdString();
	get_db_info(username_local);

	if (verifyPasswordAuth(password.toStdString(), pwd_hash_db)) {
		status = true;
		updateStatusDb(1);
	} else {
		status = false;
	}

	emit LoginStatus(area, status);
}

void Auth::get_db_info(std::string& username) {
	QVector<std::string> data = m_db->get_user_info(username);

	if (data.isEmpty()) {
		return;
	}
	pwd_hash_db = data[0];
	area = QString::fromStdString(data[1]);
}

bool Auth::verifyPasswordAuth(const std::string& pwd_hash_local, const std::string& pwd_hash_db) {
	return m_argon->verifyPassword(pwd_hash_local, pwd_hash_db);
}

void Auth::updateStatusDb(const int& status) {
	m_db->updateStatusDb(username_local, status);
}
void Auth::updateStatusDb_close(const int& status) {
	m_db->updateStatusDb(username_local, status);
	emit endSession_success();
}
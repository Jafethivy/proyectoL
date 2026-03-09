#include "db.h"

DB::DB(QObject* parent) : QObject(parent){
    sess = connectToDatabase();
}

Session* DB::connectToDatabase() {
	try {
		sess = new Session("127.0.0.1", 33060, "root", "@bunnixsupremacy13@", "tecnm");
		return sess;
	}
    catch (const mysqlx::Error& err) {
        return nullptr;
    }
    catch (std::exception& ex) {
        return nullptr;
    }

}

void DB::debug() {
    if (sess) {
        qDebug() << "Connected to database successfully!";
    } else {
        qDebug() << "Failed to connect to database.";
    }
}

QVector<std::string> DB::get_user_info(std::string& username) {
    QVector<std::string> resultado;

    try {
        auto result = sess->sql("SELECT pwd_hash, area, status FROM tecnm.user_credentials WHERE id_emp = ?")
            .bind(username)
            .execute();

        Row row = result.fetchOne();

		status = row[2].get<std::string>() == "online" ? 1 : 0;
        
        if (status) {
            return resultado;
        }

        if (row) {
            resultado.append(row[0].get<std::string>());
            resultado.append(row[1].get<std::string>());
        }

    }
    catch (const mysqlx::Error& err) {
        qDebug() << "Error DB:" << err.what();
    }

    return resultado;
}

void DB::updateStatusDb(const std::string& username, const int& status) {
    QVector <std::string>status_type = {"offline", "online"};
    try {
        sess->sql("UPDATE tecnm.user_credentials SET status = ? WHERE id_emp = ?")
            .bind(status_type[status], username)
            .execute();
    }
    catch (const mysqlx::Error& err) {
        qDebug() << "Error updating status in DB:" << err.what();
    }
}
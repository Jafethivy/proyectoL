#include "db.h"
#include <iostream>

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

QVariantMap DB::get_user_info(std::string& username) {
    QVariantMap resultado;

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
            resultado["pwd_hash_db"] = QString::fromStdString(row[0].get<std::string>());
            resultado["area"] = row[1].get<int>();
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

//reservations
void DB::initReservations() {
    try {
        QVariantList reservations;
        auto result = sess->sql("SELECT id_resv, name_resv, guest_resv, DATE_FORMAT(date_resv, '%Y-%m-%d') AS date_resv, TIME_FORMAT(time_resv, '%H:%i')    AS time_resv, status FROM tecnm.reservations WHERE status = 0;")
            .execute();
        auto rows = result.fetchAll();

        for (const auto& row : rows) {
            QVariantMap map;
            map["id_resv"] = row[0].get<int>();
            map["name_resv"] = row[1].get<std::string>().c_str();
            map["guest_resv"] = row[2].get<int>();
            map["date_resv"] = row[3].get<std::string>().c_str();
            map["time_resv"] = row[4].get<std::string>().c_str();
            map["status"] = row[5].get<int>();

            reservations.append(map);
        }

        emit reservationsGetter(reservations);
    }
    catch (const mysqlx::Error& e) {
        qDebug() << "[SQL Error] " << e.what();
    }
}

void DB::createReservation(QVariantMap m_data) {
    std::string name_resv = m_data["name_resv"].toString().toStdString();
    int guest_resv = m_data["guest_resv"].toInt();
    std::string date_resv = m_data["date_resv"].toString().toStdString();
    std::string time_resv = m_data["time_resv"].toString().toStdString();

    auto result = sess->sql("INSERT INTO tecnm.reservations (name_resv, guest_resv, date_resv, time_resv) VALUES (?, ?, ?, ?);")
        .bind(name_resv, guest_resv, date_resv, time_resv)
        .execute();

    int id_resv = result.getAutoIncrementValue();
    m_data.insert("id_resv", id_resv);

    emit n_ReservationCreated(m_data);
}

void DB::editReservation(QVariantMap m_data) {
    mysqlx::SqlResult result;

    std::string query = "UPDATE tecnm.reservations SET ";
    std::string columns;
    std::string where = "WHERE id_resv = ";
    bool args = false;

    for (auto it = m_data.begin(); it != m_data.end(); ++it) {
        if (it.key() == "id_resv") {
            where += it.value().toString().toStdString();
            continue;
        }
        if (it.key() == "guest_resv" && it.value().toInt() == 0) continue;
        if (it.value().toString().isEmpty()) continue;

        if (args) columns += ", ";
        columns += it.key().toStdString() + " = '" +
            it.value().toString().toStdString() + "' ";
        args = true;
    }
    query += columns + where + ";";

    try{
        result = sess->sql(query).execute();
    }catch (const mysqlx::Error& e) {
        qDebug() << "[SQL Error] " << e.what();
        m_data.clear();
        return;

    }catch (const std::exception& e) {
        qDebug() << "[Error] " << e.what();
        m_data.clear();
        return;
    }

    emit n_ReservationEdited(m_data);
}

void DB::removeReservation(QVariant index) {
    auto result = sess->sql("DELETE FROM tecnm.reservations WHERE id_resv = ?;")
        .bind(index.toInt())
        .execute();
}

//advanced query
void DB::advancedQuery(QVariantMap n_data) {
    std::string clientName = n_data["clientName"].toString().toStdString();
	std::string dateFrom = n_data["dateFrom"].toString().toStdString();
	std::string dateTo = n_data["dateTo"].toString().toStdString();
    std::string timeFrom = n_data["timeFrom"].toString().toStdString();
    std::string timeTo = n_data["timeTo"].toString().toStdString();
    int guestMax = n_data["guestsMax"].toInt();
    int guestMin = n_data["guestsMin"].toInt();
	QList states = n_data["states"].toList();
    std::vector<int> statesVec;
    for(const auto& state : states) {
        statesVec.push_back(state.toInt());
	}
	bool nameEnabled = n_data["nameEnabled"].toBool();
	bool dateEnabled = n_data["dateEnabled"].toBool();
	bool timeEnabled = n_data["timeEnabled"].toBool();
	bool guestsEnabled = n_data["guestsEnabled"].toBool();
	bool statesEnabled = n_data["statesEnabled"].toBool();

    std::string preQuery = "Select id_resv, name_resv, guest_resv, DATE_FORMAT(date_resv, '%Y-%m-%d') AS date_resv, TIME_FORMAT(time_resv, '%H:%i') AS time_resv, status";
	std::string query = " FROM tecnm.reservations WHERE 1=1 ";
    
    qDebug() << nameEnabled << dateEnabled << timeEnabled << guestsEnabled << statesEnabled;

    if (!nameEnabled && !dateEnabled && !timeEnabled && !guestsEnabled && !statesEnabled) {
        initReservations();
        return;
    }

    if (!(clientName == "") && nameEnabled) {
		query += "AND name_resv LIKE '%" + clientName + "%' ";
    }
    if (!(dateFrom == "") && dateEnabled) {
		query += "AND date_resv >= '" + dateFrom + "' ";
		query += "AND date_resv <= '" + dateTo + "' ";
    }
	if (!(timeFrom == "") && timeEnabled) {
        query += "AND time_resv >= '" + timeFrom + "' ";
		query += "AND time_resv <= '" + timeTo + "' ";
	}
    if (!(guestMax == 0) && guestsEnabled) {
		query += "AND guest_resv >= " + std::to_string(guestMin) + " ";
		query += "AND guest_resv <= " + std::to_string(guestMax) + " ";
	}
    if (!(statesVec.empty()) && statesEnabled) {
		query += "AND status IN (";
        for (size_t i = 0; i < statesVec.size(); ++i) {
            query += std::to_string(statesVec[i]);
            if (i < statesVec.size() - 1) {
                query += ", ";
            }
        }
		query += ") ";
    }

    preQuery += query;

    try {
        auto result = sess->sql(preQuery).execute();
        auto rows = result.fetchAll();
        QVariantList reservations;
        for (const auto& row : rows) {
            QVariantMap map;
            map["id_resv"] = row[0].get<int>();
            map["name_resv"] = row[1].get<std::string>().c_str();
            map["guest_resv"] = row[2].get<int>();
            map["date_resv"] = row[3].get<std::string>().c_str();
            map["time_resv"] = row[4].get<std::string>().c_str();
            map["status"] = row[5].get<int>();

            reservations.append(map);
        }
        emit reservationAdvanced(reservations);
    }
    catch (const mysqlx::Error& e) {
        qDebug() << "[SQL Error] " << e.what();
    }
}
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
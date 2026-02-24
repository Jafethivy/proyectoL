#include "db.h"

DB::DB(QObject* parent) : QObject(parent){
}

void DB::connectToDatabase() {
	try {
		Session sess("127.0.0.1", 33060, "root", "@bunnixsupremacy13@", "tecnm");

		qDebug() << "Connected to database successfully!\n";
	}
    catch (const mysqlx::Error& err) {
        return;
    }
    catch (std::exception& ex) {
        return;
    }

}

void DB::Login(const QString& user_id, const QString& password) {

}
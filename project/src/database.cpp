#include "database.h"

bool DataBase::connect()
{
	db = QSqlDatabase::addDatabase("QSQLITE");
	db.setDatabaseName(DATABASE_NAME);

	return db.open();
}

void DataBase::disconnect()
{
	db.close();
}

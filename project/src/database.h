#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QtSql>

#define DATABASE_NAME "DS.db"

class DataBase : public QObject
{
    Q_OBJECT
public:
	explicit DataBase(QObject *parent = nullptr) : QObject(parent) {}

public:
	bool connect();
	void disconnect();

private:
	QSqlDatabase db;
};

#endif // DATABASE_H

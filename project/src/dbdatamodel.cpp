#include "dbdatamodel.h"

#include <QSqlQuery>

namespace
{
	enum Columns
	{
		ID,
		DATA,
	};
}

DbDataModel::DbDataModel(QObject *parent)
	: QSqlQueryModel(parent)
{
}

DbDataModel::DbDataModel(const QString& query_text, QObject *parent)
	: QSqlQueryModel(parent)
{
	QSqlQuery query;
	query.prepare(query_text);
	query.exec();
	QSqlQueryModel::setQuery(query);
}

int DbDataModel::rowCount(const QModelIndex &parent) const
{
	return QSqlQueryModel::rowCount(parent);
}

QVariant DbDataModel::dataFromParent(QModelIndex index, int column) const
{
	return QSqlQueryModel::data(QSqlQueryModel::index(index.row(), column));
}

QVariant DbDataModel::data(const QModelIndex &item, int role) const
{
	QVariant result;

	switch( role )
	{
		case Qt::UserRole:
			result = dataFromParent(item, ID);
			break;
		case Qt::DisplayRole:
			result = dataFromParent(item, DATA);
			break;
		default:
			break;
	}

	return result;
}

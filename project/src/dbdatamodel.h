#ifndef DBDATAMODEL_H
#define DBDATAMODEL_H

#include <QSqlQueryModel>

class DbDataModel : public QSqlQueryModel
{
	Q_OBJECT
public:
	explicit DbDataModel(QObject *parent = nullptr);
	explicit DbDataModel(const QString& query_text, QObject *parent = nullptr);

public:
	virtual int rowCount(const QModelIndex &parent) const;
	virtual QVariant data(const QModelIndex &item, int role = Qt::DisplayRole) const;

private:
	QVariant dataFromParent(QModelIndex index, int column) const;

signals:

};

#endif // DBDATAMODEL_H

#ifndef AUXILIARYFUNCTIONS_H
#define AUXILIARYFUNCTIONS_H



#include <QApplication>
#include <QFile>
#include <QTextStream>
#include <QTextCodec>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QStandardItemModel>
#include <QTableView>

class AuxiliaryFunctions
{
public:
	AuxiliaryFunctions() = delete;

public:
	static bool read_file_to_string(const QString &filename, QString &str);
	static void fillStandardModelWithQueryData(QStandardItemModel *model, QString query_text);
	static double getDeliveryCost(QVector<int> &len, QVector<int> &wd, QVector<int> &hgt, QVector<double> &wt,
								   QVector<QString> &descr, QVector<int> &amt, QVector<double> &declVal);
	static int GetAddressIdIfExistElseCreateAddress(int streetId, QString houseNumber);
	static int cmpf(double a, double b, double epsilon = 0.005);
	static double roundoff(double value, int prec = 2);
	static void initModel(QSqlQueryModel *model, const QString &query_text, const QStringList &headers);
	static void setModelHeaderData(QAbstractItemModel *model, const QStringList &headers);
	static void setupTableView(QTableView *table_view, QAbstractItemModel *model, const QVector<int> &hiddenColumnIndexes);
};

#endif // AUXILIARYFUNCTIONS_H

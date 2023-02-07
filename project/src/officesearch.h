#ifndef OFFICESEARCH_H
#define OFFICESEARCH_H

#include "dbdatamodel.h"
#include "auxiliaryfunctions.h"

#include <QDialog>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QStandardItemModel>
#include <QSortFilterProxyModel>
#include <QTableView>
#include <QMessageBox>
#include <QCompleter>
#include <QSqlRecord>

namespace Ui {
class OfficeSearch;
}

class OfficeSearch : public QDialog
{
	Q_OBJECT

public:
	enum class WhichOfficesToFind;
	explicit OfficeSearch(WhichOfficesToFind what_find, int related_ofc, QWidget *parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());
	explicit OfficeSearch(WhichOfficesToFind what_find, QWidget *parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());
	~OfficeSearch();

public:
	enum class WhichOfficesToFind
	{
		AllOffices,
		CargoOffices,
		OfficesRelatedToThisOffice
	};

	enum class OfficeType
	{
		Postal = 1,
		Cargo
	};

	struct officeData {
		int id;
		int num;
		OfficeType type;
		int addrId;
		QString addr;

		officeData()
			: id(0),
			  num(0),
			  type(static_cast<OfficeType>(0)),
			  addrId(0),
			  addr("") {}
	};

public:
	officeData getOfficeData() { return office; }
	void changeWhatOfficeToFind(WhichOfficesToFind what_find);

private slots:
	void on_pbOK_clicked();
	void on_pbCancel_clicked();
	void on_tvwOfc_clicked(const QModelIndex &index);
	void on_cmbLoc_currentIndexChanged(int index);
	void on_cmbLoc_currentTextChanged(const QString &arg1);

private:
	void init(WhichOfficesToFind what_find);
	void updatetvwOfc();

private:
	Ui::OfficeSearch *ui;

	QStandardItemModel *locMdl;
	QCompleter *locCmpltr;
	QSqlQueryModel *ofcMdl;

	officeData office;
	bool locIsPresent = false;
	bool prevLocId = 0;

	QString ofcTextQuery;
	QString locTextQuery;
	QString locality_id = " and loc.id = %1";
	int relatedOffice;
};

#endif // OFFICESEARCH_H

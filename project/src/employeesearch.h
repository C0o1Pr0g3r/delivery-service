#ifndef EMPLOYEESEARCH_H
#define EMPLOYEESEARCH_H

#include "auxiliaryfunctions.h"
#include "dsdbmapper.h"

#include <QDialog>
#include <QStandardItemModel>
#include <QCompleter>

namespace Ui {
class EmployeeSearch;
}

class EmployeeSearch : public QDialog
{
	Q_OBJECT

public:
	enum class WhichEmployeeToFind;

public:
	explicit EmployeeSearch(WhichEmployeeToFind what_find, DsDbMapper::Office certain_office = DsDbMapper::Office(), QWidget *parent = nullptr);
	~EmployeeSearch();

public:
	enum class WhichEmployeeToFind {
		All,
		RelatedToCertainOffice
	};

public:
	DsDbMapper::Employee getEmployee() { return employee; }

private slots:
	void on_pbOK_clicked();
	void on_pbCancel_clicked();

private:
	void init(WhichEmployeeToFind what_find);

private:
	Ui::EmployeeSearch *ui;

	QStandardItemModel *employeeMdl;
	QCompleter *employeeCmpltr;
	DsDbMapper::Employee employee;
	DsDbMapper::Office certainOffice;
};

#endif // EMPLOYEESEARCH_H

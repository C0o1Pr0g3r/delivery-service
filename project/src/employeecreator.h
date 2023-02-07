#ifndef EMPLOYEECREATOR_H
#define EMPLOYEECREATOR_H

#include "auxiliaryfunctions.h"
#include "dbdatamodel.h"
#include "dsdbmapper.h"
#include "childwindow.h"
#include "passwordcreator.h"

#include <QWidget>
#include <QMessageBox>

namespace Ui {
class EmployeeCreator;
}

class EmployeeCreator : public ChildWindow
{
	Q_OBJECT

public:
	explicit EmployeeCreator(DsDbMapper::Office office/* = DsDbMapper::Office()*/, QWidget *parent = nullptr);
	~EmployeeCreator();

signals:
	void closed();

private slots:
	void on_pbCreateEmployee_clicked();

	void on_pbCancel_clicked();

private:
	void closeEvent(QCloseEvent *event);
	void init();

private:
	Ui::EmployeeCreator *ui;

	DbDataModel *postModel;
	DbDataModel *ofcModel;
	DsDbMapper::Office employeeOffice;
};

#endif // EMPLOYEECREATOR_H

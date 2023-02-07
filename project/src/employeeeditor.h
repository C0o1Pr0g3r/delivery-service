#ifndef EMPLOYEEEDITOR_H
#define EMPLOYEEEDITOR_H

#include "auxiliaryfunctions.h"
#include "dbdatamodel.h"
#include "dsdbmapper.h"
#include "childwindow.h"

#include <QWidget>
#include <QSqlQuery>
#include <QMessageBox>

namespace Ui {
class EmployeeEditor;
}

class EmployeeEditor : public ChildWindow
{
	Q_OBJECT

public:
	explicit EmployeeEditor(QWidget *parent = nullptr);
	EmployeeEditor(int employee_id, QWidget *parent = nullptr);
	~EmployeeEditor();

signals:
	void closed();

private slots:
	void on_pbUpdateEmployeeData_clicked();
	void on_pbCancel_clicked();

private:
	void closeEvent(QCloseEvent *event);
	void init();
	void fillFormWithData(int employee_id);

private:
	Ui::EmployeeEditor *ui;

	int employeeID;
	DbDataModel *postModel;
	DbDataModel *ofcModel;
	DsDbMapper::Office office;
};

#endif // EMPLOYEEEDITOR_H

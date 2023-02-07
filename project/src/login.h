#ifndef LOGIN_H
#define LOGIN_H

#include "auxiliaryfunctions.h"
#include "childwindow.h"
#include "dsdbmapper.h"

#include <QWidget>
#include <QStandardItemModel>
#include <QCompleter>
#include <QDialog>
#include <QMessageBox>
#include <QCryptographicHash>

namespace Ui {
class Login;
}

class Login : public ChildWindow
{
	Q_OBJECT

public:
	explicit Login(int *employee_id, QWidget *parent = nullptr);
	~Login();

private slots:
	void on_pbLogin_clicked();
	void on_pbCancel_clicked();

	void on_tbShowHidePassword_clicked(bool checked);

private:
	void init();

private:
	Ui::Login *ui;

	QStandardItemModel *eeMdl;
	QCompleter *eeCmpltr;
	int *employeeId;
};

#endif // LOGIN_H

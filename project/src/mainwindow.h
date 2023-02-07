#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "mailingcreator.h"
#include "mailingeditor.h"
#include "mailingsearch.h"
#include "shpcreator.h"
#include "shpeditor.h"
#include "shpsearch.h"
#include "stgcreator.h"
#include "addressselector.h"
#include "clientcreator.h"
#include "clienteditor.h"
#include "employeecreator.h"
#include "employeeeditor.h"
#include "employeesearch.h"
#include "database.h"
#include "login.h"

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = nullptr);
	~MainWindow();

signals:
	void showed();

private slots:
	void on_pbCreateMailing_clicked();

	void on_pbEditMailing_clicked();

	void on_pbCreateShp_clicked();

	void on_pbEditShp_clicked();

	void on_pbCreateStg_clicked();

	void on_pbCreateClient_clicked();

	void on_pbEditClient_clicked();

	void on_pbCreateEmployee_clicked();

	void on_pbEditEmployee_clicked();

	void handleLogin();

private:
	void init();

private:
	Ui::MainWindow *ui;

	DataBase *db;
	DsDbMapper::Employee employee;
	DsDbMapper::Office office;
};
#endif // MAINWINDOW_H

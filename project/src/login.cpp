#include "login.h"
#include "ui_login.h"

Login::Login(int *employee_id, QWidget *parent) :
	ChildWindow(parent),
	ui(new Ui::Login),
	employeeId(employee_id)
{
	ui->setupUi(this);
	init();
}

Login::~Login()
{
	delete ui;
}

void Login::init()
{
	eeMdl = new QStandardItemModel(this);
	QString query_text;
	AuxiliaryFunctions::read_file_to_string("sql/login/employee.sql", query_text);
	AuxiliaryFunctions::fillStandardModelWithQueryData(eeMdl, query_text);
	eeCmpltr = new QCompleter(eeMdl);
	eeCmpltr->setCaseSensitivity(Qt::CaseInsensitive);
	eeCmpltr->setCompletionRole(Qt::DisplayRole);
	ui->cmbEe->setCompleter(eeCmpltr);
	ui->cmbEe->setModel(eeMdl);
	ui->lePassword->setEchoMode(QLineEdit::Password);
}

void Login::on_pbLogin_clicked()
{
	QString query_text = QString("select password from passwords where ee_id = %1").arg(ui->cmbEe->currentData().toInt());
	QSqlQuery query;
	query.exec(query_text);
	if (query.next())
	{
		if (ui->lePassword->text() == query.value(0).toByteArray())
		{
			*employeeId = ui->cmbEe->currentData().toInt();
			close();
		}
		else
		{
			QMessageBox msgBox(QMessageBox::Warning, "Помилка", "Невірний пароль!", QMessageBox::Ok);
			msgBox.exec();
			return;
		}
	}
	else
	{
		QMessageBox msgBox(QMessageBox::Warning, "Помилка", "Виникла непередбачувана помилка. Спробуйте ще раз.", QMessageBox::Ok);
		msgBox.exec();
		return;
	}
}

void Login::on_pbCancel_clicked()
{
	*employeeId = 0;
	close();
}

void Login::on_tbShowHidePassword_clicked(bool checked)
{
	if (checked)
	{
		ui->lePassword->setEchoMode(QLineEdit::Normal);
	}
	else
	{
		ui->lePassword->setEchoMode(QLineEdit::Password);
	}
}

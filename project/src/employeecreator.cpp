#include "employeecreator.h"
#include "ui_employeecreator.h"

EmployeeCreator::EmployeeCreator(DsDbMapper::Office office, QWidget *parent) :
	ChildWindow(parent),
	ui(new Ui::EmployeeCreator),
	employeeOffice(office)
{
	ui->setupUi(this);
	init();
}

EmployeeCreator::~EmployeeCreator()
{
	delete ui;
}

void EmployeeCreator::closeEvent(QCloseEvent *event)
{
	emit closed();
}

void EmployeeCreator::init()
{
	postModel = new DbDataModel("select * from post", this);
	ui->cmbPost->setModel(postModel);
	ofcModel = new DbDataModel(this);
	QString query_text;
	AuxiliaryFunctions::read_file_to_string("sql/employee-creator/certain_office.sql", query_text);
	query_text = query_text.arg(employeeOffice.id);
	ofcModel->setQuery(query_text);
	ui->cmbOfc->setModel(ofcModel);
}

void EmployeeCreator::on_pbCreateEmployee_clicked()
{
	if (	ui->leLastname->text().isEmpty()
		||	ui->leFirstname->text().isEmpty()
		||	ui->lePatronymic->text().isEmpty()
		||	ui->leMob->text().isEmpty())
	{
		QMessageBox msgBox(QMessageBox::Warning, "Помилка", "Не всі поля заповнені.", QMessageBox::Ok);
		msgBox.exec();
		return;
	}

	QString query_text;
	QSqlQuery query;

	query_text = QString("select id from staff where mob = '%1'").arg(ui->leMob->text());
	query.exec(query_text);
	if (query.next())
	{
		QMessageBox msgBox(QMessageBox::Warning, "Помилка", "Співробітник з вказаним номером телефона вже існує.", QMessageBox::Ok);
		msgBox.exec();
		return;
	}

	QString password;
	int employee_id = 0;
	bool need_to_continue = false;

	if (ui->cmbPost->currentData().toInt() == 1)
	{
		PasswordCreator pc;
		if (pc.exec() == QDialog::Accepted)
		{
			password = pc.getPassword();
			need_to_continue = true;
		}
	}
	else
	{
		need_to_continue = true;
	}

	if (need_to_continue)
	{
		query_text = "insert into staff (lastname, firstname, patronymic, DOB, mob, post_id, ofc_id) values (:lastname, :firstname, :patronymic, :DOB, :mob, :post_id, :ofc_id)";
		query.prepare(query_text);
		query.bindValue(":lastname", ui->leLastname->text());
		query.bindValue(":firstname", ui->leFirstname->text());
		query.bindValue(":patronymic", ui->lePatronymic->text());
		query.bindValue(":DOB", ui->deDOB->date().toString(Qt::ISODate));
		query.bindValue(":mob", ui->leMob->text());
		query.bindValue(":post_id", ui->cmbPost->currentData().toInt());
		query.bindValue(":ofc_id", ui->cmbOfc->currentData().toInt());
		query.exec();

		employee_id = query.lastInsertId().toInt();

		if (ui->cmbPost->currentData().toInt() == 1)
		{
			query_text = "insert into passwords (ee_id, password) values (:ee_id, :password)";
			query.prepare(query_text);
			query.bindValue(":ee_id", employee_id);
			query.bindValue(":password", password);
			query.exec();
		}

		QMessageBox msgBox(QMessageBox::Information, "Операція завершена", "Співробітника успішно створено.", QMessageBox::Ok);
		msgBox.exec();

		close();
	}
}

void EmployeeCreator::on_pbCancel_clicked()
{
	close();
}

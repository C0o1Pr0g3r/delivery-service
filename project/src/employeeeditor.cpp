#include "employeeeditor.h"
#include "ui_employeeeditor.h"

EmployeeEditor::EmployeeEditor(QWidget *parent) :
	ChildWindow(parent),
	ui(new Ui::EmployeeEditor)
{
	ui->setupUi(this);
}

EmployeeEditor::EmployeeEditor(int employee_id, QWidget *parent) :
	EmployeeEditor(parent)
{
	employeeID = employee_id;
	init();
	fillFormWithData(employee_id);
}

EmployeeEditor::~EmployeeEditor()
{
	delete ui;
}

void EmployeeEditor::on_pbUpdateEmployeeData_clicked()
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

	query_text = QString("select id from staff where mob = '%1' and id <> %2").arg(ui->leMob->text()).arg(employeeID);
	query.exec(query_text);
	if (query.next())
	{
		QMessageBox msgBox(QMessageBox::Warning, "Помилка", "Співробітник з вказаним номером телефона вже існує.", QMessageBox::Ok);
		msgBox.exec();
		return;
	}

	query_text = QString("update staff set lastname = :lastname, firstname = :firstname, patronymic = :patronymic, DOB = :DOB, mob = :mob, post_id = :post_id, ofc_id = :ofc_id where id = %1").arg(employeeID);
	query.prepare(query_text);
	query.bindValue(":lastname", ui->leLastname->text());
	query.bindValue(":firstname", ui->leFirstname->text());
	query.bindValue(":patronymic", ui->lePatronymic->text());
	query.bindValue(":DOB", ui->deDOB->date().toString(Qt::ISODate));
	query.bindValue(":mob", ui->leMob->text());
	query.bindValue(":post_id", ui->cmbPost->currentData().toInt());
	query.bindValue(":ofc_id", ui->cmbOfc->currentData().toInt());
	query.exec();
}

void EmployeeEditor::on_pbCancel_clicked()
{
	close();
}

void EmployeeEditor::closeEvent(QCloseEvent *event)
{
	emit closed();
}

void EmployeeEditor::init()
{
	postModel = new DbDataModel("select * from post", this);
	ui->cmbPost->setModel(postModel);
	ofcModel = new DbDataModel(this);
	QString query_text;
	AuxiliaryFunctions::read_file_to_string("sql/employee-creator/all-offices.sql", query_text);
	ofcModel->setQuery(query_text);
	ui->cmbOfc->setModel(ofcModel);
}

void EmployeeEditor::fillFormWithData(int employee_id)
{
	QString query_text;
	QSqlQuery query;

	query_text = QString("select * from staff where id = %1").arg(employee_id);
	query.exec(query_text);
	if (query.next())
	{
		ui->leLastname->setText(query.value(1).toString());
		ui->leFirstname->setText(query.value(2).toString());
		ui->lePatronymic->setText(query.value(3).toString());
		ui->deDOB->setDate(QDate::fromString(query.value(4).toString(), Qt::ISODate));
		ui->leMob->setText(query.value(5).toString());
		ui->cmbPost->setCurrentIndex(ui->cmbPost->findData(query.value(6)));
		ui->cmbOfc->setCurrentIndex(ui->cmbOfc->findData(query.value(7)));
	}
}

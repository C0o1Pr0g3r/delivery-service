#include "clientcreator.h"
#include "ui_clientcreator.h"

ClientCreator::ClientCreator(QWidget *parent) :
	ChildWindow(parent),
	ui(new Ui::ClientCreator)
{
	ui->setupUi(this);
}

ClientCreator::~ClientCreator()
{
	delete ui;
}

void ClientCreator::on_rbIndv_toggled(bool checked)
{
	if (checked)
	{
		ui->grpInfo->setTitle("Персональна інформація");
		ui->stackedWidget->setCurrentIndex(0);
	}
	else
	{
		ui->grpInfo->setTitle("Загальна інформація");
		ui->stackedWidget->setCurrentIndex(1);
	}
}

void ClientCreator::closeEvent(QCloseEvent *event)
{
	emit closed();
}

void ClientCreator::on_pbCreateClient_clicked()
{
	QString query_text;
	QSqlQuery query;
	int client_sub_id;

	if (ui->rbIndv->isChecked())
	{
		if (	ui->leIndvFirstname->text().isEmpty()
			||	ui->leIndvLastname->text().isEmpty()
			||	ui->leIndvPatronymic->text().isEmpty()
			||	ui->leMob->text().isEmpty())
		{
			QMessageBox msgBox(QMessageBox::Warning, "Помилка", "Не всі поля заповнені.", QMessageBox::Ok);
			msgBox.exec();
			return;
		}
		else
		{
		}

		query_text = QString("select id from indv where mob = '%1'").arg(ui->leMob->text());
		query.exec(query_text);
		if (query.next())
		{
			QMessageBox msgBox(QMessageBox::Warning, "Помилка", "Приватна особа з вказаним номером телефона вже існує.", QMessageBox::Ok);
			msgBox.exec();
			return;
		}

		query_text = "insert into indv (lastname, firstname, patronymic, mob) values (:lastname, :firstname, :patronymic, :mob)";
		query.prepare(query_text);
		query.bindValue(":lastname", ui->leIndvLastname->text());
		query.bindValue(":firstname", ui->leIndvFirstname->text());
		query.bindValue(":patronymic", ui->leIndvPatronymic->text());
		query.bindValue(":mob", ui->leMob->text());
		query.exec();

		client_sub_id = query.lastInsertId().toInt();

		query_text = "insert into client (indv_id) values (:indv_id)";
		query.prepare(query_text);
		query.bindValue(":indv_id", client_sub_id);
		query.exec();
	}
	else
	{
		if (	ui->leOrgName->text().isEmpty()
			||	ui->leOrgUSREOU->text().isEmpty()
			||	ui->leOrgDirector->text().isEmpty()
			||	ui->leMob->text().isEmpty())
		{
			QMessageBox msgBox(QMessageBox::Warning, "Помилка", "Не всі поля заповнені.", QMessageBox::Ok);
			msgBox.exec();
			return;
		}

		bool exit = false;
		QString query_text2;
		QSqlQuery query2;
		query_text = QString("select id from org where mob = '%1'").arg(ui->leMob->text());
		query_text2 = QString("select id from org where USREOU = '%1'").arg(ui->leOrgUSREOU->text());
		query.exec(query_text);
		query2.exec(query_text2);
		if (query.next())
		{
			QMessageBox msgBox(QMessageBox::Warning, "Помилка", "Організація з вказаним номером телефона вже існує.", QMessageBox::Ok);
			msgBox.exec();
			exit = true;
		}
		if (query2.next())
		{
			QMessageBox msgBox(QMessageBox::Warning, "Помилка", "Організація з вказаним ЄДРПОУ вже існує.", QMessageBox::Ok);
			msgBox.exec();
			exit = true;
		}
		if (exit)
		{
			return;
		}

		query_text = "insert into org (title, USREOU, director, mob) values (:title, :USREOU, :director, :mob)";
		query.prepare(query_text);
		query.bindValue(":title", ui->leOrgName->text());
		query.bindValue(":USREOU", ui->leOrgUSREOU->text());
		query.bindValue(":director", ui->leOrgDirector->text());
		query.bindValue(":mob", ui->leMob->text());
		query.exec();

		client_sub_id = query.lastInsertId().toInt();

		query_text = "insert into client (org_id) values (:org_id)";
		query.prepare(query_text);
		query.bindValue(":org_id", client_sub_id);
		query.exec();
	}

	QMessageBox msgBox(QMessageBox::Information, "Операція завершена", "Клієнта успішно створено.", QMessageBox::Ok);
	msgBox.exec();

	close();
}

void ClientCreator::on_pbCancel_clicked()
{
	close();
}

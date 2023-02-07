#include "clienteditor.h"
#include "ui_clienteditor.h"

ClientEditor::ClientEditor(QWidget *parent) :
	ChildWindow(parent),
	ui(new Ui::ClientEditor)
{
	ui->setupUi(this);
}

ClientEditor::ClientEditor(int clientId, bool isIndv, QWidget *parent) :
	ClientEditor(parent)
{
	clientID = clientId;
	this->isIndv = isIndv;
	fillFormWithData(clientId, isIndv);
}

ClientEditor::~ClientEditor()
{
	delete ui;
}

void ClientEditor::closeEvent(QCloseEvent *event)
{
	emit closed();
}

void ClientEditor::fillFormWithData(int client_id, bool is_indv)
{
	QString query_text;
	QSqlQuery query;
	int client_sub_id = 0;

	query_text = QString("select * from client where id = %1").arg(client_id);
	query.exec(query_text);
	if (query.next())
	{
		if (is_indv)
		{
			client_sub_id = query.value(1).toInt();
		}
		else
		{
			client_sub_id = query.value(2).toInt();
		}
	}

	if (is_indv)
	{
		query_text = QString("select * from indv where id = %1").arg(client_sub_id);
		query.exec(query_text);
	}
	else
	{
		query_text = QString("select * from org where id = %1").arg(client_sub_id);
		query.exec(query_text);
		ui->lFirst->setText("Назва");
		ui->lSecond->setText("ЄДРПОУ");
		ui->lThird->setText("Директор");
		ui->leSecond->setInputMask("00000000");
	}
	if (query.next())
	{
		ui->leFirst->setText(query.value(1).toString());
		ui->leSecond->setText(query.value(2).toString());
		ui->leThird->setText(query.value(3).toString());
		ui->leMob->setText(query.value(5).toString());
	}

	clientSubID = client_sub_id;
}

void ClientEditor::on_pbUpdateClientData_clicked()
{
	if (	ui->leFirst->text().isEmpty()
		||	ui->leSecond->text().isEmpty()
		||	ui->leThird->text().isEmpty()
		||	ui->leMob->text().isEmpty())
	{
		QMessageBox msgBox(QMessageBox::Warning, "Помилка", "Не всі поля заповнені.", QMessageBox::Ok);
		msgBox.exec();
		return;
	}

	QString query_text;
	QSqlQuery query;

	if (isIndv)
	{
		query_text = QString("select id from indv where mob = '%1' and id <> %2").arg(ui->leMob->text()).arg(clientSubID);
		query.exec(query_text);
		if (query.next())
		{
			QMessageBox msgBox(QMessageBox::Warning, "Помилка", "Приватна особа з вказаним номером телефона вже існує.", QMessageBox::Ok);
			msgBox.exec();
			return;
		}

		query_text = QString("update indv set lastname = :lastname, firstname = :firstname, patronymic = :patronymic, mob = :mob where id = %1").arg(clientSubID);
		query.prepare(query_text);
		query.bindValue(":lastname", ui->leFirst->text());
		query.bindValue(":firstname", ui->leSecond->text());
		query.bindValue(":patronymic", ui->leThird->text());
		query.bindValue(":mob", ui->leMob->text());
		query.exec();
	}
	else
	{
		bool exit = false;
		QString query_text2;
		QSqlQuery query2;
		query_text = QString("select id from org where mob = '%1' and id <> %2").arg(ui->leMob->text()).arg(clientSubID);
		query_text2 = QString("select id from org where USREOU = '%1' and id <> %2").arg(ui->leSecond->text()).arg(clientSubID);
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

		query_text = QString("update org set title = :title, USREOU = :USREOU, director = :director, mob = :mob where id = %1").arg(clientSubID);
		query.prepare(query_text);
		query.bindValue(":title", ui->leFirst->text());
		query.bindValue(":USREOU", ui->leSecond->text());
		query.bindValue(":director", ui->leThird->text());
		query.bindValue(":mob", ui->leMob->text());
		query.exec();
	}
}

void ClientEditor::on_pbCancel_clicked()
{
	close();
}

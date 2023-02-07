#include "passwordcreator.h"
#include "ui_passwordcreator.h"

PasswordCreator::PasswordCreator(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::PasswordCreator)
{
	ui->setupUi(this);
	init();
}

PasswordCreator::~PasswordCreator()
{
	delete ui;
}

void PasswordCreator::init()
{
	ui->lePassword->setEchoMode(QLineEdit::Password);
}

QString PasswordCreator::getPassword()
{
	return ui->lePassword->text();
}

void PasswordCreator::on_tbShowHidePassword_clicked(bool checked)
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

void PasswordCreator::on_pbOK_clicked()
{
	accept();
}

void PasswordCreator::on_pbCancel_clicked()
{
	reject();
}

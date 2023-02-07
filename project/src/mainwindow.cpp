#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
	, ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	init();
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::init()
{
	db = new DataBase();
	db->connect();

	Login *l = new Login(&employee.id);
	connect(l, &Login::closed, this, &MainWindow::handleLogin);
	connect(this, &MainWindow::showed, l, &Login::deleteLater);
	l->show();
}

void MainWindow::handleLogin()
{
	if (employee.id == 0)
	{
		QApplication::quit();
	}

	QSqlQuery query;
	QString query_text;

	AuxiliaryFunctions::read_file_to_string("sql/login/employee-with-office.sql", query_text);
	query.exec(QString(query_text).arg(employee.id));
	if (query.next())
	{
		employee.id = query.value(0).toInt();
		employee.LFP = query.value(1).toString();
		employee.post_str = query.value(2).toString();
		office.id = query.value(3).toInt();
		office.number = query.value(4).toInt();
		office.type_str = query.value(5).toString();
		office.loc_str = query.value(6).toString();
		office.address_str = query.value(7).toString();
	}

	QString status_bar_str = QString("%1: %2 | Відділення №%3, %4").arg(employee.post_str).arg(employee.LFP).arg(office.number).arg(office.loc_str);
	statusBar()->showMessage(status_bar_str);
	show();
	emit showed();
}

void MainWindow::on_pbCreateMailing_clicked()
{
	MailingCreator *mc = new MailingCreator(office);
	connect(mc, &MailingCreator::closed, &MailingCreator::deleteLater);
	connect(mc, &MailingCreator::closed, this, &MainWindow::show);
	mc->show();
	this->close();
}

void MainWindow::on_pbEditMailing_clicked()
{
	MailingSearch ms(MailingSearch::WhichMailingsToFind::RelatedToCertainOffice, office);
	if (ms.exec() == QDialog::Accepted)
	{
		MailingEditor *me = new MailingEditor(ms.getMailingId());
		connect(me, &MailingEditor::closed, &MailingEditor::deleteLater);
		connect(me, &MailingEditor::closed, this, &MainWindow::show);
		me->show();
		this->close();
	}
}

void MainWindow::on_pbCreateShp_clicked()
{
	ShpCreator *rm = new ShpCreator(office);
	connect(rm, &ShpCreator::closed, &ShpCreator::deleteLater);
	connect(rm, &ShpCreator::closed, this, &MainWindow::show);
	rm->show();
	this->close();
}

void MainWindow::on_pbEditShp_clicked()
{
	ShpSearch ss(ShpSearch::WhicShpToFind::RelatedToCertainOffice, office);
	if (ss.exec() == QDialog::Accepted)
	{
		ShpEditor *me = new ShpEditor(ss.getShpId());
		connect(me, &ShpEditor::closed, &ShpEditor::deleteLater);
		connect(me, &ShpEditor::closed, this, &MainWindow::show);
		me->show();
		this->close();
	}
}

void MainWindow::on_pbCreateStg_clicked()
{
	StgCreator *sc = new StgCreator(office);
	connect(sc, &StgCreator::closed, &StgCreator::deleteLater);
	connect(sc, &StgCreator::closed, this, &MainWindow::show);
	sc->show();
	this->close();
}

void MainWindow::on_pbCreateClient_clicked()
{
	ClientCreator *ec = new ClientCreator();
	connect(ec, &ClientCreator::closed, &ClientCreator::deleteLater);
	connect(ec, &ClientCreator::closed, this, &MainWindow::show);
	ec->show();
	this->close();
}

void MainWindow::on_pbEditClient_clicked()
{
	ClientSearch cs(true, nullptr, Qt::SubWindow);
	if (cs.exec() == QDialog::Accepted)
	{
		ClientSearch::clientData client = cs.getClientData();
		int client_id = client.id;
		bool isIndv = static_cast<int>(client.type) == 1 ? true : false;
		ClientEditor *ce = new ClientEditor(client_id, isIndv);
		connect(ce, &ClientEditor::closed, &ClientEditor::deleteLater);
		connect(ce, &ClientEditor::closed, this, &MainWindow::show);
		ce->show();
		this->close();
	}
}

void MainWindow::on_pbCreateEmployee_clicked()
{
	EmployeeCreator *ec = new EmployeeCreator(office);
	connect(ec, &EmployeeCreator::closed, &EmployeeCreator::deleteLater);
	connect(ec, &EmployeeCreator::closed, this, &MainWindow::show);
	ec->show();
	this->close();
}

void MainWindow::on_pbEditEmployee_clicked()
{
	EmployeeSearch es(EmployeeSearch::WhichEmployeeToFind::RelatedToCertainOffice, office);
	if (es.exec() == QDialog::Accepted)
	{
		EmployeeEditor *ee = new EmployeeEditor(es.getEmployee().id);
		connect(ee, &EmployeeEditor::closed, &EmployeeEditor::deleteLater);
		connect(ee, &EmployeeEditor::closed, this, &MainWindow::show);
		ee->show();
		this->close();
	}
}

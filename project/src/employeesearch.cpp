#include "employeesearch.h"
#include "ui_employeesearch.h"

EmployeeSearch::EmployeeSearch( WhichEmployeeToFind what_find, DsDbMapper::Office certain_office, QWidget *parent) :
	QDialog(parent),
	ui(new Ui::EmployeeSearch),
	certainOffice(certain_office)
{
	ui->setupUi(this);
	init(what_find);
}

EmployeeSearch::~EmployeeSearch()
{
	delete ui;
}

void EmployeeSearch::init(WhichEmployeeToFind what_find)
{
	employeeMdl = new QStandardItemModel(this);
	QString query_text;
	if (what_find == WhichEmployeeToFind::All)
	{
		AuxiliaryFunctions::read_file_to_string("sql/employee-search/all-employees.sql", query_text);
	}
	else if (what_find == WhichEmployeeToFind::RelatedToCertainOffice)
	{
		AuxiliaryFunctions::read_file_to_string("sql/employee-search/employees-related-to-certain-office.sql", query_text);
		query_text = query_text.arg(certainOffice.id);
	}
	AuxiliaryFunctions::fillStandardModelWithQueryData(employeeMdl, query_text);
	employeeCmpltr = new QCompleter(employeeMdl);
	employeeCmpltr->setCaseSensitivity(Qt::CaseInsensitive);
	employeeCmpltr->setCompletionRole(Qt::DisplayRole);
	ui->cmbEmployeeId->setCompleter(employeeCmpltr);
	ui->cmbEmployeeId->setModel(employeeMdl);
}

void EmployeeSearch::on_pbOK_clicked()
{
	employee.id = ui->cmbEmployeeId->currentData().toInt();
	accept();
}

void EmployeeSearch::on_pbCancel_clicked()
{
	reject();
}

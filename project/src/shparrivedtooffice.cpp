#include "shparrivedtooffice.h"
#include "ui_shparrivedtooffice.h"

ShpArrivedToOffice::ShpArrivedToOffice(DsDbMapper::Office arrival_office, QWidget *parent) :
	QDialog(parent),
	ui(new Ui::ShpArrivedToOffice),
	arrivalOffice(arrival_office)
{
	ui->setupUi(this);
	init();
}

ShpArrivedToOffice::~ShpArrivedToOffice()
{
	delete ui;
}

void ShpArrivedToOffice::init()
{
	QString query_text;
	AuxiliaryFunctions::read_file_to_string("sql/shp-creator/ee-who-give-mailings.sql", query_text);
	query_text = query_text.arg(arrivalOffice.id);
	eeMdl = new DbDataModel(this);
	eeMdl->setQuery(query_text);
	ui->cmbEeWhoTake->setModel(eeMdl);
	ui->dte->setDateTime(QDateTime::currentDateTime());
}

void ShpArrivedToOffice::on_pbOK_clicked()
{
	date_and_time = ui->dte->dateTime();
	employee.id = ui->cmbEeWhoTake->currentData().toInt();
	employee.LFP = ui->cmbEeWhoTake->currentData(Qt::DisplayRole).toString();
	accept();
}

void ShpArrivedToOffice::on_pbCancel_clicked()
{
	reject();
}

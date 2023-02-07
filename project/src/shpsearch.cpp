#include "shpsearch.h"
#include "ui_shpsearch.h"

ShpSearch::ShpSearch(WhicShpToFind what_find, DsDbMapper::Office office, QWidget *parent, Qt::WindowFlags f) :
	QDialog(parent, f),
	ui(new Ui::ShpSearch),
	certainOffice(office)
{
	ui->setupUi(this);
	init(what_find);
}

ShpSearch::~ShpSearch()
{
	delete ui;
}

void ShpSearch::init(WhicShpToFind what_find)
{
	shpMdl = new QStandardItemModel(this);
	QString query_text;
	if (what_find == WhicShpToFind::AllActive)
	{
		AuxiliaryFunctions::read_file_to_string("sql/shp-search/all-active-shippings.sql", query_text);
	}
	else if (what_find == WhicShpToFind::RelatedToCertainOffice)
	{
		AuxiliaryFunctions::read_file_to_string("sql/shp-search/active-shippings-related-to-certain-office.sql", query_text);
		query_text = QString(query_text).arg(certainOffice.id).arg(certainOffice.id);
	}
	AuxiliaryFunctions::fillStandardModelWithQueryData(shpMdl, query_text);
	shpCmpltr = new QCompleter(shpMdl);
	shpCmpltr->setCaseSensitivity(Qt::CaseInsensitive);
	shpCmpltr->setCompletionRole(Qt::DisplayRole);
	ui->cmbShpId->setCompleter(shpCmpltr);
	ui->cmbShpId->setModel(shpMdl);
}

void ShpSearch::on_pb_OK_clicked()
{
	shpId = ui->cmbShpId->itemData(ui->cmbShpId->currentIndex(), Qt::UserRole).toInt();
	accept();
}

void ShpSearch::on_pb_cancel_clicked()
{
	reject();
}

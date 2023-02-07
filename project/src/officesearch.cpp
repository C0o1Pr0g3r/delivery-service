#include "officesearch.h"
#include "ui_officesearch.h"

OfficeSearch::OfficeSearch(WhichOfficesToFind what_find, int related_ofc, QWidget *parent, Qt::WindowFlags f) :
	QDialog(parent, f),
	ui(new Ui::OfficeSearch),
	office(),
	relatedOffice(related_ofc)
{
	ui->setupUi(this);
	init(what_find);
}

OfficeSearch::OfficeSearch(WhichOfficesToFind what_find, QWidget *parent, Qt::WindowFlags f) :
	OfficeSearch(what_find, 0, parent, f)
{
}

OfficeSearch::~OfficeSearch()
{
	delete ui;
}

void OfficeSearch::changeWhatOfficeToFind(WhichOfficesToFind what_find)
{
	ofcTextQuery = "";
	if (what_find == WhichOfficesToFind::AllOffices)
	{
		AuxiliaryFunctions::read_file_to_string("sql/office-search/all-offices.sql", ofcTextQuery);
		AuxiliaryFunctions::read_file_to_string("sql/office-search/all-locs.sql", locTextQuery);
	}
	else if (what_find == WhichOfficesToFind::CargoOffices)
	{
		AuxiliaryFunctions::read_file_to_string("sql/office-search/cargo-offices.sql", ofcTextQuery);
		AuxiliaryFunctions::read_file_to_string("sql/office-search/all-locs.sql", locTextQuery);
	}
	else if (what_find == WhichOfficesToFind::OfficesRelatedToThisOffice)
	{
		AuxiliaryFunctions::read_file_to_string("sql/office-search/offices-related-to-this-office.sql", ofcTextQuery);
		ofcTextQuery.replace("related_office", QString::number(relatedOffice));
		AuxiliaryFunctions::read_file_to_string("sql/office-search/locs-for-offices-related-to-this-office.sql", locTextQuery);
		locTextQuery.replace("related_office", QString::number(relatedOffice));
		locality_id = " and loc_1.id = %1";
	}
}

void OfficeSearch::on_tvwOfc_clicked(const QModelIndex &index)
{
	office.id = ofcMdl->record(index.row()).value(0).toInt();
	office.num = ofcMdl->record(index.row()).value(1).toInt();
	office.addrId = ofcMdl->record(index.row()).value(3).toInt();
	office.addr = ofcMdl->record(index.row()).value(4).toString();
	ui->lCurrentOffice->setText(QString("Відділення №%1 | %2").arg(office.num).arg(office.addr));
}

void OfficeSearch::on_cmbLoc_currentIndexChanged(int index)
{
	locIsPresent = index != -1 ? true : false;
	updatetvwOfc();
	prevLocId = ui->cmbLoc->currentData().toInt();
}

void OfficeSearch::on_cmbLoc_currentTextChanged(const QString &arg1)
{
	locIsPresent = false;
	updatetvwOfc();
}

void OfficeSearch::on_pbOK_clicked()
{
	if (office.id == 0)
	{
		QMessageBox msgBox(QMessageBox::Warning, "Помилка", "Ви не обрали відділення.", QMessageBox::Ok);
		msgBox.exec();
		return;
	}

	accept();
}

void OfficeSearch::on_pbCancel_clicked()
{
	reject();
}

void OfficeSearch::init(WhichOfficesToFind what_find)
{
	changeWhatOfficeToFind(what_find);

	QString query_text(ofcTextQuery);
	QStringList headers {"ID", "Номер", "Тип", "ID адреси", "Адреса"};
	ofcMdl = new QSqlQueryModel(this);
	AuxiliaryFunctions::initModel(ofcMdl, query_text, headers);

	AuxiliaryFunctions::setupTableView(ui->tvwOfc, ofcMdl, QVector<int>({0, 3}));

	locMdl = new QStandardItemModel(this);
	AuxiliaryFunctions::fillStandardModelWithQueryData(locMdl, locTextQuery);
	locCmpltr = new QCompleter(locMdl, this);
	locCmpltr->setCaseSensitivity(Qt::CaseInsensitive);
	locCmpltr->setCompletionRole(Qt::DisplayRole);
	connect(locCmpltr, QOverload<const QModelIndex &>::of(&QCompleter::activated),
		[=](const QModelIndex &index){
			if (index.data(Qt::UserRole).toInt() != prevLocId)
			{
				locIsPresent = true;
				updatetvwOfc();
			}
		}
	);

	ui->cmbLoc->setModel(locMdl);
	ui->cmbLoc->setCompleter(locCmpltr);
	ui->cmbLoc->setCurrentIndex(-1);
}

void OfficeSearch::updatetvwOfc()
{
	QString query_text(ofcTextQuery);
	if (locIsPresent)
	{
		query_text.append(QString(locality_id).arg(ui->cmbLoc->currentData().toInt()));
	}
	ofcMdl->setQuery(query_text);
	AuxiliaryFunctions::setupTableView(ui->tvwOfc, ofcMdl, QVector<int>({0, 3}));
}


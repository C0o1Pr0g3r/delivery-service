#include "mailingsearch.h"
#include "ui_mailingsearch.h"

MailingSearch::MailingSearch(WhichMailingsToFind what_find, DsDbMapper::Office office, QWidget *parent, Qt::WindowFlags f) :
	QDialog(parent, f),
	ui(new Ui::MailingSearch),
	certainOffice(office)
{
	ui->setupUi(this);
	init(what_find);
}

MailingSearch::~MailingSearch()
{
	delete ui;
}

void MailingSearch::init(WhichMailingsToFind what_find)
{
	mailingModel = new QStandardItemModel(this);
	QString query_text;
	if (what_find == WhichMailingsToFind::All)
	{
		AuxiliaryFunctions::read_file_to_string("sql/mailing-search/all-mailings.sql", query_text);
	}
	else if (what_find == WhichMailingsToFind::RelatedToCertainOffice)
	{
		AuxiliaryFunctions::read_file_to_string("sql/mailing-search/mailings-related-to-certain-office.sql", query_text);
		query_text = QString(query_text).arg(certainOffice.id).arg(certainOffice.id);
	}
	else if (what_find == WhichMailingsToFind::WhichDontHaveRecordInStg)
	{
		AuxiliaryFunctions::read_file_to_string("sql/mailing-search/mailings-which-dont-have-record-in-stg.sql", query_text);
	}
	else if (what_find == WhichMailingsToFind::WhichDontHaveRecordInStgAndRelatedToCertainOffice)
	{
		AuxiliaryFunctions::read_file_to_string("sql/mailing-search/mailings-which-dont-have-record-in-stg-and-related-to-certain-office.sql", query_text);
		query_text = query_text.arg(certainOffice.id);
	}
	AuxiliaryFunctions::fillStandardModelWithQueryData(mailingModel, query_text);
	mailingCmpltr = new QCompleter(mailingModel);
	mailingCmpltr->setCaseSensitivity(Qt::CaseInsensitive);
	mailingCmpltr->setCompletionRole(Qt::DisplayRole);
	ui->cmb_MailingID->setCompleter(mailingCmpltr);
	ui->cmb_MailingID->setModel(mailingModel);
}

void MailingSearch::on_pb_OK_clicked()
{
	mailingId = ui->cmb_MailingID->itemData(ui->cmb_MailingID->currentIndex(), Qt::UserRole).toInt();
	accept();
}

void MailingSearch::on_pb_cancel_clicked()
{
	reject();
}

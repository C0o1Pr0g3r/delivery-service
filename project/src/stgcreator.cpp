#include "stgcreator.h"
#include "ui_stgcreator.h"

StgCreator::StgCreator(DsDbMapper::Office office, QWidget *parent) :
	ChildWindow(parent),
	ui(new Ui::StgCreator),
	storageOffice(office)
{
	ui->setupUi(this);
	init();
}

StgCreator::~StgCreator()
{
	delete ui;
}

void StgCreator::on_pbChangeMailingWbNum_clicked()
{
	MailingSearch ms(MailingSearch::WhichMailingsToFind::WhichDontHaveRecordInStgAndRelatedToCertainOffice, storageOffice);
	if (ms.exec() == QDialog::Accepted)
	{
		mailing.id = ms.getMailingId();
		QString query_text;
		QSqlQuery query;
		query_text = QString("select wb_num from mailing where mailing.id = %1").arg(mailing.id);
		query.exec(query_text);
		if (query.next())
		{
			mailing.waybillNumber = query.value(0).toString();
		}

		ui->leMailingWbNum->setText(mailing.waybillNumber);
	}
}

void StgCreator::on_pbOpenMailingInEditor_clicked()
{
	MailingEditor *me = new MailingEditor(mailing.id);
	connect(me, &MailingEditor::closed, &MailingEditor::deleteLater);
	me->show();
}

void StgCreator::on_pbOK_clicked()
{
	QSqlQuery query;
	QString query_text;
	query_text = QString(	"update mailing set cur_loc_id = 3 where id = %1").arg(mailing.id);
	query.exec(query_text);

	query_text =	"insert into mailing_log (mailing_id, sts_id, DAT_when)"
					" values (:mailing_id, 4, datetime('now', 'localtime'))";
	query.prepare(query_text);
	query.bindValue(":mailing_id", mailing.id);
	query.exec();

	query_text = QString("insert into stg (ofc_id, mailing_id, ee_t_id, DAT_crt) values(:ofc_id, :mailing_id, :ee_t_id, datetime('now', 'localtime'))");
	query.prepare(query_text);
	query.bindValue(":ofc_id", storageOffice.id);
	query.bindValue(":mailing_id", mailing.id);
	query.bindValue(":ee_t_id", ui->cmbEeWhoTake->currentData().toInt());
	query.exec();

	QMessageBox msgBox(QMessageBox::Information, "Операція завершена", "Запис про зберігання відправлення успішно створено.", QMessageBox::Ok);
	msgBox.exec();

	close();
}

void StgCreator::on_pbCancel_clicked()
{
	close();
}

void StgCreator::init()
{
	QString query_text;
	eeMdl = new DbDataModel(this);
	AuxiliaryFunctions::read_file_to_string("sql/stg-creator/employee.sql", query_text);
	query_text = query_text.arg(storageOffice.id);
	eeMdl->setQuery(query_text);
	ui->cmbEeWhoTake->setModel(eeMdl);
	ui->leStorageOfc->setText(QString("Відділення №%1 | %2").arg(storageOffice.number).arg(storageOffice.address_str));
}

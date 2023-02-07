#include "shpcreator.h"
#include "ui_shpcreator.h"

ShpCreator::ShpCreator(DsDbMapper::Office dispatch_office, QWidget *parent) :
	ChildWindow(parent),
	ui(new Ui::ShpCreator),
	dispatchOffice(dispatch_office)
{
	ui->setupUi(this);
	init();
}

ShpCreator::~ShpCreator()
{
	delete ui;
}

void ShpCreator::init()
{
	mailingMdl = new QSqlQueryModel();
	AuxiliaryFunctions::read_file_to_string("sql/shp-creator/mailings-that-are-going-from-this-office-to-certain.sql", mailingQueryText);
	mailingMdl->setQuery(QString(mailingQueryText).arg(dispatchOffice.id).arg(0));
	AuxiliaryFunctions::setModelHeaderData(mailingMdl, QStringList({"ID", "Номер накладної", "Відправник", "Отримувач", "Пункт відправки", "Пункт прибуття", "Поточне місцезнаходження"}));
	AuxiliaryFunctions::setupTableView(ui->tvwMailing, mailingMdl, QVector<int>({0}));

	QString query_text;
	AuxiliaryFunctions::read_file_to_string("sql/shp-creator/ee-who-give-mailings.sql", query_text);
	query_text = query_text.arg(dispatchOffice.id);
	eeMdl = new DbDataModel(this);
	eeMdl->setQuery(query_text);
	ui->cmbEe->setModel(eeMdl);

	ui->leDspchOfc->setText(QString("Відділення №%1 | %2").arg(dispatchOffice.number).arg(dispatchOffice.address_str));
	ui->dteDateDspch->setDateTime(QDateTime::currentDateTime());
}

void ShpCreator::on_pbChangeArrOfc_clicked()
{
	OfficeSearch os(OfficeSearch::WhichOfficesToFind::OfficesRelatedToThisOffice, dispatchOffice.id, nullptr, Qt::SubWindow);
	if (os.exec() == QDialog::Accepted)
	{
		OfficeSearch::officeData office = os.getOfficeData();
		arrivalOffice.id = office.id;
		arrivalOffice.number = office.num;
		arrivalOffice.address_str = office.addr;
		ui->leArrOfc->setText(QString("Відділення №%1 | %2").arg(office.num).arg(office.addr));
		mailingMdl->setQuery(QString(mailingQueryText).arg(dispatchOffice.id).arg(office.id));
		ui->tvwMailing->resizeColumnsToContents();
	}
}

void ShpCreator::on_pbCreateShp_clicked()
{
	int visibleColumnCount;
		int hiddenColumnCount = 0;
		for (int i = 0; i < mailingMdl->columnCount(); ++i)
		{
			if (ui->tvwMailing->isColumnHidden(i))
				++hiddenColumnCount;
		}
		visibleColumnCount = mailingMdl->columnCount() - hiddenColumnCount;
		QModelIndexList selectedCells = ui->tvwMailing->selectedCells();

	QSqlQuery query;
	QString query_text_pattern = QString("update stg set ee_g_id = :ee_g_id, DAT_cmplt = datetime('now', 'localtime') where mailing_id = %1");
	QString query_text;

	for (int i = 0; i < selectedCells.size() / (mailingMdl->columnCount() - hiddenColumnCount); ++i)
	{
		int mailing_id = mailingMdl->record(selectedCells[visibleColumnCount * i].row()).value(0).toInt();

		query_text = query_text_pattern.arg(mailing_id);
		query.prepare(query_text);
		query.bindValue(":ee_g_id", ui->cmbEe->currentData().toInt());
		query.exec();
	}

	int shp_id;
	query_text = QString("insert into shp (dspch_pt, arr_pt, ee_g_id, DAT_crt, DAT_dspch) values(:dspch_pt, :arr_pt, :ee_g_id, datetime('now', 'localtime'), :DAT_dspch)");
	query.prepare(query_text);
	query.bindValue(":dspch_pt", dispatchOffice.id);
	query.bindValue(":arr_pt", arrivalOffice.id);
	query.bindValue(":ee_g_id",  ui->cmbEe->currentData().toInt());
	QString date_and_time("date time");
	date_and_time.replace("date", QString(ui->dteDateDspch->date().toString(Qt::ISODate)));
	date_and_time.replace("time", QString(ui->dteDateDspch->time().toString(Qt::ISODate)));
	query.bindValue(":DAT_dspch", date_and_time);
	query.exec();
	shp_id = query.lastInsertId().toInt();

	query_text_pattern = "insert into shp_cont (shp_id, mailing_id) values(:shp_id, :mailing_id)";
	for (int i = 0; i < selectedCells.size() / (mailingMdl->columnCount() - hiddenColumnCount); ++i)
	{
		int mailing_id = mailingMdl->record(selectedCells[visibleColumnCount * i].row()).value(0).toInt();

		query.prepare(query_text_pattern);
		query.bindValue(":shp_id", shp_id);
		query.bindValue(":mailing_id", mailing_id);
		query.exec();

		query_text = QString("update mailing set cur_loc_id = 5 where id = %1").arg(mailing_id);
		query.exec(query_text);

		query_text =	"insert into mailing_log (mailing_id, sts_id, DAT_when)"
						" values (:mailing_id, 6, datetime('now', 'localtime'))";
		query.prepare(query_text);
		query.bindValue(":mailing_id", mailing_id);
		query.exec();
	}

	QMessageBox msgBox(QMessageBox::Information, "Операція завершена", "Запис про перевезення успішно створено.", QMessageBox::Ok);
	msgBox.exec();

	close();
}

void ShpCreator::on_pbCancel_clicked()
{
	close();
}

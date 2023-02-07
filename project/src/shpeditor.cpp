#include "shpeditor.h"
#include "ui_shpeditor.h"

ShpEditor::ShpEditor(int shp_id, QWidget *parent) :
	ChildWindow(parent),
	ui(new Ui::ShpEditor),
	shipping(shp_id)
{
	ui->setupUi(this);
	init();
}

ShpEditor::~ShpEditor()
{
	delete ui;
}

void ShpEditor::init()
{
	QString query_text;
	QSqlQuery query;

	AuxiliaryFunctions::read_file_to_string("sql/shp-editor/shp.sql", query_text);
	query_text = query_text.arg(shipping.id);
	query.exec(query_text);

	QVariant DAT_arr;
	if (query.next())
	{
		ui->leWaybillNumber->setText(query.value(1).toString());
		shipping.dispatchOffice.id = query.value(2).toInt();
		shipping.arrivalOffice.id = query.value(3).toInt();
		shipping.ee_g = query.value(4).toInt();
		QStringList date_and_time(query.value(7).toString().split(" "));
		ui->dteDateDspch->setDateTime(QDateTime(QDate::fromString(date_and_time.at(0), Qt::ISODate), QTime::fromString(date_and_time.at(1), Qt::ISODate)));
		DAT_arr = query.value(8);
	}

	query_text = "";
	AuxiliaryFunctions::read_file_to_string("sql/shp-editor/dispatch-office.sql", query_text);
	query_text = query_text.arg(shipping.dispatchOffice.id);
	query.exec(query_text);
	if (query.next())
	{
		shipping.dispatchOffice.number = query.value(1).toInt();
		shipping.dispatchOffice.address_str = query.value(4).toString();
		ui->leDspchOfc->setText(QString("Відділення №%1 | %2").arg(shipping.dispatchOffice.number).arg(shipping.dispatchOffice.address_str));
	}

	query_text = "";
	AuxiliaryFunctions::read_file_to_string("sql/shp-editor/dispatch-office.sql", query_text);
	query_text = query_text.arg(shipping.arrivalOffice.id);
	query.exec(query_text);
	if (query.next())
	{
		shipping.arrivalOffice.number = query.value(1).toInt();
		shipping.arrivalOffice.address_str = query.value(4).toString();
		ui->leArrOfc->setText(QString("Відділення №%1 | %2").arg(shipping.arrivalOffice.number).arg(shipping.arrivalOffice.address_str));
	}

	query_text = "";
	AuxiliaryFunctions::read_file_to_string("sql/shp-editor/ee-who-give-mailings.sql", query_text);
	query_text = query_text.arg(shipping.dispatchOffice.id);
	query.exec(query_text);
	if (query.next())
	{
		shipping.ee_g = query.value(0).toInt();
		ui->leEeWhoGave->setText(query.value(1).toString());
	}

	if (DAT_arr.isNull())
	{
		curSts = 1;

		ui->lEeWhoTake->hide();
		ui->leEeWhoTake->hide();
		ui->lDateArr->hide();
		ui->dteDateArr->hide();
		ui->pbAddMailingsToStorage->hide();
		ui->pbShpArrived->setDefault(true);
		ui->pbShpArrived->setAutoDefault(true);
	}
	else
	{
		curSts = 2;
		ui->pbShpArrived->hide();
		ui->pbAddMailingsToStorage->setDefault(true);
		ui->pbAddMailingsToStorage->setAutoDefault(true);

		QStringList date_and_time(DAT_arr.toString().split(" "));
		ui->dteDateArr->setDateTime(QDateTime(QDate::fromString(date_and_time.at(0), Qt::ISODate), QTime::fromString(date_and_time.at(1), Qt::ISODate)));

		QSqlQuery query;
		query_text = "";
		AuxiliaryFunctions::read_file_to_string("sql/shp-editor/ee-who-give-mailings.sql", query_text);
		query_text = query_text.arg(shipping.arrivalOffice.id);
		query.exec(query_text);
		if (query.next())
		{
			shipping.ee_t = query.value(0).toInt();
			ui->leEeWhoTake->setText(query.value(1).toString());
		}
	}
	ui->leCurSts->setText(statusNames.at(curSts));

	mailingMdl = new QSqlQueryModel();
	AuxiliaryFunctions::read_file_to_string("sql/shp-editor/mailings-in-shp.sql", mailingQueryText);
	mailingMdl->setQuery(QString(mailingQueryText).arg(shipping.dispatchOffice.id).arg(shipping.arrivalOffice.id));
	AuxiliaryFunctions::setModelHeaderData(mailingMdl, QStringList({"ID", "Номер накладної", "Відправник", "Отримувач", "Пункт відправки", "Пункт прибуття", "Поточне місцезнаходження"}));
	AuxiliaryFunctions::setupTableView(ui->tvwMailing, mailingMdl, QVector<int>({0}));
}

void ShpEditor::on_pbShpArrived_clicked()
{
	ShpArrivedToOffice asto(shipping.arrivalOffice);
	if (asto.exec() == QDialog::Accepted)
	{
		ui->dteDateArr->setDateTime(asto.getDateTime());
		DsDbMapper::Employee leEeWhoTake = asto.getEmployee();
		shipping.ee_t = leEeWhoTake.id;
		ui->leEeWhoTake->setText(QString("%1").arg(leEeWhoTake.LFP));
		ui->lDateArr->show();
		ui->dteDateArr->show();
		ui->lEeWhoTake->show();
		ui->leEeWhoTake->show();

		curSts = 2;
		ui->leCurSts->setText(statusNames.at(curSts));
		ui->pbShpArrived->hide();

		QString query_text;
		QSqlQuery query;

		query_text = QString("update shp set ee_t_id = :ee_t_id, DAT_arr = :DAT_arr where shp.id = %1").arg(shipping.id);
		query.prepare(query_text);
		QString date_and_time("date time");
		date_and_time.replace("date", QString(ui->dteDateArr->date().toString(Qt::ISODate)));
		date_and_time.replace("time", QString(ui->dteDateArr->time().toString(Qt::ISODate)));
		query.bindValue(":ee_t_id", leEeWhoTake.id);
		query.bindValue(":DAT_arr", date_and_time);
		query.exec();

		for (int i = 0; i < mailingMdl->rowCount(); ++i)
		{
			int mailing_id = mailingMdl->index(i, 0).data().toInt();

			query_text = QString("update mailing set cur_loc_id = 4 where id = %1").arg(mailing_id);
			query.exec(query_text);

			query_text =	"insert into mailing_log (mailing_id, sts_id, DAT_when)"
							" values (:mailing_id, 5, datetime('now', 'localtime'))";
			query.prepare(query_text);
			query.bindValue(":mailing_id", mailing_id);
			query.exec();
		}
	}

	ui->pbAddMailingsToStorage->show();
}

void ShpEditor::on_pbAddMailingsToStorage_clicked()
{
	QString query_text;
	QSqlQuery query;

	query_text = QString("update shp set DAT_cmplt = datetime('now', 'localtime') where shp.id = %1").arg(shipping.id);
	query.exec(query_text);

	for (int i = 0; i < mailingMdl->rowCount(); ++i)
	{
		int mailing_id = mailingMdl->index(i, 0).data().toInt();

		query_text =	"insert into mailing_log (mailing_id, sts_id, DAT_when)"
						" values (:mailing_id, 13, datetime('now', 'localtime'))";
		query.prepare(query_text);
		query.bindValue(":mailing_id", mailing_id);
		query.exec();

		query_text = QString("insert into stg (ofc_id, mailing_id, ee_t_id, DAT_crt) values(:ofc_id, :mailing_id, :ee_t_id, datetime('now', 'localtime'))");
		query.prepare(query_text);
		query.bindValue(":ofc_id", shipping.arrivalOffice.id);
		query.bindValue(":mailing_id", mailing_id);
		query.bindValue(":ee_t_id", shipping.ee_t);
		query.exec();
	}

	close();
}

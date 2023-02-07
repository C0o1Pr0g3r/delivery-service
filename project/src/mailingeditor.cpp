#include "mailingeditor.h"
#include "ui_mailingeditor.h"

MailingEditor::MailingEditor(int mailing_id, QWidget *parent) :
	ChildWindow(parent),
	ui(new Ui::MailingEditor),
	mailing()
{
	ui->setupUi(this);
	init();
	fillFormWithData(mailing_id);
}

MailingEditor::~MailingEditor()
{
	delete ui;
}

void MailingEditor::init()
{

}

void MailingEditor::fillFormWithData(int mailing_id)
{
	QString query_text;
	QSqlQuery query;

	query_text = QString(	"select mailing.id, mailing.wb_num, mailing.sender_id, mailing.recipient_id,"
							" mailing.ofc_whence_id, mailing.ofc_where_id, mailing_loc.descr"
							" from mailing"
							" inner join mailing_loc on mailing.cur_loc_id = mailing_loc.id"
							" where mailing.id = %1").arg(mailing_id);
	query.exec(query_text);
	if (query.next())
	{
		ui->leWaybillNumber->setText(query.value(1).toString());
		ui->leMailingLoc->setText(query.value(6).toString());
		mailing.id = query.value(0).toInt();
		mailing.sender.id = query.value(2).toInt();
		mailing.recipient.id = query.value(3).toInt();
		mailing.dispatchOffice.id = query.value(4).toInt();
		mailing.arrivalOffice.id = query.value(5).toInt();
	}

	query_text = QString("select * from client where id = %1").arg(mailing.sender.id);
	query.exec(query_text);
	if (query.next())
	{
		QString query_text2;
		QSqlQuery query2;
		if (query.value(2).isNull())
		{
			query_text2 = QString("select indv.lastname || ' ' || indv.firstname || ' ' || indv.patronymic, indv.mob from indv where id = %1").arg(query.value(1).toInt());
		}
		else
		{
			query_text2 = QString("select org.title, org.mob from org where id = %1").arg(query.value(2).toInt());
		}
		query2.exec(query_text2);
		if (query2.next())
		{
			ui->leSndrName->setText(query2.value(0).toString());
			ui->leSndrMob->setText(query2.value(1).toString());
		}
	}

	query_text = QString("select * from client where id = %1").arg(mailing.recipient.id);
	query.exec(query_text);
	if (query.next())
	{
		QString query_text2;
		QSqlQuery query2;
		if (query.value(2).isNull())
		{
			query_text2 = QString("select indv.lastname || ' ' || indv.firstname || ' ' || indv.patronymic, indv.mob from indv where id = %1").arg(query.value(1).toInt());
		}
		else
		{
			query_text2 = QString("select org.title, org.mob from org where id = %1").arg(query.value(2).toInt());
		}
		query2.exec(query_text2);
		if (query2.next())
		{
			ui->leRcptName->setText(query2.value(0).toString());
			ui->leRcptMob->setText(query2.value(1).toString());
		}
	}

	query_text = QString(	"select ofc.id, ofc.num, ofc_tp.descr, ofc.addr_id, loc_tp.abbrname || ' ' || loc.title || ', ' || st_tp.abbrname || ' ' || st.title || ', ' || addr.hse_num AS address"
							" from ofc"
							" inner join ofc_tp on ofc.tp_id = ofc_tp.id"
							" inner join addr on ofc.addr_id = addr.id"
							" inner join st on addr.st_id = st.id"
							" inner join st_tp on st.tp_id = st_tp.id"
							" inner join loc on st.loc_id = loc.id"
							" inner join loc_tp on loc.tp_id = loc_tp.id"
							" where ofc.id = %1").arg(mailing.dispatchOffice.id);
	query.exec(query_text);
	if (query.next())
	{
		ui->leDspchOfc->setText(QString("№%1 (%2)").arg(query.value(1).toString()).arg(query.value(2).toString()));
		ui->leDspchAddr->setText(query.value(4).toString());
	}

	query_text = QString(	"select ofc.id, ofc.num, ofc_tp.descr, ofc.addr_id, loc_tp.abbrname || ' ' || loc.title || ', ' || st_tp.abbrname || ' ' || st.title || ', ' || addr.hse_num AS address"
							" from ofc"
							" inner join ofc_tp on ofc.tp_id = ofc_tp.id"
							" inner join addr on ofc.addr_id = addr.id"
							" inner join st on addr.st_id = st.id"
							" inner join st_tp on st.tp_id = st_tp.id"
							" inner join loc on st.loc_id = loc.id"
							" inner join loc_tp on loc.tp_id = loc_tp.id"
							" where ofc.id = %1").arg(mailing.arrivalOffice.id);
	query.exec(query_text);
	if (query.next())
	{
		ui->leArrOfc->setText(QString("№%1 (%2)").arg(query.value(1).toString()).arg(query.value(2).toString()));
		ui->leArrAddr->setText(query.value(4).toString());
	}

	query_text = QString("select * from mailing_cont where mailing_id = %1").arg(mailing_id);
	QStringList headers {"ID", "ID відправлення", "Опис", "Оголошена вартість", "Кількість", "Довжина", "Ширина", "Висота", "Вага"};
	mailingContMdl = new QSqlQueryModel(this);
	AuxiliaryFunctions::initModel(mailingContMdl, query_text, headers);
	AuxiliaryFunctions::setupTableView(ui->tvwMailingCont, mailingContMdl, QVector<int>({0, 1}));

	query_text = QString(	"select mailing_svc.id, mailing_svc.mailing_id, svc.descr as svc_tp, pymt_txn_partic.descr as payer,"
							" pymt_meth.descr as pymt_meth, mailing_svc.cost, mailing_svc.DAT_crt from mailing_svc inner join svc on mailing_svc.svc_id = svc.id"
							" inner join pymt_txn_partic on mailing_svc.payer_id = pymt_txn_partic.id"
							" inner join pymt_meth on mailing_svc.pymt_meth_id = pymt_meth.id"
							" where mailing_id = %1").arg(mailing_id);
	headers = QStringList({"ID", "ID відправлення", "Послуга", "Платник", "Спосіб оплати", "Вартість", "Дата та час замовлення"});
	mailingSvcMdl = new QSqlQueryModel(this);
	AuxiliaryFunctions::initModel(mailingSvcMdl, query_text, headers);
	AuxiliaryFunctions::setupTableView(ui->tvwMailingSvc, mailingSvcMdl, QVector<int>({0, 1}));


	query_text = QString(	"select pymt_txn.id, svc.descr as svc, pymt_txn_partic.descr as sender, pymt_txn_partic_1.descr as recipient,"
							" pymt_meth.descr as pymt_meth, pymt_txn.sum, pymt_txn.DAT_when from pymt_txn"
							" inner join mailing_svc on pymt_txn.mailing_svc_id = mailing_svc.id inner join svc on mailing_svc.svc_id = svc.id"
							" inner join mailing on mailing_svc.mailing_id = mailing.id"
							" inner join pymt_txn_partic on pymt_txn.sender_id = pymt_txn_partic.id"
							" inner join pymt_txn_partic as pymt_txn_partic_1 on pymt_txn.recipient_id = pymt_txn_partic_1.id"
							" inner join pymt_meth on pymt_txn.pymt_meth_id = pymt_meth.id"
							" where mailing.id = %1").arg(mailing_id);

	headers = QStringList({"ID", "Послуга", "Відправник", "Отримувач", "Спосіб оплати", "Сума", "Дата та час здійснення"});
	mailingPymtTxnMdl = new QSqlQueryModel(this);
	AuxiliaryFunctions::initModel(mailingPymtTxnMdl, query_text, headers);
	AuxiliaryFunctions::setupTableView(ui->tvwMailingPymtTxn, mailingPymtTxnMdl, QVector<int>({0, 1}));

	query_text = QString(	"select mailing_log.id, mailing_log.mailing_id, mailing_sts.descr, mailing_log.DAT_when from mailing_log"
							" inner join mailing_sts on mailing_log.sts_id = mailing_sts.id"
							" where mailing_log.mailing_id = %1").arg(mailing_id);

	headers = QStringList({"ID", "ID відправлення", "Подія", "Дата та час"});
	mailingLogMdl = new QSqlQueryModel(this);
	AuxiliaryFunctions::initModel(mailingLogMdl, query_text, headers);
	AuxiliaryFunctions::setupTableView(ui->tvwMailingLog, mailingLogMdl, QVector<int>({0, 1}));

	query_text = QString("select sts_id from mailing_log where mailing_id = %1 order by id desc limit 1").arg(mailing.id);
	query.exec(query_text);
	if (query.next())
	{
		if (query.value(0).toInt() != 13)
		{
			ui->pbIssueMailing->setEnabled(false);
		}
	}

	query_text = QString("select sts_id from mailing where id = %1").arg(mailing.id);
	query.exec(query_text);
	if (query.next())
	{
		if (query.value(0).toInt() == 2)
		{
			ui->pbChangeSndr->setEnabled(false);
			ui->pbChangeRcpt->setEnabled(false);
		}
	}
}

void MailingEditor::on_pbChangeSndr_clicked()
{
	changeClient(&mailing.sender.id, ui->lSndrType, ui->lSndrName, ui->leSndrName, ui->leSndrMob);
}

void MailingEditor::on_pbChangeRcpt_clicked()
{
	changeClient(&mailing.recipient.id, ui->lRcptType, ui->lRcptName, ui->leRcptName, ui->leRcptMob);
}

void MailingEditor::changeClient(int *client_id, QLabel *client_type, QLabel *lfp_or_name, QLineEdit *client_name, QLineEdit *client_mob)
{
	ClientSearch cs(false, nullptr, Qt::SubWindow);
	if (cs.exec() == QDialog::Accepted)
	{
		ClientSearch::clientData client = cs.getClientData();
		*client_id = client.id;
		if (client.type == ClientSearch::ClientType::Individual)
		{
			client_type->setText("Приватна особа");
			lfp_or_name->setText("ПІБ");
		}
		else
		{
			client_type->setText("Організация");
			lfp_or_name->setText("Назва");
		}
		client_name->setText(client.name);
		client_mob->setText(client.mob);
	}
}

void MailingEditor::changePlc(int index, int *addr_id, QFormLayout *formlyt, QLabel *l_ofc, QLineEdit *le_ofc, QLineEdit *le_addr)
{
	switch (index)
	{
		case 0:
			l_ofc->setVisible(false);
			le_ofc->setVisible(false);
			formlyt->takeRow(l_ofc);
		break;
		case 1:
			l_ofc->setVisible(true);
			le_ofc->setVisible(true);
			formlyt->insertRow(1, l_ofc, le_ofc);
		break;
	}
	le_ofc->setText("");
	le_addr->setText("");
	*addr_id = 0;
}

void MailingEditor::changePt(int *ofc_id, QLineEdit * l_ofc, QLineEdit * l_addr)
{
	bool onlyCargoOffice = false;
	QVector<int> len;
	QVector<int> wd;
	QVector<int> hgt;
	QVector<double> wt;
	QVector<QString> descr;
	QVector<int> amt;
	QVector<double> declVal;

	for (int i = 0; i < len.size(); ++i)
	{
		if (len[i] > 120 || wd[i] > 120 || hgt[i] > 120 || AuxiliaryFunctions::cmpf(wt[i],30.0) == 1)
		{
			onlyCargoOffice = true;
			goto ThereIsOnlyCargoOffice;
		}
	}

	ThereIsOnlyCargoOffice:

	OfficeSearch os(OfficeSearch::WhichOfficesToFind::AllOffices, nullptr, Qt::SubWindow);
	if (onlyCargoOffice)
	{
		os.changeWhatOfficeToFind(OfficeSearch::WhichOfficesToFind::CargoOffices);
	}
	if (os.exec() == QDialog::Accepted)
	{
		OfficeSearch::officeData office = os.getOfficeData();
		*ofc_id = office.id;
		l_ofc->setText(QString("Відділення №%1").arg(office.num));
		l_addr->setText(office.addr);
	}
}

void MailingEditor::on_pbUpdateMailing_clicked()
{
	QString query_text;
	QSqlQuery query;

	query_text =	QString("update mailing set sender_id = :sender_id, recipient_id = :recipient_id where id = %1").arg(mailing.id);
	query.prepare(query_text);
	query.bindValue(":sender_id", mailing.sender.id);
	query.bindValue(":recipient_id", mailing.recipient.id);
	query.exec();
}

void MailingEditor::on_pbCancelMailingUpdating_clicked()
{
	close();
}

void MailingEditor::on_pbIssueMailing_clicked()
{
	QString query_text;
	QSqlQuery query;

	query_text =	QString("update mailing set cur_loc_id = 2, sts_id = 2 where id = %1").arg(mailing.id);
	query.exec(query_text);

	query_text =	"insert into mailing_log (mailing_id, sts_id, DAT_when)"
					" values (:mailing_id, 7, datetime('now', 'localtime')),"
					"        (:mailing_id, 3, datetime('now', 'localtime'))";
	query.prepare(query_text);
	query.bindValue(":mailing_id", mailing.id);
	query.exec();

	int ee = 0;
	query_text = QString("select staff.id from staff where staff.post_id = 1 and ofc_id = %1").arg(mailing.arrivalOffice.id);
	query.exec(query_text);
	if (query.next())
	{
		ee = query.value(0).toInt();
	}

	query_text =	QString("update stg set ee_g_id = :ee_g_id, DAT_cmplt = datetime('now', 'localtime') where mailing_id = %1 and DAT_cmplt is null").arg(mailing.id);
	query.prepare(query_text);
	query.bindValue(":ee_g_id", ee);
	query.exec();

	struct {
		int id = 0;
		int sender_id = 0;
		int pymt_meth_id = 0;
		int cost = 0;
	} maiiling_svc;

	query_text =	QString("select id, payer_id, pymt_meth_id, cost from mailing_svc where mailing_id = %1").arg(mailing.id);
	query.exec(query_text);
	if (query.next())
	{
		maiiling_svc.id = query.value(0).toInt();
		maiiling_svc.sender_id = query.value(1).toInt();
		maiiling_svc.pymt_meth_id = query.value(2).toInt();
		maiiling_svc.cost = query.value(3).toDouble();
	}

	query_text =	QString("insert into pymt_txn (mailing_svc_id, sender_id, recipient_id, pymt_meth_id, sum, DAT_when)"
							"values (:mailing_svc_id, :sender_id, 1, :pymt_meth_id, :sum, datetime('now', 'localtime'))");
	query.prepare(query_text);
	query.bindValue(":mailing_svc_id", maiiling_svc.id);
	query.bindValue(":sender_id", maiiling_svc.sender_id);
	query.bindValue(":pymt_meth_id", maiiling_svc.pymt_meth_id);
	query.bindValue(":sum", maiiling_svc.cost);
	query.exec();

	fillFormWithData(mailing.id);
}

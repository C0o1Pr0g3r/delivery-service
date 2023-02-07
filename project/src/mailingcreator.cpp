#include "mailingcreator.h"
#include "ui_mailingcreator.h"

MailingCreator::MailingCreator(DsDbMapper::Office dispatch_office, QWidget *parent) :
	ChildWindow(parent),
	ui(new Ui::MailingCreator),
	dispatchOffice(dispatch_office)
{
	ui->setupUi(this);
	init();
}

MailingCreator::~MailingCreator()
{
	delete ui;
}

void MailingCreator::init()
{
	pymtMethMdl = new DbDataModel("select * from pymt_meth", this);
	payerMdl = new DbDataModel("select * from pymt_txn_partic where id > 1", this);

	ui->cmbPymtMeth->setModel(pymtMethMdl);
	ui->cmbPayer->setModel(payerMdl);

	ui->vlytCargoPlc->setAlignment(Qt::AlignTop);
	ui->pbChooseDspchOfc->hide();
	ui->leDspchOfc->setText(QString("Відділення №%1").arg(dispatchOffice.number));
	ui->leDspchAddr->setText(QString(dispatchOffice.address_str));
	dspchOfcId = dispatchOffice.id;
}

void MailingCreator::on_pbAddCargoPlace_clicked()
{
	GroupBoxWithCloseButton *grp = new GroupBoxWithCloseButton(QString("Місце №%1").arg(ui->vlytCargoPlc->count() + 1), this);
	QHBoxLayout *hLayout = new QHBoxLayout(grp);
	QFormLayout *formLayout = new QFormLayout();
	QFormLayout *formLayout2 = new QFormLayout();
	QLabel *lLen = new QLabel("Довжина", grp);
	QLabel *lWd = new QLabel("Ширина", grp);
	QLabel *lHgt = new QLabel("Висота", grp);
	QLabel *lWt = new QLabel("Вага", grp);
	QLabel *lDescr = new QLabel("Опис", grp);
	QLabel *lAmount = new QLabel("Кількість", grp);
	QLabel *lDeclValue = new QLabel("Оголошена вартість", grp);
	QSpinBox *spnLen = new QSpinBox (grp);
	QSpinBox *spnWd = new QSpinBox (grp);
	QSpinBox *spnHgt = new QSpinBox (grp);
	QDoubleSpinBox  *dspnWt = new QDoubleSpinBox (grp);
	QLineEdit *leDescr = new QLineEdit(grp);
	QSpinBox *spnAmount = new QSpinBox(grp);
	QDoubleSpinBox  *dspnDeclValue = new QDoubleSpinBox (grp);

	formLayout->addRow(lLen, spnLen);
	formLayout->addRow(lWd, spnWd);
	formLayout->addRow(lHgt, spnHgt);
	formLayout->addRow(lWt, dspnWt);
	formLayout2->addRow(lDescr, leDescr);
	formLayout2->addRow(lAmount , spnAmount);
	formLayout2->addRow(lDeclValue, dspnDeclValue);

	hLayout->addLayout(formLayout);
	hLayout->addStretch();
	hLayout->addLayout(formLayout2);

	grp->setLayout(hLayout);

	spnLen->setObjectName(QString("len"));
	spnWd->setObjectName(QString("wd"));
	spnHgt->setObjectName(QString("hgt"));
	dspnWt->setObjectName(QString("wt"));
	leDescr->setObjectName(QString("descr"));
	spnAmount->setObjectName(QString("amt"));
	dspnDeclValue->setObjectName(QString("declVal"));

	spnLen->setRange(1, 300);
	spnWd->setRange(1, 300);
	spnHgt->setRange(1, 170);
	dspnWt->setRange(0.0, 1000.0);
	spnAmount->setRange(1, 1000);
	dspnDeclValue->setRange(0.0, 100000.0);

	dspnWt->setDecimals(3);

	connect(grp, SIGNAL(closeButtonReleased()), this, SLOT(removeCargoPlace()));

	ui->vlytCargoPlc->addWidget(grp);
	ui->lPlcCount->setText(QString("Усього місць: %1").arg(ui->vlytCargoPlc->count()));
}

void MailingCreator::removeCargoPlace()
{
	GroupBoxWithCloseButton *grp = qobject_cast<GroupBoxWithCloseButton *>(sender());
	ui->vlytCargoPlc->removeWidget(grp);
	grp->deleteLater();
	ui->lPlcCount->setText(QString("Усього місць: %1").arg(ui->vlytCargoPlc->count()));
	for (int i = 0; i < ui->vlytCargoPlc->count(); ++i)
	{
		GroupBoxWithCloseButton *plc = qobject_cast<GroupBoxWithCloseButton *>(ui->vlytCargoPlc->itemAt(i)->widget());
		plc->setTitle(QString("Місце №%1").arg(i + 1));
	}
}

void MailingCreator::getCargoInfo(QVector<int> &len, QVector<int> &wd, QVector<int> &hgt, QVector<double> &wt,
								  QVector<QString> &descr, QVector<int> &amt, QVector<double> &declVal)
{
	for (int i = 0; i < ui->vlytCargoPlc->count(); ++i)
	{
		QList<QObject *> children = ui->vlytCargoPlc->itemAt(i)->widget()->children();
		for (int j = 0; j < children.size(); ++j)
		{
			if (children[j]->objectName() == "len")
			{
				QSpinBox *spn_len = qobject_cast<QSpinBox *>(children[j]);
				len.append(spn_len->value());
			}
			else if (children[j]->objectName() == "wd")
			{
				QSpinBox *spn_wd = qobject_cast<QSpinBox *>(children[j]);
				wd.append(spn_wd->value());
			}
			else if (children[j]->objectName() == "hgt")
			{
				QSpinBox *spn_hgt = qobject_cast<QSpinBox *>(children[j]);
				hgt.append(spn_hgt->value());
			}
			else if (children[j]->objectName() == "wt")
			{
				QDoubleSpinBox *dspn_wt = qobject_cast<QDoubleSpinBox *>(children[j]);
				wt.append(dspn_wt->value());
			}
			else if (children[j]->objectName() == "descr")
			{
				QLineEdit *le_descr = qobject_cast<QLineEdit *>(children[j]);
				descr.append(le_descr->text());
			}
			else if (children[j]->objectName() == "amt")
			{
				QSpinBox *spn_amt = qobject_cast<QSpinBox *>(children[j]);
				amt.append(spn_amt->value());
			}
			else if (children[j]->objectName() == "declVal")
			{
				QDoubleSpinBox *dspn_declVal = qobject_cast<QDoubleSpinBox *>(children[j]);
				declVal.append(dspn_declVal->value());
			}
		}
	}
}

void MailingCreator::on_pbChooseSndr_clicked()
{
	chooseClient(&sndrId, ui->lSndrTp, ui->lSndrName, ui->leSndrName, ui->leSndrMob);
}

void MailingCreator::on_pbChooseRcpt_clicked()
{
	chooseClient(&rcptId, ui->lRcptTp, ui->lRcptName, ui->leRcptName, ui->leRcptMob);
}

void MailingCreator::on_pbChooseDspchOfc_clicked()
{
	choosePt(&dspchOfcId, ui->leDspchOfc, ui->leDspchAddr);
}

void MailingCreator::on_pbChooseArrOfc_clicked()
{
	choosePt(&arrOfcId, ui->leArrOfc, ui->leArrAddr);
}

void MailingCreator::chooseClient(int *client_id, QLabel *client_type, QLabel *lfp_or_name, QLineEdit *client_name, QLineEdit *client_mob)
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
			client_type->setText("Організація");
			lfp_or_name->setText("Назва");
		}
		client_name->setText(client.name);
		client_mob->setText(client.mob);
	}
}

void MailingCreator::choosePt(int *ofc_id, QLineEdit * l_ofc, QLineEdit * l_addr)
{
	bool onlyCargoOffice = false;
	QVector<int> len;
	QVector<int> wd;
	QVector<int> hgt;
	QVector<double> wt;
	QVector<QString> descr;
	QVector<int> amt;
	QVector<double> declVal;

	getCargoInfo(len, wd, hgt, wt, descr, amt, declVal);

	if (len.size() == 0)
	{
		QMessageBox msgBox(QMessageBox::Warning, "Помилка", "Не створено жодного місця у вмісті відправлення.", QMessageBox::Ok);
		msgBox.exec();
		return;
	}

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

void MailingCreator::on_pbCreateMailing_clicked()
{
	QVector<int> len;
	QVector<int> wd;
	QVector<int> hgt;
	QVector<double> wt;
	QVector<QString> descr;
	QVector<int> amt;
	QVector<double> declVal;

	getCargoInfo(len, wd, hgt, wt, descr, amt, declVal);

	if (len.size() == 0)
	{
		QMessageBox msgBox(QMessageBox::Warning, "Помилка", "Не створено жодного місця у вмісті відправлення.", QMessageBox::Ok);
		msgBox.exec();
		return;
	}

	for (int i = 0; i < descr.size(); ++i)
	{
		if (descr[i].isEmpty())
		{
			QMessageBox msgBox(QMessageBox::Warning, "Помилка", "Не всі поля у вмісті відправлення заповнені.", QMessageBox::Ok);
			msgBox.exec();
			return;
		}
	}

	if (		sndrId == 0
			||	rcptId == 0
			||	dspchOfcId == 0
			||	arrOfcId == 0)
	{
		QMessageBox msgBox(QMessageBox::Warning, "Помилка", "Не всі поля заповнені.", QMessageBox::Ok);
		msgBox.exec();
		return;
	}

	int mailing_id;
	double cost = AuxiliaryFunctions::getDeliveryCost(len, wd, hgt, wt, descr, amt, declVal);

	QString query_text;
	QSqlQuery query;

	query_text =	"insert into mailing (sender_id, recipient_id, ofc_whence_id, ofc_where_id, cur_loc_id, sts_id)"
					" values (:sender_id, :recipient_id, :ofc_whence_id, :ofc_where_id, 1, 1)";
	query.prepare(query_text);
	query.bindValue(":sender_id", sndrId);
	query.bindValue(":recipient_id", rcptId);
	query.bindValue(":ofc_whence_id", dspchOfcId);
	query.bindValue(":ofc_where_id", arrOfcId);
	query.exec();
	mailing_id = query.lastInsertId().toInt();

	query_text =	"insert into mailing_cont (mailing_id, descr, decl_val, amt, len, wd, hgt, wt)"
					" values (:mailing_id, :descr, :decl_val, :amt, :len, :wd, :hgt, :wt)";
	for (int i = 0; i < len.size(); ++i)
	{
		query.prepare(query_text);
		query.bindValue(":mailing_id", mailing_id);
		query.bindValue(":descr", descr.at(i));
		query.bindValue(":decl_val", declVal.at(i));
		query.bindValue(":amt", amt.at(i));
		query.bindValue(":len", len.at(i));
		query.bindValue(":wd", wd.at(i));
		query.bindValue(":hgt", hgt.at(i));
		query.bindValue(":wt", wt.at(i));
		query.exec();
	}

	query_text =	"insert into mailing_svc (mailing_id, svc_id, payer_id, pymt_meth_id, cost, DAT_crt)"
					" values (:mailing_id, 1, :payer_id, :pymt_meth_id, :cost, datetime('now', 'localtime'))";
	query.prepare(query_text);
	query.bindValue(":mailing_id", mailing_id);
	query.bindValue(":payer_id", ui->cmbPayer->currentData().toInt());
	query.bindValue(":pymt_meth_id", ui->cmbPymtMeth->currentData().toInt());
	query.bindValue(":cost", cost);
	query.exec();

	query_text =	"insert into mailing_log (mailing_id, sts_id, DAT_when)"
					" values (:mailing_id, 1, datetime('now', 'localtime'))";
	query.prepare(query_text);
	query.bindValue(":mailing_id", mailing_id);
	query.exec();

	QMessageBox msgBox(QMessageBox::Information, "Операція завершена", "Відправлення успішно створено.", QMessageBox::Ok);
	msgBox.exec();

	close();
}

void MailingCreator::on_pbCancelMailingCreating_clicked()
{
	close();
}

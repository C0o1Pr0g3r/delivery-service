#ifndef MAILINGEDITOR_H
#define MAILINGEDITOR_H

#include "auxiliaryfunctions.h"
#include "childwindow.h"
#include "database.h"
#include "dbdatamodel.h"
#include "groupboxwithclosebutton.h"
#include "clientsearch.h"
#include "addressselector.h"
#include "officesearch.h"
#include "dsdbmapper.h"

#include <QWidget>
#include <QFormLayout>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QCheckBox>

namespace Ui {
class MailingEditor;
}

class MailingEditor : public ChildWindow
{
	Q_OBJECT

public:
	explicit MailingEditor(int mailing_id, QWidget *parent = nullptr);
	~MailingEditor();

private slots:
	void on_pbChangeSndr_clicked();
	void on_pbChangeRcpt_clicked();
//	void on_pbChangeDspchOfc_clicked();
//	void on_pbChangeArrOfc_clicked();

	void on_pbUpdateMailing_clicked();
	void on_pbCancelMailingUpdating_clicked();
	void on_pbIssueMailing_clicked();

private:
	void init();
	void changeClient(int *client_id, QLabel *client_type, QLabel *lfp_or_name, QLineEdit *client_name, QLineEdit *client_mob);
	void changePlc(int index, int *addr_id, QFormLayout *formlyt, QLabel *l_ofc, QLineEdit *le_ofc, QLineEdit *le_addr);
	void changePt(int * addr_id, QLineEdit * l_ofc, QLineEdit * l_addr);
	void fillFormWithData(int mailing_id);

private:
	Ui::MailingEditor *ui;

	QSqlQueryModel *mailingContMdl;
	QSqlQueryModel *mailingSvcMdl;
	QSqlQueryModel *mailingPymtTxnMdl;
	QSqlQueryModel *mailingLogMdl;

	DsDbMapper::Mailing mailing;
};

#endif // MAILINGEDITOR_H

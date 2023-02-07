#ifndef MAILINGCREATOR_H
#define MAILINGCREATOR_H

#include "auxiliaryfunctions.h"
#include "childwindow.h"
#include "database.h"
#include "dbdatamodel.h"
#include "groupboxwithclosebutton.h"
#include "clientsearch.h"
#include "addressselector.h"
#include "officesearch.h"
#include "mailingeditor.h"

#include <QWidget>
#include <QFormLayout>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QCheckBox>


namespace Ui {
class MailingCreator;
}

class MailingCreator : public ChildWindow
{
	Q_OBJECT

public:
	explicit MailingCreator(DsDbMapper::Office dispatch_office, QWidget *parent = nullptr);
	~MailingCreator();

private slots:
	void on_pbAddCargoPlace_clicked();
	void removeCargoPlace();
	void getCargoInfo(QVector<int> &len, QVector<int> &wd, QVector<int> &hgt, QVector<double> &wt,
					  QVector<QString> &descr, QVector<int> &amt, QVector<double> &declVal);


	void on_pbChooseSndr_clicked();
	void on_pbChooseRcpt_clicked();
	void on_pbChooseDspchOfc_clicked();
	void on_pbChooseArrOfc_clicked();
	void on_pbCreateMailing_clicked();
	void on_pbCancelMailingCreating_clicked();

private:
	void init();
	void chooseClient(int *client_id, QLabel *client_type, QLabel *lfp_or_name, QLineEdit *client_name, QLineEdit *client_mob);
	void choosePt(int *ofc_id, QLineEdit * l_ofc, QLineEdit * l_addr);

private:
	Ui::MailingCreator *ui;

	DbDataModel *pymtMethMdl;
	DbDataModel *payerMdl;

	int sndrId = 0;
	int rcptId = 0;
	int dspchOfcId = 0;
	int arrOfcId = 0;

	QSqlQueryModel *employerMdl;
	int employerId = 0;
	DsDbMapper::Office dispatchOffice;
};

#endif // MAILINGCREATOR_H

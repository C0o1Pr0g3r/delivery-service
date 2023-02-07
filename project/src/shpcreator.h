#ifndef SHPCREATOR_H
#define SHPCREATOR_H

#include "childwindow.h"
#include "auxiliaryfunctions.h"
#include "officesearch.h"
#include "tableviewwithmultipleselection.h"
#include "dbdatamodel.h"
#include "dsdbmapper.h"

#include <QWidget>
#include <QSqlQueryModel>

namespace Ui {
class ShpCreator;
}

class ShpCreator : public ChildWindow
{
	Q_OBJECT

public:
	explicit ShpCreator(DsDbMapper::Office dispatch_office, QWidget *parent = nullptr);
	~ShpCreator();

private slots:
	void on_pbChangeArrOfc_clicked();
	void on_pbCreateShp_clicked();
	void on_pbCancel_clicked();

private:
	void init();

private:
	Ui::ShpCreator *ui;

	QSqlQueryModel *mailingMdl;
	QString mailingQueryText;
	DsDbMapper::Office dispatchOffice;
	DsDbMapper::Office arrivalOffice;
	DbDataModel *eeMdl;
};

#endif // SHPCREATOR_H

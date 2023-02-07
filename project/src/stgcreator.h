#ifndef STGCREATOR_H
#define STGCREATOR_H

#include "childwindow.h"
#include "auxiliaryfunctions.h"
#include "mailingsearch.h"
#include "mailingeditor.h"
#include "shpeditor.h"
#include "shparrivedtooffice.h"
#include "dsdbmapper.h"
#include "dbdatamodel.h"

#include <QWidget>

namespace Ui {
class StgCreator;
}

class StgCreator : public ChildWindow
{
	Q_OBJECT

public:
	explicit StgCreator(DsDbMapper::Office office = DsDbMapper::Office(), QWidget *parent = nullptr);
	~StgCreator();

private slots:
	void on_pbChangeMailingWbNum_clicked();

	void on_pbOpenMailingInEditor_clicked();

	void on_pbOK_clicked();

	void on_pbCancel_clicked();

private:
	void init();

private:
	Ui::StgCreator *ui;

	DsDbMapper::Mailing mailing;
	DsDbMapper::Employee employee;
	DsDbMapper::Office storageOffice;
	DbDataModel *eeMdl;
};

#endif // STGCREATOR_H

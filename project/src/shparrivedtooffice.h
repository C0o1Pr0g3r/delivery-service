#ifndef SHPARRIVEDTOOFFICE_H
#define SHPARRIVEDTOOFFICE_H

#include "auxiliaryfunctions.h"
#include "dbdatamodel.h"
#include "dsdbmapper.h"

#include <QDialog>
#include <QDateTime>

namespace Ui {
class ShpArrivedToOffice;
}

class ShpArrivedToOffice : public QDialog
{
	Q_OBJECT

public:
	explicit ShpArrivedToOffice(DsDbMapper::Office arrival_office, QWidget *parent = nullptr);
	~ShpArrivedToOffice();


public:
	QDateTime getDateTime() { return date_and_time; }
	DsDbMapper::Employee getEmployee() { return employee; }

private slots:
	void on_pbOK_clicked();
	void on_pbCancel_clicked();

private:
	void init();

private:
	Ui::ShpArrivedToOffice *ui;
	QDateTime date_and_time;
	DsDbMapper::Office arrivalOffice;
	DsDbMapper::Employee employee;
	DbDataModel *eeMdl;
};

#endif // SHPARRIVEDTOOFFICE_H

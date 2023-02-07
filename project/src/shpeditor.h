#ifndef SHPEDITOR_H
#define SHPEDITOR_H

#include "childwindow.h"
#include "auxiliaryfunctions.h"
#include "shparrivedtooffice.h"

#include <QWidget>
#include <QSqlQueryModel>

namespace Ui {
class ShpEditor;
}

class ShpEditor : public ChildWindow
{
	Q_OBJECT

public:
	explicit ShpEditor(int shp_id, QWidget *parent = nullptr);
	~ShpEditor();

public:
	struct Shipping {
		int id;
		DsDbMapper::Office dispatchOffice;
		DsDbMapper::Office arrivalOffice;
		int ee_g;
		int ee_t;

		Shipping(int id)
			: id(id),
			  dispatchOffice(),
			  arrivalOffice(),
			  ee_g(0),
			  ee_t(0) {}
	};

private slots:
	void on_pbShpArrived_clicked();

	void on_pbAddMailingsToStorage_clicked();

private:
	void init();

private:
	Ui::ShpEditor *ui;

	struct Status {
		int id;
		QString name;

		Status(int id, QString name) :
			id(id),
			name(name) {}
	};

	QSqlQueryModel *mailingMdl;
	QString mailingQueryText;
	Shipping shipping;
	QStringList statusNames = QStringList({"Завантаження відправлень", "Транспортування", "Розвантаження відправлень"});
	int curSts = 0;
};

#endif // SHPEDITOR_H

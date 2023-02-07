#ifndef SHPSEARCH_H
#define SHPSEARCH_H

#include "auxiliaryfunctions.h"
#include "dsdbmapper.h"

#include <QDialog>
#include <QStandardItemModel>
#include <QCompleter>

namespace Ui {
class ShpSearch;
}

class ShpSearch : public QDialog
{
	Q_OBJECT
public:
	enum class WhicShpToFind;
public:
	explicit ShpSearch(WhicShpToFind what_find = WhicShpToFind::AllActive, DsDbMapper::Office office = DsDbMapper::Office(), QWidget *parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());
	~ShpSearch();

public:
	enum class WhicShpToFind
	{
		AllActive,
		RelatedToCertainOffice
	};

public:
	int getShpId() { return shpId; }

private slots:
	void on_pb_OK_clicked();
	void on_pb_cancel_clicked();

private:
	void init(WhicShpToFind what_find);

private:
	Ui::ShpSearch *ui;

	QStandardItemModel *shpMdl;
	QCompleter *shpCmpltr;
	int shpId;
	DsDbMapper::Office certainOffice;
};

#endif // SHPSEARCH_H

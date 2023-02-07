#ifndef MAILINGSEARCH_H
#define MAILINGSEARCH_H

#include "auxiliaryfunctions.h"
#include "dsdbmapper.h"

#include <QDialog>
#include <QCompleter>
#include <QStandardItemModel>

namespace Ui {
class MailingSearch;
}

class MailingSearch : public QDialog
{
	Q_OBJECT

public:
	enum class WhichMailingsToFind;
	explicit MailingSearch(WhichMailingsToFind what_find = WhichMailingsToFind::All, DsDbMapper::Office office = DsDbMapper::Office(), QWidget *parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());
	~MailingSearch();

public:
	enum class WhichMailingsToFind
	{
		All,
		RelatedToCertainOffice,
		WhichDontHaveRecordInStg,
		WhichDontHaveRecordInStgAndRelatedToCertainOffice,
	};

	int getMailingId() { return mailingId; }

private slots:
	void on_pb_OK_clicked();
	void on_pb_cancel_clicked();

private:
	void init(WhichMailingsToFind what_find);

private:
	Ui::MailingSearch *ui;

	QStandardItemModel *mailingModel;
	QCompleter *mailingCmpltr;
	int mailingId;
	DsDbMapper::Office certainOffice;
};

#endif // MAILINGSEARCH_H

#ifndef CLIENTSEARCH_H
#define CLIENTSEARCH_H

#include "auxiliaryfunctions.h"

#include <QDialog>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSortFilterProxyModel>
#include <QTableView>
#include <QMessageBox>

namespace Ui { class ClientSearch; }

class ClientSearch : public QDialog
{
	Q_OBJECT

public:
	explicit ClientSearch(bool enableAddEditClientButtons = false, QWidget *parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());
	~ClientSearch();

public:
	enum class ClientType
	{
		Individual = 1,
		Organization
	};
	struct clientData {
		int id;
		int subId;
		ClientType type;
		QString name;
		QString mob;
		QString USREOU;

		clientData()
			: id(0),
			  subId(0),
			  type(static_cast<ClientType>(0)),
			  name(""),
			  mob(""),
			  USREOU("") {}
	};

public:
	clientData getClientData() { return client; }

private slots:
	void on_rb_indv_clicked(bool checked);
	void on_rb_org_clicked(bool checked);
	void on_cmb_searchBy_currentIndexChanged(int index);
	void on_le_search_textChanged(const QString &arg1);
	void on_tvw_client_clicked(const QModelIndex &index);
	void on_pbOK_clicked();
	void on_pbCancel_clicked();

private:
	void init();

private:
	Ui::ClientSearch *ui;

	QSqlQueryModel *indvQueryMdl;
	QSqlQueryModel *orgQueryMdl;
	QSortFilterProxyModel *proxyMdl;

	clientData client;
};

#endif // CLIENTSEARCH_H

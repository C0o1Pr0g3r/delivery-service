#ifndef CLIENTEDITOR_H
#define CLIENTEDITOR_H

#include "childwindow.h"

#include <QWidget>
#include <QSqlQuery>
#include <QMessageBox>

namespace Ui {
class ClientEditor;
}

class ClientEditor : public ChildWindow
{
	Q_OBJECT

public:
	explicit ClientEditor(QWidget *parent = nullptr);
	ClientEditor(int clientId, bool isIndv, QWidget *parent = nullptr);
	~ClientEditor();

signals:
	void closed();

private slots:

	void on_pbUpdateClientData_clicked();

	void on_pbCancel_clicked();

private:
	void closeEvent(QCloseEvent *event);
	void fillFormWithData(int client_id, bool is_indv);

private:
	Ui::ClientEditor *ui;

	int clientID;
	int clientSubID;
	bool isIndv;
};

#endif // CLIENTEDITOR_H

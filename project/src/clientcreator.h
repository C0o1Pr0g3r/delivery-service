#ifndef CLIENTCREATOR_H
#define CLIENTCREATOR_H

#include "childwindow.h"

#include <QWidget>
#include <qDebug>
#include <QMessageBox>
#include <QSqlQuery>

namespace Ui {
class ClientCreator;
}

class ClientCreator : public ChildWindow
{
	Q_OBJECT

public:
	explicit ClientCreator(QWidget *parent = nullptr);
	~ClientCreator();

signals:
	void closed();

private slots:
	void on_rbIndv_toggled(bool checked);
	void on_pbCreateClient_clicked();
	void on_pbCancel_clicked();

private:
	void closeEvent(QCloseEvent *event);

private:
	Ui::ClientCreator *ui;
};

#endif // CLIENTCREATOR_H

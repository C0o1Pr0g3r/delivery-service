#ifndef ADDRESSSELECTOR_H
#define ADDRESSSELECTOR_H

#include "dbdatamodel.h"
#include "auxiliaryfunctions.h"

#include <QDialog>
#include <QStandardItemModel>
#include <QCompleter>
#include <QMessageBox>

namespace Ui {
class AddressSelector;
}

class AddressSelector : public QDialog
{
	Q_OBJECT

public:
	explicit AddressSelector(QWidget *parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());
	~AddressSelector();

public:
	struct addressData {
		int id;
		QString title;

		addressData()
			: id(0),
			  title("") {}
	} address;

public:
	addressData getAddressData() { return address; }

private slots:
	void on_leHseNum_textChanged(const QString &arg1);
	void on_cmbLoc_currentIndexChanged(int index);
	void on_cmbStreet_currentIndexChanged(int index);
	void on_cmbLoc_currentTextChanged(const QString &arg1);
	void on_cmbStreet_currentTextChanged(const QString &arg1);
	void on_cmbLoc_activated(int index);
	void on_cmbStreet_activated(int index);
	void on_pbOK_clicked();


	void on_pbCancel_clicked();

private:
	void init();
	void setCurrentAddress();
	void resetLocStreetIsPresent();

private:
	Ui::AddressSelector *ui;

	QStandardItemModel *locMdl;
	QStandardItemModel *streetMdl;
	QCompleter *locCmpltr;
	QCompleter *streetCmpltr;

	bool locIdIsPresent = false;
	bool streetIdIsPresent = false;
	int prevLocId = 0;
};

#endif // ADDRESSSELECTOR_H

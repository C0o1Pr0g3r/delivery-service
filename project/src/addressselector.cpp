#include "addressselector.h"
#include "ui_addressselector.h"

AddressSelector::AddressSelector(QWidget *parent, Qt::WindowFlags f) :
	QDialog(parent, f),
	ui(new Ui::AddressSelector),
	address()
{
	ui->setupUi(this);
	init();
}

AddressSelector::~AddressSelector()
{
	delete ui;
}

void AddressSelector::on_cmbLoc_currentIndexChanged(int index)
{
	streetMdl->clear();
	AuxiliaryFunctions::fillStandardModelWithQueryData(streetMdl, QString("select st.id, st_tp.abbrname || ' ' || st.title from st inner join st_tp on st.tp_id = st_tp.id and st.loc_id = %1")
					  .arg(ui->cmbLoc->itemData(index, Qt::UserRole).toInt()));
	ui->cmbStreet->setCurrentIndex(-1);
	ui->leHseNum->setText("");
	resetLocStreetIsPresent();
	locIdIsPresent = index != -1 ? true : false;
	setCurrentAddress();
	prevLocId = ui->cmbLoc->itemData(index, Qt::UserRole).toInt();
}

void AddressSelector::on_cmbStreet_currentIndexChanged(int index)
{
	streetIdIsPresent = index != -1 ? true : false;
	setCurrentAddress();
}

void AddressSelector::on_cmbLoc_currentTextChanged(const QString &arg1)
{
	locIdIsPresent = false;
	setCurrentAddress();
}

void AddressSelector::on_cmbStreet_currentTextChanged(const QString &arg1)
{
	streetIdIsPresent = false;
	setCurrentAddress();
}

void AddressSelector::on_leHseNum_textChanged(const QString &arg1)
{
	setCurrentAddress();
}

void AddressSelector::on_cmbLoc_activated(int index)
{
	locIdIsPresent = true;
	setCurrentAddress();
}

void AddressSelector::on_cmbStreet_activated(int index)
{
	streetIdIsPresent = true;
	setCurrentAddress();
}

void AddressSelector::on_pbOK_clicked()
{
	if (!locIdIsPresent || !streetIdIsPresent || ui->leHseNum->text().isEmpty())
	{
		QMessageBox msgBox(QMessageBox::Warning, "Помилка", "Не всі поля заповнені.", QMessageBox::Ok);
		msgBox.exec();
		return;
	}

	address.id = AuxiliaryFunctions::GetAddressIdIfExistElseCreateAddress(	ui->cmbStreet->itemData(ui->cmbStreet->currentIndex(), Qt::UserRole).toInt()
																, ui->leHseNum->text());
	address.title = ui->lCurrentAddress->text();
	accept();
}

void AddressSelector::on_pbCancel_clicked()
{
	reject();
}

void AddressSelector::init()
{
	locMdl = new QStandardItemModel(this);
	AuxiliaryFunctions::fillStandardModelWithQueryData(	locMdl,
														"select loc.id, loc_tp.abbrname || ' ' || loc.title from loc"
														" inner join loc_tp on loc.tp_id = loc_tp.id"
														" inner join dist on loc.dist_id = dist.id"
														" where dist.rgn_id < 12");
	streetMdl = new QStandardItemModel(this);

	locCmpltr = new QCompleter(locMdl, this);
	locCmpltr->setCaseSensitivity(Qt::CaseInsensitive);
	locCmpltr->setCompletionRole(Qt::DisplayRole);
	connect(locCmpltr, QOverload<const QModelIndex &>::of(&QCompleter::activated),
		[=](const QModelIndex &index){
			if (index.data(Qt::UserRole).toInt() != prevLocId)
			{
				streetMdl->clear();
				AuxiliaryFunctions::fillStandardModelWithQueryData(streetMdl, QString("select st.id, st_tp.abbrname || ' ' || st.title from st inner join st_tp on st.tp_id = st_tp.id and st.loc_id = %1")
								  .arg(index.data(Qt::UserRole).toInt()));
				ui->cmbStreet->setCurrentIndex(-1);
				resetLocStreetIsPresent();
				locIdIsPresent = true;
				setCurrentAddress();
				prevLocId = index.data(Qt::UserRole).toInt();
			}
		}
	);

	streetCmpltr = new QCompleter(streetMdl, this);
	streetCmpltr->setCaseSensitivity(Qt::CaseInsensitive);
	streetCmpltr->setCompletionRole(Qt::DisplayRole);
	connect(streetCmpltr, QOverload<const QModelIndex &>::of(&QCompleter::activated),
		[=](const QModelIndex &index){
			streetIdIsPresent = true;
			setCurrentAddress();
		}
	);

	ui->cmbLoc->setCompleter(locCmpltr);
	ui->cmbStreet->setCompleter(streetCmpltr);
	ui->cmbLoc->setModel(locMdl);
	ui->cmbStreet->setModel(streetMdl);
}

void AddressSelector::setCurrentAddress()
{
	QString loc(ui->cmbLoc->currentText());
	QString street(ui->cmbStreet->currentText());
	QString hse_num(ui->leHseNum->text());
	QString address;
	if (!loc.isEmpty())
	{
		address += loc;
	}
	else
	{
		ui->lCurrentAddress->setText(address);
		return;
	}
	if (!street.isEmpty())
	{
		address += ", " + street;
	}
	else
	{
		ui->lCurrentAddress->setText(address);
		return;
	}
	if (!hse_num.isEmpty())
	{
		address += ", " + hse_num;
	}
	else
	{
		ui->lCurrentAddress->setText(address);
		return;
	}
	ui->lCurrentAddress->setText(address);
}

void AddressSelector::resetLocStreetIsPresent()
{
	locIdIsPresent = false;
	streetIdIsPresent = false;
}

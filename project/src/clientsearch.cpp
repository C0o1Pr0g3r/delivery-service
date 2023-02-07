#include "clientsearch.h"
#include "ui_clientsearch.h"

ClientSearch::ClientSearch(bool enableAddEditClientButtons, QWidget *parent, Qt::WindowFlags f) :
	QDialog(parent, f),
	ui(new Ui::ClientSearch),
	client()
{
	ui->setupUi(this);
	init();
	ui->pbCreateClient->setVisible(enableAddEditClientButtons);
	ui->pbEditClient->setVisible(enableAddEditClientButtons);
}

ClientSearch::~ClientSearch()
{
	delete ui;
}

void ClientSearch::on_rb_indv_clicked(bool checked)
{
	int index = ui->cmb_searchBy->currentIndex();
	ui->cmb_searchBy->clear();
	proxyMdl->setSourceModel(indvQueryMdl);
	ui->cmb_searchBy->addItems(QStringList({"ПІБ", "Мобільним номером"}));
	if (index == 1)
		ui->cmb_searchBy->setCurrentIndex(index);
}

void ClientSearch::on_rb_org_clicked(bool checked)
{
	int index = ui->cmb_searchBy->currentIndex();
	ui->cmb_searchBy->clear();
	proxyMdl->setSourceModel(orgQueryMdl);
	ui->cmb_searchBy->addItems(QStringList({"Назвою", "Мобільним номером", "ЄДРПОУ"}));
	ui->cmb_searchBy->setCurrentIndex(index);
}

void ClientSearch::on_cmb_searchBy_currentIndexChanged(int index)
{
	proxyMdl->setFilterKeyColumn(index + 2);
}

void ClientSearch::on_le_search_textChanged(const QString &arg1)
{
	proxyMdl->setFilterRegExp(QRegExp(QString("^" + arg1), Qt::CaseInsensitive));
}

void ClientSearch::on_tvw_client_clicked(const QModelIndex &index)
{
	client.id = proxyMdl->data(proxyMdl->index(index.row(), 0)).toInt();
	client.subId = proxyMdl->data(proxyMdl->index(index.row(), 1)).toInt();
	client.type = ui->rb_indv->isChecked() ? ClientType::Individual : ClientType::Organization;
	client.name = proxyMdl->data(proxyMdl->index(index.row(), 2)).toString();
	client.mob = proxyMdl->data(proxyMdl->index(index.row(), 3)).toString();
	if (ui->rb_org->isChecked())
	{
		client.USREOU = proxyMdl->data(proxyMdl->index(index.row(), 4)).toString();
	}
	ui->lCurrentClient->setText(client.name + " | " + client.mob);
}

void ClientSearch::on_pbOK_clicked()
{
	if (client.id == 0)
	{
		QMessageBox msgBox(QMessageBox::Warning, "Помилка", "Ви не обрали клієнта.", QMessageBox::Ok);
		msgBox.exec();
		return;
	}

	accept();
}

void ClientSearch::on_pbCancel_clicked()
{
	reject();
}

void ClientSearch::init()
{
	QString query_text("select client.id, indv.id, indv.lastname || ' ' || substr(indv.firstname, 1, 1) || '.' || substr(indv.patronymic, 1, 1) || '.' as LFP,"
							" indv.mob from client inner join indv on client.indv_id = indv.id");
	QStringList headers {"ID", "SubID", "ПІБ", "Телефон"};
	indvQueryMdl = new QSqlQueryModel(this);
	AuxiliaryFunctions::initModel(indvQueryMdl, query_text, headers);

	query_text ="select client.id, org.id,  org.title, org.mob, org.USREOU from client"
				" inner join org on client.org_id = org.id";
	headers = QStringList({"ID", "SubID", "Назва", "Телефон", "ЄДРПОУ"});
	orgQueryMdl = new QSqlQueryModel(this);
	AuxiliaryFunctions::initModel(orgQueryMdl, query_text, headers);


	proxyMdl = new QSortFilterProxyModel(this);
	proxyMdl->setSourceModel(indvQueryMdl);

	AuxiliaryFunctions::setupTableView(ui->tvw_client, proxyMdl, QVector<int>({0, 1}));

	ui->cmb_searchBy->addItems(QStringList({"ПІБ", "Мобільним номером"}));
}

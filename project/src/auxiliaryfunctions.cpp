#include "auxiliaryfunctions.h"
#include "math.h"

bool AuxiliaryFunctions::read_file_to_string(const QString &filename, QString &str)
{
	QFile f(filename);
	if (f.open(QFile::ReadOnly | QFile::Text))
	{
		QTextStream in(&f);
		in.setCodec("UTF-8");
		while (!in.atEnd())
		{
			str += in.readLine();
		}
		return true;
	}
	else
	{
		return false;
	}
}

void AuxiliaryFunctions::fillStandardModelWithQueryData(QStandardItemModel *model, QString query_text)
{
	QSqlQuery query;
	query.exec(query_text);
	while (query.next())
	{
		QStandardItem *item = new QStandardItem();
		item->setData(query.value(0).toInt(), Qt::UserRole);
		item->setData(query.value(1).toString(), Qt::DisplayRole);
		model->appendRow(item);
	}
}

double AuxiliaryFunctions::getDeliveryCost(QVector<int> &len, QVector<int> &wd, QVector<int> &hgt, QVector<double> &wt,
							   QVector<QString> &descr, QVector<int> &amt, QVector<double> &declVal)
{
	double weight_range[] {0.5, 1.0, 2.0, 5.0, 10.0, 20.0, 30.0};
	unsigned int weight_range_size = sizeof(weight_range) / sizeof(weight_range[0]);
	double cost_range[] {45.0, 50.0, 55.0, 60.0, 70.0, 90.0, 110.0};
	unsigned int cost_range_size = sizeof(cost_range) / sizeof(cost_range[0]);
	double surcharge_per_kilogram_of_weight = 4.0;
	double surcharge_per_100_kilograms_of_weight = 75.0;
	int cost = 0;
	int plc_count = len.size();

	QVector<double> volume_weight;
	for (int i = 0; i < plc_count; ++i)
	{
		volume_weight.append(static_cast<double>((len[i] * wd[i] * hgt[i])) / 4000);
	}
	QVector<double> volume_weight_multiplied_by_amount;
	for (int i = 0; i < plc_count; ++i)
	{
		volume_weight_multiplied_by_amount.append(volume_weight[i] * amt[i]);
	}
	QVector<double> weight_multiplied_by_amount;
	for (int i = 0; i < plc_count; ++i)
	{
		weight_multiplied_by_amount.append(wt[i] * amt[i]);
	}
	QVector<double> declared_value_multiplied_by_amount;
	for (int i = 0; i < plc_count; ++i)
	{
		declared_value_multiplied_by_amount.append(declVal[i] * amt[i]);
	}

	double total_volume_weight = 0.0;
	for (int i = 0; i < plc_count; ++i)
	{
		total_volume_weight += volume_weight_multiplied_by_amount[i];
	}
	double total_weight = 0.0;
	for (int i = 0; i < plc_count; ++i)
	{
		total_weight += weight_multiplied_by_amount[i];
	}
	double total_declared_value = 0.0;
	for (int i = 0; i < plc_count; ++i)
	{
		total_declared_value += declared_value_multiplied_by_amount[i];
	}

	double needed_weight = cmpf(total_weight, total_volume_weight) == 1 ? total_weight : total_volume_weight;
	needed_weight = std::ceil(needed_weight);

	double excess_weight = needed_weight - weight_range[weight_range_size - 1];

	unsigned int k;
	for (k = 0; k < weight_range_size; ++k)
	{
		if (cmpf(needed_weight, weight_range[k]) != 1)
		{
			cost += cost_range[k];
			break;
		}
	}

	if (k == weight_range_size)
	{
		cost = cost_range[cost_range_size - 1] + excess_weight * surcharge_per_kilogram_of_weight;
	}

	if (cmpf(total_declared_value, 200.0) != 1)
	{
		cost += 1;
	}
	else
	{
		double commission = roundoff(total_declared_value * 0.005);
		cost += qRound(commission);
	}

	return cost;
}

int AuxiliaryFunctions::cmpf(double a, double b, double epsilon)
{
	int difference = a - b;
	if (fabs(difference) < epsilon)
		return 0;
	else
	{
		if (difference < 0)
			return -1;
		else
			return 1;
	}
}

double AuxiliaryFunctions::roundoff(double value, int prec)
{
  double pow_10 = pow(10.0, (double)prec);
  return round(value * pow_10) / pow_10;
}

int AuxiliaryFunctions::GetAddressIdIfExistElseCreateAddress(int streetId, QString houseNumber)
{
	QString query_text;
	QSqlQuery query;
	int address_id;

	query_text = QString("select id from addr where st_id = %1 and hse_num = '%2'").arg(streetId).arg(houseNumber);
	query.exec(query_text);
	if (query.next())
	{
		address_id = query.value(0).toInt();
	}
	else
	{
		QString query2_text("insert into addr (st_id, hse_num) values (:st_id,:hse_num)");
		QSqlQuery query2;
		query2.prepare(query2_text);
		query2.bindValue(":st_id", streetId);
		query2.bindValue(":hse_num", houseNumber);
		query2.exec();
		address_id = query2.lastInsertId().toInt();
	}

	return address_id;
}

void AuxiliaryFunctions::initModel(QSqlQueryModel *model, const QString &query_text, const QStringList &headers)
{
	model->setQuery(query_text);
	setModelHeaderData(model, headers);
}

void AuxiliaryFunctions::setModelHeaderData(QAbstractItemModel *model, const QStringList &headers)
{
	for (int i = 0; i < headers.size(); ++i)
	{
		model->setHeaderData(i, Qt::Horizontal, headers.at(i));
	}
}

void AuxiliaryFunctions::setupTableView(QTableView *table_view, QAbstractItemModel *model, const QVector<int> &hiddenColumnIndexes)
{
	table_view->setModel(model);
	for (auto index : hiddenColumnIndexes)
	{
		table_view->setColumnHidden(index, true);
	}
	table_view->setSelectionBehavior(QAbstractItemView::SelectRows);
	table_view->setEditTriggers(QAbstractItemView::NoEditTriggers);
	table_view->resizeColumnsToContents();
}

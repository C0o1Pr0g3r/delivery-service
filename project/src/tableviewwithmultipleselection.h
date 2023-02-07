#ifndef TABLEVIEWWITHMULTIPLESELECTION_H
#define TABLEVIEWWITHMULTIPLESELECTION_H

#include <QWidget>
#include <QTableView>

class TableViewWithMultipleSelection : public QTableView
{
	Q_OBJECT
public:
	explicit TableViewWithMultipleSelection(QWidget *parent = nullptr) : QTableView(parent) {}

	QModelIndexList selectedCells()
	{
		return QTableView::selectedIndexes();
	}
};

#endif // TABLEVIEWWITHMULTIPLESELECTION_H

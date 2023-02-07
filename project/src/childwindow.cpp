#include "childwindow.h"

ChildWindow::ChildWindow(QWidget *parent) :
	QWidget(parent),
	parent(parent)
{

}

void ChildWindow::closeEvent(QCloseEvent *event)
{
	emit closed();
}

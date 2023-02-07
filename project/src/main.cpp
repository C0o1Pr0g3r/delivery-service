#include "mainwindow.h"
#include "database.h"
#include "dsdbmapper.h"

#include <QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	MainWindow w;
	int status = a.exec();
	return status;
}

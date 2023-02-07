#ifndef CHILDWINDOW_H
#define CHILDWINDOW_H

#include <QWidget>

class ChildWindow : public QWidget
{
	Q_OBJECT
public:
	explicit ChildWindow(QWidget *parent = nullptr);

signals:
	void closed();

protected:
	QWidget *parent;

private:
	void closeEvent(QCloseEvent *event);
};

#endif // CHILDWINDOW_H

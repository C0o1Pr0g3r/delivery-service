#ifndef PASSWORDCREATOR_H
#define PASSWORDCREATOR_H

#include <QDialog>

namespace Ui {
class PasswordCreator;
}

class PasswordCreator : public QDialog
{
	Q_OBJECT

public:
	explicit PasswordCreator(QWidget *parent = nullptr);
	~PasswordCreator();

public:
	QString getPassword();

private slots:
	void on_tbShowHidePassword_clicked(bool checked);

	void on_pbOK_clicked();

	void on_pbCancel_clicked();

private:
	void init();

private:
	Ui::PasswordCreator *ui;
};

#endif // PASSWORDCREATOR_H

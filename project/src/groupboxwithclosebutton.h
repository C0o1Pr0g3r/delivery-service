#ifndef GROUPBOXWITHCLOSEBUTTON_H
#define GROUPBOXWITHCLOSEBUTTON_H

#include <QGroupBox>
#include <QPushButton>
#include <QLabel>
#include <QEvent>

class GroupBoxWithCloseButton : public QGroupBox
{
	Q_OBJECT
public:
	explicit GroupBoxWithCloseButton(QWidget *parent = nullptr);
	explicit GroupBoxWithCloseButton(const QString &title, QWidget *parent = nullptr);

signals:
	void closeButtonPressed();
	void closeButtonReleased();

protected:
	void resizeEvent(QResizeEvent *event);
	bool eventFilter(QObject *watched, QEvent *event);

private:
	void updateCloseButtonGeometry();

private:
	QLabel *closeButton;
	QSize size = QSize(16, 16);
	QSize size2 = QSize(18, 18);
	int indent_x = (size2 -size).width();
	int indent_y = (size2 -size).height();
	int frame_edge = 0;
	const QString normal_image = ":/images/group-box-with-close-button/close-button-normal.png";
	const QString hovered_image = ":/images/group-box-with-close-button/close-button-hovered.png";
	const QString pressed_image = ":/images/group-box-with-close-button/close-button-pressed.png";
};

#endif // GROUPBOXWITHCLOSEBUTTON_H

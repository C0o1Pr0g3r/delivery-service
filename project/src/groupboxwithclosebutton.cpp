#include "groupboxwithclosebutton.h"

GroupBoxWithCloseButton::GroupBoxWithCloseButton(QWidget *parent) :
	QGroupBox(parent),
	closeButton(new QLabel(this))
{
	closeButton->resize(size.width(), size.height());
	closeButton->setPixmap(QPixmap(normal_image));
	closeButton->installEventFilter(this);
	closeButton->setScaledContents(true);
}

GroupBoxWithCloseButton::GroupBoxWithCloseButton(const QString &title, QWidget *parent) :
	GroupBoxWithCloseButton(parent)
{
	this->setTitle(title);
}

void GroupBoxWithCloseButton::resizeEvent(QResizeEvent *event)
{
	updateCloseButtonGeometry();
	QWidget::resizeEvent(event);
}

bool GroupBoxWithCloseButton::eventFilter(QObject *watched, QEvent *event)
{
	if (watched == closeButton)
	{
		if (event->type() == QEvent::Enter)
		{
			closeButton->setPixmap(QPixmap(hovered_image));
			closeButton->resize(closeButton->width() + (size2- size).width(), closeButton->height() + (size2- size).height());
			updateCloseButtonGeometry();
			return true;
		}
		if (event->type() == QEvent::Leave)
		{
			closeButton->setPixmap(QPixmap(normal_image));
			closeButton->resize(closeButton->width() - (size2- size).width(), closeButton->height() - (size2- size).height());
			updateCloseButtonGeometry();
			return true;
		}
		if (event->type() == QEvent::MouseButtonPress)
		{
			emit closeButtonPressed();
			closeButton->setPixmap(QPixmap(pressed_image));
			return true;
		}
		if (event->type() == QEvent::MouseButtonRelease)
		{
			emit closeButtonReleased();
			closeButton->setPixmap(QPixmap(normal_image));
			return true;
		}
	}
	return false;
}

void GroupBoxWithCloseButton::updateCloseButtonGeometry()
{
	if (closeButton->size() == size)
	{
		closeButton->setGeometry(this->width() - closeButton->width() - (size2 - size).width() / 2, (size2 - size).height() / 2 + frame_edge, closeButton->width(), closeButton->height());
	}
	else
	{
		closeButton->setGeometry(this->width() - closeButton->width(), 0 + frame_edge, closeButton->width(), closeButton->height());
	}
}

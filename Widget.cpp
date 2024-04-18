#include "Widget.h"

Widget::Widget(QWidget* parent)
	:QWidget(parent),roleLabel(new QLabel(this))
{
	update_role();
	roleLabel->resize(400,225);
}

Widget::~Widget()
{

}
void Widget::update_role() {
	roleLabel->setStyleSheet("background_image:url(:/resource/beimihu.jpg)£»");
}
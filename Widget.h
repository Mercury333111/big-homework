#pragma once
#include<QWidget>
#include<Qpixmap>
#include<qlabel.h>
class Widget:public QWidget
{
	Q_OBJECT
public:
	Widget(QWidget* parent=nullptr);
	~Widget();
	void update_role();
private:
	QLabel* roleLabel;
};


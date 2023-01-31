#pragma once

#include <QWidget>
#include "ui_newWidget.h"

class newWidget : public QWidget
{
	Q_OBJECT

public:
	newWidget(QWidget *parent = Q_NULLPTR);
	~newWidget();
public:
	void getData_(QString data);
public slots:
	void getData(QString data);//2.²Ûº¯Êý´«Öµ

private:
	Ui::newWidget *ui;
};

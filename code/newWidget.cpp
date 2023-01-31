#include "newWidget.h"
#include<QDebug>

newWidget::newWidget(QWidget *parent)
	: QWidget(parent),
	ui(new Ui::newWidget)
{
	ui->setupUi(this);
	QFont Font("Microsoft YaHei", 150, 75);
	ui->label->setFont(Font);
	//ui->label->setText("D_P123_23011214:25");
	
}

newWidget::~newWidget()
{
}

void newWidget::getData(QString data) {
	ui->label->setText(data);
	
}
void newWidget::getData_(QString data) {
	ui->label->setText(data);
}


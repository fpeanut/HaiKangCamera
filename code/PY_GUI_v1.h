#pragma once
#pragma execution_character_set("utf-8")
#include <QtWidgets/QWidget>
#include "ui_PY_GUI_v1.h"
#include"newWidget.h"
//header
#include<iostream>
#include "MvCameraControl.h"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "QtCommonFunction.h"
#include <Windows.h>
#include <QCloseEvent>
using namespace std;
//QT
#include <qdir.h>
#include <QDebug>
#include <QDateTime>
#include <Qpainter>
#include <QMessageBox>
#include <QtSerialPort/QSerialPort>         // �ṩ���ʴ��ڵĹ���
#include <QtSerialPort/QSerialPortInfo>     // �ṩϵͳ�д��ڵĴ�����Ϣ
#include <qicon.h>
#include <QThread>
#include <QTimer>
#include <QTime>
#include <QScreen>
#include <QCompleter> 
#include<qlistview.h>
#define GB (1024*1024)

class PY_GUI_v1 : public QWidget
{
	Q_OBJECT

public:
	PY_GUI_v1(QWidget *parent = Q_NULLPTR);
	~PY_GUI_v1();
public:
	//�������
	unsigned int g_nPayloadSize = 0;//������������߶�
	MV_CC_DEVICE_INFO_LIST stDeviceList;
	int nRet = MV_OK;
	void* handle = NULL;
	unsigned int device_id = -1;
	int rec_size = 30;
	//
	QDateTime dataTime;
	QString path;//�������ļ���·��
	QString SerialPort_info = "";//������Ϣ
	cv::Mat srcImage,srcImage_copy;
	long long int preTime, curTime;
	QTimer *pTimer = nullptr;//��ʱ��
	int current_hour;//ĿǰСʱ��
	QString DayNight;//Ŀǰ��ҹ���
	QString pmlList;//��Ʒ�ͺ�
	QString ImgSerialNumber;
	//config����
	QString comName_ini;
	QString LastProInfo_ini;
	QString diskName;
	int btNumber_ini;
	int exportTime_ini;
	int delay_sec=0;
	bool enable_ini;
	
	//����
	//(a)�ɹ������豸��Ϣ��ȡ
	bool PrintDeviceInfo(MV_CC_DEVICE_INFO* pstMVDevInfo);
	//(b)��ȡ������id
	unsigned int GetDeviceId(MV_CC_DEVICE_INFO_LIST &stDeviceList);
	//(c)���ԭʼͼ������תMatͼ��
	bool Convert2Mat(MV_FRAME_OUT_INFO_EX* pstImageInfo, unsigned char * pData);
	//(d)��ȡͼ�񱣴��ַ
	QString GetSavePath();
	//(e)״ָ̬ʾ��
	void setLED(QLabel* label, int color, int size);
	//(f)���ô��ڱ���ͼƬ�����С�ƽ�̡����š�����
	void paintEvent(QPaintEvent * ev);
	//(g)��ʼ�����
	void initCam();
	//(h)��ȡ���̿ռ�
	void getDiskSpace();
	//ʱ����
	long long int GetTimeStamp();
	void closeEvent(QCloseEvent *event);
	
public slots:
	//1.��ȡ������Ϣ
	void sls_serialRecvMsg();
	//2.�����ļ���
	void createDir();
	//3.����Ч��
	void on_pushButton_pressed();
	void on_pushButton_released();
	//4.���ڶϿ�����
	void handleSerialError(QSerialPort::SerialPortError error);
	//5.�ɼ�ͼ��
	void GrapImg();
//signals:
//	void sendData(QString info);
private:
	QSerialPort *m_serial;
	Ui::PY_GUI_v1Class *ui;
	newWidget *form;//�ӽ���ui
};

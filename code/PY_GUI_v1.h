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
#include <QtSerialPort/QSerialPort>         // 提供访问串口的功能
#include <QtSerialPort/QSerialPortInfo>     // 提供系统中存在的串口信息
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
	//相机变量
	unsigned int g_nPayloadSize = 0;//网络最佳网包尺度
	MV_CC_DEVICE_INFO_LIST stDeviceList;
	int nRet = MV_OK;
	void* handle = NULL;
	unsigned int device_id = -1;
	int rec_size = 30;
	//
	QDateTime dataTime;
	QString path;//创建的文件夹路径
	QString SerialPort_info = "";//串口信息
	cv::Mat srcImage,srcImage_copy;
	long long int preTime, curTime;
	QTimer *pTimer = nullptr;//定时器
	int current_hour;//目前小时数
	QString DayNight;//目前昼夜情况
	QString pmlList;//产品型号
	QString ImgSerialNumber;
	//config参数
	QString comName_ini;
	QString LastProInfo_ini;
	QString diskName;
	int btNumber_ini;
	int exportTime_ini;
	int delay_sec=0;
	bool enable_ini;
	
	//函数
	//(a)成功连接设备信息获取
	bool PrintDeviceInfo(MV_CC_DEVICE_INFO* pstMVDevInfo);
	//(b)获取相机设别id
	unsigned int GetDeviceId(MV_CC_DEVICE_INFO_LIST &stDeviceList);
	//(c)相机原始图像数据转Mat图像
	bool Convert2Mat(MV_FRAME_OUT_INFO_EX* pstImageInfo, unsigned char * pData);
	//(d)获取图像保存地址
	QString GetSavePath();
	//(e)状态指示灯
	void setLED(QLabel* label, int color, int size);
	//(f)设置窗口背景图片，居中、平铺、缩放、拉伸
	void paintEvent(QPaintEvent * ev);
	//(g)初始化相机
	void initCam();
	//(h)获取磁盘空间
	void getDiskSpace();
	//时间间隔
	long long int GetTimeStamp();
	void closeEvent(QCloseEvent *event);
	
public slots:
	//1.读取串口信息
	void sls_serialRecvMsg();
	//2.创建文件夹
	void createDir();
	//3.按键效果
	void on_pushButton_pressed();
	void on_pushButton_released();
	//4.串口断开连接
	void handleSerialError(QSerialPort::SerialPortError error);
	//5.采集图像
	void GrapImg();
//signals:
//	void sendData(QString info);
private:
	QSerialPort *m_serial;
	Ui::PY_GUI_v1Class *ui;
	newWidget *form;//子界面ui
};

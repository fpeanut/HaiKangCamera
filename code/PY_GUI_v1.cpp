#include "PY_GUI_v1.h"

PY_GUI_v1::PY_GUI_v1(QWidget *parent)
	: QWidget(parent),
	ui(new Ui::PY_GUI_v1Class)
{
	ui->setupUi(this);
	//显示界面
	form = new newWidget();
	form->show();
	//connect(this, SIGNAL(sendData(QString)), form, SLOT(getData(QString)));//2.槽函数传值
	QList<QScreen *> screen_list= QGuiApplication::screens();//多显示器
	//qDebug() << screen_list.length();
	form->setGeometry(screen_list.at(0)->geometry());
	//这里可以增加判断是否是当前屏幕，我这里只有两个屏幕，直接写死了
	
	//样式
	this->setWindowTitle("PYGUI");
	setWindowFlags(Qt::MSWindowsFixedSizeDialogHint);//固定窗口大小，无法改变
	ui->groupBox->setAlignment(Qt::AlignCenter);//group标题居中
	ui->exportTimeText->setToolTip("范围：46.0000至2499233.0000");
	setLED(ui->comLed1, 0, rec_size);
	setLED(ui->comLed2, 0, rec_size);
	setLED(ui->camLed1, 0, rec_size);
	setLED(ui->camLed2, 0, rec_size);
	setLED(ui->comTime1, 0, rec_size);
	setLED(ui->comTime2, 0, rec_size);

	//获取当前时间
	current_hour= QTime::currentTime().hour();
	if (current_hour >= 6 && current_hour <= 18) {
		setLED(ui->comTime1, 2, rec_size);
		DayNight = "Day";
	}
	else
	{
		setLED(ui->comTime2, 2, rec_size);
		DayNight = "Night";
	}

	//定时器 
	pTimer = new QTimer(this);
	connect(pTimer, SIGNAL(timeout()), this, SLOT(GrapImg()));

	//读取config配置参数
	F_loadQstring_INI(QCoreApplication::applicationDirPath() + "/config/config.ini", "Parameter", "SearlName", comName_ini);
	F_loadQstring_INI(QCoreApplication::applicationDirPath() + "/config/config.ini", "Parameter", "LastProductInfo", LastProInfo_ini);
	F_loadQstring_INI(QCoreApplication::applicationDirPath() + "/config/config.ini", "Parameter", "disname", diskName);
	F_loadInt_INI(QCoreApplication::applicationDirPath() + "/config/config.ini", "Parameter", "btNumber", btNumber_ini);
	F_loadInt_INI(QCoreApplication::applicationDirPath() + "/config/config.ini", "Parameter", "expTime", exportTime_ini);
	F_loadInt_INI(QCoreApplication::applicationDirPath() + "/config/config.ini", "Parameter", "delay", delay_sec);
	F_loadBool_INI(QCoreApplication::applicationDirPath() + "/config/config.ini", "Parameter", "enable", enable_ini);
	F_loadQstring_INI(QCoreApplication::applicationDirPath() + "/config/config.ini", "Parameter", "PML", pmlList);
	ui->text_print->append("配置参数:");
	ui->text_print->append("串口号:" + comName_ini);
	ui->text_print->append("波特率:" + QString::number(btNumber_ini));
	ui->text_print->append("曝光时间:" + QString::number(exportTime_ini));
	ui->text_print->append("延时时间:" + QString::number(delay_sec));
	// 模糊搜素
	ui->comsearch->setEditable(true);
	QStringList m_list = pmlList.split("-");
	ui->comsearch->addItems(m_list);
	ui->comsearch->setView(new QListView());
	ui->comsearch->setEditable(true);
	ui->comsearch->setLineEdit(ui->LEsearch);
	ui->comsearch->setMaxVisibleItems(100);	//下拉最大高度
	ui->LEsearch->clear();
	QCompleter *pCompleter = new QCompleter(m_list, this);
	ui->LEsearch->setCompleter(pCompleter);
	pCompleter->setCaseSensitivity(Qt::CaseInsensitive);
	ui->comsearch->setCompleter(pCompleter);
	//ui->text_dir->setText(LastProInfo_ini);
	//磁盘空间进度条初始化
	ui->SSDSpace->setRange(0, 947200);//范围最大值到最小值diskName
	ui->SSDSpace->setValue(0);

	//自动创建文件夹
	ui->text_dir->setText(QDateTime::currentDateTime().toString("yyyy_MM_dd_hh_mm"));
	path = GetSavePath();

	//建立串口连接
	QList<QSerialPortInfo> list;
	list = QSerialPortInfo::availablePorts();
	QString comName;
	for (int i = 0; i < list.size(); i++)
	{
		if (list.at(i).description() == "USB-SERIAL CH340") {
			comName= list.at(i).portName();
			ui->comText->setText(comName);
		}
	
	}
	m_serial=new QSerialPort;
	connect(m_serial, SIGNAL(readyRead()), this, SLOT(sls_serialRecvMsg()));		//串口接收数据
	//设置串口参数
	connect(m_serial, SIGNAL(errorOccurred(QSerialPort::SerialPortError)),
		this, SLOT(handleSerialError(QSerialPort::SerialPortError)));
	m_serial->setPortName(comName);							//串口号
	m_serial->setBaudRate(btNumber_ini);							//波特率
	m_serial->setDataBits(QSerialPort::Data8);				//数据位
	m_serial->setParity(QSerialPort::NoParity);				//校验位
	m_serial->setStopBits(QSerialPort::OneStop);			//停止位
	m_serial->setFlowControl(QSerialPort::NoFlowControl);	//设置流控制
	m_serial->setReadBufferSize(4096);						//缓存大小

	if (m_serial->open(QIODevice::ReadWrite))				
	{
		ui->text_print->append("串口打开成功。");
		ui->text_print->append("串口号:" + comName + ",波特率:" + ui->bitText->toPlainText());
		setLED(ui->comLed1, 2, rec_size);
	}
	else
	{
		int errCode = m_serial->error();
		ui->text_print->append("串口打开失败");
		setLED(ui->comLed2, 1, rec_size);
		m_serial->close();
	}//读写方式打开

	//初始化相机
	initCam();
	ui->CreateBTN->setEnabled(enable_ini);
		
}

PY_GUI_v1::~PY_GUI_v1() {
	setLED(ui->comLed1, 0, rec_size);
	setLED(ui->comLed2, 0, rec_size);
	setLED(ui->camLed1, 0, rec_size);
	setLED(ui->camLed2, 0, rec_size);

	pTimer->stop();
	ui->text_print->clear();
	m_serial->close();
	delete form;
	// 停止采图
	nRet = MV_CC_StopGrabbing(handle);
	if (MV_OK != nRet)
	{
		printf("Stop Grabbing fail! nRet [0x%x]\n", nRet);
		return;
	}
	// 关闭设备
	nRet = MV_CC_CloseDevice(handle);
	if (MV_OK != nRet)
	{
		printf("ClosDevice fail! nRet [0x%x]\n", nRet);
		return;
	}
	// 销毁句柄
	nRet = MV_CC_DestroyHandle(handle);
	if (MV_OK != nRet)
	{
		printf("Destroy Handle fail! nRet [0x%x]\n", nRet);
		return;
	}
	
}

//(a)输出已经连接成功的设备信息
bool PY_GUI_v1::PrintDeviceInfo(MV_CC_DEVICE_INFO* pstMVDevInfo)
{
	if (NULL == pstMVDevInfo)
	{
		printf("The Pointer of pstMVDevInfo is NULL!\n");
		return false;
	}
	if (pstMVDevInfo->nTLayerType == MV_GIGE_DEVICE)
	{
		int nIp1 = ((pstMVDevInfo->SpecialInfo.stGigEInfo.nCurrentIp & 0xff000000) >> 24);
		int nIp2 = ((pstMVDevInfo->SpecialInfo.stGigEInfo.nCurrentIp & 0x00ff0000) >> 16);
		int nIp3 = ((pstMVDevInfo->SpecialInfo.stGigEInfo.nCurrentIp & 0x0000ff00) >> 8);
		int nIp4 = (pstMVDevInfo->SpecialInfo.stGigEInfo.nCurrentIp & 0x000000ff);

		// print current ip and user defined name
		printf("CurrentIp: %d.%d.%d.%d\n", nIp1, nIp2, nIp3, nIp4);
		ui->text_print->append("CurrentIp:" + QString::number(nIp1) + "." + QString::number(nIp2) + "." + QString::number(nIp3) + "." + QString::number(nIp4) + ".");
		printf("UserDefinedName: %s\n\n", pstMVDevInfo->SpecialInfo.stGigEInfo.chUserDefinedName);
	}
	else if (pstMVDevInfo->nTLayerType == MV_USB_DEVICE)
	{
		printf("UserDefinedName: %s\n", pstMVDevInfo->SpecialInfo.stUsb3VInfo.chUserDefinedName);
		printf("Serial Number: %s\n", pstMVDevInfo->SpecialInfo.stUsb3VInfo.chSerialNumber);
		printf("Device Number: %d\n\n", pstMVDevInfo->SpecialInfo.stUsb3VInfo.nDeviceNumber);
	}
	else
	{
		printf("Not support.\n");
		ui->text_print->append("没有支持的相机设备。");
	}

	return true;
}

//(b)获取相机设备id
unsigned int PY_GUI_v1::GetDeviceId(MV_CC_DEVICE_INFO_LIST &stDeviceList) {
	int temp_id = -1;
	::memset(&stDeviceList, 0, sizeof(MV_CC_DEVICE_INFO_LIST));//为大空间结构体或大数组空间初始化或清零
	nRet = MV_CC_EnumDevices(MV_GIGE_DEVICE | MV_USB_DEVICE, &stDeviceList);//枚举连接成功的设备
	if (MV_OK != nRet)
	{
		printf("Enum Devices fail! nRet [0x%x]\n", nRet);
		return 0;
	}
	if (stDeviceList.nDeviceNum == 1)
	{
		temp_id = 0;
		MV_CC_DEVICE_INFO* pDeviceInfo = stDeviceList.pDeviceInfo[0];
		if (NULL == pDeviceInfo)
			return 0;
		PrintDeviceInfo(pDeviceInfo);
	}//打印设备信息
	else if (stDeviceList.nDeviceNum > 1)
	{
		printf("device is over two!");
		return 0;
	}
	else
	{
		printf("enum error!");
		return 0;
	}
	return temp_id;
}

//(c)图像类型转换
bool PY_GUI_v1::Convert2Mat(MV_FRAME_OUT_INFO_EX* pstImageInfo, unsigned char * pData) {
	srcImage.create(pstImageInfo->nHeight, pstImageInfo->nWidth, CV_8UC3);
	cv::Mat img = cv::Mat::zeros(pstImageInfo->nHeight, pstImageInfo->nWidth, CV_8UC1);//放外面
	memcpy(img.data, pData, pstImageInfo->nWidth* pstImageInfo->nHeight);//内存拷贝
	cv::cvtColor(img, srcImage, CV_BayerGB2RGB);
	return true;
}

//(d)获取图像保存地址
QString PY_GUI_v1::GetSavePath() {
	QString currentPath = QDir::currentPath();//获取当前路径
	dataTime = QDateTime::currentDateTime();//获取当前时间
	QString timeToday;
	timeToday = dataTime.toString("yyyy_MM_dd");
	QDir dir, dir_next0;
	QString firstPath = currentPath + "/SaveImage/";
	QString secondtPath;
	//创建SaveImage
	if (!dir.exists(firstPath)) {
		dir.mkdir(firstPath);
	}
	else
	{
		qDebug() << "SaveImage文件夹已创建";
	}
	//创建SaveImage/年月日
	if (dir.exists(firstPath)) {
		secondtPath = firstPath + timeToday + "/";
		if (!dir_next0.exists(secondtPath)) {
			dir_next0.mkdir(secondtPath);
		}
		else
		{
			qDebug() << "SaveImage/" + timeToday + "文件夹已创建";
		}
	}
	//创建SaveImage/年月日/种类
	if (dir_next0.exists(secondtPath)) {
		QDir dir_next1;
		QString thirthPath = secondtPath + ui->text_dir->toPlainText()+"/";
		if (!dir_next1.exists(thirthPath)) {
			dir_next1.mkdir(thirthPath);			
			return thirthPath;
		}
		else
		{
			qDebug() << "SaveImage/" + timeToday + "/" + ui->text_dir->toPlainText() + "文件夹已创建";
			return thirthPath;
		}
	}
}

//(g)初始化相机
void PY_GUI_v1::initCam() {
	//1.获取设备的编号
	device_id = GetDeviceId(stDeviceList);
	//2.创建图像数据句柄
	nRet = MV_CC_CreateHandle(&handle, stDeviceList.pDeviceInfo[device_id]);
	if (MV_OK != nRet)
	{
		printf("Create Handle fail! nRet [0x%x]\n", nRet);
		return;
	}
	//3.打开相机设备
	nRet = MV_CC_OpenDevice(handle);
	if (MV_OK != nRet)
	{
		printf("Open Device fail! nRet [0x%x]\n", nRet);
		ui->text_print->append("相机打开失败！");
		setLED(ui->camLed2, 1, rec_size);
		setLED(ui->camLed1, 0, rec_size);
		return;
	}
	else
	{
		setLED(ui->camLed1, 2, rec_size);
		setLED(ui->camLed2, 0, rec_size);
	}
	//4.检测网络最佳封装尺寸（仅适用于GigE相机）
	if (stDeviceList.pDeviceInfo[device_id]->nTLayerType == MV_GIGE_DEVICE)//确认传输协议类型，网线，usb
	{
		int nPacketSize = MV_CC_GetOptimalPacketSize(handle);
		if (nPacketSize > 0)
		{
			nRet = MV_CC_SetIntValue(handle, "GevSCPSPacketSize", nPacketSize); //返回给调用者有关相机宽度的信息结构体指针
			if (nRet != MV_OK)
			{
				printf("Warning: Set Packet Size fail nRet [0x%x]!", nRet);
			}
		}
		else
		{
			printf("Warning: Get Packet Size fail nRet [0x%x]!", nPacketSize);
		}
	}
	//5.将触发模式设置为关闭
	nRet = MV_CC_SetEnumValue(handle, "TriggerMode", 0);
	if (MV_OK != nRet)
	{
		printf("Set Trigger Mode fail! nRet [0x%x]\n", nRet);
		return;
	}
	//6.获取有效载荷大小
	MVCC_INTVALUE stParam;
	::memset(&stParam, 0, sizeof(MVCC_INTVALUE));
	nRet = MV_CC_GetIntValue(handle, "PayloadSize", &stParam);
	if (MV_OK != nRet)
	{
		printf("Get PayloadSize fail! nRet [0x%x]\n", nRet);
		return;
	}
	g_nPayloadSize = stParam.nCurValue;
	//7.设置曝光时间
	nRet = MV_CC_SetEnumValue(handle, "ExposureMode", 0);//0：Timed
	nRet = MV_CC_SetFloatValue(handle, "ExposureTime", exportTime_ini);
	nRet = MV_CC_StartGrabbing(handle);
	if (MV_OK != nRet)
	{
		printf("Start Grabbing fail! nRet [0x%x]\n", nRet);
		return;
	}
}

//(h)获取磁盘空间
void PY_GUI_v1::getDiskSpace() {
	const wchar_t * encodedName = reinterpret_cast<const wchar_t  *>(diskName.utf16());//qstring强转LPCSTR
	BOOL bResult = FALSE;
	DWORD64 qwFreeBytes, qwFreeBytesTocaller, qwTotalBytes;
	bResult = GetDiskFreeSpaceEx(encodedName,
		(PULARGE_INTEGER)&qwFreeBytesTocaller,//磁盘空闲容量
		(PULARGE_INTEGER)&qwTotalBytes,//总容量
		(PULARGE_INTEGER)&qwFreeBytes);//可用磁盘空间
	ui->SSDSpace->setValue((qwTotalBytes - qwFreeBytesTocaller) / GB);
	qDebug() << (qwTotalBytes - qwFreeBytesTocaller) / GB;
	int process_ = (ui->SSDSpace->value() - ui->SSDSpace->minimum())*100.0 / (ui->SSDSpace->maximum() - ui->SSDSpace->minimum());//计算百分比
	QString s1 = "QProgressBar {\
    text-align: center;\
	}";
	QString s2 = "QProgressBar::chunk {\
    background-color: #ff0000;\
	}";
	if (process_ > 80.0) {
		ui->SSDSpace->setStyleSheet(s1 + s2);
	}

}

#pragma region 槽函数

//1.串口接收数据
void PY_GUI_v1::sls_serialRecvMsg()
{
	//获取串口数据
	QByteArray recvBuffer = m_serial->readAll();
	SerialPort_info = QString::fromLocal8Bit(recvBuffer);

	if (SerialPort_info == "START") {
		pTimer->stop();
		pTimer->start(delay_sec * 1000);
		//preTime = GetTimeStamp();
	}
	if (SerialPort_info == "OVER") {
		pTimer->stop();
	}
}

//2.创建文件夹
void PY_GUI_v1::createDir() {
	if (ui->text_dir->toPlainText() != "") {
		path=GetSavePath();
		F_saveQstring_INI(QDir::currentPath() + "/config/config.ini", "Parameter", "LastProductInfo", ui->text_dir->toPlainText());		
	}
	else
	{
		QMessageBox::warning(this, tr("警示"), tr("请输入文件夹名字"),
			QMessageBox::Ok, QMessageBox::Ok);
	}
}

//4.串口断开连接
void PY_GUI_v1::handleSerialError(QSerialPort::SerialPortError error)//检测串口连接错误
{
	if (error == QSerialPort::ResourceError || error == QSerialPort::PermissionError) {//包括串口被拔出来的错误
		qDebug() << "串口连接中断，请检查是否正确连接!";		
		setLED(ui->comLed2, 1, rec_size);
		setLED(ui->comLed1, 0, rec_size);
		m_serial->close();
	}
}

//5.采集图像
void PY_GUI_v1::GrapImg() {
	qDebug() << "采图";
	do {
		MV_FRAME_OUT_INFO_EX stImageInfo = { 0 };
		::memset(&stImageInfo, 0, sizeof(MV_FRAME_OUT_INFO_EX));
		unsigned char * pData = (unsigned char *)malloc(sizeof(unsigned char) * (g_nPayloadSize));
		if (pData == NULL)
		{
			printf("Allocate memory failed.\n");
			break;
		}
		nRet = MV_CC_GetOneFrameTimeout(handle, pData, g_nPayloadSize, &stImageInfo, 1000);// 从相机获取一帧超时= 1000ms
		if (nRet != MV_OK)
		{
			printf("No data[0x%x]\n", nRet);
			free(pData);
			pData = NULL;
			break;
		}
		bool bConvertRet = false;
		//图像类型转换
		bConvertRet = Convert2Mat(&stImageInfo, pData);
		//图像显示在qt界面上,默认自适应填充
		F_showMat(srcImage, ui->label_show, true);
		//信号打印
		ui->text_print->append(SerialPort_info);
		//读取磁盘空间
		getDiskSpace();
		//图像保存
		current_hour = QTime::currentTime().hour();//获取当前时间
		if (current_hour >= 6 && current_hour < 18) {
			setLED(ui->comTime1, 2, rec_size);
			DayNight = "D";
		}
		else
		{
			setLED(ui->comTime2, 2, rec_size);
			DayNight = "N";
		}
		int str_len = ui->comsearch->currentText().length();
		QString product_info = ui->comsearch->currentText().mid(str_len-4,str_len);//取产品型号后四位
		ImgSerialNumber = DayNight + "_" + product_info + "_" +
			QDateTime::currentDateTime().toString("yyyyMMddhhmm");//图像保存编号
		//string img_path = (path + QDateTime::currentDateTime().toString("yyyy_MM_dd_hh_mm_ss_zzz")).toStdString();
		form->getData_(ImgSerialNumber);//第二界面ui显示
		cv::cvtColor(srcImage, srcImage_copy, cv::COLOR_RGB2BGR);
		cv::imwrite((path+ImgSerialNumber).toStdString() + ".bmp", srcImage_copy);
		//if ((SerialPort_info == "START") && (saveflag == true)) {
		//	getDiskSpace();//读取磁盘空间
		//	/*ui->text_print->append(SerialPort_info);*/
		//	string img_path = (path + QDateTime::currentDateTime().toString("yyyy_MM_dd_hh_mm_ss_zzz")).toStdString();
		//	cv::cvtColor(srcImage, srcImage_copy, cv::COLOR_RGB2BGR);
		//	cv::imwrite(img_path + ".bmp", srcImage_copy);
		//	SerialPort_info = "";
		//}
		//释放缓存
		pTimer->stop();
		if (bConvertRet)
		{
			free(pData);
			pData = NULL;
		}
		else
		{
			printf("OpenCV format convert failed.\n");
			free(pData);
			pData = NULL;
		}
		srcImage.release();
	} while (0);
}
#pragma endregion

#pragma region 控件样式
//(e)设置状态指示灯
void PY_GUI_v1::setLED(QLabel* label, int color, int size)
{
	// 该函数将label控件变成一个圆形指示灯，需要指定颜色color以及直径size
	// color 0:grey 1:red 2:green 3:yellow
	// size  单位是像素
	// 将label中的文字清空
	label->setText("");
	// 先设置矩形大小
	// 如果ui界面设置的label大小比最小宽度和高度小，矩形将被设置为最小宽度和最小高度；
	// 如果ui界面设置的label大小比最小宽度和高度大，矩形将被设置为最大宽度和最大高度；
	QString min_width = QString("min-width: %1px;").arg(size);              // 最小宽度：size
	QString min_height = QString("min-height: %1px;").arg(size);            // 最小高度：size
	QString max_width = QString("max-width: %1px;").arg(size);              // 最小宽度：size
	QString max_height = QString("max-height: %1px;").arg(size);            // 最小高度：size
	// 再设置边界形状及边框
	QString border_radius = QString("border-radius: %1px;").arg(size / 2);    // 边框是圆角，半径为size/2
	QString border = QString("border:1px solid black;");                    // 边框为1px黑色
	// 最后设置背景颜色
	QString background = "background-color:";
	switch (color) {
	case 0:
		// 灰色
		background += "rgb(190,190,190)";
		break;
	case 1:
		// 红色
		background += "rgb(255,0,0)";
		break;
	case 2:
		// 绿色
		background += "rgb(0,255,0)";
		break;
	case 3:
		// 黄色
		background += "rgb(255,255,0)";
		break;
	default:
		break;
	}

	const QString SheetStyle = min_width + min_height + max_width + max_height + border_radius + border + background;
	label->setStyleSheet(SheetStyle);
}
//(f)设置窗口背景图片，居中、平铺、缩放、拉伸
void PY_GUI_v1::paintEvent(QPaintEvent * ev) {
	QString _currentPath = QDir::currentPath();
	QPainter painter(this);
	painter.drawPixmap(rect(), QPixmap(_currentPath+"/css/bg.png"), QRect());
}
//3.按键效果
void PY_GUI_v1::on_pushButton_pressed() {
	ui->CreateBTN->setGeometry(ui->CreateBTN->x() + 5, ui->CreateBTN->y() + 5, ui->CreateBTN->width() - 10, ui->CreateBTN->height() - 10);
}
void PY_GUI_v1::on_pushButton_released() {
	ui->CreateBTN->setGeometry(ui->CreateBTN->x() - 5, ui->CreateBTN->y() - 5, ui->CreateBTN->width() + 10, ui->CreateBTN->height() + 10);
}
#pragma endregion

//时间间隔
long long int PY_GUI_v1::GetTimeStamp()
{
	QDateTime datetime = QDateTime::currentDateTime();
	return datetime.toMSecsSinceEpoch();
}

void PY_GUI_v1::closeEvent(QCloseEvent *event)
{
	QMessageBox::StandardButton resBtn = QMessageBox::question(this, "PYGUI",
		tr("Are you sure?\n"),
		QMessageBox::Cancel | QMessageBox::No | QMessageBox::Yes,
		QMessageBox::Yes);
	if (resBtn != QMessageBox::Yes) {
		event->ignore();
	}
	else {
		event->accept();
		form->close();
	}
}
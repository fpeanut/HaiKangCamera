#include "PY_GUI_v1.h"

PY_GUI_v1::PY_GUI_v1(QWidget *parent)
	: QWidget(parent),
	ui(new Ui::PY_GUI_v1Class)
{
	ui->setupUi(this);
	//��ʾ����
	form = new newWidget();
	form->show();
	//connect(this, SIGNAL(sendData(QString)), form, SLOT(getData(QString)));//2.�ۺ�����ֵ
	QList<QScreen *> screen_list= QGuiApplication::screens();//����ʾ��
	//qDebug() << screen_list.length();
	form->setGeometry(screen_list.at(0)->geometry());
	//������������ж��Ƿ��ǵ�ǰ��Ļ��������ֻ��������Ļ��ֱ��д����
	
	//��ʽ
	this->setWindowTitle("PYGUI");
	setWindowFlags(Qt::MSWindowsFixedSizeDialogHint);//�̶����ڴ�С���޷��ı�
	ui->groupBox->setAlignment(Qt::AlignCenter);//group�������
	ui->exportTimeText->setToolTip("��Χ��46.0000��2499233.0000");
	setLED(ui->comLed1, 0, rec_size);
	setLED(ui->comLed2, 0, rec_size);
	setLED(ui->camLed1, 0, rec_size);
	setLED(ui->camLed2, 0, rec_size);
	setLED(ui->comTime1, 0, rec_size);
	setLED(ui->comTime2, 0, rec_size);

	//��ȡ��ǰʱ��
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

	//��ʱ�� 
	pTimer = new QTimer(this);
	connect(pTimer, SIGNAL(timeout()), this, SLOT(GrapImg()));

	//��ȡconfig���ò���
	F_loadQstring_INI(QCoreApplication::applicationDirPath() + "/config/config.ini", "Parameter", "SearlName", comName_ini);
	F_loadQstring_INI(QCoreApplication::applicationDirPath() + "/config/config.ini", "Parameter", "LastProductInfo", LastProInfo_ini);
	F_loadQstring_INI(QCoreApplication::applicationDirPath() + "/config/config.ini", "Parameter", "disname", diskName);
	F_loadInt_INI(QCoreApplication::applicationDirPath() + "/config/config.ini", "Parameter", "btNumber", btNumber_ini);
	F_loadInt_INI(QCoreApplication::applicationDirPath() + "/config/config.ini", "Parameter", "expTime", exportTime_ini);
	F_loadInt_INI(QCoreApplication::applicationDirPath() + "/config/config.ini", "Parameter", "delay", delay_sec);
	F_loadBool_INI(QCoreApplication::applicationDirPath() + "/config/config.ini", "Parameter", "enable", enable_ini);
	F_loadQstring_INI(QCoreApplication::applicationDirPath() + "/config/config.ini", "Parameter", "PML", pmlList);
	ui->text_print->append("���ò���:");
	ui->text_print->append("���ں�:" + comName_ini);
	ui->text_print->append("������:" + QString::number(btNumber_ini));
	ui->text_print->append("�ع�ʱ��:" + QString::number(exportTime_ini));
	ui->text_print->append("��ʱʱ��:" + QString::number(delay_sec));
	// ģ������
	ui->comsearch->setEditable(true);
	QStringList m_list = pmlList.split("-");
	ui->comsearch->addItems(m_list);
	ui->comsearch->setView(new QListView());
	ui->comsearch->setEditable(true);
	ui->comsearch->setLineEdit(ui->LEsearch);
	ui->comsearch->setMaxVisibleItems(100);	//�������߶�
	ui->LEsearch->clear();
	QCompleter *pCompleter = new QCompleter(m_list, this);
	ui->LEsearch->setCompleter(pCompleter);
	pCompleter->setCaseSensitivity(Qt::CaseInsensitive);
	ui->comsearch->setCompleter(pCompleter);
	//ui->text_dir->setText(LastProInfo_ini);
	//���̿ռ��������ʼ��
	ui->SSDSpace->setRange(0, 947200);//��Χ���ֵ����СֵdiskName
	ui->SSDSpace->setValue(0);

	//�Զ������ļ���
	ui->text_dir->setText(QDateTime::currentDateTime().toString("yyyy_MM_dd_hh_mm"));
	path = GetSavePath();

	//������������
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
	connect(m_serial, SIGNAL(readyRead()), this, SLOT(sls_serialRecvMsg()));		//���ڽ�������
	//���ô��ڲ���
	connect(m_serial, SIGNAL(errorOccurred(QSerialPort::SerialPortError)),
		this, SLOT(handleSerialError(QSerialPort::SerialPortError)));
	m_serial->setPortName(comName);							//���ں�
	m_serial->setBaudRate(btNumber_ini);							//������
	m_serial->setDataBits(QSerialPort::Data8);				//����λ
	m_serial->setParity(QSerialPort::NoParity);				//У��λ
	m_serial->setStopBits(QSerialPort::OneStop);			//ֹͣλ
	m_serial->setFlowControl(QSerialPort::NoFlowControl);	//����������
	m_serial->setReadBufferSize(4096);						//�����С

	if (m_serial->open(QIODevice::ReadWrite))				
	{
		ui->text_print->append("���ڴ򿪳ɹ���");
		ui->text_print->append("���ں�:" + comName + ",������:" + ui->bitText->toPlainText());
		setLED(ui->comLed1, 2, rec_size);
	}
	else
	{
		int errCode = m_serial->error();
		ui->text_print->append("���ڴ�ʧ��");
		setLED(ui->comLed2, 1, rec_size);
		m_serial->close();
	}//��д��ʽ��

	//��ʼ�����
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
	// ֹͣ��ͼ
	nRet = MV_CC_StopGrabbing(handle);
	if (MV_OK != nRet)
	{
		printf("Stop Grabbing fail! nRet [0x%x]\n", nRet);
		return;
	}
	// �ر��豸
	nRet = MV_CC_CloseDevice(handle);
	if (MV_OK != nRet)
	{
		printf("ClosDevice fail! nRet [0x%x]\n", nRet);
		return;
	}
	// ���پ��
	nRet = MV_CC_DestroyHandle(handle);
	if (MV_OK != nRet)
	{
		printf("Destroy Handle fail! nRet [0x%x]\n", nRet);
		return;
	}
	
}

//(a)����Ѿ����ӳɹ����豸��Ϣ
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
		ui->text_print->append("û��֧�ֵ�����豸��");
	}

	return true;
}

//(b)��ȡ����豸id
unsigned int PY_GUI_v1::GetDeviceId(MV_CC_DEVICE_INFO_LIST &stDeviceList) {
	int temp_id = -1;
	::memset(&stDeviceList, 0, sizeof(MV_CC_DEVICE_INFO_LIST));//Ϊ��ռ�ṹ��������ռ��ʼ��������
	nRet = MV_CC_EnumDevices(MV_GIGE_DEVICE | MV_USB_DEVICE, &stDeviceList);//ö�����ӳɹ����豸
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
	}//��ӡ�豸��Ϣ
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

//(c)ͼ������ת��
bool PY_GUI_v1::Convert2Mat(MV_FRAME_OUT_INFO_EX* pstImageInfo, unsigned char * pData) {
	srcImage.create(pstImageInfo->nHeight, pstImageInfo->nWidth, CV_8UC3);
	cv::Mat img = cv::Mat::zeros(pstImageInfo->nHeight, pstImageInfo->nWidth, CV_8UC1);//������
	memcpy(img.data, pData, pstImageInfo->nWidth* pstImageInfo->nHeight);//�ڴ濽��
	cv::cvtColor(img, srcImage, CV_BayerGB2RGB);
	return true;
}

//(d)��ȡͼ�񱣴��ַ
QString PY_GUI_v1::GetSavePath() {
	QString currentPath = QDir::currentPath();//��ȡ��ǰ·��
	dataTime = QDateTime::currentDateTime();//��ȡ��ǰʱ��
	QString timeToday;
	timeToday = dataTime.toString("yyyy_MM_dd");
	QDir dir, dir_next0;
	QString firstPath = currentPath + "/SaveImage/";
	QString secondtPath;
	//����SaveImage
	if (!dir.exists(firstPath)) {
		dir.mkdir(firstPath);
	}
	else
	{
		qDebug() << "SaveImage�ļ����Ѵ���";
	}
	//����SaveImage/������
	if (dir.exists(firstPath)) {
		secondtPath = firstPath + timeToday + "/";
		if (!dir_next0.exists(secondtPath)) {
			dir_next0.mkdir(secondtPath);
		}
		else
		{
			qDebug() << "SaveImage/" + timeToday + "�ļ����Ѵ���";
		}
	}
	//����SaveImage/������/����
	if (dir_next0.exists(secondtPath)) {
		QDir dir_next1;
		QString thirthPath = secondtPath + ui->text_dir->toPlainText()+"/";
		if (!dir_next1.exists(thirthPath)) {
			dir_next1.mkdir(thirthPath);			
			return thirthPath;
		}
		else
		{
			qDebug() << "SaveImage/" + timeToday + "/" + ui->text_dir->toPlainText() + "�ļ����Ѵ���";
			return thirthPath;
		}
	}
}

//(g)��ʼ�����
void PY_GUI_v1::initCam() {
	//1.��ȡ�豸�ı��
	device_id = GetDeviceId(stDeviceList);
	//2.����ͼ�����ݾ��
	nRet = MV_CC_CreateHandle(&handle, stDeviceList.pDeviceInfo[device_id]);
	if (MV_OK != nRet)
	{
		printf("Create Handle fail! nRet [0x%x]\n", nRet);
		return;
	}
	//3.������豸
	nRet = MV_CC_OpenDevice(handle);
	if (MV_OK != nRet)
	{
		printf("Open Device fail! nRet [0x%x]\n", nRet);
		ui->text_print->append("�����ʧ�ܣ�");
		setLED(ui->camLed2, 1, rec_size);
		setLED(ui->camLed1, 0, rec_size);
		return;
	}
	else
	{
		setLED(ui->camLed1, 2, rec_size);
		setLED(ui->camLed2, 0, rec_size);
	}
	//4.���������ѷ�װ�ߴ磨��������GigE�����
	if (stDeviceList.pDeviceInfo[device_id]->nTLayerType == MV_GIGE_DEVICE)//ȷ�ϴ���Э�����ͣ����ߣ�usb
	{
		int nPacketSize = MV_CC_GetOptimalPacketSize(handle);
		if (nPacketSize > 0)
		{
			nRet = MV_CC_SetIntValue(handle, "GevSCPSPacketSize", nPacketSize); //���ظ��������й������ȵ���Ϣ�ṹ��ָ��
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
	//5.������ģʽ����Ϊ�ر�
	nRet = MV_CC_SetEnumValue(handle, "TriggerMode", 0);
	if (MV_OK != nRet)
	{
		printf("Set Trigger Mode fail! nRet [0x%x]\n", nRet);
		return;
	}
	//6.��ȡ��Ч�غɴ�С
	MVCC_INTVALUE stParam;
	::memset(&stParam, 0, sizeof(MVCC_INTVALUE));
	nRet = MV_CC_GetIntValue(handle, "PayloadSize", &stParam);
	if (MV_OK != nRet)
	{
		printf("Get PayloadSize fail! nRet [0x%x]\n", nRet);
		return;
	}
	g_nPayloadSize = stParam.nCurValue;
	//7.�����ع�ʱ��
	nRet = MV_CC_SetEnumValue(handle, "ExposureMode", 0);//0��Timed
	nRet = MV_CC_SetFloatValue(handle, "ExposureTime", exportTime_ini);
	nRet = MV_CC_StartGrabbing(handle);
	if (MV_OK != nRet)
	{
		printf("Start Grabbing fail! nRet [0x%x]\n", nRet);
		return;
	}
}

//(h)��ȡ���̿ռ�
void PY_GUI_v1::getDiskSpace() {
	const wchar_t * encodedName = reinterpret_cast<const wchar_t  *>(diskName.utf16());//qstringǿתLPCSTR
	BOOL bResult = FALSE;
	DWORD64 qwFreeBytes, qwFreeBytesTocaller, qwTotalBytes;
	bResult = GetDiskFreeSpaceEx(encodedName,
		(PULARGE_INTEGER)&qwFreeBytesTocaller,//���̿�������
		(PULARGE_INTEGER)&qwTotalBytes,//������
		(PULARGE_INTEGER)&qwFreeBytes);//���ô��̿ռ�
	ui->SSDSpace->setValue((qwTotalBytes - qwFreeBytesTocaller) / GB);
	qDebug() << (qwTotalBytes - qwFreeBytesTocaller) / GB;
	int process_ = (ui->SSDSpace->value() - ui->SSDSpace->minimum())*100.0 / (ui->SSDSpace->maximum() - ui->SSDSpace->minimum());//����ٷֱ�
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

#pragma region �ۺ���

//1.���ڽ�������
void PY_GUI_v1::sls_serialRecvMsg()
{
	//��ȡ��������
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

//2.�����ļ���
void PY_GUI_v1::createDir() {
	if (ui->text_dir->toPlainText() != "") {
		path=GetSavePath();
		F_saveQstring_INI(QDir::currentPath() + "/config/config.ini", "Parameter", "LastProductInfo", ui->text_dir->toPlainText());		
	}
	else
	{
		QMessageBox::warning(this, tr("��ʾ"), tr("�������ļ�������"),
			QMessageBox::Ok, QMessageBox::Ok);
	}
}

//4.���ڶϿ�����
void PY_GUI_v1::handleSerialError(QSerialPort::SerialPortError error)//��⴮�����Ӵ���
{
	if (error == QSerialPort::ResourceError || error == QSerialPort::PermissionError) {//�������ڱ��γ����Ĵ���
		qDebug() << "���������жϣ������Ƿ���ȷ����!";		
		setLED(ui->comLed2, 1, rec_size);
		setLED(ui->comLed1, 0, rec_size);
		m_serial->close();
	}
}

//5.�ɼ�ͼ��
void PY_GUI_v1::GrapImg() {
	qDebug() << "��ͼ";
	do {
		MV_FRAME_OUT_INFO_EX stImageInfo = { 0 };
		::memset(&stImageInfo, 0, sizeof(MV_FRAME_OUT_INFO_EX));
		unsigned char * pData = (unsigned char *)malloc(sizeof(unsigned char) * (g_nPayloadSize));
		if (pData == NULL)
		{
			printf("Allocate memory failed.\n");
			break;
		}
		nRet = MV_CC_GetOneFrameTimeout(handle, pData, g_nPayloadSize, &stImageInfo, 1000);// �������ȡһ֡��ʱ= 1000ms
		if (nRet != MV_OK)
		{
			printf("No data[0x%x]\n", nRet);
			free(pData);
			pData = NULL;
			break;
		}
		bool bConvertRet = false;
		//ͼ������ת��
		bConvertRet = Convert2Mat(&stImageInfo, pData);
		//ͼ����ʾ��qt������,Ĭ������Ӧ���
		F_showMat(srcImage, ui->label_show, true);
		//�źŴ�ӡ
		ui->text_print->append(SerialPort_info);
		//��ȡ���̿ռ�
		getDiskSpace();
		//ͼ�񱣴�
		current_hour = QTime::currentTime().hour();//��ȡ��ǰʱ��
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
		QString product_info = ui->comsearch->currentText().mid(str_len-4,str_len);//ȡ��Ʒ�ͺź���λ
		ImgSerialNumber = DayNight + "_" + product_info + "_" +
			QDateTime::currentDateTime().toString("yyyyMMddhhmm");//ͼ�񱣴���
		//string img_path = (path + QDateTime::currentDateTime().toString("yyyy_MM_dd_hh_mm_ss_zzz")).toStdString();
		form->getData_(ImgSerialNumber);//�ڶ�����ui��ʾ
		cv::cvtColor(srcImage, srcImage_copy, cv::COLOR_RGB2BGR);
		cv::imwrite((path+ImgSerialNumber).toStdString() + ".bmp", srcImage_copy);
		//if ((SerialPort_info == "START") && (saveflag == true)) {
		//	getDiskSpace();//��ȡ���̿ռ�
		//	/*ui->text_print->append(SerialPort_info);*/
		//	string img_path = (path + QDateTime::currentDateTime().toString("yyyy_MM_dd_hh_mm_ss_zzz")).toStdString();
		//	cv::cvtColor(srcImage, srcImage_copy, cv::COLOR_RGB2BGR);
		//	cv::imwrite(img_path + ".bmp", srcImage_copy);
		//	SerialPort_info = "";
		//}
		//�ͷŻ���
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

#pragma region �ؼ���ʽ
//(e)����״ָ̬ʾ��
void PY_GUI_v1::setLED(QLabel* label, int color, int size)
{
	// �ú�����label�ؼ����һ��Բ��ָʾ�ƣ���Ҫָ����ɫcolor�Լ�ֱ��size
	// color 0:grey 1:red 2:green 3:yellow
	// size  ��λ������
	// ��label�е��������
	label->setText("");
	// �����þ��δ�С
	// ���ui�������õ�label��С����С��Ⱥ͸߶�С�����ν�������Ϊ��С��Ⱥ���С�߶ȣ�
	// ���ui�������õ�label��С����С��Ⱥ͸߶ȴ󣬾��ν�������Ϊ����Ⱥ����߶ȣ�
	QString min_width = QString("min-width: %1px;").arg(size);              // ��С��ȣ�size
	QString min_height = QString("min-height: %1px;").arg(size);            // ��С�߶ȣ�size
	QString max_width = QString("max-width: %1px;").arg(size);              // ��С��ȣ�size
	QString max_height = QString("max-height: %1px;").arg(size);            // ��С�߶ȣ�size
	// �����ñ߽���״���߿�
	QString border_radius = QString("border-radius: %1px;").arg(size / 2);    // �߿���Բ�ǣ��뾶Ϊsize/2
	QString border = QString("border:1px solid black;");                    // �߿�Ϊ1px��ɫ
	// ������ñ�����ɫ
	QString background = "background-color:";
	switch (color) {
	case 0:
		// ��ɫ
		background += "rgb(190,190,190)";
		break;
	case 1:
		// ��ɫ
		background += "rgb(255,0,0)";
		break;
	case 2:
		// ��ɫ
		background += "rgb(0,255,0)";
		break;
	case 3:
		// ��ɫ
		background += "rgb(255,255,0)";
		break;
	default:
		break;
	}

	const QString SheetStyle = min_width + min_height + max_width + max_height + border_radius + border + background;
	label->setStyleSheet(SheetStyle);
}
//(f)���ô��ڱ���ͼƬ�����С�ƽ�̡����š�����
void PY_GUI_v1::paintEvent(QPaintEvent * ev) {
	QString _currentPath = QDir::currentPath();
	QPainter painter(this);
	painter.drawPixmap(rect(), QPixmap(_currentPath+"/css/bg.png"), QRect());
}
//3.����Ч��
void PY_GUI_v1::on_pushButton_pressed() {
	ui->CreateBTN->setGeometry(ui->CreateBTN->x() + 5, ui->CreateBTN->y() + 5, ui->CreateBTN->width() - 10, ui->CreateBTN->height() - 10);
}
void PY_GUI_v1::on_pushButton_released() {
	ui->CreateBTN->setGeometry(ui->CreateBTN->x() - 5, ui->CreateBTN->y() - 5, ui->CreateBTN->width() + 10, ui->CreateBTN->height() + 10);
}
#pragma endregion

//ʱ����
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
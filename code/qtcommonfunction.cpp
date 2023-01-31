#include "QtCommonFunction.h"

//1)���exe���������·��
QString F_getCurRunPath(){
	return QCoreApplication::applicationDirPath();
}

//2)���exe�����������һ��·��
QString F_getCurUpPath(){
	QDir cur_path(QCoreApplication::applicationDirPath());
	cur_path.cdUp();
	return cur_path.absolutePath();
}

//3)�ļ����Ƿ����
bool F_isDirExist(QString dir_path) {
	QDir dir(dir_path);
	if (dir.exists())
	{
		return true;
	}
	return false;
}

//4)�ļ��Ƿ����
bool F_isFileExist(QString file_path) {
	QFileInfo fileInfo(file_path);
	if (fileInfo.isFile())
	{
		return true;
	}
	return false;
}

//5)�����ļ���
bool F_createDir(QString dir_path) {
	QDir dir(dir_path);
	if (dir.exists())//�ļ����Ѵ��ڣ����贴��
	{
		return true;
	}
	else {
		if (dir.mkpath(dir_path)) {//�����ļ���·����mkpath���������ļ���mkdir
			return true;
		}
		else {
			return false;
		}
	}
}

//6)�����ļ�
bool F_createFile(QString file_path) {
	//6-1)�ļ��Ƿ����
	if (F_isFileExist(file_path)) {
		return true;
	}
	//6-2)�ļ����Ƿ����
	QString dir_path = file_path.section("//", 0, -2);
	if (!F_isDirExist(dir_path)) {
		if (!F_createDir(dir_path)) {// �½��ļ���
			return false;//����ʧ��
		}
	}
	//6-3)�����ļ�
	QFile file(file_path);
	//�Զ�д�ķ�ʽ���ļ����ɹ�����true��ʧ�ܷ���false
	bool ok = file.open(QIODevice::ReadWrite);
	if (ok) {
		file.close();
		return true;
	}
	else {
		return false;
	}
	return false;
}

//7)��ȡ�����ļ�����
bool F_loadQstring_INI(QString INI_Path, QString Key_Name, QString Elem_Name, QString& qstr_value) {
	if (F_isFileExist(INI_Path) == false) {//�����ļ�������
		return false;
	}
	QSettings *configIniRead = nullptr;
	configIniRead = new QSettings(INI_Path, QSettings::IniFormat);
	QString search_qstr, temp_qstr;
	//ProcessName
	search_qstr = "/" + Key_Name + "/" + Elem_Name;
	temp_qstr = configIniRead->value(search_qstr).toString();
	delete configIniRead;//ɾ��ָ��
	if (temp_qstr.length() == 0) {
		return false;
	}
	else {
		qstr_value = temp_qstr;
		return true;
	}
}
bool F_loadInt_INI(QString INI_Path, QString Key_Name, QString Elem_Name, int& int_value) {
	if (F_isFileExist(INI_Path) == false) {//�����ļ�������
		return false;
	}
	QSettings *configIniRead = nullptr;
	configIniRead = new QSettings(INI_Path, QSettings::IniFormat);
	QString search_qstr, temp_qstr;
	//ProcessName
	search_qstr = "/" + Key_Name + "/" + Elem_Name;
	temp_qstr = configIniRead->value(search_qstr).toString();
	delete configIniRead;//ɾ��ָ��
	if (temp_qstr.length() == 0) {
		return false;
	}
	else {
		int_value = temp_qstr.toInt();
		return true;
	}
}
bool F_loadBool_INI(QString INI_Path, QString Key_Name, QString Elem_Name, bool& bool_value) {
	if (F_isFileExist(INI_Path) == false) {//�����ļ�������
		return false;
	}
	QSettings *configIniRead = nullptr;
	configIniRead = new QSettings(INI_Path, QSettings::IniFormat);
	QString search_qstr, temp_qstr;
	//ProcessName
	search_qstr = "/" + Key_Name + "/" + Elem_Name;
	temp_qstr = configIniRead->value(search_qstr).toString();
	delete configIniRead;//ɾ��ָ��
	if (temp_qstr.length() == 0) {
		return false;
	}
	else {
		if (temp_qstr == "True" || temp_qstr == "TRUE" || temp_qstr == "true") {
			bool_value = true;
		}
		else {
			bool_value = false;
		}
		return true;
	}
}
bool F_loadFloat_INI(QString INI_Path, QString Key_Name, QString Elem_Name, float& float_value) {
	if (F_isFileExist(INI_Path) == false) {//�����ļ�������
		return false;
	}
	QSettings *configIniRead = nullptr;
	configIniRead = new QSettings(INI_Path, QSettings::IniFormat);

	QString search_qstr, temp_qstr;
	//ProcessName
	search_qstr = "/" + Key_Name + "/" + Elem_Name;
	temp_qstr = configIniRead->value(search_qstr).toString();
	delete configIniRead;//ɾ��ָ��
	if (temp_qstr.length() == 0) {
		return false;
	}
	else {
		float_value = temp_qstr.toFloat();
		return true;
	}
}

//8)���������ļ�����
void F_saveQstring_INI(QString INI_Path, QString Key_Name, QString Elem_Name, QString param) {
	//д��
	QSettings *configIniWrite = new QSettings(INI_Path, QSettings::IniFormat);
	configIniWrite->setIniCodec(QTextCodec::codecForName("UTF-8"));
	//��ini�ļ���д������,setValue���������������Ǽ�ֵ��
	QString Head = "/" + Key_Name + "/" + Elem_Name;
	//��ini�ļ��ĵ�һ����д������,ip���µĵ�һ������
	configIniWrite->setValue(Head, param);
	//д����ɺ�ɾ��ָ��
	delete configIniWrite;
	return;
}
void F_saveInt_INI(QString INI_Path, QString Key_Name, QString Elem_Name, int param) {
	QString param_str = QString::number(param);
	//QSettings���캯���ĵ�һ��������ini�ļ���·��,�ڶ���������ʾ���ini�ļ�,��������������ȱʡ
	QSettings *configIniWrite = new QSettings(INI_Path, QSettings::IniFormat);
	configIniWrite->setIniCodec(QTextCodec::codecForName("UTF-8"));

	//��ini�ļ���д������,setValue���������������Ǽ�ֵ��
	QString Head = "/" + Key_Name + "/" + Elem_Name;
	//��ini�ļ��ĵ�һ����д������,ip���µĵ�һ������
	configIniWrite->setValue(Head, param_str);
	//д����ɺ�ɾ��ָ��
	delete configIniWrite;
}
void F_saveBool_INI(QString INI_Path, QString Key_Name, QString Elem_Name, bool param){
	QString param_str;
	if (param) {
		param_str = "TRUE";
	}
	else {
		param_str = "FALSE";
	}
	//QSettings���캯���ĵ�һ��������ini�ļ���·��,�ڶ���������ʾ���ini�ļ�,��������������ȱʡ
	QSettings *configIniWrite = new QSettings(INI_Path, QSettings::IniFormat);
	configIniWrite->setIniCodec(QTextCodec::codecForName("UTF-8"));

	//��ini�ļ���д������,setValue���������������Ǽ�ֵ��
	QString Head = "/" + Key_Name + "/" + Elem_Name;
	//��ini�ļ��ĵ�һ����д������,ip���µĵ�һ������
	configIniWrite->setValue(Head, param_str);
	//д����ɺ�ɾ��ָ��
	delete configIniWrite;
}
void F_saveFloat_INI(QString INI_Path, QString Key_Name, QString Elem_Name, float param, int dot_number){
	QString param_str = QString::number(param, 'f', dot_number);
	//QSettings���캯���ĵ�һ��������ini�ļ���·��,�ڶ���������ʾ���ini�ļ�,��������������ȱʡ
	QSettings *configIniWrite = new QSettings(INI_Path, QSettings::IniFormat);
	configIniWrite->setIniCodec(QTextCodec::codecForName("UTF-8"));

	//��ini�ļ���д������,setValue���������������Ǽ�ֵ��
	QString Head = "/" + Key_Name + "/" + Elem_Name;
	//��ini�ļ��ĵ�һ����д������,ip���µĵ�һ������
	configIniWrite->setValue(Head, param_str);
	//д����ɺ�ɾ��ָ��
	delete configIniWrite;
	return;
}

//9)QString��string��ת(��������������)
std::string F_qstr2str(const QString qstr){
	QByteArray cdata = qstr.toLocal8Bit();
	return std::string(cdata);
}
QString F_str2qstr(const std::string str) {
	return QString::fromLocal8Bit(str.data());
}

//10)��ȡ�ļ��������е�ͼ��
std::vector<std::string> F_loadFileImages(QString file_path) {
	std::vector<std::string> all_image_paths;
	QDir dir_image(file_path);
	foreach(QFileInfo imageFileInfo, dir_image.entryInfoList(QStringList()
		<< "*.jpg" << "*.png" << "*.bmp", QDir::Files, QDir::NoSort))
	{
		QString  imgPath = imageFileInfo.absoluteFilePath();//�ļ�����·��
		all_image_paths.push_back(F_qstr2str(imgPath));
	}
	return all_image_paths;
}

//11������ʱ���ms
int F_differQDataTime(QDateTime  t1, QDateTime  t2){
	qint64 nmsec = t1.msecsTo(t2);
	return abs(nmsec);
}


/*������*/
//a)٤����ǿ
void F_gammaTransform(const cv::Mat srcImage, cv::Mat& dstImage, double gamma)
{
	unsigned char lut[256];
	for (int i = 0; i < 256; i++)
	{
		lut[i] = cv::saturate_cast<uchar>(pow((float)i / 255.0, gamma) * 255.0f);
	}
	dstImage = srcImage.clone();
	int channels = srcImage.channels();
	switch (channels)
	{
	case 1:
	{
		cv::MatIterator_<uchar> it = dstImage.begin<uchar>();
		cv::MatIterator_<uchar> end = dstImage.end<uchar>();
		while (it != end)
		{
			*it = lut[(*it)];
			it++;
		}
		break;
	}
	case 3:
	{
		cv::MatIterator_<cv::Vec3b> it = dstImage.begin<cv::Vec3b>();
		cv::MatIterator_<cv::Vec3b> end = dstImage.end<cv::Vec3b>();
		while (it != end)
		{
			(*it)[0] = lut[(*it)[0]];
			(*it)[1] = lut[(*it)[1]];
			(*it)[2] = lut[(*it)[2]];
			it++;
		}
		break;
	}
	default:
		break;
	}
}

//b)��������ֵ�õ���ӦRGBͨ��ֵ
void F_assignRGB(int pixel_value, int &red, int &green, int &blue, float nearDist, float farDist)
{
	if (pixel_value > nearDist && pixel_value < farDist)
	{
		float x = 1.0 * (pixel_value - nearDist) / (farDist - nearDist); //normalization
		red = 255 * (x < 0.5 ? 1 : -(2.0 * x) + 2);
		green = 255 * (x < 0.5 ? (2.0 * x) : 1);
		blue = 0;
	}
	else if (pixel_value <= nearDist)
	{
		red = 0xFF;
		green = 0xFF;
		blue = 0xFF;
	}
	else if (pixel_value >= farDist)
	{
		red = 0;
		green = 0;
		blue = 0;
	}
}

//c)�Ҷ�ͼ��תα��ͼ��
cv::Mat F_grayToRGB(cv::Mat frame_gray, float nearDist, float farDist) {
	cv::Mat rgb_image = cv::Mat::ones(frame_gray.size(), CV_8UC3);
	for (int i = 0; i < frame_gray.rows; i++) {
		for (int j = 0; j < frame_gray.cols; j++) {
			int depth = frame_gray.at<uchar>(i, j) / 255.0 * (farDist - nearDist) + nearDist;
			//α����ͨ��
			int red = 0, green = 0, blue = 0;
			if (depth < farDist && depth > nearDist) {
				F_assignRGB(depth, red, green, blue, nearDist, farDist);
			}
			rgb_image.at<uchar>(i, j * 3 + 0) = blue;
			rgb_image.at<uchar>(i, j * 3 + 1) = green;
			rgb_image.at<uchar>(i, j * 3 + 2) = red;
		}
	}
	return rgb_image;
}

//d)����������
void F_drawFace(cv::Mat& temp_frame, std::vector<cv::Point> temp_points) {
	if (temp_points.size() == 468) {
		for (int i = 0; i < temp_points.size(); i++) {
			cv::circle(temp_frame, temp_points[i], 1, cv::Scalar(0, 255, 0), -1);
		}
	}
}
void F_drawPose(cv::Mat& temp_frame, std::vector<cv::Point> temp_points) {
	std::vector<cv::Point2i> line_id;
	line_id.push_back(cv::Point2i(11, 12));
	line_id.push_back(cv::Point2i(11, 13));
	line_id.push_back(cv::Point2i(13, 15));
	line_id.push_back(cv::Point2i(12, 14));
	line_id.push_back(cv::Point2i(14, 16));
	line_id.push_back(cv::Point2i(12, 24));
	line_id.push_back(cv::Point2i(11, 23));
	line_id.push_back(cv::Point2i(23, 24));
	line_id.push_back(cv::Point2i(24, 26));
	line_id.push_back(cv::Point2i(23, 25));
	line_id.push_back(cv::Point2i(26, 28));
	line_id.push_back(cv::Point2i(25, 27));
	if (temp_points.size() == 33) {
		for (int i = 0; i < line_id.size(); i++) {
			cv::Point2i p1 = temp_points[line_id[i].x];
			cv::Point2i p2 = temp_points[line_id[i].y];
			if (p1.x > 0 && p1.y > 0 && p2.x > 0 && p2.y > 0) {
				cv::line(temp_frame, p1, p2, cv::Scalar(0, 0, 255), 2);
			}
		}
		for (int i = 0; i < temp_points.size(); i++) {
			if (temp_points[i].x > 0 && temp_points[i].y > 0) {
				cv::circle(temp_frame, temp_points[i], 5, cv::Scalar(0, 255, 0), -1);
			}
		}
	}
}

void F_drawHand(cv::Mat& temp_frame, std::vector<cv::Point> temp_points, cv::Scalar temp_color) {
	if (temp_frame.channels() == 1) {
		cv::cvtColor(temp_frame, temp_frame, cv::COLOR_GRAY2BGR);
	}
	if (temp_points.size() == 21) {
		//����
		std::vector<cv::Point2i> line_points = {
			cv::Point2i(0, 1), cv::Point2i(1, 2), cv::Point2i(2, 3), cv::Point2i(3, 4),
			cv::Point2i(1, 5), cv::Point2i(5, 6), cv::Point2i(6, 7), cv::Point2i(7, 8),
			cv::Point2i(5, 9), cv::Point2i(9, 10), cv::Point2i(10, 11), cv::Point2i(11, 12),
			cv::Point2i(9, 13), cv::Point2i(13, 14), cv::Point2i(14, 15), cv::Point2i(15, 16),
			cv::Point2i(13, 17), cv::Point2i(17, 18), cv::Point2i(18, 19), cv::Point2i(19, 20),
			cv::Point2i(17, 0)
		};
		for (int i = 0; i < line_points.size(); i++) {
			cv::Point2i p1 = temp_points[line_points[i].x];
			cv::Point2i p2 = temp_points[line_points[i].y];
			if (p1.x > 0 && p1.y > 0 && p2.x > 0 && p2.y > 0)
				cv::line(temp_frame, p1, p2, cv::Scalar(0, 0, 255), 2, 8, 0);
		}
		//��ʾ�Ǽܵ�
		for (int i = 0; i < temp_points.size(); i++) {
			cv::circle(temp_frame, temp_points[i], 5, temp_color, cv::FILLED);
		}
	}

}

//e)ͼ����ʾ��qt������
void F_showMat(cv::Mat image, QLabel *show_image, bool autosize_flag) {
	//ͼƬ����ʾ��Ҫ���⻹������QImage��Mat��ʽ�Ĳ�ͬ������QImage��RGB���У���Mat ��BGR��ͬʱ���߸�ʽ����������ҳ��ͬ�������Ҫ����ת�����ܹ���ʾ��
	cv::Mat rgb_image;
	QImage q_image;
	if (image.channels() == 3) {//3ͨ��	
		cv::cvtColor(image, image, /*CV_BGR2RGB*/4);//��ɫ�ռ�ת��
		q_image = QImage((const uchar*)(image.data), image.cols, image.rows, image.cols * image.channels(), QImage::Format_RGB888);
	}else {//��ͨ��
	
		q_image = QImage((const uchar*)(image.data), image.cols, image.rows, image.cols*image.channels(), QImage::Format_Indexed8);
	}
	QImage imgScaled;
	if (autosize_flag) {
		imgScaled = q_image.scaled(show_image->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);//��������
	}else {
		imgScaled = q_image.scaled(show_image->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);//����ԭͼ�Ŀ�߱�
	}
	show_image->setPixmap(QPixmap::fromImage(imgScaled));
}
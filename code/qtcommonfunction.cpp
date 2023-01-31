#include "QtCommonFunction.h"

//1)获得exe程序的运行路径
QString F_getCurRunPath(){
	return QCoreApplication::applicationDirPath();
}

//2)获得exe程序的运行上一级路径
QString F_getCurUpPath(){
	QDir cur_path(QCoreApplication::applicationDirPath());
	cur_path.cdUp();
	return cur_path.absolutePath();
}

//3)文件夹是否存在
bool F_isDirExist(QString dir_path) {
	QDir dir(dir_path);
	if (dir.exists())
	{
		return true;
	}
	return false;
}

//4)文件是否存在
bool F_isFileExist(QString file_path) {
	QFileInfo fileInfo(file_path);
	if (fileInfo.isFile())
	{
		return true;
	}
	return false;
}

//5)创建文件夹
bool F_createDir(QString dir_path) {
	QDir dir(dir_path);
	if (dir.exists())//文件夹已存在，无需创建
	{
		return true;
	}
	else {
		if (dir.mkpath(dir_path)) {//创建文件夹路径：mkpath、创建单文件：mkdir
			return true;
		}
		else {
			return false;
		}
	}
}

//6)创建文件
bool F_createFile(QString file_path) {
	//6-1)文件是否存在
	if (F_isFileExist(file_path)) {
		return true;
	}
	//6-2)文件夹是否存在
	QString dir_path = file_path.section("//", 0, -2);
	if (!F_isDirExist(dir_path)) {
		if (!F_createDir(dir_path)) {// 新建文件夹
			return false;//创建失败
		}
	}
	//6-3)创建文件
	QFile file(file_path);
	//以读写的方式打开文件，成功返回true，失败返回false
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

//7)读取配置文件参数
bool F_loadQstring_INI(QString INI_Path, QString Key_Name, QString Elem_Name, QString& qstr_value) {
	if (F_isFileExist(INI_Path) == false) {//配置文件不存在
		return false;
	}
	QSettings *configIniRead = nullptr;
	configIniRead = new QSettings(INI_Path, QSettings::IniFormat);
	QString search_qstr, temp_qstr;
	//ProcessName
	search_qstr = "/" + Key_Name + "/" + Elem_Name;
	temp_qstr = configIniRead->value(search_qstr).toString();
	delete configIniRead;//删除指针
	if (temp_qstr.length() == 0) {
		return false;
	}
	else {
		qstr_value = temp_qstr;
		return true;
	}
}
bool F_loadInt_INI(QString INI_Path, QString Key_Name, QString Elem_Name, int& int_value) {
	if (F_isFileExist(INI_Path) == false) {//配置文件不存在
		return false;
	}
	QSettings *configIniRead = nullptr;
	configIniRead = new QSettings(INI_Path, QSettings::IniFormat);
	QString search_qstr, temp_qstr;
	//ProcessName
	search_qstr = "/" + Key_Name + "/" + Elem_Name;
	temp_qstr = configIniRead->value(search_qstr).toString();
	delete configIniRead;//删除指针
	if (temp_qstr.length() == 0) {
		return false;
	}
	else {
		int_value = temp_qstr.toInt();
		return true;
	}
}
bool F_loadBool_INI(QString INI_Path, QString Key_Name, QString Elem_Name, bool& bool_value) {
	if (F_isFileExist(INI_Path) == false) {//配置文件不存在
		return false;
	}
	QSettings *configIniRead = nullptr;
	configIniRead = new QSettings(INI_Path, QSettings::IniFormat);
	QString search_qstr, temp_qstr;
	//ProcessName
	search_qstr = "/" + Key_Name + "/" + Elem_Name;
	temp_qstr = configIniRead->value(search_qstr).toString();
	delete configIniRead;//删除指针
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
	if (F_isFileExist(INI_Path) == false) {//配置文件不存在
		return false;
	}
	QSettings *configIniRead = nullptr;
	configIniRead = new QSettings(INI_Path, QSettings::IniFormat);

	QString search_qstr, temp_qstr;
	//ProcessName
	search_qstr = "/" + Key_Name + "/" + Elem_Name;
	temp_qstr = configIniRead->value(search_qstr).toString();
	delete configIniRead;//删除指针
	if (temp_qstr.length() == 0) {
		return false;
	}
	else {
		float_value = temp_qstr.toFloat();
		return true;
	}
}

//8)保存配置文件参数
void F_saveQstring_INI(QString INI_Path, QString Key_Name, QString Elem_Name, QString param) {
	//写入
	QSettings *configIniWrite = new QSettings(INI_Path, QSettings::IniFormat);
	configIniWrite->setIniCodec(QTextCodec::codecForName("UTF-8"));
	//向ini文件中写入内容,setValue函数的两个参数是键值对
	QString Head = "/" + Key_Name + "/" + Elem_Name;
	//向ini文件的第一个节写入内容,ip节下的第一个参数
	configIniWrite->setValue(Head, param);
	//写入完成后删除指针
	delete configIniWrite;
	return;
}
void F_saveInt_INI(QString INI_Path, QString Key_Name, QString Elem_Name, int param) {
	QString param_str = QString::number(param);
	//QSettings构造函数的第一个参数是ini文件的路径,第二个参数表示针对ini文件,第三个参数可以缺省
	QSettings *configIniWrite = new QSettings(INI_Path, QSettings::IniFormat);
	configIniWrite->setIniCodec(QTextCodec::codecForName("UTF-8"));

	//向ini文件中写入内容,setValue函数的两个参数是键值对
	QString Head = "/" + Key_Name + "/" + Elem_Name;
	//向ini文件的第一个节写入内容,ip节下的第一个参数
	configIniWrite->setValue(Head, param_str);
	//写入完成后删除指针
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
	//QSettings构造函数的第一个参数是ini文件的路径,第二个参数表示针对ini文件,第三个参数可以缺省
	QSettings *configIniWrite = new QSettings(INI_Path, QSettings::IniFormat);
	configIniWrite->setIniCodec(QTextCodec::codecForName("UTF-8"));

	//向ini文件中写入内容,setValue函数的两个参数是键值对
	QString Head = "/" + Key_Name + "/" + Elem_Name;
	//向ini文件的第一个节写入内容,ip节下的第一个参数
	configIniWrite->setValue(Head, param_str);
	//写入完成后删除指针
	delete configIniWrite;
}
void F_saveFloat_INI(QString INI_Path, QString Key_Name, QString Elem_Name, float param, int dot_number){
	QString param_str = QString::number(param, 'f', dot_number);
	//QSettings构造函数的第一个参数是ini文件的路径,第二个参数表示针对ini文件,第三个参数可以缺省
	QSettings *configIniWrite = new QSettings(INI_Path, QSettings::IniFormat);
	configIniWrite->setIniCodec(QTextCodec::codecForName("UTF-8"));

	//向ini文件中写入内容,setValue函数的两个参数是键值对
	QString Head = "/" + Key_Name + "/" + Elem_Name;
	//向ini文件的第一个节写入内容,ip节下的第一个参数
	configIniWrite->setValue(Head, param_str);
	//写入完成后删除指针
	delete configIniWrite;
	return;
}

//9)QString与string互转(不存在中文乱码)
std::string F_qstr2str(const QString qstr){
	QByteArray cdata = qstr.toLocal8Bit();
	return std::string(cdata);
}
QString F_str2qstr(const std::string str) {
	return QString::fromLocal8Bit(str.data());
}

//10)读取文件夹下所有的图像
std::vector<std::string> F_loadFileImages(QString file_path) {
	std::vector<std::string> all_image_paths;
	QDir dir_image(file_path);
	foreach(QFileInfo imageFileInfo, dir_image.entryInfoList(QStringList()
		<< "*.jpg" << "*.png" << "*.bmp", QDir::Files, QDir::NoSort))
	{
		QString  imgPath = imageFileInfo.absoluteFilePath();//文件绝对路径
		all_image_paths.push_back(F_qstr2str(imgPath));
	}
	return all_image_paths;
}

//11）计算时间戳ms
int F_differQDataTime(QDateTime  t1, QDateTime  t2){
	qint64 nmsec = t1.msecsTo(t2);
	return abs(nmsec);
}


/*相机相关*/
//a)伽马增强
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

//b)根据像素值得到对应RGB通道值
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

//c)灰度图像转伪彩图像
cv::Mat F_grayToRGB(cv::Mat frame_gray, float nearDist, float farDist) {
	cv::Mat rgb_image = cv::Mat::ones(frame_gray.size(), CV_8UC3);
	for (int i = 0; i < frame_gray.rows; i++) {
		for (int j = 0; j < frame_gray.cols; j++) {
			int depth = frame_gray.at<uchar>(i, j) / 255.0 * (farDist - nearDist) + nearDist;
			//伪彩三通道
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

//d)画出特征点
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
		//连线
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
		//显示骨架点
		for (int i = 0; i < temp_points.size(); i++) {
			cv::circle(temp_frame, temp_points[i], 5, temp_color, cv::FILLED);
		}
	}

}

//e)图像显示在qt界面上
void F_showMat(cv::Mat image, QLabel *show_image, bool autosize_flag) {
	//图片的显示主要问题还是在于QImage与Mat格式的不同，其中QImage是RGB排列，而Mat 是BGR，同时两者格式包含的内容页不同，因此需要经过转换才能够显示。
	cv::Mat rgb_image;
	QImage q_image;
	if (image.channels() == 3) {//3通道	
		cv::cvtColor(image, image, /*CV_BGR2RGB*/4);//颜色空间转换
		q_image = QImage((const uchar*)(image.data), image.cols, image.rows, image.cols * image.channels(), QImage::Format_RGB888);
	}else {//单通道
	
		q_image = QImage((const uchar*)(image.data), image.cols, image.rows, image.cols*image.channels(), QImage::Format_Indexed8);
	}
	QImage imgScaled;
	if (autosize_flag) {
		imgScaled = q_image.scaled(show_image->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);//饱满缩放
	}else {
		imgScaled = q_image.scaled(show_image->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);//保持原图的宽高比
	}
	show_image->setPixmap(QPixmap::fromImage(imgScaled));
}
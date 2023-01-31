/*
QT���ú�������
Copyright(c) 2022  ����
Company: ��������˹�������޹�˾

��1���޸ģ�2022.6.10������ˣ�1-8���Ⱥ���
��2���޸ģ�2022.6.14�������ˣ�a-d���Ⱥ���
��3���޸ģ�2022.6.16��������(9-11)��(e)�Ⱥ���
*/

#ifndef QTCOMMONFUNCTION
#define QTCOMMONFUNCTION

#include "QtCommonHeader.h"
//OpenCVͷ�ļ�
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>  
#include <opencv2/imgproc/imgproc.hpp>
#include<opencv2/videoio.hpp>

//1)���exe���������·��
QString F_getCurRunPath();

//2)���exe�����������һ��·��
QString F_getCurUpPath();

//3)�ļ����Ƿ����
bool F_isDirExist(QString dir_path);

//4)�ļ��Ƿ����
bool F_isFileExist(QString file_path);

//5)�����ļ���
bool F_createDir(QString dir_path);

//6)�����ļ�:�ļ�·�����ļ���
bool F_createFile(QString file_path);

//7)��ȡ�����ļ�����
bool F_loadQstring_INI(QString INI_Path, QString Key_Name, QString Elem_Name, QString& param);
bool F_loadInt_INI(QString INI_Path, QString Key_Name, QString Elem_Name, int& param);
bool F_loadBool_INI(QString INI_Path, QString Key_Name, QString Elem_Name, bool& param);
bool F_loadFloat_INI(QString INI_Path, QString Key_Name, QString Elem_Name, float& param);

//8)���������ļ�����
void F_saveQstring_INI(QString INI_Path, QString Key_Name, QString Elem_Name, QString param);
void F_saveInt_INI(QString INI_Path, QString Key_Name, QString Elem_Name, int param);
void F_saveBool_INI(QString INI_Path, QString Key_Name, QString Elem_Name, bool param);
void F_saveFloat_INI(QString INI_Path, QString Key_Name, QString Elem_Name, float param, int dot_number=4);

//9)QString��string��ת(��������������)
std::string F_qstr2str(const QString qstr);
QString F_str2qstr(const std::string str);

//10)��ȡ�ļ��������е�ͼ��
std::vector<std::string> F_loadFileImages(QString file_path);

//11������ʱ���ms
int F_differQDataTime(QDateTime  t1, QDateTime  t2);


/*������*/
//a)gamma��ǿ
void F_gammaTransform(const cv::Mat srcImage, cv::Mat& dstImage, double gamma);

//b)�Ҷ�ͼ��תα��ͼ��
cv::Mat F_grayToRGB(cv::Mat frame_gray, float nearDist, float farDist);

//c)�Ҷ�תbgr
void F_assignRGB(int pixel_value, int &red, int &green, int &blue, float nearDist, float farDist);

//d)����������
void F_drawFace(cv::Mat& temp_frame, std::vector<cv::Point> temp_points);
void F_drawPose(cv::Mat& temp_frame, std::vector<cv::Point> temp_points);
void F_drawHand(cv::Mat& temp_frame, std::vector<cv::Point> temp_points, cv::Scalar temp_color = cv::Scalar(0, 0, 255));

//e)ͼ����ʾ��qt������,Ĭ������Ӧ���
void F_showMat(cv::Mat image, QLabel *show_image, bool autosize_flag = true);
#endif QTCOMMONFUNCTION
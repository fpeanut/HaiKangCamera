# HaiKangCamera
海康相机sdk调用解析(QT C++编写)<br>
相机型号：MV-CS200-10GC 4千万像素<br>
软件采图模式：结合光电开关的触发式采图。<br>
软件工作流程图：<br>
![Image text](https://github.com/fpeanut/HaiKangCamera/blob/main/img/1.jpg)<br>
软件界面：<br>
![Image text](https://github.com/fpeanut/HaiKangCamera/blob/main/img/2.jpg)<br>
![Image text](https://github.com/fpeanut/HaiKangCamera/blob/main/img/3.jpg)<br>
使用说明<br>
核心函数：<br>
  //(a)成功连接设备信息获取<br>
	bool PrintDeviceInfo(MV_CC_DEVICE_INFO* pstMVDevInfo);<br>
	//(b)获取相机设别id<br>
	unsigned int GetDeviceId(MV_CC_DEVICE_INFO_LIST &stDeviceList);<br>
	//(c)相机原始图像数据转Mat图像<br>
	bool Convert2Mat(MV_FRAME_OUT_INFO_EX* pstImageInfo, unsigned char * pData);<br>
  //(g)初始化相机<br>
	void initCam();<br>
  //5.采集图像<br>
	void GrapImg();<br>
  

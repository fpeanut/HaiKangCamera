#include "PY_GUI_v1.h"
//#include "newWidget.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	if (QT_VERSION >= QT_VERSION_CHECK(5, 6, 0))
		QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
	QApplication a(argc, argv);
	PY_GUI_v1 w;
	//newWidget w1;
	QIcon icon;
	icon.addFile(QStringLiteral("./css/title.ico"), QSize(), QIcon::Normal, QIcon::Off);
	w.setWindowIcon(icon);

	w.show();
	//w1.show();
	return a.exec();
}

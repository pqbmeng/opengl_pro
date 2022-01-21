#include "pro7.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	pro7 w;
	w.show();
	return a.exec();
}

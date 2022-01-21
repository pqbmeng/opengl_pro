#include "pro0.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	pro0 w;
	w.show();
	return a.exec();
}

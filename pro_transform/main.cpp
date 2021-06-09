#include "pro_transform.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    pro_transform w;
    w.show();
    return a.exec();
}

#include "pro_transform_perspective.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    pro_transform_perspective w;
    w.show();
    return a.exec();
}

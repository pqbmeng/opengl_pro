#include "pro_transform_perspective_depth.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    pro_transform_perspective_depth w;
    w.show();
    return a.exec();
}

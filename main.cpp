#include "carouseltest.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    carouselTest w;
    w.show();
    return a.exec();
}

#include "TestTreeWidget.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    TestTreeWidget w;
    w.show();
    return a.exec();
}

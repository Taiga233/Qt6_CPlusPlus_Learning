#include "TestTableWidget.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    TestTableWidget w;
    w.show();
    return a.exec();
}

#include "TestSpinBox.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    TestSpinBox w;
    w.show();
    return a.exec();
}

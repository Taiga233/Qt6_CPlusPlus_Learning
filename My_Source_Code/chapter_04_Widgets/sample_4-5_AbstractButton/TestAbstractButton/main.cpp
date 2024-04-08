#include "TestAbstractButton.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    TestAbstractButton w;
    w.show();
    return a.exec();
}

#include "TestQStyledItemDelegate.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    TestQStyledItemDelegate w;
    w.show();
    return a.exec();
}

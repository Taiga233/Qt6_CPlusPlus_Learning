#include "TestComboBox.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    TestComboBox w;
    w.show();
    return a.exec();
}

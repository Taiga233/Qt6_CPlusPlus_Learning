#include "TestTimerElapsedTimer.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    TestTimerElapsedTimer w;
    w.show();
    return a.exec();
}

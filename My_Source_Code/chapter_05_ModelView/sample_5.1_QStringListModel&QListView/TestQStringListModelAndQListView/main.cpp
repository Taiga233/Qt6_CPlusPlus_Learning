#include "ListModelView.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ListModelView w;
    w.show();
    return a.exec();
}

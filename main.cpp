#include "leerenametool.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    leeRenameTool w;
    w.show();
    return a.exec();
}

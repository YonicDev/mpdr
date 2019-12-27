#include "randomizergui.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    RandomizerGUI w;
    w.show();
    return a.exec();
}

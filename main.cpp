#include "settings.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    Settings *w = new Settings();
    w->show();

    return app.exec();
}

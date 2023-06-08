#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QApplication>
#include "appcore.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    AppCore appCore;

    QQmlApplicationEngine engine;
    QQmlContext *context = engine.rootContext();
    context->setContextProperty("appCore", &appCore);
    engine.load(QUrl("qrc:/qml/qml_ui/mainWindow.qml"));

    appCore.loadFromDatabase();

    return a.exec();
}

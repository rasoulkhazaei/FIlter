#include "signalTest.h"
#include <QApplication>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QQmlApplicationEngine engine;
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    SignalTest *sig = new SignalTest();
    engine.rootContext()->setContextProperty("figureCpp", sig);
    engine.loadFromModule("untitled", "Main");

    return app.exec();
}

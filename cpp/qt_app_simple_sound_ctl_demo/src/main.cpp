#include <QApplication>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QSystemTrayIcon>

#include "media_ctl.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QQmlApplicationEngine engine;

    QMediaCtl mctl;
    auto status = mctl.init();

    if (status == QMediaCtl::Status::INIT_FAILED)
    {
        qFatal("Error: Failed to initialize MediaCtl!");
        return -1;
    }

    if (!QSystemTrayIcon::isSystemTrayAvailable())
    {
        qWarning("System tray is not available on this system.");
        return -1;
    }
    qmlRegisterSingletonInstance("MediaModule", 1, 0, "MediaCtl", &mctl);

    const QUrl url("qrc:/app/qml/main.qml");
    engine.load(url);

    return app.exec();
}

#include "ErrorCollector.h"
#include "SpixOSRTest.h"
#include <OpenSR/Engine.h>
#include <QGuiApplication>
#include <Spix/QtQmlBot.h>

namespace
{
static const QString SETTINGS_ORGANIZATION = "OpenSR";
static const QString SETTINGS_APPLICATION = "OpenSR";
static const QString STARTUP_SCRIPT = "res:/opensr.js";
static const QString MAIN_QML = "res:/OpenSR/GameWindow.qml";
} // namespace

int main(int argc, char *argv[])
{
    OpenSR::Engine engine(argc, argv);

    QApplication::setOrganizationName(SETTINGS_ORGANIZATION);
    QApplication::setApplicationName(SETTINGS_APPLICATION);
    engine.setStartupScript(STARTUP_SCRIPT);
    engine.setMainQML(MAIN_QML);

    GUITest::SpixOSRTest::instance().prepare(argc, argv);
    auto bot = new spix::QtQmlBot();
    bot->runTestServer(GUITest::SpixOSRTest::instance());

    engine.run();

    return GUITest::SpixOSRTest::instance().testResult();
}

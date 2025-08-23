#include <OpenSR/Engine.h>

#include <QGuiApplication>
#include <Spix/Data/Variant.h>
#include <Spix/Events/Identifiers.h>
#include <Spix/QtQmlBot.h>
#include <gtest/gtest.h>

namespace
{
static const QString SETTINGS_ORGANIZATION = "OpenSR";
static const QString SETTINGS_APPLICATION = "OpenSR";
static const QString STARTUP_SCRIPT = "res:/opensr.js";
static const QString MAIN_QML = "res:/OpenSR/GameWindow.qml";
} // namespace

class SpixGTest;
static SpixGTest *srv;

class SpixGTest : public spix::TestServer
{
public:
    SpixGTest(int argc, char *argv[]) : m_argc(argc), m_argv(argv)
    {
    }

    int testResult()
    {
        return m_result.load();
    }

protected:
    int m_argc;
    char **m_argv;
    std::atomic<int> m_result{0};

    void executeTest() override
    {
        srv = this;
        ::testing::InitGoogleTest(&m_argc, m_argv);
        auto testResult = RUN_ALL_TESTS();
        m_result.store(testResult);
    }
};

TEST(UITest, ButtonTest)
{
    srv->wait(std::chrono::milliseconds(1000));
    srv->mouseClick(spix::ItemPath("gameScreen/menu/newButton"));
    srv->wait(std::chrono::milliseconds(1000));
    srv->invokeMethod("gameScreen/view", "exitToMenu", std::vector<spix::Variant>{});
    srv->wait(std::chrono::milliseconds(1000));
    srv->mouseClick(spix::ItemPath("gameScreen/menu/exitButton"));
    srv->quit();

    EXPECT_EQ(srv->getErrors(), std::vector<std::string>{});
}

int main(int argc, char *argv[])
{
    OpenSR::Engine engine(argc, argv);

    QApplication::setOrganizationName(SETTINGS_ORGANIZATION);
    QApplication::setApplicationName(SETTINGS_APPLICATION);
    engine.setStartupScript(STARTUP_SCRIPT);
    engine.setMainQML(MAIN_QML);

    SpixGTest tests(argc, argv);
    auto bot = new spix::QtQmlBot();
    bot->runTestServer(tests);

    engine.run();

    return tests.testResult();
}

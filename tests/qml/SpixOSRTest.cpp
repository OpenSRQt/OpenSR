#include "SpixOSRTest.h"
#include "ErrorCollector.h"
#include <Spix/Data/Variant.h>
#include <Spix/Events/Identifiers.h>
#include <Spix/QtQmlBot.h>
#include <atomic>
#include <gtest/gtest.h>

namespace GUITest
{
SpixOSRTest::SpixOSRTest()
{
    ErrorCollector::instance().install();
}

SpixOSRTest::~SpixOSRTest()
{
    ErrorCollector::instance().uninstall();
}

void SpixOSRTest::prepare(int argc, char *argv[])
{
    clearErrors();
    m_argc = argc;
    m_argv = argv;
}

SpixOSRTest &SpixOSRTest::instance()
{
    static SpixOSRTest inst;
    return inst;
}

std::vector<std::string> SpixOSRTest::getAppErrors() const
{
    const auto &qtErrors = ErrorCollector::instance().errors();
    std::vector<std::string> result;
    result.reserve(qtErrors.size());

    for (const auto &error : qtErrors)
    {
        result.push_back(error.toStdString());
    }

    return result;
}

void SpixOSRTest::clearErrors()
{
    ErrorCollector::instance().clear();
}

int SpixOSRTest::testResult()
{
    return m_result.load();
}

void SpixOSRTest::executeTest()
{

    ::testing::InitGoogleTest(&m_argc, m_argv);
    auto testResult = RUN_ALL_TESTS();
    std::cout << "return code in executeTest(): " << testResult << std::endl;
    m_result.store(testResult);
}

TEST(UITest, ButtonTest)
{
    SpixOSRTest::instance().wait(std::chrono::milliseconds(1000));
    SpixOSRTest::instance().mouseClick(spix::ItemPath("gameScreen/menu/newButton"));
    SpixOSRTest::instance().wait(std::chrono::milliseconds(1000));
    SpixOSRTest::instance().invokeMethod("gameScreen/view", "exitToMenu", std::vector<spix::Variant>{});
    SpixOSRTest::instance().wait(std::chrono::milliseconds(1000));
    SpixOSRTest::instance().mouseClick(spix::ItemPath("gameScreen/menu/exitButton"));

    EXPECT_EQ(SpixOSRTest::instance().getAppErrors(), std::vector<std::string>{});
    EXPECT_EQ(SpixOSRTest::instance().getErrors(), std::vector<std::string>{});

    SpixOSRTest::instance().quit();
}
} // namespace GUITest
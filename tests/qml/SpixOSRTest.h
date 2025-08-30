#pragma once

#include <Spix/Data/Variant.h>
#include <Spix/Events/Identifiers.h>
#include <Spix/QtQmlBot.h>
#include <gtest/gtest.h>

namespace GUITest
{
class SpixOSRTest : public spix::TestServer
{
public:
    ~SpixOSRTest() override;

    static SpixOSRTest &instance();
    std::vector<std::string> getAppErrors() const;
    void clearErrors();
    int testResult();
    void prepare(int argc, char *argv[]);

protected:
    int m_argc{};
    char **m_argv{};
    std::atomic<int> m_result{0};

    void executeTest() override;

private:
    SpixOSRTest();
};
} // namespace GUITest
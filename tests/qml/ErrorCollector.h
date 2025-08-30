#pragma once

#include <QString>
#include <QVector>
#include <QtMessageHandler>

namespace GUITest
{
class ErrorCollector
{
public:
    static ErrorCollector &instance();
    void install();
    void uninstall();
    const QVector<QString> &errors() const;
    void clear();
    static void messageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg);

private:
    ErrorCollector() = default;
    QVector<QString> m_errors;
    QtMessageHandler m_oldHandler = nullptr;
};
} // namespace GUITest
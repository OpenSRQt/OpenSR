#include "ErrorCollector.h"

namespace GUITest
{
ErrorCollector &ErrorCollector::instance()
{
    static ErrorCollector collector;
    return collector;
}

void ErrorCollector::install()
{
    m_errors.clear();
    m_oldHandler = qInstallMessageHandler(ErrorCollector::messageHandler);
}

void ErrorCollector::uninstall()
{
    qInstallMessageHandler(m_oldHandler);
}

const QVector<QString> &ErrorCollector::errors() const
{
    return m_errors;
}
void ErrorCollector::clear()
{
    m_errors.clear();
}

void ErrorCollector::messageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    if (type == QtWarningMsg || type == QtCriticalMsg || type == QtFatalMsg)
    {
        QString formattedMsg =
            QString("[%1] %2:%3 - %4\n").arg(context.category).arg(context.file).arg(context.line).arg(msg);

        instance().m_errors.append(formattedMsg);
    }

    if (instance().m_oldHandler)
    {
        instance().m_oldHandler(type, context, msg);
    }
}
} // namespace GUITest
#include "OpenSR/CursorManager.h"
#include "OpenSR/AnimatedCursor.h"
#include <QApplication>

namespace OpenSR
{

CursorManager::CursorManager(QQuickItem* window) : m_animatedCursor(new AnimatedCursor(window)), current(CursorType::Main)
{
    cursors[Enter] = loadCursor("data/DATA/Cursor/Enter.GAI");
    cursors[FireFull] = loadCursor("data/DATA/Cursor/FireFull.GAI");
    cursors[FireSmall] = loadCursor("data/DATA/Cursor/FireSmall.GAI");
    cursors[Main] = loadCursor("data/DATA/Cursor/Main.GAI");
    cursors[ScanFull] = loadCursor("data/DATA/Cursor/ScanFull.GAI");
    cursors[ScanSmall] = loadCursor("data/DATA/Cursor/ScanSmall.GAI");
    cursors[Scroll] = loadCursor("data/DATA/Cursor/Scroll.GAI");
    cursors[Take] = loadCursor("data/DATA/Cursor/Take.GAI");
    cursors[TalkFull] = loadCursor("data/DATA/Cursor/TalkFull.GAI");
    cursors[TalkSmall] = loadCursor("data/DATA/Cursor/TalkSmall.GAI");

    QApplication::setOverrideCursor(Qt::BlankCursor);
    changeCursor(Main);
}

void CursorManager::setWindow(QQuickItem* window)
{
    if(m_animatedCursor) {
        m_animatedCursor->setParentItem(window);
    } else {
        m_animatedCursor = new AnimatedCursor(window);
    }
}

QMovie *&CursorManager::getCursor(CursorManager::CursorType type)
{
    auto it = cursors.find(type);
    if (it != cursors.end() && it.value())
    {
        return it.value();
    }
    qWarning() << "CursorManager: Cursor not found for type:" << type;
    return cursors.find(Main).value();
}

void CursorManager::hideCursor()
{
    m_animatedCursor->setVisible(false);
}

void CursorManager::showCursor()
{
    m_animatedCursor->setVisible(true);
}

void CursorManager::changeCursor(CursorManager::CursorType type)
{
    QMovie *previous = m_animatedCursor->setCursor(getCursor(type));
    if (!previous)
    {
        return;
    }
    cursors[current] = previous;
    current = type;
}

QMovie *CursorManager::loadCursor(const QString &source)
{
    QMovie *cursorMovie = new QMovie(source);
    if (!cursorMovie->isValid())
    {
        qWarning() << "Failed to load cursor from" << source;
        return nullptr;
    }
    return cursorMovie;
}

} // namespace OpenSR
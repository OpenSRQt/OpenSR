#include "CursorManager.h"
#include "OpenSR/AnimatedCursor.h"
#include <QApplication>

namespace OpenSR
{
namespace World
{

CursorManager::CursorManager() : m_animatedCursor(new AnimatedCursor()), current(CursorType::Main)
{
    cursors[Enter] = loadCursor("data/cursors/Enter.GAI");
    cursors[FireFull] = loadCursor("data/cursors/FireFull.GAI");
    cursors[FireSmall] = loadCursor("data/cursors/FireSmall.GAI");
    cursors[Main] = loadCursor("data/cursors/Main.GAI");
    cursors[ScanFull] = loadCursor("data/cursors/ScanFull.GAI");
    cursors[ScanSmall] = loadCursor("data/cursors/ScanSmall.GAI");
    cursors[Scroll] = loadCursor("data/cursors/Scroll.GAI");
    cursors[Take] = loadCursor("data/cursors/Take.GAI");
    cursors[TalkFull] = loadCursor("data/cursors/TalkFull.GAI");
    cursors[TalkSmall] = loadCursor("data/cursors/TalkSmall.GAI");

    QApplication::setOverrideCursor(Qt::BlankCursor);
    changeCursor(Main);
}

QMovie *&CursorManager::getCursor(CursorManager::CursorType type)
{
    auto it = cursors.find(type);
    if (it != cursors.end() && it.value())
    {
        return it.value();
    }
    qWarning() << "Cursor not found for type:" << type;
    return cursors.find(Main).value();
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

} // namespace World

} // namespace OpenSR
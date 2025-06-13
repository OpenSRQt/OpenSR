#include "CursorManager.h"
#include "Engine.h"

namespace OpenSR {

namespace World {

CursorManager::CursorManager()
{
  cursors[Enter] = loadCursor("Bm.Cursor.Enter", 0, 0);
  cursors[FireFull] = loadCursor("dat:/Bm.Cursor.FireFull", 0, 0);
  cursors[FireSmall] = loadCursor("dat:/Bm.Cursor.FireSmall", 0, 0);
  cursors[Main] = loadCursor("data/DATA/Cursor/Main.GAI", 0, 0);
  cursors[ScanFull] = loadCursor("dat:/Bm.Cursor.ScanFull", 0, 0);
  cursors[ScanSmall] = loadCursor("dat:/Bm.Cursor.ScanSmall", 0, 0);
  cursors[Scroll] = loadCursor("dat:/Bm.Cursor.Scroll", 0, 0);
  cursors[Take] = loadCursor("dat:/Bm.Cursor.Take", 0, 0);
  cursors[TalkFull] = loadCursor("dat:/Bm.Cursor.TalkFull", 0, 0);
  cursors[TalkSmall] = loadCursor("dat:/Bm.Cursor.TalkSmall", 0, 0);
}

const QCursor& CursorManager::getCursor(CursorManager::CursorType type) const
{
  auto it = cursors.find(type);
  if (it != cursors.end()) {
      return it.value();
  }
  static QCursor fallback(Qt::ArrowCursor);
  qWarning() << "Cursor not found for type:" << type;
  return fallback;
}

QCursor CursorManager::loadCursor(const QString &path, int hotspotX, int hotspotY) {
  QPixmap pixmap(path);
  if (pixmap.isNull()) {
    qWarning() << "Failed to load cursor from" << path;
    return QCursor(Qt::ArrowCursor);
  }
  return QCursor(pixmap, hotspotX, hotspotY);
}

} // namespace World

} // namespace OpenSR
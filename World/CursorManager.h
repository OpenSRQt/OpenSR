#ifndef OPENSR_WORLD_CURSOR_H
#define OPENSR_WORLD_CURSOR_H

#include <QCursor>
#include <QPixmap>
#include <QDebug>

namespace OpenSR {

namespace World {

class CursorManager {
  Q_GADGET
public:

  CursorManager();

  enum CursorType {
    Enter,
    FireFull,
    FireSmall,
    Main,
    ScanFull,
    ScanSmall,
    Scroll,
    Take,
    TalkFull,
    TalkSmall
  };
  Q_ENUM(CursorType)

  Q_INVOKABLE const QCursor& getCursor(CursorType type) const;
  // Q_INVOKABLE void setCursors(CursorType type);

private:
  QMap<CursorType, QCursor> cursors;
  QCursor loadCursor(const QString &path, int hotspotX, int hotspotY);


};

} // namespace World

} // namespace OpenSR

#endif // OPENSR_WORLD_CURSOR_H
#ifndef OPENSR_WORLD_CURSOR_H
#define OPENSR_WORLD_CURSOR_H

#include "OpenSR/AnimatedCursor.h"

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

  Q_INVOKABLE QMovie * &getCursor(CursorManager::CursorType type);
  void changeCursor(CursorManager::CursorType type);

private:
  QMap<CursorType, QMovie*> cursors;
  QMovie* loadCursor(const QString &path);

  AnimatedCursor *m_animatedCursor{};
  CursorType current;

};

} // namespace World

} // namespace OpenSR

#endif // OPENSR_WORLD_CURSOR_H
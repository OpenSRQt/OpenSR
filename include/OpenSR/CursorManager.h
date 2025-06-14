#ifndef OPENSR_WORLD_CURSOR_H
#define OPENSR_WORLD_CURSOR_H

#include "OpenSR/AnimatedCursor.h"

namespace OpenSR {

class CursorManager : public QObject {
  Q_OBJECT
public:

  CursorManager(QQuickItem* window = nullptr);

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
  Q_INVOKABLE void changeCursor(CursorManager::CursorType type);
  Q_INVOKABLE void showCursor();
  Q_INVOKABLE void hideCursor();
  Q_INVOKABLE void setWindow(QQuickItem* window);

private:
  QMap<CursorType, QMovie*> cursors;
  QMovie* loadCursor(const QString &path);

  AnimatedCursor *m_animatedCursor{};
  CursorType current;

};

} // namespace OpenSR

#endif // OPENSR_WORLD_CURSOR_H
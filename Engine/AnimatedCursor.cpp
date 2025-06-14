#include "OpenSR/AnimatedCursor.h"

AnimatedCursor::AnimatedCursor(QQuickItem *parent)
    : QQuickPaintedItem(parent), m_moveTimer(new QTimer(this))
{
    setFlag(ItemHasContents, true);
    connect(m_moveTimer, &QTimer::timeout, this, [this]() {
        if (!window()) return;
        QPoint global = QCursor::pos();
        QScreen *screen = QGuiApplication::screenAt(global);
        if (!screen || !screen->geometry().contains(global))
            return;
        QPointF local = window()->mapFromGlobal(global);
        setPosition(local);
    });
    m_moveTimer->start(10);
}

AnimatedCursor::~AnimatedCursor()
{
    if (cursorMovie) cursorMovie->stop();
}

void AnimatedCursor::componentComplete()
{
    QQuickPaintedItem::componentComplete();
    if (!window())
        qWarning() << "AnimatedCursor: window() is nullptr.";
}

QString AnimatedCursor::cursorSource() const
{
    return m_cursorSource;
}

void AnimatedCursor::setCursorMovie(const QString &source)
{
    if (!cursorMovie->isValid()) {
        qWarning() << "Invalid cursor movie: " << source;
        delete cursorMovie;
    } else {
        connect(cursorMovie, &QMovie::frameChanged, this, [this](int){
            update();
        });
        cursorMovie->start();
        setWidth(cursorMovie->frameRect().width());
        setHeight(cursorMovie->frameRect().height());
        emit cursorChanged();
    }
}

void AnimatedCursor::setCursor(const QString &source)
{
    if (cursorMovie) {
        cursorMovie->stop();
        delete cursorMovie;
    }

    m_cursorSource = source;
    cursorMovie = new QMovie(source);
    setCursorMovie(source);
}

QMovie* AnimatedCursor::setCursor(QMovie* source)
{
    if(!source) return cursorMovie;
    if (cursorMovie) {
        cursorMovie->stop();
    }

    QMovie* old = cursorMovie;
    cursorMovie = source;
    setCursorMovie();
    return old;
}

void AnimatedCursor::paint(QPainter *painter)
{
    if (cursorMovie && cursorMovie->isValid()) {
        painter->drawImage(0, 0, cursorMovie->currentImage());
    }
}

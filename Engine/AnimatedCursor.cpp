#include "OpenSR/AnimatedCursor.h"
#include <QApplication>
#include <QScreen>

AnimatedCursor::AnimatedCursor(QWidget *parent)
    : QWidget(nullptr), m_moveTimer(new QTimer(this)), cursorLabel(new QLabel(this)), cursorMovie(nullptr)
{
    setAttribute(Qt::WA_TransparentForMouseEvents, true);
    setAttribute(Qt::WA_NoSystemBackground, true);
    setAttribute(Qt::WA_TranslucentBackground, true);
    setAttribute(Qt::WA_ShowWithoutActivating, true);
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::ToolTip | Qt::X11BypassWindowManagerHint);
    setMouseTracking(true);

    cursorLabel->setAlignment(Qt::AlignCenter);
    cursorLabel->setStyleSheet("background: transparent; border: none;");

    connect(m_moveTimer, &QTimer::timeout, this, &AnimatedCursor::moveToCursor);
    m_moveTimer->start(10);
}

QMovie *AnimatedCursor::setCursor(QMovie *source)
{
    if (source && cursorMovie && cursorMovie == source)
    {
        return nullptr;
    }
    QMovie *old = cursorMovie;
    cursorMovie = source;
    if (cursorMovie->isValid())
    {
        cursorLabel->setMovie(cursorMovie);
        cursorMovie->start();
        resize(cursorMovie->frameRect().size());
        cursorLabel->resize(size());
    }
    else
    {
        qWarning() << "Failed to load cursor animation:" << source;
        cursorLabel->setText("+");
        cursorLabel->setStyleSheet("color: red; font: bold 24px; background: transparent;");
        resize(32, 32);
        cursorLabel->resize(32, 32);
    }

    moveToCursor();
    show();
    raise();
    activateWindow();
    return old;
}

void AnimatedCursor::setCursor(const QString &source)
{
    if (cursorMovie)
    {
        cursorMovie->stop();
        delete cursorMovie;
        cursorMovie = nullptr;
    }

    cursorMovie = new QMovie(source);

    if (cursorMovie && cursorMovie->isValid())
    {
        cursorLabel->setMovie(cursorMovie);
        cursorMovie->start();
        resize(cursorMovie->frameRect().size());
        cursorLabel->resize(size());
    }
    else
    {
        qWarning() << "Failed to load cursor animation:" << source;
        cursorLabel->setText("+");
        cursorLabel->setStyleSheet("color: red; font: bold 24px; background: transparent;");
        resize(32, 32);
        cursorLabel->resize(32, 32);
    }

    moveToCursor();
    show();
    raise();
    activateWindow();
}

void AnimatedCursor::moveToCursor()
{
    QPoint cursorPos = QCursor::pos();
    QScreen *activeScreen = QGuiApplication::screenAt(cursorPos);
    if (activeScreen)
    {
        QRect screenGeo = activeScreen->geometry();
        if (!screenGeo.contains(cursorPos))
        {
            return;
        }
    }
    QPoint offset = QPoint(0, 0);
    QPoint newPos = cursorPos - offset;
    if (newPos != pos())
    {
        move(newPos);
    }
}

AnimatedCursor::~AnimatedCursor()
{
    if (cursorMovie)
    {
        cursorMovie->stop();
        delete cursorMovie;
    }
}

QString AnimatedCursor::cursorSource() const
{
    return m_cursorSource;
}
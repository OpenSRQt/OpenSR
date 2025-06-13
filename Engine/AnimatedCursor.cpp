// animatedcursor.cpp
#include "OpenSR/AnimatedCursor.h"
#include <QLabel>
#include <QMovie>
#include <QMouseEvent>
#include <QApplication>
#include <QCursor>
#include <QTimer>
#include <QDebug>
#include <QScreen>

AnimatedCursor::AnimatedCursor(QWidget* parent) : QWidget(parent) {
    setParent(nullptr);
    setAttribute(Qt::WA_TransparentForMouseEvents, true);
    setAttribute(Qt::WA_NoSystemBackground, true);
    setAttribute(Qt::WA_TranslucentBackground, true);
    setAttribute(Qt::WA_ShowWithoutActivating, true);
    setWindowFlags(Qt::FramelessWindowHint |
                  Qt::WindowStaysOnTopHint |
                  Qt::ToolTip |
                  Qt::X11BypassWindowManagerHint);
}

void AnimatedCursor::setCursor(const QString& source) {
    cursorLabel = new QLabel(this);
    cursorLabel->setAlignment(Qt::AlignCenter);
    cursorLabel->setStyleSheet("background: transparent; border: none;");

    QString cursorPath = source; //"data/DATA/Cursor/Main.GAI";
    cursorMovie = new QMovie(cursorPath);

    if (cursorMovie->isValid()) {
        cursorLabel->setMovie(cursorMovie);
        cursorMovie->start();
        resize(cursorMovie->frameRect().size());
        cursorLabel->resize(size());
        qDebug() << "Cursor animation loaded. Size:" << size();
    } else {
        qWarning() << "Failed to load cursor animation:" << cursorPath;
        cursorLabel->setText("+");
        cursorLabel->setStyleSheet("color: red; font: bold 24px; background: transparent;");
        resize(32, 32);
        cursorLabel->resize(32, 32);
    }

    moveToCursor();

    QTimer* timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, [this]() {
        moveToCursor();
    });
    timer->start(10);

    show();
    raise();
    activateWindow();
    qDebug() << "Cursor widget created at" << pos() << "size:" << size();
}

void AnimatedCursor::moveToCursor() {
    QPoint cursorPos = QCursor::pos();
    QScreen* activeScreen = QGuiApplication::screenAt(cursorPos);
    if (activeScreen) {
        QRect screenGeo = activeScreen->geometry();
        if (!screenGeo.contains(cursorPos)) {
            cursorPos = screenGeo.center();
        }
    }
    if (cursorPos != pos()) {
        move(cursorPos);
    }
}

void AnimatedCursor::showEvent(QShowEvent* event) {
    raise();
    activateWindow();
    QWidget::showEvent(event);
}

AnimatedCursor::~AnimatedCursor() {
    if (cursorMovie) {
        cursorMovie->stop();
        delete cursorMovie;
    }
}

QString AnimatedCursor::cursorSource() const {
    return m_cursorSource;
}

// void AnimatedCursor::setCursor(const QString& source) {
//     if (m_cursorSource == source)
//         return;

//     cursorLabel = new QLabel(this);
//     cursorLabel->setAlignment(Qt::AlignCenter);
//     cursorLabel->setStyleSheet("background: transparent; border: none;");

//     m_cursorSource = source;
//     QUrl url(source);
//     // Update movie
//     if (cursorMovie) {
//         cursorMovie->stop();
//         delete cursorMovie;
//     }
//     cursorMovie = new QMovie(url.toString());
//     if (cursorMovie && cursorMovie->isValid()) {
//         try{
//             cursorLabel->setMovie(cursorMovie);
//         }catch(std::exception* e) {
//             throw e;
//         }
        
//         cursorMovie->start();
//         resize(cursorMovie->frameRect().size());
//         cursorLabel->resize(size());
//         qDebug() << "Cursor animation loaded. Size:" << size();
//     } else {
//         qWarning() << "Failed to load cursor animation:" << source;
//     }
//     cursorMovie->start();
    
//     emit cursorChanged();
// }
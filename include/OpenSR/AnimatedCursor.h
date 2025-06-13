#pragma once

#include <QWidget>
#include <QMovie>
#include <QTimer>
#include <QLabel>
#include <QQmlEngine>

class AnimatedCursor : public QWidget {
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(QString cursorSource READ cursorSource WRITE setCursor NOTIFY cursorChanged)
    
public:
    Q_INVOKABLE explicit AnimatedCursor(QWidget* parent = nullptr);
    
    QString cursorSource() const;
    Q_INVOKABLE void setCursor(const QString& source);
    
    void moveToCursor();
    void showEvent(QShowEvent* event);
    ~AnimatedCursor();

signals:
    void cursorChanged();

private:
    QLabel* cursorLabel = nullptr;
    QMovie* cursorMovie = nullptr;
    QString m_cursorSource = nullptr;
};
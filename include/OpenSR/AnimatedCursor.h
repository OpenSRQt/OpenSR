#ifndef OPENSR_ANIMATEDCURSOR_H
#define OPENSR_ANIMATEDCURSOR_H

#include <QQuickPaintedItem>
#include <QMovie>
#include <QTimer>
#include <QPainter>
#include <QGuiApplication>
#include <QQuickWindow>

class AnimatedCursor : public QQuickPaintedItem
{
    Q_OBJECT
    Q_PROPERTY(QString cursorSource READ cursorSource WRITE setCursor NOTIFY cursorChanged)

public:
    explicit AnimatedCursor(QQuickItem *parent = nullptr);
    ~AnimatedCursor() override;

    QString cursorSource() const;
    Q_INVOKABLE void setCursor(const QString &source);
    Q_INVOKABLE QMovie* setCursor(QMovie* source);

signals:
    void cursorChanged();

protected:
    void paint(QPainter *painter) override;
    void componentComplete() override;

private:
    void setCursorMovie(const QString &source = "");
    QMovie *cursorMovie = nullptr;
    QTimer  *m_moveTimer = nullptr;
    QString  m_cursorSource;
};

#endif // OPENSR_ANIMATEDCURSOR_H

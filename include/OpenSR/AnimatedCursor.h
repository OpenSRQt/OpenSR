#ifndef OPENSR_ANIMATEDCURSOR_H
#define OPENSR_ANIMATEDCURSOR_H

#include <QLabel>
#include <QMovie>
#include <QTimer>
#include <QWidget>

class AnimatedCursor : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(QString cursorSource READ cursorSource WRITE setCursor NOTIFY cursorChanged)

public:
    Q_INVOKABLE explicit AnimatedCursor(QWidget *parent = nullptr);

    QString cursorSource() const;
    Q_INVOKABLE QMovie *setCursor(QMovie *source);
    Q_INVOKABLE void setCursor(const QString &source);
    void moveToCursor();
    ~AnimatedCursor() override;

signals:
    void cursorChanged();

private:
    QTimer *m_moveTimer;
    QLabel *cursorLabel = nullptr;
    QMovie *cursorMovie = nullptr;
    QString m_cursorSource = nullptr;
};

#endif // OPENSR_ANIMATEDCURSOR_H
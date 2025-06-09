/*
    OpenSR - opensource multi-genre game based upon "Space Rangers 2: Dominators"
    Copyright (C) 2015 Kosyak <ObKo@mail.ru>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef OPENSR_SPACEMOUSEAREA_H
#define OPENSR_SPACEMOUSEAREA_H

#include <OpenSR/OpenSR.h>
#include <QQuickItem>
#include <QTimer>

namespace OpenSR
{
class SpaceMouseArea : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(bool pressed READ isPressed NOTIFY pressedChanged)
    Q_PROPERTY(bool containsMouse READ containsMouse NOTIFY containsMouseChanged)
    Q_PROPERTY(qreal radius READ radius WRITE setRadius NOTIFY radiusChanged)

public:
    SpaceMouseArea(QQuickItem *parent = nullptr);

    bool contains(const QPointF &point) const override;

    bool isPressed() const;
    bool containsMouse() const;
    qreal radius() const;

    void setRadius(qreal radius);

signals:
    void pressed();
    void released();
    void clicked();
    void canceled();
    void entered();
    void exited();

    void pressedChanged();
    void containsMouseChanged();
    void radiusChanged();

protected:
    void setPressed(bool pressed);
    void setContainsMouse(bool containsMouse);
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void hoverEnterEvent(QHoverEvent *event) override;
    void hoverLeaveEvent(QHoverEvent *event) override;
    void mouseUngrabEvent() override;

private:
    bool m_pressed;
    QPointF m_pressPoint;
    bool m_containsMouse;
    qreal m_radius;
    QTimer m_enterTimer;
};
} // namespace OpenSR

#endif // OPENSR_SPACEMOUSEAREA_H

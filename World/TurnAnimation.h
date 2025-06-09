#ifndef OPENSR_WORLD_TURNANIMATION_H
#define OPENSR_WORLD_TURNANIMATION_H

#include "World.h"
#include <QAbstractAnimation>

namespace OpenSR
{
namespace World
{
class OPENSR_WORLD_API TurnAnimation : public QAbstractAnimation
{
    Q_OBJECT
public:
    TurnAnimation(QObject *parent = nullptr);
    ~TurnAnimation() override = default;

    int duration() const override;
    void updateCurrentTime(int currentTime) override;
    void setPrevTime(int prevTime);
    void setTurnDurationLock(bool turnDurationLock);

    static const int DEFAULT_TURN_DURATION = 4000;

private:
    int m_prevTime = 0;
    bool m_turnDurationLock = false;
};
} // namespace World
} // namespace OpenSR
#endif // OPENSR_WORLD_TURNANIMATION_H

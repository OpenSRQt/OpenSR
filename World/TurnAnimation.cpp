#include "TurnAnimation.h"
#include "WorldContext.h"
#include "WorldManager.h"

namespace OpenSR
{
namespace World
{
TurnAnimation::TurnAnimation(QObject *parent) : QAbstractAnimation(parent)
{
}

int TurnAnimation::duration() const
{
    return m_turnDurationLock ? DEFAULT_TURN_DURATION : -1;
}

void TurnAnimation::setPrevTime(int prevTime)
{
    m_prevTime = prevTime;
}

void TurnAnimation::updateCurrentTime(int currentTime)
{
    if (state() != TurnAnimation::Running)
        return;

    const float dt = std::max(static_cast<float>(currentTime - m_prevTime), 0.0f);
    m_prevTime = currentTime;

    WorldContext *ctx = WorldManager::instance()->context();
    class InhabitedPlanet* planetToEnter = qobject_cast<class InhabitedPlanet*>(ctx->planetToEnter());
    class Ship* playerShip = qobject_cast<class Ship*>(ctx->playerShip());
    auto shipPos = playerShip->position();
    if (ctx){
        ctx->checkPlanetProximity(planetToEnter, shipPos);
        ctx->processTurn(dt);
    }
        
}

void TurnAnimation::setTurnDurationLock(bool turnDurationLock)
{
    m_turnDurationLock = turnDurationLock;
}
} // namespace World
} // namespace OpenSR

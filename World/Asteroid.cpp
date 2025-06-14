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

#include "Asteroid.h"

#include "ResourceManager.h"

#include <QtQml>

namespace OpenSR
{
namespace World
{
namespace
{
const float TIME_DELTA_FOR_SPEED_CALC_MS = 16.0f;
const float MS_IN_ONE_SECOND = 1000.0f;
const int ITERATION_COUNTS = 10;
} // namespace

template <> void WorldObject::registerType<Asteroid>(QQmlEngine *qml, QJSEngine *script)
{
    qRegisterMetaType<AsteroidStyle>();
    qRegisterMetaType<AsteroidStyle::Data>();
    qmlRegisterType<Asteroid>("OpenSR.World", 1, 0, "Asteroid");
}

template <> Asteroid *WorldObject::createObject(WorldObject *parent, quint32 id)
{
    return new Asteroid(parent, id);
}

template <> quint32 WorldObject::staticTypeId<Asteroid>()
{
    static const quint32 id = typeIdFromClassName(Asteroid::staticMetaObject.className());
    return id;
}

template <> const QMetaObject *WorldObject::staticTypeMeta<Asteroid>()
{
    return &Asteroid::staticMetaObject;
}

QString AsteroidStyle::texture() const
{
    return getData<Data>().texture;
}

QColor AsteroidStyle::color() const
{
    return getData<Data>().color;
}

void AsteroidStyle::setTexture(const QString &texture)
{
    auto d = getData<Data>();
    d.texture = texture;
    setData(d);
}

void AsteroidStyle::setColor(const QColor &color)
{
    auto d = getData<Data>();
    d.color = color;
    setData(d);
}

QDataStream &operator<<(QDataStream &stream, const AsteroidStyle &style)
{
    return stream << style.id();
}

QDataStream &operator>>(QDataStream &stream, AsteroidStyle &style)
{
    quint32 id{};
    stream >> id;
    ResourceManager *m = ResourceManager::instance();
    Q_ASSERT(m != 0);
    Resource::replaceData(style, m->getResource(id));
    return stream;
}

QDataStream &operator<<(QDataStream &stream, const AsteroidStyle::Data &data)
{
    return stream << data.color << data.texture;
}

QDataStream &operator>>(QDataStream &stream, AsteroidStyle::Data &data)
{
    return stream >> data.color >> data.texture;
}

Asteroid::Asteroid(WorldObject *parent, quint32 id)
    : SpaceObject(parent, id), m_angle(0), m_period(0), m_time(0), m_speed(0), m_e(0.0f)
{
}

Asteroid::~Asteroid()
{
}

AsteroidStyle Asteroid::style() const
{
    return m_style;
}

QPointF Asteroid::semiAxis() const
{
    return m_semiAxis;
}

float Asteroid::angle() const
{
    return m_angle;
}

float Asteroid::period() const
{
    return m_period;
}

float Asteroid::time() const
{
    return m_time;
}

float Asteroid::speed() const
{
    return m_speed;
}

void Asteroid::setStyle(const AsteroidStyle &style)
{
    m_style = style;
    emit(styleChanged());
}

void Asteroid::setSemiAxis(const QPointF &axis)
{
    if (axis != m_semiAxis)
    {
        if (axis.x() < axis.y())
        {
            qWarning() << "Asteroid: semi-major axis is less than semi-minor, axis will be swapped";
            m_semiAxis.setX(axis.y());
            m_semiAxis.setY(axis.x());
        }
        else
        {
            m_semiAxis = axis;
        }

        emit(semiAxisChanged());
        calcEccentricity();
        calcPosition();
        calcSpeed();
        updateTrajectory();
    }
}

void Asteroid::setAngle(float angle)
{
    if (angle != m_angle)
    {
        m_angle = angle;
        emit(angleChanged());
        calcPosition();
        calcSpeed();
        updateTrajectory();
    }
}

void Asteroid::setPeriod(float period)
{
    if (period != m_period)
    {
        m_period = period;
        emit(periodChanged());
        calcPosition();
        calcSpeed();
        updateTrajectory();
    }
}

void Asteroid::setTime(float time)
{
    if (time != m_time)
    {
        m_time = time;
        emit(timeChanged());
        calcPosition();
        calcSpeed();
        updateTrajectory();
    }
}

quint32 Asteroid::typeId() const
{
    return staticTypeId<Asteroid>();
}

QString Asteroid::namePrefix() const
{
    return tr("Asteroid");
}

void Asteroid::prepareSave()
{
    m_style.registerResource();
}

void Asteroid::calcEccentricity()
{
    m_e = static_cast<float>(sqrt(1 - (m_semiAxis.y() * m_semiAxis.y()) / (m_semiAxis.x() * m_semiAxis.x())));
}

void Asteroid::updateOrbitalTime(float dt)
{
    setTime(m_time + dt);
    float periodInMS = m_period * MS_IN_ONE_SECOND;
    if (m_period > 0.0f)
    {
        m_time = static_cast<float>(fmod(m_time, periodInMS));
    }
}

void Asteroid::calcPosition()
{
    QPointF next = E(solveKepler(m_time));
    setPosition(next);
}

void Asteroid::calcSpeed()
{
    QPointF nextPos = E(solveKepler(m_time + TIME_DELTA_FOR_SPEED_CALC_MS));
    float dx = static_cast<float>(nextPos.x() - position().x());
    float dy = static_cast<float>(nextPos.y() - position().y());
    const float dtInMS = TIME_DELTA_FOR_SPEED_CALC_MS / MS_IN_ONE_SECOND;
    m_speed = static_cast<float>(sqrt(dx * dx + dy * dy)) / dtInMS;
    emit(speedChanged());
}

float Asteroid::solveKepler(float t)
{
    // http://en.wikipedia.org/wiki/Kepler's_equation
    float periodInMS = m_period * MS_IN_ONE_SECOND;
    float M = static_cast<float>((2.0f * M_PI / periodInMS)) * t;
    float E = M;
    for (int j = 0; j < ITERATION_COUNTS; j++)
    {
        E = static_cast<float>(m_e * sin(E) + M);
    }
    return E;
}

QPointF Asteroid::E(float eta)
{
    QPointF p;
    p.setX(-(m_semiAxis.x() * m_e) * cos(m_angle) + m_semiAxis.x() * cos(m_angle) * cos(eta) -
           m_semiAxis.y() * sin(m_angle) * sin(eta));
    p.setY(-(m_semiAxis.x() * m_e) * sin(m_angle) + m_semiAxis.x() * sin(m_angle) * cos(eta) +
           m_semiAxis.y() * cos(m_angle) * sin(eta));
    return p;
}

QPointF Asteroid::Ederiv(float eta)
{
    QPointF p;
    p.setX(-m_semiAxis.x() * cos(m_angle) * sin(eta) - m_semiAxis.y() * sin(m_angle) * cos(eta));
    p.setY(-m_semiAxis.x() * sin(m_angle) * sin(eta) + m_semiAxis.y() * cos(m_angle) * cos(eta));
    return p;
}

void Asteroid::updateTrajectory()
{
    QList<BezierCurve> trajectory;
    float periodInMS = m_period * MS_IN_ONE_SECOND;

    float prev = solveKepler(m_time);

    for (int i = 1; i < int(round(m_period)) + 1; i++)
    {
        float t = m_time + static_cast<float>(i) * MS_IN_ONE_SECOND;
        if (fabs(t) > periodInMS)
        {
            t = static_cast<float>(fmod(t, periodInMS));
        }

        if (t < 0)
        {
            t = periodInMS + t;
        }

        float eta = solveKepler(t);

        QPointF p;
        BezierCurve c;

        // http://www.spaceroots.org/documents/ellipse/elliptical-arc.pdf
        p = E(prev);
        c.p0 = QPointF(p.x(), p.y());
        p = E(eta);
        c.p3 = QPointF(p.x(), p.y());

        float tangent = static_cast<float>(tan((eta - prev) / 2.0f));
        float k = static_cast<float>(sin(eta - prev) * (sqrt(4.0f + 3.0f * tangent * tangent) - 1.0f) / 3.0f);

        p = Ederiv(prev);
        c.p1 = c.p0 + k * QPointF(p.x(), p.y());
        p = Ederiv(eta);
        c.p2 = c.p3 - k * QPointF(p.x(), p.y());

        trajectory.push_back(c);

        prev = eta;
    }
    setTrajectory(trajectory);
}

void Asteroid::startTurn()
{
    SpaceObject::startTurn();
}

void Asteroid::processTurn(float time)
{
    // updateOrbitalTime(time);
    // calcPosition();
    SpaceObject::processTurn(time);
}

void Asteroid::finishTurn()
{
    SpaceObject::finishTurn();
}

void Asteroid::damageObject(int damage)
{
    destroyObject();
    emit asteroidDamaged(damage);
}

void Asteroid::destroyObject()
{
    emit asteroidDestroyed();
}

} // namespace World
} // namespace OpenSR

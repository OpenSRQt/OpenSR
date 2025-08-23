#include "TestShip.h"
#include "Ship.h"

namespace OpenSR
{
namespace World
{
void TestShip::testNormalizeAngle_data()
{
    QTest::addColumn<float>("angle");
    QTest::addColumn<float>("expectedAngle");

    const auto pi = static_cast<float>(M_PI);
    const auto twoPi = static_cast<float>(2 * M_PI);

    QTest::newRow("angle within range") << 0.0f << 0.0f;
    QTest::newRow("positive angle within range") << pi / 2 << pi / 2;
    QTest::newRow("negative angle within range") << -pi / 2 << -pi / 2;
    QTest::newRow("exactly pi") << pi << pi;
    QTest::newRow("exactly -pi") << -pi << -pi;
    QTest::newRow("angle > pi") << 3 * pi / 2 << -pi / 2;
    QTest::newRow("angle > 2pi") << twoPi + pi / 4 << pi / 4;
    QTest::newRow("angle much > pi") << 5 * pi << pi;
    QTest::newRow("angle < -pi") << -3 * pi / 2 << pi / 2;
    QTest::newRow("angle < -2pi") << -twoPi - pi / 4 << -pi / 4;
    QTest::newRow("angle much < -pi") << -5 * pi << -pi;
}

void TestShip::testNormalizeAngle()
{
    QFETCH(float, angle);
    QFETCH(float, expectedAngle);

    auto actualAngle = angle;
    Ship ship{};
    ship.normalizeAngle(actualAngle);

    QVERIFY2(qFuzzyCompare(actualAngle, expectedAngle),
             qPrintable(QString("Actual: %1, Expected: %2").arg(actualAngle).arg(expectedAngle)));
}

void TestShip::testInitTargetAngle_data()
{
    QTest::addColumn<QPointF>("position");
    QTest::addColumn<QPointF>("destination");
    QTest::addColumn<float>("expectedAngle");

    const float pi = static_cast<float>(M_PI);

    QTest::newRow("east direction") << QPointF(0, 0) << QPointF(10, 0) << 0.0f;
    QTest::newRow("north direction") << QPointF(0, 0) << QPointF(0, 10) << pi / 2;
    QTest::newRow("west direction") << QPointF(0, 0) << QPointF(-10, 0) << pi;
    QTest::newRow("south direction") << QPointF(0, 0) << QPointF(0, -10) << -pi / 2;
    QTest::newRow("northeast direction") << QPointF(0, 0) << QPointF(10, 10) << pi / 4;
    QTest::newRow("southwest direction") << QPointF(0, 0) << QPointF(-10, -10) << -3 * pi / 4;
    QTest::newRow("non zero start position") << QPointF(5, 5) << QPointF(15, 5) << 0.0f;
    QTest::newRow("small coordinates") << QPointF(0.1f, 0.1f) << QPointF(0.2f, 0.1f) << 0.0f;
}

void TestShip::testInitTargetAngle()
{
    QFETCH(QPointF, position);
    QFETCH(QPointF, destination);
    QFETCH(float, expectedAngle);

    Ship ship{};
    ship.initTargetAngle(position, destination);

    const auto actualAngle = ship.m_targetAngle;

    QVERIFY2(qFuzzyCompare(actualAngle, expectedAngle),
             qPrintable(QString("Actual angle: %1, Expected: %2").arg(actualAngle).arg(expectedAngle)));
}

void TestShip::testCorrectLinearSpeed_data()
{
    QTest::addColumn<QPointF>("position");
    QTest::addColumn<QPointF>("destination");
    QTest::addColumn<float>("initialAngle");
    QTest::addColumn<float>("expectedSpeed");

    const auto pi = static_cast<float>(M_PI);
    const auto normalSpeed = Ship::normalLinearSpeed;

    QTest::newRow("straight movement no correction") << QPointF(0, 0) << QPointF(100, 0) << 0.0f << normalSpeed;
    QTest::newRow("sharp turn 90 deg requires correction") << QPointF(0, 0) << QPointF(10, 10) << pi / 2 << 0.03f;
    QTest::newRow("medium turn 45 deg no correction") << QPointF(0, 0) << QPointF(100, 100) << pi / 4 << normalSpeed;
    QTest::newRow("backward movement") << QPointF(0, 0) << QPointF(-100, 0) << pi << normalSpeed;
    QTest::newRow("small distance correction") << QPointF(0, 0) << QPointF(1, 1) << 0.0f << 0.003f;
}

void TestShip::testCorrectLinearSpeed()
{
    QFETCH(QPointF, position);
    QFETCH(QPointF, destination);
    QFETCH(float, initialAngle);
    QFETCH(float, expectedSpeed);

    Ship ship{};
    ship.m_angle = initialAngle;

    ship.correctLinearSpeed(destination, position);

    const auto actualSpeed = ship.speed();

    QVERIFY2(qFuzzyCompare(actualSpeed, expectedSpeed), qPrintable(QString("Speed correction failed\n"
                                                                           "Actual: %1\n"
                                                                           "Expected: %2\n"
                                                                           "Initial angle: %3 rad (%4 degrees)")
                                                                       .arg(actualSpeed)
                                                                       .arg(expectedSpeed)
                                                                       .arg(initialAngle)
                                                                       .arg(qRadiansToDegrees(initialAngle))));
}

void TestShip::testCalcAngle_data()
{
    QTest::addColumn<float>("dt_ms");
    QTest::addColumn<float>("currentAngle");
    QTest::addColumn<QPointF>("position");
    QTest::addColumn<QPointF>("destination");
    QTest::addColumn<float>("expectedAngle");

    const auto pi = static_cast<float>(M_PI);
    const auto angularSpeedPerMs = Ship::normalAngularSpeed;

    const auto dt_60fps = 17.0f;
    const auto dt_30fps = 33.0f;

    QTest::newRow("60fps already at target")
        << dt_60fps << pi / 4.0f << QPointF(0.0f, 0.0f) << QPointF(10.0f, 10.0f) << pi / 4.0f;
    QTest::newRow("60fps counter clockwise")
        << dt_60fps << 0.0f << QPointF(0.0f, 0.0f) << QPointF(-10.0f, 0.1f) << 0.0f + dt_60fps * angularSpeedPerMs;
    QTest::newRow("60fps clockwise") << dt_60fps << pi << QPointF(0.0f, 0.0f) << QPointF(10.0f, 0.1f)
                                     << pi - dt_60fps * angularSpeedPerMs;
    QTest::newRow("30fps medium rotation")
        << dt_30fps << 0.0f << QPointF(0.0f, 0.0f) << QPointF(-10.0f, 10.0f) << 0.0f + dt_30fps * angularSpeedPerMs;
    QTest::newRow("60fps small rotation")
        << dt_60fps << pi - 0.001f << QPointF(0.0f, 0.0f) << QPointF(-10.0f, 0.0f) << pi;
    QTest::newRow("60fps reach target in one frame")
        << dt_60fps << 0.0f << QPointF(0.0f, 0.0f) << QPointF(0.0f, 0.1f) << dt_60fps * angularSpeedPerMs;
}

void TestShip::testCalcAngle()
{
    QFETCH(float, dt_ms);
    QFETCH(float, currentAngle);
    QFETCH(QPointF, position);
    QFETCH(QPointF, destination);
    QFETCH(float, expectedAngle);

    Ship ship{};
    ship.setAngle(currentAngle);

    const auto resultAngle = ship.calcAngle(dt_ms, currentAngle, position, destination);

    QVERIFY2(qFuzzyCompare(resultAngle, expectedAngle), qPrintable(QString("Angle calculation failed\n"
                                                                           "Time step: %1 s\n"
                                                                           "Start angle: %2 rad (%3 degrees)\n"
                                                                           "Result angle: %4 rad (%5 degrees)\n"
                                                                           "Expected angle: %6 rad (%7 degrees)")
                                                                       .arg(dt_ms)
                                                                       .arg(currentAngle)
                                                                       .arg(qRadiansToDegrees(currentAngle))
                                                                       .arg(resultAngle)
                                                                       .arg(qRadiansToDegrees(resultAngle))
                                                                       .arg(expectedAngle)
                                                                       .arg(qRadiansToDegrees(expectedAngle))));
}

void TestShip::testCalcPosition_data()
{
    QTest::addColumn<float>("dt_ms");
    QTest::addColumn<float>("angle");
    QTest::addColumn<QPointF>("position");
    QTest::addColumn<QPointF>("destination");
    QTest::addColumn<QPointF>("expectedPosition");

    const auto pi = static_cast<float>(M_PI);
    const auto linearSpeedPerMs = Ship::normalLinearSpeed;

    const auto dt_60fps = 17.0f;
    const auto dt_30fps = 33.0f;

    QTest::newRow("already at destination")
        << dt_60fps << 0.0f << QPointF(10.0f, 10.0f) << QPointF(10.0f, 10.0f) << QPointF(10.0f, 10.0f);
    QTest::newRow("60fps move east") << dt_60fps << 0.0f << QPointF(0.0f, 0.0f) << QPointF(100.0f, 0.0f)
                                     << QPointF(dt_60fps * linearSpeedPerMs, 0.0f);
    QTest::newRow("30fps move north") << dt_30fps << pi / 2 << QPointF(0, 0) << QPointF(0, 100)
                                      << QPointF(0.0f, dt_30fps * linearSpeedPerMs);
    QTest::newRow("60fps move northeast")
        << dt_60fps << pi / 4 << QPointF(0.0f, 0.0f) << QPointF(100.0f, 100.0f)
        << QPointF(dt_60fps * linearSpeedPerMs * std::cos(pi / 4), dt_60fps * linearSpeedPerMs * std::sin(pi / 4));
    QTest::newRow("60fps move southwest")
        << dt_60fps << -pi / 4 << QPointF(0.0f, 0.0f) << QPointF(-100.0f, -100.0f)
        << QPointF(dt_60fps * linearSpeedPerMs * std::cos(-pi / 4), dt_60fps * linearSpeedPerMs * std::sin(-pi / 4));
    QTest::newRow("reach destination exactly")
        << (10.0f / linearSpeedPerMs) << 0.0f << QPointF(0.0f, 0.0f) << QPointF(10.0f, 0.0f) << QPointF(10.0f, 0.0f);
}

void TestShip::testCalcPosition()
{
    QFETCH(float, dt_ms);
    QFETCH(float, angle);
    QFETCH(QPointF, position);
    QFETCH(QPointF, destination);
    QFETCH(QPointF, expectedPosition);

    Ship ship{};
    const auto result = ship.calcPosition(dt_ms, angle, position, destination);

    auto fuzzyEqual = [](float a, float b) { return qFuzzyCompare(a, b) || qAbs(a - b) < 1e-6f; };

    const bool match = fuzzyEqual(static_cast<float>(result.x()), static_cast<float>(expectedPosition.x())) &&
                       fuzzyEqual(static_cast<float>(result.y()), static_cast<float>(expectedPosition.y()));

    QVERIFY2(match, qPrintable(QString("Position mismatch"
                                       "Actual: (%1, %2)\n"
                                       "Expected: (%3, %4)\n"
                                       "dt: %5 ms, angle: %6 rad")
                                   .arg(result.x())
                                   .arg(result.y())
                                   .arg(expectedPosition.x())
                                   .arg(expectedPosition.y())
                                   .arg(dt_ms)
                                   .arg(angle)));
}

void TestShip::testUpdatePositionMoving_data()
{
    QTest::addColumn<float>("dt_ms");
    QTest::addColumn<QPointF>("expectedPosition");
    QTest::addColumn<bool>("shouldArrive");

    const auto linearSpeedPerMs = Ship::normalLinearSpeed;
    const auto dt_60fps = 17.0f;
    const auto dt_30fps = 33.0f;

    QTest::newRow("60fps movement") << dt_60fps << QPointF(dt_60fps * linearSpeedPerMs, 0.0f) << false;
    QTest::newRow("30fps movement") << dt_30fps << QPointF(33.0f * linearSpeedPerMs, 0.0f) << false;
    QTest::newRow("exact arrival") << static_cast<float>(100 / linearSpeedPerMs) << QPointF(100.0f, 0.0f) << true;
    QTest::newRow("near arrival") << 330.0f << QPointF(99.0f, 0.0f) << false;
}

void TestShip::testUpdatePositionMoving()
{
    QFETCH(float, dt_ms);
    QFETCH(QPointF, expectedPosition);
    QFETCH(bool, shouldArrive);

    Ship ship{};
    ship.setPosition(QPointF(0, 0));
    ship.setDestination(QPointF(100, 0));
    ship.setAngle(0.0f);
    ship.setIsMoving(true);
    ship.updatePosition(dt_ms);

    const bool match =
        qFuzzyCompare(static_cast<float>(ship.position().x()), static_cast<float>(expectedPosition.x())) &&
        qFuzzyCompare(static_cast<float>(ship.position().y()), static_cast<float>(expectedPosition.y()));

    QVERIFY2(match, qPrintable(QString("Position mismatch.\n"
                                       "Actual: (%1, %2)\n"
                                       "Expected: (%3, %4)\n"
                                       "dt: %5 ms")
                                   .arg(ship.position().x())
                                   .arg(ship.position().y())
                                   .arg(expectedPosition.x())
                                   .arg(expectedPosition.y())
                                   .arg(dt_ms)));

    QCOMPARE(ship.isMoving(), !shouldArrive);
}

void TestShip::testUpdatePositionSignals()
{
    Ship ship{};
    ship.setPosition(QPointF(0.0f, 0.0f));
    ship.setDestination(QPointF(100.0f, 0.0f));
    ship.setAngle(0.0f);
    ship.setIsMoving(true);

    QSignalSpy arrivedSpy(&ship, &Ship::shipArrived);

    ship.updatePosition(16.0f);
    QCOMPARE(arrivedSpy.count(), 0);

    ship.setPosition(QPointF(99.9f, 0));
    ship.updatePosition(1.0f);
    QCOMPARE(arrivedSpy.count(), 1);
}

} // namespace World
} // namespace OpenSR

QTEST_APPLESS_MAIN(OpenSR::World::TestShip)
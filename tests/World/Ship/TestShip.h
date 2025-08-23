#pragma once

#include <QtTest>

namespace OpenSR
{
namespace World
{

class TestShip : public QObject
{
    Q_OBJECT

private slots:
    void testNormalizeAngle_data();
    void testNormalizeAngle();
    void testInitTargetAngle_data();
    void testInitTargetAngle();
    void testCorrectLinearSpeed_data();
    void testCorrectLinearSpeed();
    void testCalcAngle_data();
    void testCalcAngle();
    void testCalcPosition_data();
    void testCalcPosition();
    void testUpdatePositionMoving_data();
    void testUpdatePositionMoving();
    void testUpdatePositionSignals();
};
} // namespace World
} // namespace OpenSR
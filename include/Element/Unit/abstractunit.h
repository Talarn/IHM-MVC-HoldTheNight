#pragma once

#include "include/Element/element.h"
/**
 * @brief The AbstractUnit class
 * An AbstractUnit has several method to set the position and angle
 */
class AbstractUnit : public Element {

public:
    AbstractUnit(QPair<uint16_t, uint16_t> position, uint16_t life, uint8_t moveSpeed,
                 uint8_t attackDamage, uint8_t viewBox, QStringList attackableUnits);
    ~AbstractUnit();
    void attackElement(Element* element) const;
    bool isNearPosition(QPair<uint16_t, uint16_t> targetPosition, uint8_t dist) const;
    bool searchAndAttackUnit(const QHash<QString, QList<AbstractUnit *> > &enemies);
    void calculateNewPositionAndAngle(QPair<uint16_t, uint16_t> targetPosition);
    void calculateNewAngle(QPair<uint16_t, uint16_t> targetPosition);
    virtual void test() = 0;
    bool isGoingLeft();

protected:
    uint8_t m_attackDamage;
    uint8_t m_moveSpeed;
    uint8_t m_viewBox;
    QStringList m_attackableUnits;
    bool m_isGoingLeft = false;
};



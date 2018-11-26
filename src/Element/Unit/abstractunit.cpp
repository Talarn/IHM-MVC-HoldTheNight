#include "include/Element/Unit/abstractunit.h"
#include <QHash>
#include <QString>
#include <QList>
#include <QDebug>
AbstractUnit::AbstractUnit(QPair<uint16_t, uint16_t> position, uint16_t life, uint8_t moveSpeed,
                           uint8_t attackDamage, uint8_t viewBox, QStringList attackableUnits)
    : Element(position,life), m_moveSpeed(moveSpeed),m_attackDamage(attackDamage),m_viewBox(viewBox),
      m_attackableUnits(attackableUnits){

}

AbstractUnit::~AbstractUnit(){

}

void AbstractUnit::attackElement(Element* element) const{
    element->takeDamage(m_attackDamage);
}

bool AbstractUnit::searchAndAttackUnit(const QHash<QString, QList<AbstractUnit*>>& enemies) {
    for(int i = 0; i < m_attackableUnits.size(); i++){
        QList<AbstractUnit*> enemiesList = enemies[m_attackableUnits[i]];
        for(int j = 0; j < enemiesList.size(); j++){
            if(isNearPosition(enemiesList[j]->getPosition(),m_viewBox)){
                if(rand() % 10 == 1) attackElement(enemiesList[j]);
                return true;
            }
        }
    }
    return false;
}

void AbstractUnit::calculateNewAngle(QPair<uint16_t, uint16_t> targetPosition) {

    QPair<int16_t, int16_t> targetVector;
    targetVector.first = targetPosition.first - m_position.first;
    targetVector.second = targetPosition.second - m_position.second;
    uint16_t newAngle = atan(targetVector.second/targetVector.first);
    setAngle(newAngle);
}

void AbstractUnit::calculateNewPositionAndAngle(QPair<uint16_t, uint16_t> targetPosition) {

    QPair<int16_t, int16_t> targetVector;
    targetVector.first = targetPosition.first - m_position.first;
    targetVector.second = targetPosition.second - m_position.second;

    QPair<double, double> normalizedVector;
    normalizedVector.first = 0.0;
    normalizedVector.second = 0.0;

    double vectorLength = (sqrt(pow(targetVector.first, 2) + pow(targetVector.second, 2)));

    if(vectorLength > 0){
        normalizedVector.first = (double)targetVector.first / vectorLength;
        normalizedVector.second = (double)targetVector.second / vectorLength;
    }

    QPair<double, double> finalVector;
    finalVector.first = normalizedVector.first * m_moveSpeed;
    finalVector.second = normalizedVector.second * m_moveSpeed;

    QPair<uint16_t, uint16_t> newPosition;
    newPosition.first = m_position.first + finalVector.first;
    newPosition.second = m_position.second + finalVector.second;

    if(newPosition.first > m_position.first){
        m_isGoingLeft = false;
    } else {
        m_isGoingLeft = true;
    }

    uint16_t newAngle = atan(finalVector.second/finalVector.first);
    setAngle(newAngle);
    setPosition(newPosition);

}

bool AbstractUnit::isNearPosition(QPair<uint16_t, uint16_t> targetPosition,uint8_t dist) const {

    uint16_t distance;
    uint16_t xa = m_position.first;
    uint16_t ya = m_position.second;
    uint16_t xb = targetPosition.first;
    uint16_t yb = targetPosition.second;
    distance = sqrt((pow(xb - xa, 2)) + (pow(yb - ya, 2)));

    if(distance < dist){
        return true;
    }
    return false;
}

bool AbstractUnit::isGoingLeft(){
    return m_isGoingLeft;
}

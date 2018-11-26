#include "include/Element/Unit/AllyUnit/bomber.h"
#include "include/Element/Resource/resource.h"

Bomber::Bomber(QPair<uint16_t, uint16_t> position, uint16_t life, uint8_t moveSpeed, uint8_t attackDamage,
               uint8_t viewBox, QStringList attackableUnits, QStringList attackableResources, QPair<uint16_t, uint16_t> targetPosition)
    : AllyUnit(position,life,moveSpeed,attackDamage,viewBox,attackableUnits,attackableResources),m_targetPosition(targetPosition),m_hasReachTargetPoint(false) {

}


void Bomber::updateUnits(const QHash<QString, QList<AbstractUnit *> > &enemies,
                          const QHash<QString, QList<QPair<uint16_t, uint16_t>>>& pheromoneTrails,
                           QList<Resource*>& resourcesArray, const QPair<uint16_t,uint16_t>& spawnPosition) {

    if(!m_hasReachTargetPoint){
        calculateNewPositionAndAngle(m_targetPosition);
        if(isNearPosition(m_targetPosition,10)){
            m_hasReachTargetPoint = true;
            for(int i = 0; i < m_attackableUnits.size(); i++){
                QList<AbstractUnit*> enemiesList = enemies[m_attackableUnits[i]];
                for(int j = 0; j < enemiesList.size(); j++){
                    if(isNearPosition(enemiesList[j]->getPosition(),m_viewBox)){
                        attackElement(enemiesList[j]);
                    }
                }
            }
        }
    } else {
        calculateNewPositionAndAngle(spawnPosition);
        if(isNearPosition(spawnPosition,10)){
            m_jobDone = true;
        }
    }
}

#include "include/Element/Unit/AllyUnit/allyunit.h"
#include "include/Element/Resource/resource.h"
#include <QDebug>
AllyUnit::AllyUnit(QPair<uint16_t, uint16_t> position, uint16_t life, uint8_t moveSpeed, uint8_t attackDamage,
                   uint8_t viewBox, QStringList attackableUnits, QStringList attackableResources)
    : AbstractUnit(position,life,moveSpeed,attackDamage,viewBox,attackableUnits),
      m_attackableResources(attackableResources),m_hasReachPathLimit(false),
      m_jobDone(false), m_targetPointIndex(0),m_hasFoundResources(false) {


}

void AllyUnit::searchAndCollectResource(QList<Resource *> &resourcesArray) {

    for(int i = 0; i < resourcesArray.size(); i++){
        if(isNearPosition(resourcesArray[i]->getPosition(), 10)){
            m_hasFoundResources = true;
            return;
        }
    }
}


void AllyUnit::followPheromoneTrails(const QList<QPair<uint16_t, uint16_t>>& trails, const QPair<uint16_t, uint16_t>& spawnPosition) {

    if(isNearPosition(trails[trails.size() - 1], 10) && !m_hasReachPathLimit && (m_targetPointIndex == (trails.size() - 1 ))) {
        m_hasReachPathLimit = true;
        if(trails.size() > 2) m_targetPointIndex--;
    }

    if(isNearPosition(trails[m_targetPointIndex],10)) {
        if((m_hasReachPathLimit == true || m_hasFoundResources == true) && (m_targetPointIndex != 0)) m_targetPointIndex--;
        if(m_hasReachPathLimit == false) m_targetPointIndex++;
    }

    if((m_hasReachPathLimit == true || m_hasFoundResources == true) && (m_targetPointIndex == 0)){
        calculateNewPositionAndAngle(spawnPosition);
    } else {
        calculateNewPositionAndAngle(trails[m_targetPointIndex]);
    }

    if((m_hasReachPathLimit == true || m_hasFoundResources == true) && isNearPosition(spawnPosition,10)) {
        m_jobDone = true;
    }
}

bool AllyUnit::getIsJobDone(){
    return m_jobDone;
}

bool AllyUnit::hasFoundResources(){
    return m_hasFoundResources;
}

void AllyUnit::test(){

}

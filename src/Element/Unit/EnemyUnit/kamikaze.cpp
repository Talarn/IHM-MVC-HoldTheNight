#include "include/Element/Unit/EnemyUnit/kamikaze.h"
#include "include/Element/Building/building.h"

Kamikaze::Kamikaze(QPair<uint16_t, uint16_t> position, uint16_t life, uint8_t moveSpeed,
                   uint8_t attackDamage, uint8_t viewBox, QStringList attackableUnits)
    : EnemyUnit(position,life,moveSpeed,attackDamage,viewBox,attackableUnits) {

}

void Kamikaze::updateUnits(QHash<QString, QList<AbstractUnit*>>& allies, Building *allySpawn) {
    calculateNewPositionAndAngle(allySpawn->getPosition());
    if(!m_isDead){
        if(isNearPosition(allySpawn->getPosition(), 30)){
            attackElement(allySpawn);
            m_isDead = true;
        }
    }
}




#include "include/Element/Unit/EnemyUnit/enemyunit.h"

EnemyUnit::EnemyUnit(QPair<uint16_t, uint16_t> position, uint16_t life, uint8_t moveSpeed,
                     uint8_t attackDamage, uint8_t viewBox, QStringList attackableUnits)
    : AbstractUnit(position,life,moveSpeed,attackDamage,viewBox,attackableUnits),
      m_hasReachAllySpawn(false) {

}

void EnemyUnit::moveToSpawnAndAttackAlly(QHash<QString, QList<AbstractUnit*>>& allies, Building *allySpawn) {
    if(!m_hasReachAllySpawn){
        if(!searchAndAttackUnit(allies)){
            if(rand() % 20 == 1) calculateNewPositionAndAngle(allySpawn->getPosition());
            if(isNearPosition(allySpawn->getPosition(),10)){
                m_hasReachAllySpawn = true;
            }
        }
    } else {
        attackElement(allySpawn);
    }
}

void EnemyUnit::test(){

}

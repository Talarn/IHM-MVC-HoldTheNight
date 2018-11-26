#include "include/Element/Unit/EnemyUnit/basic.h"
#include "include/Element/Building/building.h"

Basic::Basic(QPair<uint16_t, uint16_t> position, uint16_t life, uint8_t moveSpeed,
                   uint8_t attackDamage, uint8_t viewBox, QStringList attackableUnits)
    : EnemyUnit(position,life,moveSpeed,attackDamage,viewBox,attackableUnits) {

}
void Basic::updateUnits(QHash<QString, QList<AbstractUnit*>>& allies, Building *allySpawn) {
    moveToSpawnAndAttackAlly(allies, allySpawn);
}







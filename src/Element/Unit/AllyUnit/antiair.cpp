#include "include/Element/Unit/AllyUnit/antiair.h"
#include "include/Element/Resource/resource.h"


AntiAir::AntiAir(QPair<uint16_t, uint16_t> position, uint16_t life, uint8_t moveSpeed, uint8_t attackDamage,
               uint8_t viewBox, QStringList attackableUnits, QStringList attackableResources)
    : AllyUnit(position,life,moveSpeed,attackDamage,viewBox,attackableUnits,attackableResources) {

}

void AntiAir::updateUnits(const QHash<QString, QList<AbstractUnit *> > &enemies,
                          const QHash<QString, QList<QPair<uint16_t, uint16_t>>>& pheromoneTrails,
                           QList<Resource*>& resourcesArray, const QPair<uint16_t,uint16_t>& spawnPosition) {

    if(!searchAndAttackUnit(enemies)){
        followPheromoneTrails(pheromoneTrails["AntiAir"],spawnPosition);
    }
}

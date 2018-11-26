#include "include/Element/Unit/AllyUnit/worker.h"
#include "include/Element/Resource/resource.h"

Worker::Worker(QPair<uint16_t, uint16_t> position, uint16_t life, uint8_t moveSpeed, uint8_t attackDamage,
               uint8_t viewBox, QStringList attackableUnits, QStringList attackableResources)
    : AllyUnit(position,life,moveSpeed,attackDamage,viewBox,attackableUnits,attackableResources) {

}

void Worker::updateUnits(const QHash<QString, QList<AbstractUnit *> > &enemies,
                         const QHash<QString, QList<QPair<uint16_t, uint16_t>>>& pheromoneTrails,
                         QList<Resource *>& resourcesArray, const QPair<uint16_t,uint16_t>& spawnPosition) {
    if(!searchAndAttackUnit(enemies)){
        followPheromoneTrails(pheromoneTrails["Worker"], spawnPosition);
        if(!m_hasFoundResources){
            searchAndCollectResource(resourcesArray);
        }
    }
}

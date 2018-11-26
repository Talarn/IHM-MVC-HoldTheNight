#pragma once

#include "include/Element/Unit/AllyUnit/allyunit.h"
/**
 * @brief The Worker class
 */
class Worker : public AllyUnit{
public:

    Worker(QPair<uint16_t, uint16_t> position, uint16_t life, uint8_t moveSpeed, uint8_t attackDamage,
           uint8_t viewBox, QStringList attackableUnits, QStringList attackableResources);
    virtual void updateUnits(const QHash<QString, QList<AbstractUnit*>>& enemies, const QHash<QString, QList<QPair<uint16_t, uint16_t>>>& pheromoneTrails,
                   QList<Resource *> &resourcesArray, const QPair<uint16_t,uint16_t>& spawnPosition);

};


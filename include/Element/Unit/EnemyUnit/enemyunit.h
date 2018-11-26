#pragma once

#include <stdint.h>
#include "include/Element/Unit/abstractunit.h"
#include "include/Element/Building/building.h"

/**
 * @brief The EnemyUnit class
 * An EnemyUnit can only move to the AllyBase and attack ally is they are nearby or the AllyBase
 */
class EnemyUnit : public AbstractUnit {

public:

    EnemyUnit(QPair<uint16_t, uint16_t> position, uint16_t life, uint8_t moveSpeed,
             uint8_t attackDamage, uint8_t viewBox, QStringList attackableUnits);
    virtual void updateUnits(QHash<QString, QList<AbstractUnit*>>& allies, Building *allySpawn) = 0;
    void moveToSpawnAndAttackAlly(QHash<QString, QList<AbstractUnit *> > &allies, Building *allySpawn);
    virtual void test();
protected:
    bool m_hasReachAllySpawn;
};


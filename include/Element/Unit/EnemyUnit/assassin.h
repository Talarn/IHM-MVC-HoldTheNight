#pragma once
#include "include/Element/Unit/EnemyUnit/enemyunit.h"
/**
 * @brief The Assassin class
 */
class Assassin : public EnemyUnit {

public :

    Assassin(QPair<uint16_t, uint16_t> position, uint16_t life, uint8_t moveSpeed,
             uint8_t attackDamage, uint8_t viewBox, QStringList attackableUnits);
    virtual void updateUnits(QHash<QString, QList<AbstractUnit*>>& allies, Building *allySpawn);

};

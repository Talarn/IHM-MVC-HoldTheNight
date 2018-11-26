#pragma once

class EnemyUnit;
class Resource;
#include "include/Element/Unit/abstractunit.h"
#include <QHash>

/**
 * @brief The AllyUnit class
 * An AllyUnit can move according to a path, it can search for resources and update his own position and angle
 */
class AllyUnit : public AbstractUnit {

public:
    AllyUnit(QPair<uint16_t, uint16_t> position,uint16_t life,uint8_t moveSpeed,
             uint8_t attackDamage,uint8_t viewBox,QStringList attackableUnits,QStringList attackableResources);
    virtual void updateUnits(const QHash<QString, QList<AbstractUnit*>>& enemies, const QHash<QString, QList<QPair<uint16_t, uint16_t>>>& pheromoneTrails,
                             QList<Resource*>& resourcesArray, const QPair<uint16_t,uint16_t>& spawnPosition) = 0;
    void searchAndCollectResource(QList<Resource*>& resourcesArray);
    void followPheromoneTrails(const QList<QPair<uint16_t, uint16_t>>& trails,const QPair<uint16_t,uint16_t>& spawnPosition);
    bool getIsJobDone();
    bool hasFoundResources();
    virtual void test();

protected:
    QStringList m_attackableResources;
    bool m_hasReachPathLimit;
    bool m_hasFoundResources;
    uint8_t m_targetPointIndex;
    bool m_jobDone;

};

#pragma once

#include <QHash>
#include <QList>
#include <QPair>
#include <QVector>
#include "include/Element/Unit/AllyUnit/allyunit.h"
#include "include/Element/Unit/EnemyUnit/enemyunit.h"
#include "include/Element/Building/building.h"
#include "include/Element/Resource/resource.h"
#include "include/settings.h"
/**
 * @brief The Map class
 * This class correspond to the Model / Database
 * All the Units / Pheromone Trails / Buildings and Resources are stored here
 */
class Map {

public:
    Map(QString filename);
    ~Map();
    QHash<QString, QList<QPair<uint16_t, uint16_t>>> getPheromoneTrails() const;
    QHash<QString, QList<AbstractUnit*>> getAlliesArray() const;
    QHash<QString, QList<AbstractUnit*>> getEnemiesArray() const;
    QList<Resource*>  getResourcesArray() const;
    Building* getAllySpawn() const;
    uint16_t getFood() const;
    uint8_t getWaveNumber() const;
    QHash<QString, uint16_t> getAvailableUnits() const;
    QStringList getAlliesNames() const;
    QStringList getEnemiesNames() const;
    Settings getSettings() const;

    void setPheromoneTrails(QHash<QString, QList<QPair<uint16_t, uint16_t>>> newPheromoneTrails);
    void setAlliesArray(QHash<QString, QList<AbstractUnit*>> newAlliesArray);
    void setEnemiesArray(QHash<QString, QList<AbstractUnit*>> newEnemiesArray);
    void setResourcesArray(QList<Resource *> newResourcesArray);
    void setAllySpawn(Building* newSpawn);
    void setFood(uint16_t newFood);
    void setWaveNumber(uint16_t newWaveNumber);
    void setAvailableUnits(QHash<QString, uint16_t> newAvailableUnits);

private:
    Settings m_settings;
    uint16_t m_food;
    uint8_t m_waveNumber;
    uint8_t m_waveProgress;
    QHash<QString, uint16_t> m_availableUnits;
    QStringList m_alliesNames;
    QStringList m_enemiesNames;
    QHash<QString, QList<QPair<uint16_t, uint16_t>>> m_pheromoneTrails;
    QHash<QString, QList<AbstractUnit*>> m_alliesArray;
    QHash<QString, QList<AbstractUnit*>> m_enemiesArray;
    QList<Resource*> m_resourcesArray;
    Building *m_pAllySpawn;
};


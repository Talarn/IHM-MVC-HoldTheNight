#pragma once
#include <QHash>
#include <QPair>
#include <QVector>
#include <stdint.h>

/**
 * @brief The Settings class
 * This class is used to read settings from a JSON files
 * It contains all the settings used to start the game, instanciate units, building etc
 */
class Settings {

public:
    Settings(QString filename);
    void parseJSON(QString filename);
    QPair<uint16_t, uint16_t> getSpawnPosition(QString spawnName) const;
    QPair<uint16_t, uint16_t> getMapSize() const;
    uint16_t getSpawnLife() const;
    uint16_t getAvailableFood() const;
    uint16_t getTimeBetweenWaves() const;
    uint16_t getAllyFoodCost(QString unitName) const;
    uint16_t getEnemyFoodDrop(QString unitName) const;
    QHash<QString, uint16_t> getAllyUnitSettings(QString unitName) const;
    QHash<QString, uint16_t> getEnemyUnitSettings(QString unitName) const;
    QStringList getAttackableUnits(QString unitName) const;
    QStringList getAttackableResources(QString unitName) const;
    QStringList getEnemyWave(uint8_t waveNumber) const;
    uint8_t getEnemyWaveSize() const;
    QStringList getAlliesNames() const;
    QStringList getEnemiesNames() const;

private:

    QHash<QString, QPair<uint16_t, uint16_t>> m_spawnsPosition;
    QPair<uint16_t, uint16_t> m_mapSize;
    uint16_t m_spawnLife;
    uint16_t m_availableFood;
    uint16_t m_timeBetweenWaves;

    QHash<QString, uint16_t> m_alliesFoodCost;
    QHash<QString, uint16_t> m_enemiesFoodDrop;
    QHash<QString, QHash<QString, uint16_t>> m_allyCharacteristics;
    QHash<QString, QHash<QString, uint16_t>> m_enemyCharacteristics;
    QHash<QString, QStringList> m_attackableUnits;
    QHash<QString, QStringList> m_attackableResources;
    QVector<QStringList> m_enemyWaves;
    QStringList m_alliesNames;
    QStringList m_enemiesNames;

};

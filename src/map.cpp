#include "include/map.h"
#include "include/settings.h"
#include <QDebug>

Map::Map(QString filename)
    : m_settings(Settings(filename)), m_waveNumber(0), m_waveProgress(0)
{
    m_alliesNames = m_settings.getAlliesNames();
    m_enemiesNames = m_settings.getEnemiesNames();
    m_food = m_settings.getAvailableFood();
    for(QString unit : m_alliesNames) {
        m_availableUnits[unit] = 0;
        m_pheromoneTrails[unit] = QList<QPair<uint16_t,uint16_t>>();
        m_alliesArray[unit] = QList<AbstractUnit*>();
    }
    for(QString unit : m_enemiesNames){
        m_enemiesArray[unit] = QList<AbstractUnit*>();
    }

    m_pAllySpawn = new Building(m_settings.getSpawnPosition("Ally"), m_settings.getSpawnLife());
    QPair<uint16_t,uint16_t> resourcePosition;
    resourcePosition.first = 600;
    resourcePosition.second = 600;
    m_resourcesArray.append(new Resource(resourcePosition,100));
}
Map::~Map(){
    delete m_pAllySpawn;
}
QHash<QString, QList<QPair<uint16_t, uint16_t>>> Map::getPheromoneTrails() const{
    return m_pheromoneTrails;
}

QHash<QString, QList<AbstractUnit*>> Map::getAlliesArray() const{
    return m_alliesArray;
}

QHash<QString, QList<AbstractUnit*>> Map::getEnemiesArray() const{
    return m_enemiesArray;
}

QList<Resource *> Map::getResourcesArray() const{
    return m_resourcesArray;
}

Building* Map::getAllySpawn() const{
    return m_pAllySpawn;
}

uint16_t Map::getFood() const{
    return m_food;
}

uint8_t Map::getWaveNumber() const{
    return m_waveNumber;
}

QHash<QString, uint16_t> Map::getAvailableUnits() const{
    return m_availableUnits;
}

QStringList Map::getAlliesNames() const{
    return m_alliesNames;
}

QStringList Map::getEnemiesNames() const{
    return m_enemiesNames;
}

Settings Map::getSettings() const{
    return m_settings;
}

void Map::setPheromoneTrails(QHash<QString, QList<QPair<uint16_t, uint16_t>>> newPheromoneTrails){
    m_pheromoneTrails = newPheromoneTrails;
}

void Map::setAlliesArray(QHash<QString, QList<AbstractUnit*>> newAlliesArray){
    m_alliesArray = newAlliesArray;
}

void Map::setEnemiesArray(QHash<QString, QList<AbstractUnit*>> newEnemiesArray){
    m_enemiesArray = newEnemiesArray;
}

void Map::setResourcesArray(QList<Resource*> newResourcesArray){
    m_resourcesArray = newResourcesArray;
}

void Map::setAllySpawn(Building* newSpawn){
    m_pAllySpawn = newSpawn;
}

void Map::setFood(uint16_t newFood){
    m_food = newFood;
}

void Map::setWaveNumber(uint16_t newWaveNumber){
    m_waveNumber = newWaveNumber;
}

void Map::setAvailableUnits(QHash<QString, uint16_t> newAvailableUnits){
    m_availableUnits = newAvailableUnits;
}


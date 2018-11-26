#include "include/settings.h"
#include <QFile>
#include <QJsonDocument>
#include <QDebug>
#include <QJsonObject>
#include <QByteArray>
#include <QJsonParseError>
#include <QJsonValue>
#include <QJsonArray>

Settings::Settings(QString filename){
    parseJSON(filename);
}
/**
 * @brief Settings::parseJSON
 * This method is used to parse the JSON file settings
 * @note Look for the JSON file structure for more understanding of this code
 * @param filename is the JSON path
 */
void Settings::parseJSON(QString filename){
    QString jsonContent;
    QFile jsonFile;
    jsonFile.setFileName(filename);
    jsonFile.open(QIODevice::ReadOnly | QIODevice::Text);
    jsonContent = jsonFile.readAll();
    jsonFile.close();

    QJsonParseError *error = new QJsonParseError;
    QJsonDocument jsonDocument = QJsonDocument::fromJson(jsonContent.toUtf8(),error);

    QJsonObject jsonObject = jsonDocument.object();

    m_mapSize.first = jsonObject["WindowWidth"].toInt();
    m_mapSize.second = jsonObject["WindowHeight"].toInt();
    m_availableFood = jsonObject.value("AvailableFood").toInt();

    QJsonObject foodCostObject = jsonObject.value("FoodCost").toObject();
    for(QString key : foodCostObject.keys()){
        m_alliesFoodCost[key] = foodCostObject[key].toInt();
    }

    QJsonObject foodDropObject = jsonObject.value("FoodDrop").toObject();
    for(QString key : foodDropObject.keys()){
        m_enemiesFoodDrop[key] = foodDropObject[key].toInt();
    }

    QJsonObject unitsObject = jsonObject.value("Units").toObject();
    QJsonObject alliesObject = unitsObject.value("Allies").toObject();
    for(QString ally : alliesObject.keys()){
        m_alliesNames << ally;
        QJsonObject allyUnit = alliesObject.value(ally).toObject();
        QHash<QString, uint16_t> characteristics;
        for(QString characteristic : allyUnit.keys()){
            if((characteristic == "AttackableUnits")){
                QStringList attackableUnitsList;
                QJsonArray attackableUnitsArray = allyUnit[characteristic].toArray();
                for(int i = 0; i < attackableUnitsArray.size(); i++){
                    attackableUnitsList += attackableUnitsArray.at(i).toString();
                }
                m_attackableUnits[ally] = attackableUnitsList;
            } else if(characteristic == "AttackableResources"){
                QStringList attackableResourcesList;
                QJsonArray attackableResourcesArray = allyUnit[characteristic].toArray();
                for(int i = 0; i < attackableResourcesArray.size(); i++){
                    attackableResourcesList += attackableResourcesArray.at(i).toString();
                }
                m_attackableResources[ally] = attackableResourcesList;
            } else {
                characteristics[characteristic] = allyUnit[characteristic].toInt();
            }
        }
        m_allyCharacteristics[ally] = characteristics;
    }

    QJsonObject enemiesObject = unitsObject.value("Enemies").toObject();
    for(QString enemy : enemiesObject.keys()){
        m_enemiesNames << enemy;
        QJsonObject enemyUnit = enemiesObject.value(enemy).toObject();
        QHash<QString, uint16_t> characteristics;
        for(QString characteristic : enemyUnit.keys()){
            if(((characteristic == "AttackableUnits"))){
                QStringList attackableUnitsList;
                QJsonArray attackableUnitsArray = enemyUnit[characteristic].toArray();
                for(int i = 0; i < attackableUnitsArray.size(); i++){
                    attackableUnitsList += attackableUnitsArray.at(i).toString();
                }
                m_attackableUnits[enemy] = attackableUnitsList;
            }  else {
                characteristics[characteristic] = enemyUnit[characteristic].toInt();
            }
        }
        m_enemyCharacteristics[enemy] = characteristics;
    }

    QJsonObject wavesObject = jsonObject.value("Waves").toObject();
    m_timeBetweenWaves = wavesObject["SecondsBetweenWaves"].toInt();

    QJsonObject enemyWavesObject = wavesObject.value("EnemyWaves").toObject();
    for(QString key : enemyWavesObject.keys()){
        QJsonArray waveEnemiesArray = enemyWavesObject[key].toArray();
        QStringList waveEnemiesList;
        for(int i = 0; i < waveEnemiesArray.size(); i++){
            waveEnemiesList += waveEnemiesArray.at(i).toString();
        }
        m_enemyWaves.append(waveEnemiesList);
    }

    QJsonObject spawnObject = jsonObject.value("Spawns").toObject();
    for(QString key : spawnObject.keys()){
        QPair<uint16_t,uint16_t> position;
        QJsonObject spawn = spawnObject.value(key).toObject();
        position.first = spawn.value("x").toInt();
        position.second = spawn.value("y").toInt();
        m_spawnsPosition[key] = position;
    }
    m_spawnLife = jsonObject.value("AllySpawnLife").toInt();
}

QPair<uint16_t, uint16_t> Settings::getSpawnPosition(QString spawnName) const{
    return m_spawnsPosition[spawnName];
}

QPair<uint16_t, uint16_t> Settings::getMapSize() const{
    return m_mapSize;
}

uint16_t Settings::getSpawnLife() const{
    return m_spawnLife;
}

uint16_t Settings::getAvailableFood() const{
    return m_availableFood;
}

uint16_t Settings::getTimeBetweenWaves() const{
    return m_timeBetweenWaves;
}

uint16_t Settings::getAllyFoodCost(QString unitName) const{
    return m_alliesFoodCost[unitName];
}

uint16_t Settings::getEnemyFoodDrop(QString unitName) const{
    return m_enemiesFoodDrop[unitName];
}

QHash<QString, uint16_t> Settings::getAllyUnitSettings(QString unitName) const{
    return m_allyCharacteristics[unitName];
}

QHash<QString, uint16_t> Settings::getEnemyUnitSettings(QString unitName) const{
    return m_enemyCharacteristics[unitName];
}

QStringList Settings::getAttackableUnits(QString unitName) const{
    return m_attackableUnits[unitName];
}

QStringList Settings::getAttackableResources(QString unitName) const{
    return m_attackableResources[unitName];
}

QStringList Settings::getEnemyWave(uint8_t waveNumber) const{
    return m_enemyWaves[waveNumber];
}

QStringList Settings::getAlliesNames() const{
    return m_alliesNames;
}

QStringList Settings::getEnemiesNames() const{
    return m_enemiesNames;
}
uint8_t Settings::getEnemyWaveSize() const{
    return m_enemyWaves.size();
}

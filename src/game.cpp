#include "include/game.h"
#include "include/display.h"
#include "include/map.h"
#include "include/Element/Unit/AllyUnit/worker.h"
#include "include/Element/Unit/AllyUnit/antiair.h"
#include "include/Element/Unit/AllyUnit/warrior.h"
#include "include/Element/Unit/AllyUnit/bomber.h"
#include "include/Element/Unit/EnemyUnit/assassin.h"
#include "include/Element/Unit/EnemyUnit/basic.h"
#include "include/Element/Unit/EnemyUnit/boss.h"
#include "include/Element/Unit/EnemyUnit/kamikaze.h"
#include <QDebug>
#include <QTimer>

Game::Game(Map *map, Display *display):m_selectedUnit("Worker"),QWidget(NULL) {
    m_pDisplay = display;
    m_pMap = map;
}
Game::~Game(){
    delete m_pDisplay,m_pMap,m_waveTimer,m_gameTimer;
}
bool Game::isGameFinished(){
    return m_isGameFinished;
}
/**
 * @brief Game::startGameLoop
 * This method is triggered by the Display when all sprites are initialized
 * Two timers are initialized, one for the gameLoop (every 16ms -> 60fps) and one for the waveTimer
 */
void Game::startGameLoop(){
    m_gameTimer = new QTimer(this);
    connect(m_gameTimer, SIGNAL(timeout()),this,SLOT(gameLoop()));
    m_gameTimer->start(16);

    m_waveTimer = new QTimer(this);
    connect(m_waveTimer, SIGNAL(timeout()),this,SLOT(spawnNextEnemyWave()));
    m_waveTimer->start(m_pMap->getSettings().getTimeBetweenWaves()*1000);
}


/**
 * @brief Game::gameLoop
 * This method retrieve the following data Allies - Ennemies - Building - Settings from the Map
 * It then iterate over all the Units array and make them move or attack
 * After each iteration, some units are removed (dead one, returning to the base)
 * Available food, units etc are also updated
 * @return void
 */
void Game::gameLoop(){

    QHash<QString, QList<QPair<uint16_t, uint16_t>>> pheromoneTrails = m_pMap->getPheromoneTrails();
    QHash<QString, QList<AbstractUnit*>> alliesArray = m_pMap->getAlliesArray();
    QHash<QString, QList<AbstractUnit*>> enemiesArray = m_pMap->getEnemiesArray();
    QList<Resource*> ressourcesArray = m_pMap->getResourcesArray();
    Building* allySpawn = m_pMap->getAllySpawn();
    uint16_t food = m_pMap->getFood();
    Settings settings = m_pMap->getSettings();

    for(QString allyUnit : alliesArray.keys()){
        for(int i = 0; i < alliesArray[allyUnit].size(); i++){
            AllyUnit* ally = dynamic_cast<AllyUnit*>(alliesArray[allyUnit][i]);
            ally->updateUnits(enemiesArray,pheromoneTrails,ressourcesArray,settings.getSpawnPosition("Ally"));
            if(ally->getIsJobDone()){
                if(allyUnit == "Worker" && ally->hasFoundResources()) food += 10;
                alliesArray[allyUnit].removeAt(i);
                collectUnitInBase(allyUnit);
            }

            for(QString enemyUnit : enemiesArray.keys()){
                for(int i = 0; i < enemiesArray[enemyUnit].size(); i++){
                    if(enemiesArray[enemyUnit][i]->isDead()){
                        enemiesArray[enemyUnit].removeAt(i);
                        food += settings.getEnemyFoodDrop(enemyUnit);
                    }
                }
            }

            for(int i = 0; i < ressourcesArray.size(); i++){
                if(ressourcesArray[i]->isDead()){
                    ressourcesArray.removeAt(i);
                }
            }
        }
        if(m_lastWave){
            bool allEnemiesDead = true;
            for(QString enemyUnit : enemiesArray.keys()){
                if(!enemiesArray[enemyUnit].empty()){
                    allEnemiesDead = false;
                }
            }
            if(allEnemiesDead){
                m_gameTimer->stop();
                m_pDisplay->showEndScreen(true);
                m_isGameFinished = true;
                return;
            }
        }

        for(QString enemyUnit : enemiesArray.keys()){
            for(int i = 0; i < enemiesArray[enemyUnit].size(); i++){
                EnemyUnit* enemy = dynamic_cast<EnemyUnit*>(enemiesArray[enemyUnit][i]);
                enemy->updateUnits(alliesArray,allySpawn);

                for(QString allyUnit : alliesArray.keys()){
                    for(int i = 0; i < alliesArray[allyUnit].size(); i++){
                        if(alliesArray[allyUnit][i]->isDead()){
                            alliesArray[allyUnit].removeAt(i);
                        }
                    }
                }
                if(allySpawn->isDead()){
                    m_gameTimer->stop();
                    m_pDisplay->showEndScreen(false);
                    m_isGameFinished = true;
                    return;
                }
            }
        }

        m_pMap->setAlliesArray(alliesArray);
        m_pMap->setEnemiesArray(enemiesArray);
        m_pMap->setResourcesArray(ressourcesArray);
        m_pMap->setAllySpawn(allySpawn);
        m_pMap->setFood(food);
        refreshDisplay();
    }
}
/**
 * @brief Game::addAllyUnitOnBoard
 * This methods adds an AllyUnit in the Map
 * An unit can only be added if it has a path (except for the Bomber who don't need a path but a targetPosition)
 * and is avaible in the base
 * If the previous conditions are true, this methods retrieve all the corresponding unit settings and instanciate this unit
 * @param targetPosition is mainly used in order to create a Bomber unit, otherwise it has a default value
 * @return void
 */
void Game::addAllyUnitOnBoard(QPair<uint16_t, uint16_t> targetPosition){

    QHash<QString, QList<QPair<uint16_t, uint16_t>>> pheromoneTrails =  m_pMap->getPheromoneTrails();
    if(pheromoneTrails[m_selectedUnit].size() > 0 || m_selectedUnit == "Bomber"){
        QHash<QString,uint16_t> availableUnits = m_pMap->getAvailableUnits();
        if(availableUnits[m_selectedUnit] > 0){
            qDebug().noquote().nospace() << m_selectedUnit << " added on board";
            availableUnits[m_selectedUnit] -= 1;
            QHash<QString, QList<AbstractUnit*>> alliesArray = m_pMap->getAlliesArray();
            Settings settings = m_pMap->getSettings();
            QPair<uint16_t,uint16_t> allySpawnPosition = settings.getSpawnPosition("Ally");
            QHash<QString, uint16_t> unitSettings = settings.getAllyUnitSettings(m_selectedUnit);
            uint8_t life = unitSettings["Life"];
            uint8_t moveSpeed = unitSettings["MoveSpeed"];
            uint8_t attackDamage = unitSettings["AttackDamage"];
            uint8_t viewBox = unitSettings["ViewBox"];
            QStringList attackableUnits = settings.getAttackableUnits(m_selectedUnit);
            QStringList attackableResources = settings.getAttackableResources(m_selectedUnit);
            if(m_selectedUnit == "Worker"){
                alliesArray[m_selectedUnit].append(new Worker(allySpawnPosition,life,moveSpeed,attackDamage,viewBox,attackableUnits,attackableResources));
            } else if(m_selectedUnit == "AntiAir"){
                alliesArray[m_selectedUnit].append(new AntiAir(allySpawnPosition,life,moveSpeed,attackDamage,viewBox,attackableUnits,attackableResources));
            } else if(m_selectedUnit == "Bomber"){
                alliesArray[m_selectedUnit].append(new Bomber(allySpawnPosition,life,moveSpeed,attackDamage,viewBox,attackableUnits,attackableResources,targetPosition));
            } else if(m_selectedUnit == "Warrior"){
                alliesArray[m_selectedUnit].append(new Warrior(allySpawnPosition,life,moveSpeed,attackDamage,viewBox,attackableUnits,attackableResources));
            }
            m_pMap->setAvailableUnits(availableUnits);
            m_pMap->setAlliesArray(alliesArray);
        } else {
            qDebug().noquote().nospace() << "No units available";
        }
    } else {
        qDebug().noquote().nospace() << "No trails";
    }
}

/**
 * @brief Game::addAvailableUnits
 * This methods add units in the base if the available food is enough
 */
void Game::addAvailableUnits(){
    Settings settings = m_pMap->getSettings();
    QHash<QString,uint16_t> availableUnits = m_pMap->getAvailableUnits();
    uint8_t unitCost = settings.getAllyFoodCost(m_selectedUnit);
    uint16_t food = m_pMap->getFood();
    if(unitCost <= food) {
        qDebug().noquote().nospace() << m_selectedUnit << " added to available units";
        food -= unitCost;
        availableUnits[m_selectedUnit] += 1;
        m_pMap->setFood(food);
        m_pMap->setAvailableUnits(availableUnits);
    } else {
        qDebug().noquote().nospace() << "Not enough food";
    }
}
/**
 * @brief Game::addPheromoneTrails
 * This method add a point to the selected unit's pheromone trails
 * @param pheromone is a new point
 * @return void
 */
void Game::addPheromoneTrails(QPair<uint16_t, uint16_t> pheromone){
    QHash<QString, QList<QPair<uint16_t, uint16_t>>> pheromoneTrails =  m_pMap->getPheromoneTrails();
    pheromoneTrails[m_selectedUnit].append(pheromone);
    m_pMap->setPheromoneTrails(pheromoneTrails);

}

void Game::collectUnitInBase(QString unitName){
    QHash<QString,uint16_t> availableUnits = m_pMap->getAvailableUnits();
    availableUnits[unitName] += 1;
    m_pMap->setAvailableUnits(availableUnits);
}

void Game::removePheromoneTrails(){
    QHash<QString, QList<AbstractUnit*>> alliesArray = m_pMap->getAlliesArray();
    for(int i = 0; i < alliesArray[m_selectedUnit].size(); i++){
        collectUnitInBase(m_selectedUnit);
    }
    alliesArray[m_selectedUnit].clear();

    m_pMap->setAlliesArray(alliesArray);
    QHash<QString, QList<QPair<uint16_t, uint16_t>>> pheromoneTrails =  m_pMap->getPheromoneTrails();
    pheromoneTrails[m_selectedUnit].clear();
    m_pMap->setPheromoneTrails(pheromoneTrails);
}

void Game::setSelectedUnit(QString unitName){
    qDebug().noquote().nospace() << "Selected Unit has been changed to : "<< unitName;
    m_selectedUnit = unitName;
}

void Game::spawnNextEnemyWave(){
    Settings settings = m_pMap->getSettings();
    QHash<QString, QList<AbstractUnit*>> enemiesArray = m_pMap->getEnemiesArray();
    uint8_t waveNumber = m_pMap->getWaveNumber();
    QStringList waveList = settings.getEnemyWave(waveNumber);
    for(QString enemyUnitName : waveList){
        QPair<uint16_t,uint16_t> enemySpawnPosition = settings.getSpawnPosition("Enemy");
        enemySpawnPosition.first -= rand() % 800;
        enemySpawnPosition.second += rand() % 1000;
        QHash<QString, uint16_t> unitSettings = settings.getEnemyUnitSettings(enemyUnitName);
        uint8_t life = unitSettings["Life"];
        uint8_t moveSpeed = unitSettings["MoveSpeed"];
        uint8_t attackDamage = unitSettings["AttackDamage"];
        uint8_t viewBox = unitSettings["ViewBox"];
        QStringList attackableUnits = settings.getAttackableUnits(enemyUnitName);
        if(enemyUnitName == "Assassin"){
            enemiesArray[enemyUnitName].append(new Assassin(enemySpawnPosition,life,moveSpeed,attackDamage,viewBox,attackableUnits));
        } else if(enemyUnitName == "Basic"){
            enemiesArray[enemyUnitName].append(new Basic(enemySpawnPosition,life,moveSpeed,attackDamage,viewBox,attackableUnits));
        } else if(enemyUnitName == "Boss"){
            enemiesArray[enemyUnitName].append(new Boss(enemySpawnPosition,life,moveSpeed,attackDamage,viewBox,attackableUnits));
        } else if(enemyUnitName == "Kamikaze"){
            enemiesArray[enemyUnitName].append(new Kamikaze(enemySpawnPosition,life,moveSpeed,attackDamage,viewBox,attackableUnits));
        }
    }
    waveNumber += 1;
    m_pMap->setWaveNumber(waveNumber);
    m_pMap->setEnemiesArray(enemiesArray);
    if(waveNumber >= settings.getEnemyWaveSize()){
        m_waveTimer->stop();
        m_lastWave = true;
    }
}

void Game::refreshDisplay(){
    QHash<QString, QList<QPair<uint16_t, uint16_t>>> pheromoneTrails = m_pMap->getPheromoneTrails();
    QHash<QString, QList<AbstractUnit*>> alliesArray = m_pMap->getAlliesArray();
    QHash<QString, QList<AbstractUnit*>> enemiesArray = m_pMap->getEnemiesArray();
    QList<Resource*> ressourcesArray = m_pMap->getResourcesArray();
    m_pDisplay->refreshScene(pheromoneTrails,alliesArray,enemiesArray,ressourcesArray);
}

QString Game::getSelectedUnit() const{
    return m_selectedUnit;
}

QHash<QString,QPair<uint16_t,uint16_t>> Game::getSpawnsPosition() const{
    Settings settings = m_pMap->getSettings();
    QHash<QString,QPair<uint16_t,uint16_t>> positions;
    positions["Ally"] = settings.getSpawnPosition("Ally");
    positions["Enemy"] = settings.getSpawnPosition("Enemy");
    return positions;
}

void Game::acceptDisplay(Display *disp){
    disp->updateHUD(m_pMap);
}

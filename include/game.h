#pragma once

#include <stdint.h>
#include <QHash>
#include <QWidget>

class Display;
class Map;

/**
 * @brief The Game class
 * This class correspond to the Controller
 * All the game logic / units manipulation happens here
 * According to a certain frequency, the Map is updated and the Display refreshed
 * It also manage all the User Input triggered in the Display
 */

class Game : public QWidget{
    Q_OBJECT
public slots:
    void gameLoop();
    void spawnNextEnemyWave();

public:
    Game(Map *map, Display *display);
    ~Game();
    void addAllyUnitOnBoard(QPair<uint16_t, uint16_t> targetPosition = QPair<uint16_t,uint16_t>());
    void addAvailableUnits();
    void addPheromoneTrails(QPair<uint16_t, uint16_t> pheromone);
    void collectUnitInBase(QString unitName);
    void removePheromoneTrails();
    void setSelectedUnit(QString unitName);
    QString getSelectedUnit() const;
    void refreshDisplay();
    QHash<QString,QPair<uint16_t,uint16_t>> getSpawnsPosition() const;
    void startGameLoop();
    void acceptDisplay(Display *disp);
    bool isGameFinished();
private:
    Map *m_pMap;
    Display *m_pDisplay;
    QString m_selectedUnit;
    bool m_lastWave = false;
    bool m_isGameFinished = false;
    QTimer *m_waveTimer;
    QTimer *m_gameTimer;

};



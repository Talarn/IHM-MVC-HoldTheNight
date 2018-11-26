#pragma once

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QPixmap>
#include <QHash>
#include <QEvent>
#include <QMouseEvent>
#include <QGroupBox>
#include <QRadioButton>
#include <QGraphicsProxyWidget>
#include <QDebug>
#include <QPixmap>
#include <QPushButton>
#include <QGraphicsItemGroup>
#include <QGraphicsTextItem>
#include <QGraphicsItem>
#include <QSoundEffect>
#include <QMediaPlayer>
#include <QMediaPlaylist>

class Game;
class AbstractUnit;
class Map;
class Resource;

namespace Ui {
class Display;
}
/**
 * @brief The Display class
 * This class correspond to the View / User Interface
 * It will initialize all the assets / sprites and display them on a QGraphicsView
 * This class can communicate with the Game class in order to display all the Units, Building and Resource stored in the Map class
 */
class Display : public QMainWindow {

    Q_OBJECT

public slots:
    void changeSelectedUnit();
    void toolsTriggered();

public:
    void mousePressEvent(QMouseEvent* event);
    void keyPressEvent(QKeyEvent* event);
    explicit Display(QWidget *parent = 0);
    ~Display();

    void setControl(Game *game);
    void initializeMusic();
    void initializeUnitSelectionArea();
    void initializeUnitToolsArea();
    void initializeUnitsSprite();
    void initializePheromoneTrailsSprite();
    void initializeBackground();
    void initializeScene();

    void removeUnitsSprite();
    void removePheromoneTrailsSprite();
    void removeText();
    void updateAllyUnits(const QHash<QString, QList<AbstractUnit*>> &alliesArray);
    void updateEnemyUnits(const QHash<QString, QList<AbstractUnit*>> &enemiesArray);
    void updatePheromoneTrails(const QHash<QString, QList<QPair<uint16_t, uint16_t>>>& pheromoneTrails);
    void updateResource(const QList<Resource*>& ressources);
    void updateHUD(Map *m);
    void refreshScene(const QHash<QString, QList<QPair<uint16_t, uint16_t>>>& pheromoneTrails,
                      const QHash<QString, QList<AbstractUnit*>>& alliesArray,
                      const QHash<QString, QList<AbstractUnit*>>& enemiesArray,
                      const QList<Resource*>& ressources);
    void showEndScreen(bool hasWin);

private:
    Ui::Display *ui;
    Game *m_pGame;
    QGraphicsScene *m_pScene;

    QGraphicsPixmapItem *m_pBackgroundImage;
    QGraphicsPixmapItem *m_pTestSprite;
    QList<QGraphicsTextItem*> m_tempTextArray;
    QList<QGraphicsPixmapItem*> m_tempUnitSpriteArray;
    QList<QGraphicsPixmapItem*> m_tempTrailsSpriteArray;
    QVector<QRadioButton*> m_radioButtonArray;
    QGroupBox *m_pRadioButtonBox;
    QImage m_gameOverSprite;
    QImage m_winSprite;
    QHash<QString, QImage> m_unitsSprites;
    QHash<QString, QImage> m_trailsSprites;
    QImage m_resourceSprite;
    QSoundEffect m_buyEffect;
    QSoundEffect m_spawnEffect;
    QSoundEffect m_deleteEffect;
    QSoundEffect m_chooseEffect;

    QMediaPlayer *m_pMusic;
    QMediaPlaylist *m_pPlaylist;
};



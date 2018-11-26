#include "include/display.h"
#include "ui_display.h"
#include "include/game.h"
#include "include/Element/Unit/abstractunit.h"
#include "include/map.h"
#include <QMediaPlayer>
#include <QVector>

Display::Display(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Display) {
    ui->setupUi(this);
    setWindowState(Qt::WindowFullScreen);
    setWindowIcon(QIcon(":/images/screen/Images/icon.png"));
}

void Display::initializeMusic(){

    m_buyEffect.setSource(QUrl("qrc:/sounds/Sounds/buy.wav"));
    m_buyEffect.setVolume(0.25f);
    m_spawnEffect.setSource(QUrl("qrc:/sounds/Sounds/spawn.wav"));
    m_spawnEffect.setVolume(0.25f);
    m_deleteEffect.setSource(QUrl("qrc:/sounds/Sounds/delete.wav"));
    m_deleteEffect.setVolume(0.25f);
    m_chooseEffect.setSource(QUrl("qrc:/sounds/Sounds/choose.wav"));
    m_chooseEffect.setVolume(0.25f);

    m_pPlaylist = new QMediaPlaylist();
    m_pPlaylist->addMedia(QUrl("qrc:/sounds/Sounds/music.mp3"));
    m_pPlaylist->setPlaybackMode(QMediaPlaylist::Loop);

    m_pMusic = new QMediaPlayer();
    m_pMusic->setPlaylist(m_pPlaylist);
    m_pMusic->play();
}

void Display::showEndScreen(bool hasWin){
    m_pMusic->stop();
    m_pScene->clear();
    if(!hasWin){
        QGraphicsPixmapItem* gameOver = new QGraphicsPixmapItem(QPixmap::fromImage(m_gameOverSprite));
        m_pScene->addItem(gameOver);
        gameOver->setPos(0, 0);

    } else {
        QGraphicsPixmapItem* win = new QGraphicsPixmapItem(QPixmap::fromImage(m_winSprite));
        m_pScene->addItem(win);
        win->setPos(0, 0);
    }
    ui->graphicsView->repaint();
}

void Display::setControl(Game *game) {
    m_pGame = game;
    initializeMusic();
    initializeScene();
    initializeBackground();
    initializeUnitsSprite();
    initializePheromoneTrailsSprite();
    initializeUnitSelectionArea();
    initializeUnitToolsArea();
    ui->graphicsView->setScene(m_pScene);
    m_pGame->startGameLoop();
}

void Display::updateHUD(Map *m){
    QFont font("Times", 10, QFont::Bold);
    QGraphicsTextItem *spawnHP = m_pScene->addText("Spawn Life : " + QString::number(m->getAllySpawn()->getLife()));
    spawnHP->setPos(this->width() * 0.62, this->height() * 0.94);
    m_tempTextArray.append(spawnHP);
    QGraphicsTextItem *foodAvailable = m_pScene->addText("Food Available : " + QString::number(m->getFood()));
    foodAvailable->setPos(this->width() * 0.62, this->height() * 0.96);
    m_tempTextArray.append(foodAvailable);

    Settings s = m->getSettings();
    QGraphicsTextItem *waveNumberText = m_pScene->addText("Wave : " + QString::number(m->getWaveNumber()) + "/" +
                                                          QString::number(s.getEnemyWaveSize()));
    waveNumberText->setPos(this->width() * 0.62, this->height() * 0.92);
    m_tempTextArray.append(waveNumberText);

    QHash<QString,uint16_t> availableUnits = m->getAvailableUnits();
    QGraphicsTextItem *workerText = m_pScene->addText(QString::number(availableUnits["Worker"]));
    m_tempTextArray.append(workerText);
    QGraphicsTextItem *warriorText = m_pScene->addText(QString::number(availableUnits["Warrior"]));
    m_tempTextArray.append(warriorText);
    QGraphicsTextItem *bomberText = m_pScene->addText(QString::number(availableUnits["Bomber"]));
    m_tempTextArray.append(bomberText);
    QGraphicsTextItem *antiairText = m_pScene->addText(QString::number(availableUnits["AntiAir"]));
    m_tempTextArray.append(antiairText);

    int a = 0;
    for(int i = m_tempTextArray.size() - 4; i < m_tempTextArray.size(); i++){
        m_tempTextArray[i]->setPos(this->width() * 0.44 + (a*65), this->height()*0.92);
        a++;
    }
    for(int i = 0; i < m_tempTextArray.size(); i++){
        m_tempTextArray[i]->setFont(font);
    }
}

void Display::initializeUnitToolsArea() {
    QStringList unitToolsNames;
    unitToolsNames << "Spawn" << "Buy" << "Delete";

    for(int i = 0; i < unitToolsNames.size(); i++){
        QPushButton *toolButton = new QPushButton(unitToolsNames[i]);
        connect(toolButton,SIGNAL(clicked(bool)),this,SLOT(toolsTriggered()));
        toolButton->setAccessibleName(unitToolsNames[i]);

        QGraphicsProxyWidget *proxy = m_pScene->addWidget(toolButton);
        proxy->setPos(this->width() * 0.5 + m_pRadioButtonBox->width() * 0.5 + 10, this->height() * 0.92 + i * toolButton->height());

        QPixmap* pixmap = new QPixmap(":/images/button/Images/" + unitToolsNames[i] + "Button.png");
        pixmap->scaled(proxy->widget()->width(),proxy->widget()->height());
        QIcon icon(*pixmap);
        toolButton->setIcon(icon);

    }
}

void Display::initializeUnitSelectionArea(){
    QStringList unitNames;
    unitNames << "Worker" << "Warrior" << "Bomber" << "AntiAir";
    m_pRadioButtonBox = new QGroupBox;
    QHBoxLayout *radioButtonBoxLayout = new QHBoxLayout;
    for(int i = 0; i < unitNames.size(); i++){
        QRadioButton *unitRadioButton = new QRadioButton();
        connect(unitRadioButton, SIGNAL(toggled(bool)), this, SLOT(changeSelectedUnit()));
        unitRadioButton->setAccessibleName(unitNames[i]);
        if(unitNames[i] == "Worker") {
            unitRadioButton->setChecked(true);
        }
        QPixmap* pixmap = new QPixmap(":/images/button/Images/" + unitNames[i] + "Button.png");
        QIcon icon(*pixmap);
        unitRadioButton->setIcon(icon);
        radioButtonBoxLayout->addWidget(unitRadioButton);
        m_radioButtonArray.append(unitRadioButton);

    }
    m_pRadioButtonBox->setLayout(radioButtonBoxLayout);
    QGraphicsProxyWidget *proxy = m_pScene->addWidget(m_pRadioButtonBox);
    proxy->setPos((this->width() * 0.5) - (m_pRadioButtonBox->width()*0.5),this->height() - (m_pRadioButtonBox->height()));

}

void Display::changeSelectedUnit(){
    QRadioButton *button = dynamic_cast<QRadioButton*>(sender());
    QString unitName = button->accessibleName();
    m_pGame->setSelectedUnit(unitName);
    m_chooseEffect.play();

}

Display::~Display(){
    delete ui,m_pGame,m_pScene,m_pBackgroundImage,m_pTestSprite,m_pRadioButtonBox;
}

void Display::refreshScene(const QHash<QString, QList<QPair<uint16_t, uint16_t>>>& pheromoneTrails,
                           const QHash<QString, QList<AbstractUnit*>>& alliesArray,
                           const QHash<QString, QList<AbstractUnit*>>& enemiesArray, const QList<Resource*>& resources){
    if(!m_pGame->isGameFinished()){

        removePheromoneTrailsSprite();
        removeUnitsSprite();
        removeText();

        updateAllyUnits(alliesArray);
        updateEnemyUnits(enemiesArray);
        updatePheromoneTrails(pheromoneTrails);
        updateResource(resources);

        m_pGame->acceptDisplay(this);
        ui->graphicsView->repaint();
    }
}

void Display::mousePressEvent(QMouseEvent* event){
    if(!m_pGame->isGameFinished()){
        if(m_pRadioButtonBox->geometry().contains(event->pos())) return;
        QPair<uint16_t,uint16_t> pheromone;
        pheromone.first = event->globalX();
        pheromone.second = event->globalY();
        if(m_pGame->getSelectedUnit() == "Bomber"){
            m_pGame->addAllyUnitOnBoard(pheromone);
        } else{
            m_pGame->addPheromoneTrails(pheromone);
        }
    } else {
        QApplication::quit();
    }
}

void Display::keyPressEvent(QKeyEvent* event){
    switch(event->key()){
    case Qt::Key_Escape:
        QApplication::quit();
        break;
    case Qt::Key_A:
        m_radioButtonArray[0]->setChecked(true);
        break;
    case Qt::Key_Z:
        m_radioButtonArray[1]->setChecked(true);
        break;
    case Qt::Key_E:
        m_radioButtonArray[2]->setChecked(true);
        break;
    case Qt::Key_R:
        m_radioButtonArray[3]->setChecked(true);
        break;
    }
}

void Display::toolsTriggered(){
    QPushButton *button = dynamic_cast<QPushButton*>(sender());
    QString toolPressed = button->accessibleName();
    if(toolPressed == "Spawn"){
        if(m_pGame->getSelectedUnit() != "Bomber"){
            m_pGame->addAllyUnitOnBoard();
            m_spawnEffect.play();
        }
    }else if(toolPressed == "Buy"){
        m_pGame->addAvailableUnits();
        m_buyEffect.play();
    }else if(toolPressed == "Delete"){
        m_pGame->removePheromoneTrails();
        m_deleteEffect.play();
    }
}

void Display::removeUnitsSprite(){
    for(int i = 0; i < m_tempUnitSpriteArray.size(); i++){
        m_pScene->removeItem(m_tempUnitSpriteArray[i]);
    }
    m_tempUnitSpriteArray.clear();
}

void Display::removePheromoneTrailsSprite(){
    for(int i = 0; i < m_tempTrailsSpriteArray.size(); i++){
        m_pScene->removeItem(m_tempTrailsSpriteArray[i]);
    }
    m_tempTrailsSpriteArray.clear();
}

void Display::removeText(){
    for(int i = 0; i < m_tempTextArray.size(); i++){
        m_pScene->removeItem(m_tempTextArray[i]);
    }
    m_tempTextArray.clear();
}

void Display::updateAllyUnits(const QHash<QString, QList<AbstractUnit*>>& alliesArray){
    for(QString key : alliesArray.keys()){
        QList<AbstractUnit*> unitList = alliesArray[key];
        for(AbstractUnit* unit : unitList){
            QGraphicsPixmapItem *test = new QGraphicsPixmapItem(QPixmap::fromImage(m_unitsSprites[key]));
            m_pScene->addItem(test);
            test->setPos(unit->getPosition().first - (m_unitsSprites[key].width()/2), unit->getPosition().second - (m_unitsSprites[key].height()/2));
            test->setZValue(2);
            if(!unit->isGoingLeft()){
                test->setTransform(QTransform::fromScale(-1,1));
                test->setOffset( - m_unitsSprites[key].width(),0);
            }
            m_tempUnitSpriteArray.append(test);
        }
    }
}

void Display::updateResource(const QList<Resource*>& resources){
    for(int i = 0; i < resources.size(); i++){
        QGraphicsPixmapItem *test = new QGraphicsPixmapItem(QPixmap::fromImage(m_resourceSprite));
        m_pScene->addItem(test);
        test->setPos(600,600);
        m_tempUnitSpriteArray.append(test);
    }
}

void Display::updateEnemyUnits(const QHash<QString, QList<AbstractUnit*>> &enemiesArray){
    for(QString key : enemiesArray.keys()){
        QList<AbstractUnit*> unitList = enemiesArray[key];
        for(AbstractUnit* unit : unitList){
            QGraphicsPixmapItem *test = new QGraphicsPixmapItem(QPixmap::fromImage(m_unitsSprites[key]));
            m_pScene->addItem(test);
            test->setPos(unit->getPosition().first - (m_unitsSprites[key].width()/2), unit->getPosition().second - (m_unitsSprites[key].height()/2));
            test->setZValue(2);
            m_tempUnitSpriteArray.append(test);
        }
    }
}

void Display::updatePheromoneTrails(const QHash<QString, QList<QPair<uint16_t, uint16_t>>>& pheromoneTrails){
    for(QString key : pheromoneTrails.keys()){
        QList<QPair<uint16_t, uint16_t>> pheromoneList = pheromoneTrails[key];
        for(int i = 0; i < pheromoneList.size(); i++){
            QGraphicsPixmapItem *test = new QGraphicsPixmapItem(QPixmap::fromImage(m_trailsSprites[key]));
            m_pScene->addItem(test);
            test->setPos(pheromoneList[i].first - (m_trailsSprites[key].width()/2),pheromoneList[i].second - (m_trailsSprites[key].height()/2));
            m_tempTrailsSpriteArray.append(test);
        }
    }
}

void Display::initializeUnitsSprite() {
    m_unitsSprites["Worker"] = QImage(":/images/unitSprite/Images/worker.png").scaled(50,50);
    m_unitsSprites["AntiAir"] = QImage(":/images/unitSprite/Images/antiAir.png").scaled(50,50);
    m_unitsSprites["Warrior"] = QImage(":/images/unitSprite/Images/warrior.png").scaled(50,50);
    m_unitsSprites["Bomber"] = QImage(":/images/unitSprite/Images/bomber.png").scaled(50,50);

    m_unitsSprites["Assassin"] = QImage(":/images/unitSprite/Images/assassin.png").scaled(50,50);
    m_unitsSprites["Boss"] = QImage(":/images/unitSprite/Images/boss.png").scaled(100,100);
    m_unitsSprites["Kamikaze"] = QImage(":/images/unitSprite/Images/kamikaze.png").scaled(50,50);
    m_unitsSprites["Basic"] = QImage(":/images/unitSprite/Images/basic.png").scaled(50,50);

    m_resourceSprite = QImage(":/images/screen/Images/mine.png").scaled(100,100);
}

void Display::initializePheromoneTrailsSprite(){
    m_trailsSprites["Worker"] = QImage(":/images/trails/Images/workerTrails.png").scaled(10,10);
    m_trailsSprites["AntiAir"] = QImage(":/images/trails/Images/antiairTrails.png").scaled(10,10);
    m_trailsSprites["Warrior"] = QImage(":/images/trails/Images/warriorTrails.png").scaled(10,10);
}

void Display::initializeBackground(){

    QImage image(":/images/screen/Images/map.png");
    image = image.scaled(this->width(), this->height());
    m_pBackgroundImage = new QGraphicsPixmapItem(QPixmap::fromImage(image));

    m_pScene->addItem(m_pBackgroundImage);
    m_pBackgroundImage->setPos(0, 0);
    m_pBackgroundImage->setZValue(-1);

    m_gameOverSprite = QImage(":/images/screen/Images/lose.png");
    m_gameOverSprite = m_gameOverSprite.scaled(this->width(),this->height());
    m_winSprite = QImage(":/images/screen/Images/win.png");
    m_winSprite = m_winSprite.scaled(this->width(),this->height());
}

void Display::initializeScene() {
    m_pScene = new QGraphicsScene();
    m_pScene->setSceneRect(0, 0, this->width(), this->height());
}



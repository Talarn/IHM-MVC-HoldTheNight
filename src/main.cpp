#include "include/display.h"
#include <QApplication>
#include <QMessageBox>
#include <QTextCodec>
#include <QDialog>
#include "include/map.h"
#include "include/game.h"
#include <cstdlib>
#include <ctime>
#include <time.h>
#include <QMediaPlayer>
#include <QIcon>
int launcher();

int main(int argc, char *argv[]){
    srand(time(NULL));
    QApplication a(argc, argv);
    if(launcher() == QMessageBox::Ok){
        Display *display = new Display();
        Map *map = new Map(":/settings/level.json");
        Game *game = new Game(map,display);
        display->setControl(game);
        display->show();
    }
    return a.exec();
}


int launcher(){
    QMessageBox titleScreen;
    QImage launcherImage = QImage(":/images/screen/Images/titre.png");
    QPixmap image = QPixmap::fromImage(launcherImage);
    titleScreen.setIconPixmap(image);
    titleScreen.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    titleScreen.setDefaultButton(QMessageBox::Ok);
    titleScreen.setWindowTitle("Hold The Night");
    titleScreen.setWindowIcon(QIcon(":/images/screen/Images/icon.png"));
    return titleScreen.exec();
}

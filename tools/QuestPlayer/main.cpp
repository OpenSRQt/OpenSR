#include "PlayerWindow.h"
#include <QApplication>
#include <ctime>

int main(int argc, char *argv[])
{
    srand(time(0));

    QApplication a(argc, argv);
    OpenSR::QuestPlayer::PlayerWindow w;
    w.show();

    return a.exec();
}

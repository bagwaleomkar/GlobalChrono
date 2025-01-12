#include <QApplication>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QPixmap>
#include <QTimer>
#include "pendulum.h"

int main(int argc, char* argv[]) {
    QApplication a(argc, argv);


    QGraphicsScene scene;
    QGraphicsView view(&scene);
    view.setRenderHint(QPainter::Antialiasing);
    view.setSceneRect(0, 0, 800, 600);


    QPixmap background("C:/Users/Lenonov/Downloads/MiniProject/MiniProject/pendulum/images/b.jpg");
    if (!background.isNull()) {
        scene.setBackgroundBrush(background);
    } else {
        qDebug() << "Failed to load background image.";
    }


    Pendulum pendulum(&scene);
    Clock clock(&scene);


    QTimer timer;
    QObject::connect(&timer, &QTimer::timeout, [&]() {
        pendulum.update(TIME_STEP);
        clock.updateClockHands();
    });
    timer.start(50);


    view.show();
    return a.exec();
}

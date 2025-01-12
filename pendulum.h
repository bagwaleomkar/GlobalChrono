#ifndef PENDULUM_H
#define PENDULUM_H

#include <QGraphicsScene>
#include <QGraphicsEllipseItem>
#include <QGraphicsLineItem>
#include <QObject>
#include <QPushButton>
#include <QTimer>
#include <QTime>


const float PI = 3.14159f;
const float GRAVITY = 9.81f;
const float LENGTH = 150.0f;
const float INITIAL_ANGLE = PI / 4;
const float TIME_STEP = 0.05f;

struct CityTime {
    QString cityName;
    int utcOffset;  // UTC offset in hours
};

class Pendulum : public QObject {
    Q_OBJECT
public:
    Pendulum(QGraphicsScene* scene);
    void update(float deltaTime);

private:
    float angle;
    float angularVelocity;
    float angularAcceleration;
    int originX, originY;
    QGraphicsLineItem* pendulumLine;
    QGraphicsEllipseItem* pendulumBob;
};

class Clock : public QObject {
    Q_OBJECT
public:
    Clock(QGraphicsScene* scene);
    void updateClockHands();
    void updateWorldClock();

private slots:
    void updateDigitalTime();  // Slot to update the time on the button

private:
    QGraphicsScene* scene;
    int centerX, centerY;
    QGraphicsLineItem* hourHand = nullptr;
    QGraphicsLineItem* minuteHand = nullptr;
    QGraphicsLineItem* secondHand = nullptr;
    QList<QGraphicsTextItem*> cityTexts;
    QList<CityTime> cities;



    QPushButton* digitalClockButton;  // Button to display time
    QTimer* timer;                    // Timer to update time every second
};


#endif // PENDULUM_H

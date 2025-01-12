#include "pendulum.h"
#include <QTime>
#include <cmath>
#include <QFont>
#include <QTimer>
#include <QGraphicsTextItem>
#include <QGraphicsProxyWidget>

#define GRAVITY 9.81
#define LENGTH 194.5
#define INITIAL_ANGLE 0.8
#define TIME_SCALE 0.492

Pendulum::Pendulum(QGraphicsScene* scene) : angle(INITIAL_ANGLE), angularVelocity(0), angularAcceleration(0) {
    originX = 400;
    originY = 150;

    pendulumLine = scene->addLine(originX, originY, originX + LENGTH * std::sin(angle), originY + LENGTH * std::cos(angle));
    pendulumBob = scene->addEllipse(originX + LENGTH * std::sin(angle) - 10, originY + LENGTH * std::cos(angle) - 10, 20, 20, QPen(Qt::red), QBrush(Qt::red));
}

void Pendulum::update(float deltaTime) {
    angularAcceleration = (-GRAVITY / LENGTH) * std::sin(angle);
    float scaledDeltaTime = deltaTime * TIME_SCALE;
    angularVelocity += angularAcceleration * scaledDeltaTime;
    angle += angularVelocity * scaledDeltaTime;

    int bobX = originX + LENGTH * std::sin(angle);
    int bobY = originY + LENGTH * std::cos(angle);

    pendulumLine->setLine(originX, originY, bobX, bobY);
    pendulumBob->setRect(bobX - 10, bobY - 10, 20, 20);
}

Clock::Clock(QGraphicsScene* scene) : scene(scene), centerX(400), centerY(150) {

    cities = {
        {"New York", -5},
        {"London", 0},
        {"Tokyo", 9},
        {"Sydney", 10},
        {"Paris", 1}
    };

    for (int i = 0; i < cities.size(); ++i) {
        int yPosition = centerY + 100 + i * 30;  // Space them vertically
        QGraphicsTextItem* cityText = scene->addText(cities[i].cityName + ": --:--:--", QFont("Arial", 12));
        cityText->setDefaultTextColor(Qt::white);
        cityText->setPos(centerX - 80, yPosition);
        // Adjust position to center
        cityTexts.append(cityText);
    }

    // Draw clock background
    scene->addEllipse(centerX - 100, centerY - 100, 200, 200, QPen(Qt::white), QBrush(Qt::lightGray));

    // Add numbers to clock
    int numberRadius = 90;
    for (int i = 1; i <= 12; ++i) {
        float angle = (i * 30 - 90) * (PI / 180);
        int x = centerX + numberRadius * std::cos(angle) - 10;
        int y = centerY + numberRadius * std::sin(angle) - 10;
        QGraphicsTextItem* numberText = scene->addText(QString::number(i), QFont("Arial", 12, QFont::Bold));
        numberText->setDefaultTextColor(Qt::black);
        numberText->setPos(x, y);
    }

    // Set up the digital clock button with retro red style
    digitalClockButton = new QPushButton("00:00:00");
    digitalClockButton->setStyleSheet(
        "QPushButton {"
        "   background-color: #b11226; "  // Retro red background
        "   color: white; "                // White text color
        "   font: bold 18px 'Courier New';" // Pixelated/retro font
        "   border-radius: 5px; "          // Rounded corners for button
        "   padding: 10px; "
        "} "
        "QPushButton:hover {"
        "   background-color: #e6002a; "  // Lighter red on hover
        "} "
        );

    // Add the button to the scene as a QGraphicsProxyWidget
    QGraphicsProxyWidget* proxyWidget = scene->addWidget(digitalClockButton);
    proxyWidget->setPos(10, 10);  // Set the position of the button

    // Set up timer to update digital clock every second
    QTimer* timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &Clock::updateDigitalTime);
    timer->start(1000);  // Update every 1000 ms (1 second)

    // Update clock hands every second
    QTimer* clockHandsTimer = new QTimer(this);
    connect(clockHandsTimer, &QTimer::timeout, this, &Clock::updateClockHands);
    clockHandsTimer->start(1000);  // Update every 1000 ms (1 second)

    // Add timer to update world clock
    QTimer* worldClockTimer = new QTimer(this);
    connect(worldClockTimer, &QTimer::timeout, this, &Clock::updateWorldClock);
    worldClockTimer->start(1000);  // Update every 1000 ms (1 second)

    updateClockHands();
}

void Clock::updateWorldClock() {
    // Get the current UTC time
    QDateTime utcDateTime = QDateTime::currentDateTimeUtc();
    QTime utcTime = utcDateTime.time();  // Extract UTC time from current UTC DateTime

    // Update the time for each city based on its UTC offset
    for (int i = 0; i < cities.size(); ++i) {
        int cityUtcOffset = cities[i].utcOffset;

        // Add the city’s UTC offset to the UTC time
        QTime cityTime = utcTime.addSecs(cityUtcOffset * 3600);  // Offset in seconds

        // Update the text of the city with the formatted time
       // cityTexts[i]->setPlainText("\n\n\n\n\n\n"+cities[i].cityName + ": " + cityTime.toString("hh:mm:ss"));
        cityTexts[i]->setHtml(QString("<br><br><br><br><br><br><br>") +
                              "<font color='red'>" + cities[i].cityName + "</font>: " +
                              cityTime.toString("hh:mm:ss"));



    }

}


void Clock::updateClockHands() {
    QTime currentTime = QTime::currentTime();
    int hours = currentTime.hour();
    int minutes = currentTime.minute();
    int seconds = currentTime.second();

    // Calculate angles for each hand
    float hourAngle = ((hours % 12) + minutes / 60.0) * (PI / 6);
    float minuteAngle = (minutes + seconds / 60.0) * (PI / 30);
    float secondAngle = seconds * (PI / 30);

    // Update hour, minute, and second hands
    if (hourHand != nullptr)
        scene->removeItem(hourHand);
    hourHand = scene->addLine(centerX, centerY, centerX + 50 * std::cos(hourAngle - PI / 2), centerY + 50 * std::sin(hourAngle - PI / 2), QPen(Qt::white, 4));

    if (minuteHand != nullptr)
        scene->removeItem(minuteHand);
    minuteHand = scene->addLine(centerX, centerY, centerX + 70 * std::cos(minuteAngle - PI / 2), centerY + 70 * std::sin(minuteAngle - PI / 2), QPen(Qt::white, 2));

    if (secondHand != nullptr)
        scene->removeItem(secondHand);
    secondHand = scene->addLine(centerX, centerY, centerX + 80 * std::cos(secondAngle - PI / 2), centerY + 80 * std::sin(secondAngle - PI / 2), QPen(Qt::red, 1));
}

void Clock::updateDigitalTime() {
    // Get the current time and format it as HH:mm:ss
    QString currentTimeText = QTime::currentTime().toString("hh:mm:ss");
    digitalClockButton->setText(currentTimeText);  // Update button text
}



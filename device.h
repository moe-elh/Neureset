#ifndef DEVICE_H
#define DEVICE_H

#include "session.h"
#include "brainwave.h"
#include <QObject>
#include <QVector>
#include <QDateTime>
#include <QTimer>
#include <QElapsedTimer>

using namespace std;

class Device : public QObject
{
    Q_OBJECT

public:
    Device();
    ~Device();

    void updateDevice();
    void calcBaseline();
    void direct();
    void trad();
    void readSession();
    void writeSession();
    void printState();
    void populateSites();
    void treatmentRound();
    void writeToFile();

    //GETTERS
    Session* getSession();
    QVector<Brainwave*> getSites();
    QDateTime getDateTime();
    int getBatteryStatus();
    QString getMode();
    bool getDeviceStatus();
    QString getContactStatus();
    QElapsedTimer* getBatteryDateTimer();

    //SETTERS
    void setSession(Session* session);
    void setBrainwave(Brainwave brainwaves);
    void setDateTime(QDateTime dateTime);
    void setBatteryStatus(int batteryStatus);
    void setMode(QString deviceStatus);
    void setDeviceStatus(bool status);
    void setContactStatus(QString status);
    void setBatteryDateTimer(QElapsedTimer* batteryDateTimer);

signals:
    void printToConsole(QString text);
    void batteryAndDateTimeUpdated();

private:
    Session* session;
    QVector<Brainwave*> sites;
    QDateTime dateTime;
    bool deviceStatus;
    QString contactStatus;
    int batteryStatus;
    QString mode;
    QElapsedTimer* batteryDateTimer;
};

#endif // DEVICE_H

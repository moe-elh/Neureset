#ifndef SESSION_H
#define SESSION_H

#include <QString>
#include <QObject>
#include <QVector>
#include <QDate>
#include "brainwave.h"

using namespace std;

class Session : public QObject
{
    Q_OBJECT

public:
    Session();
    Session(QString patientID);
    ~Session();

    //GETTERS
    QString getPatientID();
    float getBaselineBefore();
    float getBaselineAfter();
    QString getStatus();

    //SETTERS
    void setPatientID(QString patientID);
    void setBaselineBefore(float baseline);
    void setBaselineAfter(float baseline);
    void setStatus(QString status);

signals:
    void printToConsole(QString text);

private:
    QString patientID;
    float baselineBefore;
    float baselineAfter;
    QString status;
};

#endif // SESSION_H

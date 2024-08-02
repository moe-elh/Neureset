#ifndef BRAINWAVE_H
#define BRAINWAVE_H

#include <QObject>
#include <QVector>
#include <QDate>

using namespace std;

class Brainwave : public QObject
{
    Q_OBJECT

public:
    Brainwave();
    ~Brainwave();
    QString getType();
    int getDominantFreq();
    void addHz(int hz);

signals:
    void printToConsole(QString text);

private:
    enum types{
        Delta,
        Theta,
        Alpha,
        Beta
    } type;

    QString typeToString(types t);
    int minFrequency(types t);
    int maxFrequency(types t);
    int minAmplitude(types t);
    int maxAmplitude(types t);

    int dominantFreq;
};

#endif // BRAINWAVE_H

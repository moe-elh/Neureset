#ifndef HISTORY_H
#define HISTORY_H

#include <session.h>
#include <QObject>
#include <QVector>
#include <QDate>

using namespace std;

class History : public QObject
{
    Q_OBJECT

public:
    History();
    ~History();

    QVector<Session*> getSessions();
    void clearHistory();

signals:
    void printToConsole(QString text);

private:
    QVector<Session*> sessions;

};

#endif // HISTORY_H

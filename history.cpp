#include "history.h"
#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <QVector>

History::History() {

}
History::~History() {
    qDeleteAll(sessions);
    sessions.clear();
}
QVector<Session*> History::getSessions() {
    QVector<Session*> sessions;
    QFile file("./log_history.txt");

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Error: Unable to open file for reading" << file.errorString();
        return sessions;
    }

    QTextStream in(&file);
    Session* currentSession = nullptr;

    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        if (line.isEmpty() || line.startsWith("#")) {
            continue;
        }

        qDebug() << line;

        if (line.startsWith("Session for Patient: ")) {
            if (currentSession != nullptr) {
                sessions.append(currentSession);
            }
            currentSession = new Session();
            currentSession->setPatientID(line.mid(QString("Session for Patient: ").length()));
        } else if (currentSession) {
            if (line.startsWith("Before Baseline: ")) {
                currentSession->setBaselineBefore(line.mid(QString("Before Baseline: ").length()).toFloat());
            } else if (line.startsWith("After Baseline: ")) {
                currentSession->setBaselineAfter(line.mid(QString("After Baseline: ").length()).toFloat());
            } else if (line.startsWith("Session Status: ")) {
                currentSession->setStatus(line.mid(QString("Session Status: ").length()));
            }
        }
    }

    if (currentSession != nullptr) {
        sessions.append(currentSession);
    }

    file.close();
    return sessions;
}

void History::clearHistory(){
    QFile file("./log_history.txt");
    if(file.open(QIODevice::WriteOnly | QIODevice::Text)){
        qDebug()<<"History cleared successfully";
    }
    else{
        qDebug()<<"Error: Unable to clear history"<<file.errorString();
    }
    file.close();
}


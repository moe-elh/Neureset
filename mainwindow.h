#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QString>
#include <QDateTime>
#include <QMainWindow>
#include <QPushButton>
#include <QTimer>
#include "device.h"
#include "history.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void updateBatteryAndDate();
    void chargeBattery();
    void depleteBattery();
    void updateTreatment();

private:
    Ui::MainWindow *ui;
    Device* device;
    Brainwave brainwave;
    QTimer* treatmentTimer;
    QElapsedTimer* elapsedTreatmentTimer;
    History* history;

    QTimer* pauseTimer;
    bool isPaused;


    void updateTimerDisplay();
    void deviceStatus(bool power);
    void power();
    void contact();
    void treatment_signal();
    void contact_lost();
    void appendToConsole(const QString &text);
    void contactOn();
    void contactOff();
    void treatment_signalOn();
    void treatment_signalOff();
    void contact_lostOn();
    void contact_lostOff();
    void initSession();
    void treatmentStart();
    void log_history();
    void sessionStopper();
    void sessionPauser();
    void saveSessionStateToFile();


    //Color Palette
    QString blueOn = "#2563eb";
    QString blueOff = "#1e3a8a";
    QString greenOn = "#16a34a";
    QString greenOff = "#14532d";
    QString redOn = "#dc2626";
    QString redOff = "#7f1d1d";
};
#endif // MAINWINDOW_H

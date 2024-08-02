#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QString>
#include <QScrollBar>
#include <QThread>
#include <QFile>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , brainwave()
{
    ui->setupUi(this);

    ui->power->setCheckable(true);
    ui->sessionFrame->setDisabled(true);
    ui->patientFrame->setDisabled(true);
    ui->testFrame->setDisabled(true);
    ui->contact->setVisible(false);
    ui->contact_lost->setVisible(false);
    ui->treatment_signal->setVisible(false);
    ui->batteryStatus->setVisible(false);
    ui->dateTime->setVisible(false);

    connect(ui->power, &QPushButton::clicked, this, &MainWindow::power);
    connect(ui->connect, &QPushButton::clicked, this, &MainWindow::contact);
    connect(ui->disconnect, &QPushButton::clicked, this, &MainWindow::contact_lost);

    appendToConsole("NEURESET --- AWAITING INITIALIZATION\n");
    device = new Device();

    treatmentTimer = new QTimer(this);
    elapsedTreatmentTimer = new QElapsedTimer();
    pauseTimer = new QTimer(this);
    pauseTimer->setSingleShot(true);


    contact_lost();
    connect(ui->chargeBattery, &QPushButton::clicked, this, &MainWindow::chargeBattery);
    connect(ui->depleteBattery, &QPushButton::clicked, this, &MainWindow::depleteBattery);

    connect(ui->patientNewSessionButton, &QPushButton::clicked, this, &MainWindow::initSession);
    connect(ui->sessionPlay, &QPushButton::clicked, this, &MainWindow::treatmentStart);
    connect(ui->sessionPause, &QPushButton::clicked, this, &MainWindow::sessionPauser);
    connect(pauseTimer, &QTimer::timeout, this, &MainWindow::saveSessionStateToFile);
    connect(ui->sessionStop, &QPushButton::clicked, this, &MainWindow::sessionStopper);

    connect(ui->log, &QPushButton::clicked, this, &MainWindow::log_history);

}

MainWindow::~MainWindow()
{
    delete ui;
    delete device;
    delete history;
    if(treatmentTimer){
        treatmentTimer->stop();
        delete treatmentTimer;
    }
}

void MainWindow::deviceStatus(bool status)
{
    device->setDeviceStatus(status);
}

void MainWindow::contact()
{
    if(device->getContactStatus() != "Contact"){
        device->setContactStatus("Contact");
        appendToConsole("Contact\n");
        contactOn();
        treatment_signalOff();
        contact_lostOff();
    }
}

void MainWindow::treatment_signal()
{
    if(device->getContactStatus() != "Treatment Signal"){
        device->setContactStatus("Treatment Signal");
        contactOff();
        treatment_signalOn();
        contact_lostOff();
    }
}

void MainWindow::contact_lost()
{
    if(device->getContactStatus() != "Contact Lost"){
        device->setContactStatus("Contact Lost");
        appendToConsole("Contact Lost\n");
        contactOff();
        treatment_signalOff();
        contact_lostOn();
    }
}

void MainWindow::appendToConsole(const QString &text)
{
    ui->console->insertPlainText(text);
    QScrollBar *scrollBar = ui->console->verticalScrollBar();
    scrollBar->setValue(scrollBar->maximum());
}

void MainWindow::updateBatteryAndDate(){
    ui->batteryStatus->setValue(device->getBatteryStatus());
    ui->dateTime->setDateTime(device->getDateTime());
    //TODO: if battery<=0, "freeze" program
}

void MainWindow::chargeBattery(){
    device->setBatteryStatus(100);
}

void MainWindow::depleteBattery(){
    device->setBatteryStatus(1);
}

void MainWindow::power(){
    deviceStatus(ui->power->isChecked());
    connect(device, &Device::batteryAndDateTimeUpdated, this, &MainWindow::updateBatteryAndDate);
    if(!ui->power->isChecked()) {
        ui->sessionFrame->setEnabled(false);
    }
    ui->patientFrame->setEnabled(ui->power->isChecked());
    ui->testFrame->setEnabled(ui->power->isChecked());
    ui->power->setChecked(ui->power->isChecked());
    ui->dateTime->setVisible(ui->power->isChecked());
    ui->batteryStatus->setVisible(ui->power->isChecked());
    ui->contact->setVisible(ui->power->isChecked());
    ui->contact_lost->setVisible(ui->power->isChecked());
    ui->treatment_signal->setVisible(ui->power->isChecked());
    if(device->getDeviceStatus() == 1){
        appendToConsole("Device On\n");
    }
    else{
        appendToConsole("Device Off\n");
    }
}

void MainWindow::contactOn(){
    ui->contact->setStyleSheet("background: "+blueOn+"; border: 1px solid black; border-radius: 5px;");
}
void MainWindow::contactOff(){
    ui->contact->setStyleSheet("background: "+blueOff+"; border: 1px solid black; border-radius: 5px;");
}
void MainWindow::treatment_signalOn(){
    ui->treatment_signal->setStyleSheet("background: "+greenOn+"; border: 1px solid black; border-radius: 5px;");
}
void MainWindow::treatment_signalOff(){
    ui->treatment_signal->setStyleSheet("background: "+greenOff+"; border: 1px solid black; border-radius: 5px;");
}
void MainWindow::contact_lostOn(){
    ui->contact_lost->setStyleSheet("background: "+redOn+"; border: 1px solid black; border-radius: 5px;");
}
void MainWindow::contact_lostOff(){
    ui->contact_lost->setStyleSheet("background: "+redOff+"; border: 1px solid black; border-radius: 5px;");
}

void MainWindow::initSession(){
    if(device->getContactStatus()!="Contact"){
        appendToConsole("EEG sites are not connected to the patient. Please connect them and try again\n");
        return;
    }
    device->setSession(new Session(ui->patientID->text()));
    appendToConsole("Patient ID: " + device->getSession()->getPatientID() + " initiated\n");

    ui->sessionFrame->setEnabled(true);
    ui->patientFrame->setDisabled(true);

    treatmentTimer = new QTimer(this);
    elapsedTreatmentTimer = new QElapsedTimer();

}

void MainWindow::treatmentStart(){
    if (isPaused) {
        isPaused = false;
        pauseTimer->stop();
        treatmentTimer->start(1000);
        elapsedTreatmentTimer->start();
        appendToConsole("Session resumed\n");
    }

    if(device->getContactStatus() != "Treatment Signal"){
        treatment_signal();
        appendToConsole("Treatment Started...\n");
        treatmentTimer->start(1000);
        elapsedTreatmentTimer->start();
        connect(treatmentTimer, &QTimer::timeout, this, &MainWindow::updateTreatment);
    }
}


void MainWindow::sessionStopper(){

    if (device->getSession() == nullptr) {
        appendToConsole("No session active right now.\n");
        return;
    }

    if (treatmentTimer->isActive()) {
        treatmentTimer->stop();
    }

    pauseTimer->stop();
    isPaused = false;

    appendToConsole("Session stopped with patient ID: " + device->getSession()->getPatientID() + "\n");

    ui->sessionFrame->setDisabled(true);
    ui->patientFrame->setEnabled(true);
    ui->sessionTimer->display(0);
    ui->sessionProgress->setValue(0);

    delete device->getSession();
    device->setSession(nullptr);

    QVector<Brainwave*> sites = device->getSites();
    for (Brainwave* bw : sites) {
        delete bw;
    }
    sites.clear();

    appendToConsole("Session stopped\n");

    if (device->getContactStatus() == "Contact") {
            contactOn();
            appendToConsole("Device still connected.\n");
        } else {
            contact();
        }
}


void MainWindow::sessionPauser() {
    if (!isPaused) {
        isPaused = true;
        treatmentTimer->stop();
        pauseTimer->start(5000);  // Start the timer to check for the pause duration
        appendToConsole("Session paused\n");
    }
}

void MainWindow::saveSessionStateToFile() {
    this->device->writeToFile();
    appendToConsole("Session state saved to file\n");
    sessionStopper();  // Stop the session after saving the state
}

void MainWindow::updateTreatment(){
    int elapsedSeconds = elapsedTreatmentTimer->elapsed()/1000;
    if(elapsedSeconds==5){
        device->populateSites();
        appendToConsole("Baseline before: "+QString::number(device->getSession()->getBaselineBefore())+"\n");
    } else if (elapsedSeconds == 6) {
        //r1
        device->treatmentRound();
    } else if (elapsedSeconds == 7) {
        //r2
        device->treatmentRound();
    } else if (elapsedSeconds == 8) {
        //r3
        device->treatmentRound();
    } else if (elapsedSeconds == 9) {
        //r4
        device->treatmentRound();
    } else if (elapsedSeconds == 14) {
        ui->sessionProgress->setValue(100);
        for(int i = 0; i < 21; i++){
            int sum = 0;
            sum += device->getSites()[i]->getDominantFreq();
            device->getSession()->setBaselineAfter(sum);
        }
        appendToConsole(QString::number(device->getSession()->getBaselineAfter())+"\n");
        device->getSession()->setStatus("Success");
        device->writeToFile();
        treatmentTimer->stop();
        elapsedTreatmentTimer->start();
    }
    ui->sessionTimer->display(elapsedSeconds);
    ui->sessionProgress->setValue(ui->sessionProgress->value() + (100/14));
}


void MainWindow::log_history(){
    QVector<Session*> sessions = this->history->getSessions();
    for(Session* session : sessions){
        QString sessionInfo = QString("Srression for Patient ID: %1 | Before basline: %2 | After basline: %3 | Session Status: %4\n")
                .arg(session->getPatientID())
                .arg(session->getBaselineBefore())
                .arg(session->getBaselineAfter())
                .arg(session->getStatus());
        appendToConsole(sessionInfo);
    }
    //rhistory->clearHistory();
}


#include "device.h"
#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <QDir>

Device::Device() {
    this->dateTime = QDateTime::currentDateTime();
    this->batteryStatus = 100;

    this->batteryDateTimer = new QElapsedTimer();
    this->batteryDateTimer->start();

    QTimer *bdtUpdate = new QTimer(this);
    connect(bdtUpdate, &QTimer::timeout, this, &Device::updateDevice);
    bdtUpdate->start(1000);
}

Device::~Device() {
    delete(batteryDateTimer);
    qDeleteAll(sites);
    sites.clear();
}

void Device::updateDevice(){
    this->dateTime = QDateTime::currentDateTime();
    if ((this->batteryDateTimer->elapsed()/1000) % 10 == 0) {
        this->batteryStatus--;
    }
    emit batteryAndDateTimeUpdated();
}

int Device::getBatteryStatus(){
    return this->batteryStatus;
}

QDateTime Device::getDateTime(){
    return this->dateTime;
}

void Device::setBatteryStatus(int batteryStatus){
    this->batteryStatus = batteryStatus;
}

void Device::setDeviceStatus(bool status)
{
    deviceStatus = status;
}

bool Device::getDeviceStatus()
{
    return deviceStatus;
}

void Device::setContactStatus(QString status)
{
    contactStatus = status;
    qDebug() << "Contact status:" << contactStatus;
}

QString Device::getContactStatus()
{
    return contactStatus;
}

void Device::setSession(Session* session){
    this->session = session;
}

Session* Device::getSession(){
    return this->session;
}

QVector<Brainwave*> Device::getSites(){
    return this->sites;
}

void Device::populateSites(){
    qDebug() << "------------------------- INTIAL SITES -------------------------";
    int sum = 0;
    for (int i = 0; i < 21; i++){
        qDebug() << "------------------------------------------------ Site " << i+1;
        Brainwave* bw = new Brainwave();
        sum += bw->getDominantFreq();
        qDebug()<<"BAND:"<<bw->getType()<<" DOM-FREQUENCY:"<<bw->getDominantFreq();
        this->sites.append(bw);
    }
    this->session->setBaselineBefore(sum/21);
    qDebug()<<"------------------------------------------------ BASLINE:"<<this->session->getBaselineBefore();
}

void Device::treatmentRound(){
    qDebug() << "------------------------- NEW ROUND -------------------------";
    for(int i = 0; i < 21; i++){
        qDebug() << "------------------------------------------------ Site " << i+1;
        this->sites[i]->addHz(5);
        qDebug()<<"BAND:"<<this->sites[i]->getType()<<" DOM-FREQUENCY:"<<this->sites[i]->getDominantFreq();
    }
}


void Device::writeToFile(){
    QString filePath = "./log_history.txt";
    qDebug() << "Current directory:" << QDir::currentPath();
    QFile file(filePath);

    if (!file.open(QIODevice::Append | QIODevice::Text)){
        qDebug()<<"Error: Unable to open file for writing";
        return;
    }

    QTextStream out(&file);
    if(this->session) {
        out<<"Session for Patient: "<<this->session->getPatientID()<<"\n";
        out<<"Before Baseline: "<<this->session->getBaselineBefore()<<"\n";
        out<<"After Baseline: "<<this->session->getBaselineAfter()<<"\n";
        out<<"Session Status: "<<this->session->getStatus()<<"\n";
    }
    else{
        out<<"No session data available.\n";
    }
    file.close();
    qDebug()<<"Session data written to:"<<filePath;
}




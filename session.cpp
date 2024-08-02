#include "session.h"


Session::Session() {
}

Session::Session(QString patientID) {
    this->patientID = patientID;
}

Session::~Session() {

}

QString Session::getPatientID(){
    return this->patientID;
}

void Session::setPatientID(QString id){
    this->patientID = id;
}

float Session::getBaselineBefore(){
    return this->baselineBefore;
}

float Session::getBaselineAfter(){
    return this->baselineAfter;
}

void Session::setBaselineBefore(float baseline){
    this->baselineBefore = baseline;
}

void Session::setBaselineAfter(float baseline){
    this->baselineAfter = baseline;
}

QString Session::getStatus(){
    return this->status;
}

void Session::setStatus(QString status){
    this->status = status;
}


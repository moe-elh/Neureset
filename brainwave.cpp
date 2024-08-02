#include "brainwave.h"
#include <QRandomGenerator>
#include <QDebug>

Brainwave::Brainwave() {
    type = static_cast<types>(QRandomGenerator::global()->bounded(Delta, Beta + 1));
    int f1 = QRandomGenerator::global()->bounded(minFrequency(type), maxFrequency(type) + 1);
    int f2 = QRandomGenerator::global()->bounded(minFrequency(type), maxFrequency(type) + 1);
    int f3 = QRandomGenerator::global()->bounded(minFrequency(type), maxFrequency(type) + 1);
    int a1 = QRandomGenerator::global()->bounded(minAmplitude(type), maxAmplitude(type) + 1);
    int a2 = QRandomGenerator::global()->bounded(minAmplitude(type), maxAmplitude(type) + 1);
    int a3 = QRandomGenerator::global()->bounded(minAmplitude(type), maxAmplitude(type) + 1);
    dominantFreq = (f1*(a1*a1)+f2*(a2*a2)+f3*(a3*a3)) /
                   ((a1*a1)+(a2*a2)+(a3*a3));
}

Brainwave::~Brainwave() {

}

QString Brainwave::typeToString(types t){
    switch(t){
        case Delta: return "Delta";
        case Theta: return "Theta";
        case Alpha: return "Alpha";
        case Beta: return "Beta";
        default: return "Unknown Type";
    }
}

QString Brainwave::getType()
{
    return typeToString(type);
}

int Brainwave::minFrequency(types t)
{
    switch(t){
        case Delta: return 1;
        case Theta: return 4;
        case Alpha: return 8;
        case Beta: return 12;
        default: return -1;
    }
}

int Brainwave::maxFrequency(types t)
{
    switch(t){
        case Delta: return 4;
        case Theta: return 8;
        case Alpha: return 12;
        case Beta: return 30;
        default: return -1;
    }
}

int Brainwave::minAmplitude(types t){
    switch(t){
        case Delta: return 20;
        case Theta: return 10;
        case Alpha: return 25;
        case Beta: return 5;
        default: return -1;
    }
}

int Brainwave::maxAmplitude(types t){
    switch(t){
        case Delta: return 200;
        case Theta: return 100;
        case Alpha: return 60;
        case Beta: return 30;
        default: return -1;
    }
}

int Brainwave::getDominantFreq(){
    return dominantFreq;
}

void Brainwave::addHz(int hz){
    this->dominantFreq += hz;
}



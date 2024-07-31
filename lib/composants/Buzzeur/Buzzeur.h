#ifndef BUZZEUR_H
#define BUZZEUR_H


#include"../BSP.h"

class Buzzeur
{
private:
    int _pin;
    int _etat;

public:
    Buzzeur(int pin)
        : _pin(pin)
    {}
    ~Buzzeur()
    {}

    void init(){
        pinMode(_pin, OUTPUT);
        _etat = BUZZEUR_DESACTIVE;
    }
    void activerBuzzeur(){
        digitalWrite(_pin, ACTIVER_BUZZEUR);
        _etat = BUZZEUR_ACTIVE;
    }
    void desactiverBuzzeur(){
        digitalWrite(_pin, DESACTIVER_BUZZEUR);
        _etat = BUZZEUR_DESACTIVE;
    }
    void beep(){
        tone(_pin, 10000, 500);
        delay(1000);
        noTone(_pin);
        delay(1000);
        _etat = BUZZEUR_DESACTIVE;
    }
    void setPin(int pin){
        _pin = pin;
    }
    int getPin(){
        return _pin;
    }
    void setEtat(bool etatBuzzeur){
        _etat = etatBuzzeur;
    }
    bool getEtat(){
        return _etat;
    }
};


#endif
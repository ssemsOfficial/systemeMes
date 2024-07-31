#ifndef LEDGENERAL_H
#define LEDGENERAL_H

#include"../BSP.h"

#include"Led.h"

class LedGeneral : public Led
{
private:
    int _pin;
    int _etat;
public:
    LedGeneral(int pin):
        _pin(pin)
    {}
    ~LedGeneral();

    void init(){
        pinMode(_pin, OUTPUT);
        _etat = digitalRead(_pin);
    }

    void allumerLed(){
        digitalWrite(_pin, ALLUMER_LED);
        _etat = digitalRead(LED_ALLUMEE);
    }

    void eteindreLed(){
        digitalWrite(_pin, ETEINDRE_LED);
        _etat = LED_ETEINT;
    }
    void setPin(int pin){
        _pin = pin;
    }
    int getPin(){
        return _pin;
    }
    void setEtat(bool etat){
        _etat = etat;
    }
    bool getEtat(){
        return _etat;
    }
};



#endif
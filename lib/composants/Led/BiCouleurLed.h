#ifndef LEDGENERAL_H
#define LEDGENERAL_H

#include"../BSP.h"

#include"Led.h"

class BiCouleurLed : public Led
{
private:
    int _pinRouge;
    int _pinVert;
    bool _etat;
    bool _couleur;
public:
    BiCouleurLed(int pinRouge, int pinVert):
      _pinRouge(pinRouge), _pinVert(pinVert)
    {}
    ~BiCouleurLed()
    {}

    
    void init(){
        pinMode(_pinRouge, OUTPUT);
        pinMode(_pinVert, OUTPUT);
        _couleur = digitalRead(_pinVert) ?  LED_ALLUMEE_VERT : LED_ALLUMEE_ROUGE;
        _etat = digitalRead(_pinRouge) || digitalRead(_pinVert) ? LED_ALLUMEE : LED_ETEINT;
    }

    void allumerLed(bool couleur){
        if(couleur == ALLUMER_LED_ROUGE)
            digitalWrite(_pinRouge, ALLUMER_LED);
        else if(couleur == ALLUMER_LED_VERT)
            digitalWrite(_pinVert, ALLUMER_LED);    
        _etat = digitalRead(LED_ALLUMEE);
    }

    void eteindreLed(){
        digitalWrite(_pinRouge, ETEINDRE_LED);
        digitalWrite(_pinVert, ETEINDRE_LED);
        _etat = LED_ETEINT;
    }
    /*
    void setPin(int pin){
        _pin = pin;
    }
    int getPin(){
        return _pin;
    }
    */
    void setEtat(bool etat){
        _etat = etat;
    }
    bool getEtat(){
        return _etat;
    }
};


#endif
#ifndef SYSTEMEGETIONINTERVENTIONMAINTENANCE_H
#define SYSTEMEGETIONINTERVENTIONMAINTENANCE_H

#include"../BSP.h"

#include"../freertos/FreeRTOS.h"
#include"../freertos/queue.h"

#include"../composants/Led/BiCouleurLed.h"
#include"../composants/Boutton/Boutton.h"

class SystemeGestionInterventionMaintenance
{
private:
    BiCouleurLed &led;
    Boutton &boutton;

    QueueHandle_t &queue;

    //evenements
    struct evenementSysteme 
    {
        struct Btn{
            bool etat = false;
            bool etatPresidente = false;
        };
        Btn surBoutton;
    };
    evenementSysteme evenements;
    
    //etats
    enum etatSysteme
    {
        INIT,
        BOUTTON_EST_CLICKE,
        BOUTTON_EST_RELACHE
    };
    etatSysteme etats= INIT;

    //actions
    struct actionSysteme
    {   
        struct ledStruct{
            bool etat = false;
            bool couleur = false;
        };
        ledStruct led;
        bool donnee = false;
    };
    actionSysteme actions;

public:
    SystemeGestionInterventionMaintenance(BiCouleurLed &led, Boutton &boutton, QueueHandle_t &queue):
        boutton(boutton), led(led), queue(queue)
    {}
    ~SystemeGestionInterventionMaintenance()
    {}

    void evenement();
    void fsm();
    void action();
};


void SystemeGestionInterventionMaintenance::evenement(){
    boutton.estClicke();
    if(boutton.getEtat() == SUR_BOUTTON_MAINTENANCE_CLICKE && boutton.getEtatPresidente() == SUR_BOUTTON_MAINTENANCE_RELACHE)
    {
        evenements.surBoutton.etat = SUR_BOUTTON_MAINTENANCE_CLICKE;
        boutton.setEtatPresidente(SUR_BOUTTON_MAINTENANCE_CLICKE);
        evenements.surBoutton.etatPresidente = SUR_BOUTTON_MAINTENANCE_CLICKE;
    }
    else if(boutton.getEtat() == SUR_BOUTTON_MAINTENANCE_RELACHE)
    {
        evenements.surBoutton.etat = SUR_BOUTTON_MAINTENANCE_RELACHE;
        boutton.setEtat(SUR_BOUTTON_MAINTENANCE_RELACHE);
    }
}

void SystemeGestionInterventionMaintenance::fsm(){
    switch (etats)
    {
        case INIT : 
            if (evenements.surBoutton.etat == SUR_BOUTTON_MAINTENANCE_CLICKE)
            {
                etats = BOUTTON_EST_CLICKE;
                actions.led.etat = ALLUMER_LED;
                actions.led.couleur =ALLUMER_LED_ROUGE;
                break;

            }
            else if (evenements.surBoutton.etat == SUR_BOUTTON_MAINTENANCE_RELACHE)
            {
                etats = BOUTTON_EST_RELACHE;
                actions.led.etat = ETEINDRE_LED;
                actions.led.couleur = ETEINDRE_LED;
                break;
            }
        break;
        
        case BOUTTON_EST_CLICKE :
            if (evenements.surBoutton.etat == SUR_BOUTTON_MAINTENANCE_RELACHE)
            {
                etats = BOUTTON_EST_RELACHE;
                actions.led.etat = ETEINDRE_LED;
                actions.led.couleur = ETEINDRE_LED;
                actions.donnee = true;
            }
        break;
        
        case BOUTTON_EST_RELACHE :
            if (evenements.surBoutton.etat == SUR_BOUTTON_MAINTENANCE_CLICKE)
            {
                etats = BOUTTON_EST_CLICKE;
                actions.led.etat = ALLUMER_LED;
                actions.led.couleur =ALLUMER_LED_ROUGE;
                actions.donnee = false;

            }
        break;
    }
}

void SystemeGestionInterventionMaintenance::action(){
    if(actions.led.etat == ALLUMER_LED && actions.led.couleur == ALLUMER_LED_ROUGE)
    {
        led.allumerLed(ALLUMER_LED_ROUGE);
    }
    else if(actions.led.etat == ETEINDRE_LED)
    {
        led.eteindreLed();
    }

    if(actions.donnee == true){
        xQueueSend(queue, &actions.donnee, portMAX_DELAY);
        boutton.setEtatPresidente(SUR_BOUTTON_MAINTENANCE_RELACHE);
    }
}


#endif
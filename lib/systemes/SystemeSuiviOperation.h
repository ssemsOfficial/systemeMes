#ifndef SYSTEMESUIVIOPERATION_H
#define SYSTEMESUIVIOPERATION_H

#include"../BSP.h"

#include"../freertos/FreeRTOS.h"
#include"../freertos/queue.h"

#include"../composants/Led/BiCouleurLed.h"
#include"../composants/Boutton/Boutton.h"
#include"../composants/Buzzeur/Buzzeur.h"

#include"../adaptateurs/RFID_Adaptateur.h"

class SystemeSuiviOperation{
private:
    BiCouleurLed &led;
    Boutton &boutton;
    Buzzeur &buzzeur;

    RFID_Adaptateur &adaptateurRFID;

    QueueHandle_t &queue;

    String dataToSend;

    //evenements
    struct evenementSysteme 
    {
        struct Tag
        {
            bool lecture = false;
            bool etatPresence = false;
            bool etatPresencePresidente = false;
        };
        
        Tag surTag;
        bool surBoutton = false;
    };
    evenementSysteme evenements;
    
    //etats
    enum etatSysteme
    {
        INIT,
        TAG_EST_DETECTE,
        TAG_EST_NON_DETECTE
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
        struct RFID{
            bool reset = false;
        };
        RFID rfid;
        bool buzzeur;
    };
    actionSysteme actions;

public:
    SystemeSuiviOperation(BiCouleurLed &led, Boutton &boutton, Buzzeur &buzzeur, RFID_Adaptateur &adaptateurRFID, QueueHandle_t &queue):
        boutton(boutton), led(led), buzzeur(buzzeur), adaptateurRFID(adaptateurRFID), queue(queue)
    {}
    ~SystemeSuiviOperation()
    {}

    void evenement();
    void fsm();
    void action();
};


void SystemeSuiviOperation::evenement(){
    adaptateurRFID.detecterCarte();
    if( adaptateurRFID.getPresenceTag() == SUR_TAG_DETECTE && adaptateurRFID.getPresenceTagPresi() == SUR_TAG_NON_DETECTE)
    {
        evenements.surTag.etatPresence = SUR_TAG_DETECTE;
        adaptateurRFID.setPresenceTagPresi(SUR_TAG_DETECTE);
        evenements.surTag.etatPresencePresidente = SUR_TAG_DETECTE;
        adaptateurRFID.lectureTag();

        if(adaptateurRFID.getLectureTag() == SUR_LECTURE_SUCCEE){
            evenements.surTag.lecture =  SUR_LECTURE_SUCCEE;
        }
        else if(adaptateurRFID.getLectureTag() == SUR_LECTURE_ECHOUE){
            evenements.surTag.lecture =  SUR_LECTURE_ECHOUE;
        }
    }
    else if(adaptateurRFID.getPresenceTag() == SUR_TAG_NON_DETECTE)
    {
        evenements.surTag.etatPresence = SUR_TAG_NON_DETECTE;
        adaptateurRFID.setPresenceTag(SUR_TAG_NON_DETECTE);
    }
}



void SystemeSuiviOperation::fsm(){
    switch (etats)
    {
        case INIT : 
            if (evenements.surTag.etatPresence == SUR_TAG_DETECTE)
            {
                etats = TAG_EST_DETECTE;
                actions.led.etat = ALLUMER_LED;
                actions.led.couleur =ALLUMER_LED_ROUGE;
                break;
            }
            else if (evenements.surTag.etatPresence == SUR_TAG_NON_DETECTE)
            {
                etats = TAG_EST_NON_DETECTE;
                actions.led.etat = ETEINDRE_LED;
                actions.led.couleur = ETEINDRE_LED;
                break;
            }
        break;
        
        case TAG_EST_DETECTE :
            if (evenements.surTag.etatPresence == SUR_TAG_NON_DETECTE && evenements.surTag.etatPresencePresidente == SUR_TAG_DETECTE)
            {
                etats = TAG_EST_NON_DETECTE;
                actions.led.etat = ETEINDRE_LED;
                actions.rfid.reset = REST_DETCTION_RFID;
                actions.buzzeur = ACTIVER_BUZZEUR;
            }
        break;
        
        case TAG_EST_NON_DETECTE :
            if (evenements.surTag.etatPresence == SUR_TAG_DETECTE)
            {
                etats = TAG_EST_DETECTE;
                actions.led.etat = ALLUMER_LED;
                if (evenements.surTag.lecture == SUR_LECTURE_SUCCEE)
                {
                    actions.led.couleur = ALLUMER_LED_VERT;
                }
                else if (evenements.surTag.lecture == SUR_LECTURE_ECHOUE)
                {
                    actions.led.couleur = ALLUMER_LED_ROUGE;
                }
            }
        break;
    }
}



void SystemeSuiviOperation::action(){
    //led
    if(actions.led.etat == ALLUMER_LED && actions.led.couleur == ALLUMER_LED_ROUGE){
        led.allumerLed(ALLUMER_LED_ROUGE);
    }
    else if(actions.led.etat == ALLUMER_LED && actions.led.couleur == ALLUMER_LED_VERT){
        led.allumerLed(ALLUMER_LED_VERT);
    }
    else if(actions.led.etat == ETEINDRE_LED)
    {
        led.eteindreLed();
    }

    //buzzeur
    if(actions.buzzeur == ACTIVER_BUZZEUR){
        buzzeur.beep();
    }
    else if(actions.buzzeur == DESACTIVER_BUZZEUR){
        buzzeur.desactiverBuzzeur();
    }
    
    //envoye donnee + reset adaptateur rfid
    if(actions.rfid.reset == REST_DETCTION_RFID)
    {
        adaptateurRFID.setPresenceTagPresi(SUR_TAG_NON_DETECTE);
        dataToSend = adaptateurRFID.getData();
        xQueueSend(queue, &dataToSend, portMAX_DELAY);
        dataToSend = "";
        actions.rfid.reset = NE_RESET_PAS_DETCTION_RFID;
    }
}


#endif
#ifndef SYSTEMEGESTIONDESDONNEES_H
#define SYSTEMEGESTIONDESDONNEES_H


#include"../BSP.h"

#include"../gestionneurs/GestionneurDonnees.h"
#include"../adaptateurs/WIFI_Adaptateur.h"

class SystemeGestionDesDonnees
{
private:

    WiFi_Adaptateur &adaptateurWifi;
    GestionneurDonnees &gestionneurDonnees;

    //evenements
    struct evenementSysteme 
    {
        struct WIFI
        {
            bool etat = false;
            bool etatPresence = false;
            bool etatPresencePresidente = false;
        };
        WIFI surWifi;
        struct Data
        {
            bool etat = false;
        };
        Data surOperation;
        Data surInterventionMaintenance;
    };
    evenementSysteme evenements;
    
    //etats
    enum etatWifi
    {
        WIFI_EST_CONNECTE,
        WIFI_EST_NON_CONNECTE
    };
    etatWifi etatsWifi = WIFI_EST_NON_CONNECTE;
    
    enum etatSysteme
    {
        INIT,
        DONNEE_EST_ENVOYE,
        OPERATION_EST_NON_ENVOYE,
        INTERVENTION_MAINTENACE_EST_NON_ENVOYE
    };
    etatSysteme etatsSysteme = INIT;

    //actions
    struct actionSysteme
    {   
        struct Data{
            bool etat = false;
            bool couleur = false;
        };
        Data operation;
        Data interventionMaintenance;

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
    SystemeGestionDesDonnees(WiFi_Adaptateur &adaptateurWifi, GestionneurDonnees &gestionneurDonnees) :
    adaptateurWifi(adaptateurWifi), gestionneurDonnees(gestionneurDonnees)
    {

    }

    ~SystemeGestionDesDonnees()
    {

    }

    void evenement();

    void fsm();
    
    void action();
};


void SystemeGestionDesDonnees::evenement(){
    //check wifi
    adaptateurWifi.estConnecte();
    if(adaptateurWifi.getWifiConnecte() == SUR_WIFI_CONNECTE){
        evenements.surWifi.etat = SUR_WIFI_CONNECTE;
    } 
    else if(adaptateurWifi.getWifiConnecte() == SUR_WIFI_NON_CONNECTE){
        evenements.surWifi.etat = SUR_WIFI_NON_CONNECTE;
    }

    //check data 
    if(gestionneurDonnees.validerQueueOperationEstVide()){
        evenements.surOperation.etat = SUR_QUEUE_OPERATION_VIDE;
    }
    else if(gestionneurDonnees.validerQueueOperationEstVide()){
        evenements.surOperation.etat = SUR_QUEUE_OPERATION_NON_VIDE;
    }

    if(gestionneurDonnees.validerQueueInterventionEstVide()){
        evenements.surInterventionMaintenance.etat = SUR_QUEUE_INTERVENTION_VIDE;
    }
    else if(gestionneurDonnees.validerQueueInterventionEstVide()){
        evenements.surInterventionMaintenance.etat = SUR_QUEUE_INTERVENTION_NON_VIDE;
    }
}

void SystemeGestionDesDonnees::fsm(){
    switch (etatsWifi)
    {
        case WIFI_EST_CONNECTE :
            if(evenements.surWifi.etat == SUR_WIFI_NON_CONNECTE){
                etatsWifi = WIFI_EST_NON_CONNECTE;
                actions.interventionMaintenance.etat = NE_ENVOYER_PAS_INTERVENTION_MAINTENANCE;
                actions.operation.etat = NE_ENVOYER_PAS_OPERATIONS;    
                //afficher oled wifi bare
               break; 
            }
            switch (etatsSysteme){
                case INIT :
                    if (evenements.surOperation.etat == SUR_QUEUE_OPERATION_NON_VIDE ){
                        etatsSysteme = OPERATION_EST_NON_ENVOYE;
                        actions.operation.etat = ENVOYER_OPERATIONS;
                    }
                    else if (evenements.surInterventionMaintenance.etat == SUR_QUEUE_INTERVENTION_NON_VIDE){
                        //etatsSysteme = INTERVENTION_MAINTENACE_EST_NON_ENVOYE ;
                        //actions.interventionMaintenance.etat = ENVOYER_INTERVENTION_MAINTENANCE;
                    }
                    else if(evenements.surOperation.etat == SUR_QUEUE_OPERATION_NON_VIDE && evenements.surInterventionMaintenance.etat == SUR_QUEUE_INTERVENTION_NON_VIDE ){
                        //etatsSysteme = DONNEE_EST_ENVOYE;
                        //actions.operation.etat = NE_ENVOYER_PAS_OPERATIONS;
                        //actions.interventionMaintenance.etat = NE_ENVOYER_PAS_INTERVENTION_MAINTENANCE;
                    }
                break;

                case DONNEE_EST_ENVOYE :
                    if (evenements.surOperation.etat == SUR_QUEUE_OPERATION_NON_VIDE){
                        etatsSysteme = OPERATION_EST_NON_ENVOYE;
                        actions.operation.etat = ENVOYER_OPERATIONS;
                    }
                    else if (evenements.surInterventionMaintenance.etat == SUR_QUEUE_INTERVENTION_NON_VIDE){
                        etatsSysteme = INTERVENTION_MAINTENACE_EST_NON_ENVOYE;
                        actions.interventionMaintenance.etat = ENVOYER_INTERVENTION_MAINTENANCE;
                    }
                break;

                case OPERATION_EST_NON_ENVOYE :
                    if(evenements.surInterventionMaintenance.etat == SUR_QUEUE_INTERVENTION_NON_VIDE && evenements.surOperation.etat == SUR_QUEUE_OPERATION_VIDE)
                    {
                        etatsSysteme = INTERVENTION_MAINTENACE_EST_NON_ENVOYE;
                        actions.interventionMaintenance.etat = ENVOYER_INTERVENTION_MAINTENANCE;
                    }
                    else if(evenements.surOperation.etat == SUR_QUEUE_OPERATION_VIDE && evenements.surInterventionMaintenance.etat == SUR_QUEUE_INTERVENTION_VIDE )
                    {
                        etatsSysteme = DONNEE_EST_ENVOYE;
                        actions.interventionMaintenance.etat = NE_ENVOYER_PAS_INTERVENTION_MAINTENANCE;
                        actions.operation.etat = NE_ENVOYER_PAS_OPERATIONS;
                    }
                break;

                case INTERVENTION_MAINTENACE_EST_NON_ENVOYE :
                    if(evenements.surInterventionMaintenance.etat == SUR_QUEUE_INTERVENTION_VIDE && evenements.surOperation.etat == SUR_QUEUE_OPERATION_NON_VIDE)
                    {
                        etatsSysteme = OPERATION_EST_NON_ENVOYE;
                        actions.operation.etat = ENVOYER_OPERATIONS;
                    }
                    else if(evenements.surOperation.etat == SUR_QUEUE_OPERATION_VIDE && evenements.surInterventionMaintenance.etat == SUR_QUEUE_INTERVENTION_VIDE )
                    {
                        etatsSysteme = DONNEE_EST_ENVOYE;
                        actions.interventionMaintenance.etat = NE_ENVOYER_PAS_INTERVENTION_MAINTENANCE;
                        actions.operation.etat = NE_ENVOYER_PAS_OPERATIONS;
                    }
                break;
            }
        break;

        case WIFI_EST_NON_CONNECTE :
            if(evenements.surWifi.etat == SUR_WIFI_CONNECTE){
               etatsWifi = WIFI_EST_NON_CONNECTE;
               //afficher oled wifi non bare
            }
        break;
    }
}

void SystemeGestionDesDonnees::action(){
    if(actions.operation.etat == ENVOYER_OPERATIONS){
        gestionneurDonnees.envoyerOperation();
    }
    if(actions.operation.etat == ENVOYER_INTERVENTION_MAINTENANCE){
        gestionneurDonnees.envoyerInterventionMaintenance();
    }
}
#endif
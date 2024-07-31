#include <Arduino.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "freertos/queue.h"

#include "../lib/composants/Boutton/Boutton.h"
#include "../lib/composants/Buzzeur/Buzzeur.h"
#include "../lib/composants/Led/Led.h"

#include "../lib/adaptateurs/RFID_Adaptateur.h"
#include "../lib/adaptateurs/WIFI_Adaptateur.h"

#include "../lib/gestionneurs/GestionneurDonnees.h"

#include "../lib/systemes/SystemeGestionDesDonnees.h"
#include "../lib/systemes/SystemeGestionInterventionMaintenance.h"
#include "../lib/systemes/SystemeSuiviOperation.h"


//composants
Boutton boutton(pinBoutton);
Buzzeur buzzeur(pinBuzzeur);
BiCouleurLed led(pinRougeLed, pinVertLed);

//adaptateurs
RFID_Adaptateur adaptateurRFID();
WiFi_Adaptateur adaptateurWifi();

//gestionneurs
GestionneurDonnees gestionneurDonnees();

//
QueueHandle_t queueOperation;
QueueHandle_t queueIntervention;

//
SemaphoreHandle_t semaphore;

SystemeGestionDesDonnees systemeGestionDesDonnees(adaptateurWifi, gestionneurDonnees);
SystemeGestionInterventionMaintenance systemeGestionInterventionMaintenance(led, boutton, queueIntervention);
SystemeSuiviOperation systemeSuiviOperation(led, boutton, buzzeur, adaptateurRFID, queueOperation);



void systemeGestionInterventionMaintenanceTask(void *parameter){
    for (;;)
    {
        systemeGestionInterventionMaintenance.evenement();
        systemeGestionInterventionMaintenance.fsm();
        systemeGestionInterventionMaintenance.action();

        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}

void suiviOperationTask(void *parameter){
    for (;;)
    {
        systemeSuiviOperation.evenement();
        systemeSuiviOperation.fsm();
        systemeSuiviOperation.action();

        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}


void senderTask(void *parameter){
    
    String operation;
    bool interventionMain;
    
    for (;;)
    {
        if (xQueueReceive(queueOperation, &operation, portMAX_DELAY)) {
                gestionneurDonnees.ajouterOperation();
            }
        if (xQueueReceive(queueIntervention, &interventionMain, portMAX_DELAY)) {
                gestionneurDonnees.ajouterInterventionMaintenance();
            }

        vTaskDelay(100 / portTICK_PERIOD_MS);    
    }
    
    
}


void gestionDesDonneesTask(void *parameter){
    for (;;)
    {
        systemeGestionDesDonnees.evenement();
        systemeGestionDesDonnees.fsm();
        systemeGestionDesDonnees.action();

        vTaskDelay(100 / portTICK_PERIOD_MS);
    }   
}



void setup() {

}

void loop() {

}


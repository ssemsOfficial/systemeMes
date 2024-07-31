#include<Arduino.h>

//buzzeur
#define pinBuzzeur 24
#define DESACTIVER_BUZZEUR 0
#define ACTIVER_BUZZEUR 1
#define BUZZEUR_DESACTIVE 0
#define BUZZEUR_ACTIVE 1

//led
#define ALLUMER_LED 1
#define ALLUMER_LED_ROUGE 0
#define ALLUMER_LED_VERT 1
#define ETEINDRE_LED 0
#define LED_ALLUMEE 1
#define LED_ALLUMEE_ROUGE 0
#define LED_ALLUMEE_VERT 1
#define LED_ETEINT 0

//boutton
#define pinRougeLed 25
#define pinVertLed 24
#define pinBoutton 23
#define SUR_BOUTTON_MAINTENANCE_CLICKE 1
#define SUR_BOUTTON_MAINTENANCE_RELACHE 0


//RFID
#define SS_PIN 21
#define RST_PIN 22

#define SUR_TAG_DETECTE 1 
#define SUR_TAG_NON_DETECTE 0 
#define SUR_LECTURE_SUCCEE 1
#define SUR_LECTURE_ECHOUE 0

#define REST_DETCTION_RFID 1
#define NE_RESET_PAS_DETCTION_RFID 0


//WiFi
#define SSID  "your_SSID"
#define PASSWORD "your_PASSWORD"
#define SERVER_URL "http://your.server.url/endpoint"

#define SUR_WIFI_CONNECTE  true
#define SUR_WIFI_NON_CONNECTE   false    


//data
#define SUR_QUEUE_OPERATION_VIDE  true
#define SUR_QUEUE_OPERATION_NON_VIDE false
#define SUR_QUEUE_INTERVENTION_VIDE true
#define SUR_QUEUE_INTERVENTION_NON_VIDE false

#define ENVOYER_OPERATIONS true
#define NE_ENVOYER_PAS_OPERATIONS false
#define ENVOYER_INTERVENTION_MAINTENANCE true
#define NE_ENVOYER_PAS_INTERVENTION_MAINTENANCE false

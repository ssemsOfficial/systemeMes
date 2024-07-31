#ifndef RFID_ADAPTATEUR_H
#define RFID_ADAPTATEUR_H

#include<SPI.h>
#include<MFRC522.h>

#include"../BSP.h"

class RFID_Adaptateur
{
private:
    bool presenceTag = false;
    bool presenceTagPresi = false;
    bool etatLectureTag = false;
    
    byte block = 1;
    byte buffer[18];
    byte size = sizeof(buffer);

    MFRC522::MIFARE_Key key;
    MFRC522 mfrc522(SS_PIN, RST_PIN);


public:
    RFID_Adaptateur()
    {}
    ~RFID_Adaptateur()
    {}

    void init(){
        SPI.begin(); 
        mfrc522.PCD_Init(); 
        // 
        for (byte i = 0; i < 6; i++) {
            key.keyByte[i] = 0xFF;
        }
    }

    bool detecterCarte(){
        if (mfrc522.PICC_IsNewCardPresent()) {
            if (mfrc522.PICC_ReadCardSerial()) {
                presenceTag = SUR_TAG_DETECTE;
                return SUR_TAG_DETECTE;
            }
        } else {
            presenceTag = SUR_TAG_NON_DETECTE;
            delay(30);
            return SUR_TAG_NON_DETECTE;
        }
    }

    bool lectureTag(){
        MFRC522::StatusCode status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, block, &key, &(mfrc522.uid));
        if (status != MFRC522::STATUS_OK) {
            etatLectureTag = SUR_LECTURE_ECHOUE
            return SUR_LECTURE_ECHOUE;
        }

        status = mfrc522.MIFARE_Read(block, buffer, &size);
        if (status != MFRC522::STATUS_OK) {
            etatLectureTag = SUR_LECTURE_ECHOUE
            return SUR_LECTURE_ECHOUE;
        }
        etatLectureTag = SUR_LECTURE_SUCCEE
        return SUR_LECTURE_SUCCEE;
    }
    
    bool getPresenceTag(){
        return presenceTag;
    }

    bool getPresenceTagPresi(){
        return presenceTagPresi;
    }

    bool getLectureTag(){
        return etatLectureTag;
    }
    String getData(){
        String data = "";
        for (byte i = 0; i < size; i++){
            data += (char) buffer[i];
        }
        return "hello";
    }

    void setPresenceTag(bool val){
        presenceTag = val;
    }

    void setPresenceTagPresi(bool val){
        presenceTagPresi = val;
    }

    void setLectureTag(bool val){
        etatLectureTag = val;
    }
};


#endif
#ifndef WIFI_ADAPTATEUR_H
#define WIFI_ADAPTATEUR_H

#include <WiFi.h>
#include "../BSP.h"

class WiFi_Adaptateur
{
private:
    bool wifiConnecte = false;
    bool donneeEnvoyee = false;

    const char* ssid = SSID;
    const char* password = PASSWORD;
    const char* serverUrl = SERVER_URL;

public:
    WiFi_Adaptateur() {}
    ~WiFi_Adaptateur() {}

    void init() {
        WiFi.begin(ssid, password);
        while (WiFi.status() != WL_CONNECTED) {
            delay(1000);
        }
        wifiConnecte = SUR_WIFI_CONNECTE;
    }

    bool estConnecte() {
        wifiConnecte = (WiFi.status() == WL_CONNECTED);
        return wifiConnecte;
    }

    bool envoyerDonnees(const String& donnees) {
        if (WiFi.status() != WL_CONNECTED) {
            wifiConnecte = false;
            return false;
        }

        WiFiClient client;
        if (client.connect(serverUrl, 80)) {
            client.println("POST /endpoint HTTP/1.1");
            client.println("Host: your.server.url");
            client.println("Content-Type: application/json");
            client.print("Content-Length: ");
            client.println(donnees.length());
            client.println();
            client.println(donnees);

            int timeout = 5000; // 5 seconds timeout
            long int startTime = millis();
            while (client.available() == 0) {
                if (millis() - startTime > timeout) {
                    client.stop();
                    donneeEnvoyee = false;
                    return false;
                }
            }

            while (client.available()) {
                String line = client.readStringUntil('\r');
                if (line.indexOf("200 OK") != -1) {
                    donneeEnvoyee = true;
                    return true;
                }
            }
            donneeEnvoyee = false;
            return false;
        } else {
            donneeEnvoyee = false;
            return false;
        }
    }

    bool getWifiConnecte() {
        return wifiConnecte;
    }

    bool getDonneeEnvoyee() {
        return donneeEnvoyee;
    }

    void setWifiConnecte(bool val) {
        wifiConnecte = val;
    }

    void setDonneeEnvoyee(bool val) {
        donneeEnvoyee = val;
    }
};

#endif
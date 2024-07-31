#ifndef GESTIONNEURDONNEES
#define GESTIONNEURDONNEES

#include<queue>
#include<string.h>

#include "../BSP.h"

class GestionneurDonnees
{
private:
    std::queue<String> operationsQueue;
    std::queue<String> maintenanceQueue;

public:
    GestionneurDonnees()
    {

    }
    ~GestionneurDonnees()
    {

    }

    void ajouterOperation(String operation){
        operationsQueue.push(operation);
    }
    void ajouterInterventionMaintenance(String intervention){
        maintenanceQueue.push(intervention);
    }
    bool envoyerOperation(){
        if (!operationsQueue.empty()) {
            String operation = operationsQueue.front();
            operationsQueue.pop();
            return true;  
        }
        return false;  
    }
    bool envoyerInterventionMaintenance(){
        if (!maintenanceQueue.empty()) {
            String maintenance = maintenanceQueue.front();
            maintenanceQueue.pop();
            return true;  
        }
        return false;  
    }

    bool validerQueueOperationEstVide(){
        return operationsQueue.empty();
    }

    bool validerQueueInterventionEstVide(){
        return maintenanceQueue.empty();
    }
};

#endif
#ifndef LEDFACTORY_H
#define LEDFACTORY_H

#include<memory>

#include"Led.h"
#include"LedGeneral.h"
#include"BiCouleurLed.h"



class LedFactory
{
private:
    
public:
    
    enum LedType{
        GENERALE,
        BICOULEUR
    };

    Led  *createLed(LedType  type){
        switch (type)
        {
        case GENERALE:
                
            break;
        
        case BICOULEUR:
            /* code */
            break;

        default:
            break;
        }
    }

    


};


#endif
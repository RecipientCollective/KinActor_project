//
//  actor.h
//  KinectTor
//
//  Created by drambald on 7/31/12.
//  Copyright (c) 2012 reicipient.cc All rights reserved.
//


/*
 * THE CLASS ACTOR STORE MULTIPLE INFO ON THE ACTOR 
 * (blob, skeleton, etc...)
 */

#ifndef KinectTor_actor_h
#define KinectTor_actor_h

#include <iostream>
#include "functions.h"

class actor
{
public:
    // costruttori
    actor();
    actor(const actor &other);
    
    // UniqueID
    std::string  code;
    
    // update
    void update();
    bool updated;
        
    // cleanup
    void clean();
};


#endif

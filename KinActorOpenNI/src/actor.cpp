//
//  actor.cpp
//  KinectTor
//
//  Created by drambald on 7/31/12.
//  Copyright (c) 2012 reicipient.cc All rights reserved.
//

#include "actor.h"

// COSTRUTTORE
actor::actor()
{
    // an actor have a CODE, a FLAG and a BLOB
	code = randomString(10, true, false, false);
    updated = true;

}


/////////////////////////////////////////////////////////////////
// copiatore
/////////////////////////////////////////////////////////////////
actor::actor(const actor &other)
{    
    // it's a copy ...
	code = other.code;
	updated = other.updated;
}

// UPDATE
void actor::update()
{
    updated = true;
}

// CLEAN
void actor::clean()
{
#ifdef DEBUG
    std::cerr << "Erasing routine called for actor: " << code << std::endl;
#endif
}
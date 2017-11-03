//
//  ForceTorque.h
//  Stonefish
//
//  Created by Patryk Cieslak on 30/10/2017.
//  Copyright (c) 2017 Patryk Cieslak. All rights reserved.
//

#ifndef __Stonefish_ForceTorque__
#define __Stonefish_ForceTorque__

#include "SimpleSensor.h"
#include "Joint.h"

class ForceTorque : public SimpleSensor
{
public:
    ForceTorque(std::string uniqueName, Joint* j, SolidEntity* attachment, const btTransform& location, btScalar frequency = btScalar(-1.), unsigned int historyLength = 0);
    
    void InternalUpdate(btScalar dt);
    void Reset();
    void SetRange(const btVector3& forceMax, const btVector3& torqueMax);
    void SetNoise(btScalar forceStdDev, btScalar torqueStdDev);
    
private:
    Joint* joint;
    SolidEntity* attach;
    btTransform g2s;
};

#endif

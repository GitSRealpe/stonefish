//
//  SimpleSensor.h
//  Stonefish
//
//  Created by Patryk Cieslak on 1/4/13.
//  Copyright (c) 2013-2017 Patryk Cieslak. All rights reserved.
//

#ifndef __Stonefish_SimpleSensor__
#define __Stonefish_SimpleSensor__

#include "UnitSystem.h"
#include "NameManager.h"
#include "Sample.h"
#include "Sensor.h"

typedef enum {QUANTITY_LENGTH, QUANTITY_ANGLE, QUANTITY_VELOCITY, QUANTITY_ANGULAR_VELOCITY, QUANTITY_ACCELERATION,
              QUANTITY_FORCE, QUANTITY_TORQUE, QUANTITY_CURRENT, QUANTITY_PRESSURE, QUANTITY_UNITLESS, QUANTITY_INVALID} QuantityType;

struct SensorChannel
{
    std::string name;
    QuantityType type;
    btScalar stdDev;
    std::normal_distribution<btScalar> noise;
    btScalar rangeMin;
    btScalar rangeMax;
    
    SensorChannel(std::string name_, QuantityType type_) : name(name_), type(type_), stdDev(0), rangeMin(-BT_LARGE_FLOAT), rangeMax(BT_LARGE_FLOAT)
    {
    }    
        
    void setStdDev(btScalar sd)
    {
        if(sd > btScalar(0))
        {
            stdDev = sd;
            noise = std::normal_distribution<btScalar>(btScalar(0), stdDev);
        }
    }
};

//Abstract class
class SimpleSensor : public Sensor
{
public:
    SimpleSensor(std::string uniqueName, btScalar frequency = btScalar(-1.), unsigned int historyLength = 0);
    virtual ~SimpleSensor();
    
	virtual void Reset();
    void ClearHistory();
    void SaveMeasurementsToTextFile(const char* path, bool includeTime = true, unsigned int fixedPrecision = 6);
    void SaveMeasurementsToOctaveFile(const char* path, bool includeTime = true, bool separateChannels = false);
    unsigned short getNumOfChannels();
	Sample getLastSample();
    const std::deque<Sample*>& getHistory();
    btScalar getValueExternal(unsigned long int index, unsigned int channel);
    btScalar getLastValueExternal(unsigned int channel);
    SensorChannel getSensorChannelDescription(unsigned int channel);
	SensorType getType();
	
	//Abstract
	virtual void InternalUpdate(btScalar dt) = 0;
    
protected:
    void AddSampleToHistory(const Sample& s);
    std::deque<Sample*> history;
    std::vector<SensorChannel> channels;
    
private:
    unsigned int historyLen;
};

#endif

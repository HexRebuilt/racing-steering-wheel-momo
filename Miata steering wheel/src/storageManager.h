#include <EEPROM.h>

class StorageManager
{
private:
    short timezone = 0;
    short nightLightValue = 0;
    short dayLightValue = 100;
    short oilResistanceValue = 1000;
    short waterResistanceValue = 1000; // ohm

    enum EEpromAddresses : uint8_t
    {
        timezoneAddress,
        nightLightValueAddress,
        dayLightValueAddress,
        oilResistanceValueAddress,
        waterResistanceValueAddress,
        totalvalues
    };
    
    EEpromAddresses addresses;
    
    int memoryPostition[totalvalues] = {0};

    /**
     * function that calculates and stores the memory position of each value stored in the eeprom
     * depending on their type
     * are all short for praticuty
     * 
     */
    void CalculateMemoryPosition(){
        addresses = timezoneAddress;
        memoryPostition[addresses]=0;  
        for (int i = 1; i < totalvalues; i++)
        {
            memoryPostition[i] = memoryPostition[i -1] + sizeof (short);
        }
    }
    

public:
    StorageManager(/* args */);
    ~StorageManager();

    void Startup()
    {
        CalculateMemoryPosition();
        EEPROM.get(memoryPostition[timezoneAddress], timezone); // get the current timezone value from the eeprom
        /*
        EEPROM.get(memoryPostition[nightLightValueAddress], nightLightValue); // get the current timezone value from the eeprom
        EEPROM.get(memoryPostition[dayLightValueAddress], dayLightValue); // get the current timezone value from the eeprom
        EEPROM.get(memoryPostition[oilResistanceValueAddress], oilResistanceValue); // get the current timezone value from the eeprom
        EEPROM.get(memoryPostition[waterResistanceValueAddress], waterResistanceValue); // get the current timezone value from the eeprom
        */
       Serial.print("timezone readed: ");
       Serial.println(timezone);
    }

    void SetTimeZone(short value)
    {
        Serial.print("timezone saved: ");
        Serial.println(value);
        timezone = value;
        EEPROM.put(memoryPostition[timezoneAddress], value);
    }

    short GetTimeZone()
    {
        return timezone;
    }
};

StorageManager::StorageManager(/* args */)
{
}

StorageManager::~StorageManager()
{
    Startup();
}

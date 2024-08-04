#ifndef DEVICESETTINGSMANAGER_H
#define DEVICESETTINGSMANAGER_H

#include <msp430.h>
#include "DeviceSettings.h"
#include "DataStore.h"
#include "AppConfig.h"

class DeviceSettingsManager {
public:
    DeviceSettingsManager() : dataStore(appconfigDEVICE_SETTINGS_BASE_ADDRESS) {}

    DeviceSettings get() const
    {
        return dataStore.get();
    }

    void save(const DeviceSettings& settings)
    {
        dataStore.store(settings);
    }

    void load()
    {
        dataStore.load();
    }

private:
    DataStore<DeviceSettings> dataStore;
};

#endif // DEVICESETTINGSMANAGER_H

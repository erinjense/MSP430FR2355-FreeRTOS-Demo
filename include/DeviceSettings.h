
#ifndef DEVICESETTINGS_H
#define DEVICESETTINGS_H

struct DeviceSettings
{
    uint32_t deviceID;
    uint32_t appVersion;

    DeviceSettings()
        : deviceID(0),
          appVersion(0) {}

    DeviceSettings(uint32_t id, uint32_t appVersion)
        : deviceID(id),
          appVersion(appVersion) {}
};

#endif // DEVICESETTINGS_H

#ifndef DATASTORE_H
#define DATASTORE_H

#include <msp430.h>
#include <cstring>

template<typename T>
class DataStore {
public:
    DataStore(BaseType_t * const baseAddress) : baseAddress(baseAddress)
    {
        load();
    }

    bool load()
    {
        std::memcpy(&data, (const void*)baseAddress, sizeof(T));
        return true;
    }

    bool store(const T& newData)
    {
        data = newData;
        FRAMCtl_write8((uint8_t*)&data, (uint8_t*)baseAddress, sizeof(T));
        return true;
    }

    T get() const
    {
        return data;
    }

private:
    T data;
    BaseType_t * const baseAddress;
};

#endif // DATASTORE_H

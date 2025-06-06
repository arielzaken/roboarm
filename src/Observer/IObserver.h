// IObserver.h
#pragma once

// Generic observer interface
template<typename T>
class IObserver {
public:
    virtual ~IObserver() = default;
    virtual void onNotify(const T& value) = 0;
};


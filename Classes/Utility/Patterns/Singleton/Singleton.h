#pragma once

template <class T>
class Singleton {
public:
    static T* getInstance() {
        if (instance == nullptr) {
            instance = new T;
        }

        return instance;
    }

    static bool hasInstance() {
        return instance != nullptr;
    }

    virtual void resetInstance() {
        if (instance != nullptr) {
            delete instance;
        }
        instance = nullptr;
    }

protected:
    Singleton() {
        instance = nullptr;
    }

    virtual ~Singleton() {
        instance = nullptr;
    }

protected:
    static T* instance;
};

template <class T>
T* Singleton<T>::instance = nullptr;

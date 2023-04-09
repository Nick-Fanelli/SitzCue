#pragma once

#include "sitzcuepch.h"

namespace SitzCue {

    template<typename T>
    class Ref {

    public:
        Ref() {
            m_InstanceCount = new uint32_t(1);
            m_IsNullptr = new bool(true);
        }

        Ref(T* dataPtr) : m_Data(dataPtr), m_InstanceCount(new uint32_t(1)), m_IsNullptr(new bool(false)) {}

        // Copy Constructor
        Ref(const Ref& other) : m_Data(other.m_Data), m_InstanceCount(other.m_InstanceCount), m_IsNullptr(other.m_IsNullptr) {
            (*m_InstanceCount)++;
        }

        // Copy Assignment Constructor
        Ref<T>& operator=(const Ref<T>& other) {
            if(this != &other) {
                (*m_InstanceCount)--;

                if(*m_InstanceCount <= 0) {
                    delete m_Data;
                    delete m_InstanceCount;
                    delete m_IsNullptr;
                }

                m_Data = other.m_Data;
                m_InstanceCount = other.m_InstanceCount;
                m_IsNullptr = other.m_IsNullptr;
                (*m_InstanceCount)++;
            }

            return *this;
        }

        // Destructor
        ~Ref() {
            (*m_InstanceCount)--;
            if(*m_InstanceCount == 0) {
                delete m_Data;
                delete m_InstanceCount;
                delete m_IsNullptr;
            }
        }

        T& operator*() const { return *m_Data; }
        T* operator->() const { return IsNull() ? nullptr : m_Data; }

        T* get() const { 
            return IsNull() ? nullptr : m_Data; 
        }

        bool IsNull() const { return *m_IsNullptr; }
        uint32_t GetInstanceCount() const { return m_InstanceCount; }

        void DeleteData() { *m_IsNullptr = true; }

        void SetValueIfSafe(const T& newValue) {
            (*m_Data) = newValue;
            *m_IsNullptr = false;
        }

    private:
        T* m_Data = nullptr;
        uint32_t* m_InstanceCount;
        bool* m_IsNullptr = nullptr;
    };

    template<typename Type, typename... Args>
    inline Ref<Type> CreateRef(Args&&... args) {
        Type* data = new Type(std::forward<Args>(args)...);
        return Ref<Type>(data);
    }

}
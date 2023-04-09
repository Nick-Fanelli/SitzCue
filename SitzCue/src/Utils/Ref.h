#pragma once

#include "sitzcuepch.h"

namespace SitzCue {

    template<typename T>
    class Ref {

    public:
        Ref() {
            m_InstanceCount = new uint32_t(1);
        }

        Ref(T* dataPtr) : m_Data(dataPtr), m_InstanceCount(new uint32_t(1)) {}

        // Copy Constructor
        Ref(const Ref& other) : m_Data(other.m_Data), m_InstanceCount(other.m_InstanceCount) {
            (*m_InstanceCount)++;
        }

        // Copy Assignment Constructor
        Ref<T>& operator=(const Ref<T>& other) {
            if(this != &other) {
                (*m_InstanceCount)--;

                if(*m_InstanceCount <= 0) {
                    delete m_Data;
                    delete m_InstanceCount;
                }

                m_Data = other.m_Data;
                m_InstanceCount = other.m_InstanceCount;
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
            }
        }

        T& operator*() const { return *m_Data; }
        T* operator->() const { return m_Data; }

        T* get() const { return m_Data; }

        bool IsNull() const { return m_Data == nullptr; }
        uint32_t GetInstanceCount() const { return m_InstanceCount; }

        void DeleteData() {
            delete m_Data;
            m_Data = nullptr;
        }

        void SetValueIfSafe(const T& newValue) {
            if(m_Data != nullptr)
                (*m_Data) = newValue;
        }

    private:
        T* m_Data = nullptr;
        uint32_t* m_InstanceCount;

    };

    template<typename Type, typename... Args>
    inline Ref<Type> CreateRef(Args&&... args) {
        Type* data = new Type(std::forward<Args>(args)...);
        return Ref<Type>(data);
    }

}
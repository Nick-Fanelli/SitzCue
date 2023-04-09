#pragma once

#include "sitzcuepch.h"

namespace SitzCue {

    template<typename T>
    class Ref {

    public:
        Ref() {
            m_InstanceCount = new uint32_t(1);
        }

        template<typename... Args>
        Ref(Args&&... args) : m_InstanceCount(new uint32_t(1)) {
            m_Data = new T(std::forward<Args>(args)...);
        }

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

        operator T*() const { return m_Data; }
        operator T*() { return m_Data; }

        operator delete() {
            delete m_Data;
            m_Data = nullptr;
        }

        bool IsNull() const { return m_Data == nullptr; }
        uint32_t GetInstanceCount() const { return m_InstanceCount; }

    private:
        T* m_Data = nullptr;
        uint32_t* m_InstanceCount;

    };

}
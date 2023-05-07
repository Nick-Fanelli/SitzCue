#pragma once

#include "sitzcuepch.h"

#include "ImGuiLayer.h"

namespace SitzQ {

    class Popup;


    class PopupWindow {

        friend class Popup;

    public:
        PopupWindow() = default;

        bool IsActive() const { return m_IsActive; }

    protected:
        virtual void EnablePopup() { m_IsActive = true; }

        virtual void Update() = 0;

    protected:

        bool m_IsActive = false;

    };


    class ConfirmationPopupWindow : public PopupWindow {

    public: 
        ConfirmationPopupWindow() = default;

        ConfirmationPopupWindow(const std::string& headerText, const std::string& message)
            : m_HeaderText(headerText), m_Message(message) {}

    protected:
        void Update() override;

    private:
        std::string m_HeaderText = "Confirmation";
        std::string m_Message = "Are you sure?";

    };


    class Popup {

        friend class ImGuiLayer;

    public:
        Popup() = delete;

        template<typename PopupType, typename... Args>
        static void DisplayPopup(Args&&... args) {
            m_PopupWindow = std::make_shared<PopupType>(std::forward<Args>(args)...);
            m_PopupWindow->EnablePopup();
        }

    private:
        static void Update();

    private:
        static inline std::shared_ptr<PopupWindow> m_PopupWindow;

    };

}
#pragma once

#include <Logger.h>
#include <TObject.h>
#include <format>
#include <string>
#include <TypeRegistry.h>

#ifdef TAKO_EDITOR
#include <DebugUIManager.h>
#endif // TAKO_EDITOR

namespace GameLog
{
    enum class Level { Info, Warning, Error };

    inline void Write(Level level, const Tako::TObject& self, const std::string& message)
    {
        const std::string text = std::format("[{}] {}", self.GetType().name, message);
        #ifdef TAKO_EDITOR
        using T = Tako::DebugUIManager::LogType;
        const T type = (level == Level::Info) ?
            T::Info : (level == Level::Warning) ?
                T::Warning : T::Error;

        Tako::LogToConsole(text, type);
        #else
        Tako::Logger::Log(text + '\n');
        #endif
    }

    template <typename... Args>
    inline void Error(const Tako::TObject& self, std::format_string<Args...> format, Args&&... args)
    {
        Write(Level::Error, self, std::format(format, std::forward<Args>(args)...));
    }

    template <typename... Args>
    inline void Warning(const Tako::TObject& self, std::format_string<Args...> format, Args&&... args)
    {
        Write(Level::Warning, self, std::format(format, std::forward<Args>(args)...));
    }

    template <typename... Args>
    inline void Info(const Tako::TObject& self, std::format_string<Args...> format, Args&&... args)
    {
        Write(Level::Info, self, std::format(format, std::forward<Args>(args)...));
    }
}



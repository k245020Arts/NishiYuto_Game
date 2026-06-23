#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <mutex>
#include <ctime>
#include "../Singleton/SingletonBase.h"

enum class LogLevel {
    INFO,
    WARNING,
    ERROR_LOG,
};

/// <summary>
/// デバックのログを外部のTextデータに出力をするためのクラス
/// </summary>
class DebugLogText  : public SingletonBase<DebugLogText>
{
public:
   
    void Log(LogLevel level, const std::string& text) {
        std::lock_guard<std::mutex> lock(mtx);

        std::string line =
            "[" + GetTime() + "][" + LevelToString(level) + "] " + text;

#ifdef _DEBUG
        // VisualStudio の出力ウィンドウにも出す
        std::cout << line << std::endl;
#endif

        buffer.push_back(line);

        if (buffer.size() >= BUFFER_LIMIT) {
            Flush();
        }
    }

    void Flush() {
        if (!file.is_open()) return;

        for (const auto& line : buffer) {
            file << line << std::endl;
        }

        buffer.clear();
    }
private:
    std::ofstream file;
    std::vector<std::string> buffer;
    std::mutex mtx;

    const size_t BUFFER_LIMIT = 100; // 溜まったら書き出す

    DebugLogText() {
        file.open("debug.txt", std::ios::app);
    }

    DebugLogText(const std::string& filename) {
        file.open(filename, std::ios::app);
    }

    ~DebugLogText() {
        Flush(); // 終了時に全部吐く
        if (file.is_open()) file.close();
    }

    std::string GetTime() {
        std::time_t now = std::time(nullptr);

        std::tm localTime;
        localtime_s(&localTime, &now);

        char buf[64];
        std::strftime(buf, sizeof(buf), "%H:%M:%S", &localTime);

        return std::string(buf);
    }

    std::string LevelToString(LogLevel level) {
        switch (level) {
        case LogLevel::INFO: return "INFO";
        case LogLevel::WARNING: return "WARN";
        case LogLevel::ERROR_LOG: return "ERROR";
        }
        return "";
    }
    friend class SingletonBase<DebugLogText>;
   
};
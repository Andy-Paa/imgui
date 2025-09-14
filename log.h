//log.h
//A simple log system with file output and ImGui display
//log levels:   INFO, WARNING, ERROR
//usage:        Log::log(Log::INFO, "message");
//quick func:   LOG_INFO("message");
//default output file: */build/Debug/app.log
//checked the StackOverflow and runoob for general help during implementation
//using imgui
#pragma once
#include <string>
#include <string_view>
#include <vector>
#include <fstream>
#include <mutex>
#include <iostream>
#include "imgui/imgui.h"

class Log {
public:
    //levels
    enum Level {
        INFO = 0,
        WARNING = 1,
        ERROR = 2
    };

    //default path
    static void init(std::string path = "app.log") {
        std::lock_guard<std::mutex> lock(mutex());
        if (ofs().is_open()) ofs().close();
        ofs().open(path, std::ios::out | std::ios::app);
        //check
    }

    //log func
    static void log(Level level, std::string_view message) {
        const char* tag = levelToTag(level);

        //for console
        std::cout << "[" << tag << "]" << message << std::endl;

        //write to file
        {
            std::lock_guard<std::mutex> lock(mutex());
            if (!ofs().is_open()) {
                //default to app.log
                ofs().open("app.log", std::ios::out | std::ios::app);
            }
            if (ofs().is_open()) {
                ofs() << "[" << tag << "]" << message << "\n";
                ofs().flush();
            }
        }
        //for imgui
        {
            std::lock_guard<std::mutex> lock(mutex());
            lines().push_back({std::string(message), level});
        }
    }

    //quick func
    static void info(std::string_view message)   { log(INFO, message); }
    static void warn(std::string_view message)   { log(WARNING, message); }
    static void error(std::string_view message)  { log(ERROR, message); }

    //log imgui
    static void DrawImGui(const char* title) {
        if (ImGui::Begin(title)) {
            //lock
            std::lock_guard<std::mutex> lock(mutex());
            //filter
            static int filterLevel = ERROR;
            ImGui::Combo("Level Filter", &filterLevel, "INFO\0WARNING\0ERROR\0");

            //logs area
            ImGui::BeginChild("LogArea", ImVec2(0, 0), true, ImGuiWindowFlags_AlwaysVerticalScrollbar);

            for (const auto& item : lines()) {
              if (item.level < filterLevel) continue;
              ImVec4 color = levelToColor(item.level);
              ImGui::PushStyleColor(ImGuiCol_Text, color);
              ImGui::TextUnformatted(item.text.c_str());
              ImGui::PopStyleColor();
            }
            //newest logs at bottom
            if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
                ImGui::SetScrollHereY(1.0f);
            ImGui::EndChild();
        }
        ImGui::End();
    }

private:
    //log line struct
    struct Line {
        std::string text;
        Level level;
    };

    //statics
    static std::vector<Line>& lines() {
        static std::vector<Line> s_lines;
        return s_lines;
    }

    static std::ofstream& ofs() {
        static std::ofstream s_ofs;
        return s_ofs;
    }

    static std::mutex& mutex() {
        static std::mutex s_mutex;
        return s_mutex;
    }

    static const char* levelToTag(Level lvl) {
        switch (lvl) {
            case INFO:    return "INFO";
            case WARNING: return "WARNING";
            case ERROR:   return "ERROR";
            default:      return "INFO";
        }
    }
    //color for imgui outputs
    static ImVec4 levelToColor(Level lvl) {
        switch (lvl) {
            case INFO:    return ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
            case WARNING: return ImVec4(1.0f, 1.0f, 0.0f, 1.0f);
            case ERROR:   return ImVec4(1.0f, 0.3f, 0.3f, 1.0f);
            default:      return ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
        }
    }
};

//quick func
#define LOG_INFO(x)    Log::log(Log::INFO,    (x))
#define LOG_WARNING(x) Log::log(Log::WARNING, (x))
#define LOG_ERROR(x)   Log::log(Log::ERROR,   (x))

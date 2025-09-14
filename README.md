A simple log system with file output and ImGui display

using ImGui

Usage:

In your .cpp
put:
#include "log.h" //log system

for logs:
Log::log(Log::INFO, "message");
LOG_INFO("message");

log levels:
INFO, WARNING, ERROR

ImGui window:
Log::DrawImGui("LOGS"); //draw log window

default output file: */build/Debug/app.log

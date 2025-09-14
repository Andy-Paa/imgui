#include "Application.h"
#include "imgui/imgui.h"
#include "log.h" //log system
#include <string>

namespace ClassGame {
        //
        // our global variables
        //
        //
        // game starting point
        // this is called by the main render loop in main.cpp
        //
        void GameStartUp() 
        {
                //standard log
                Log::log(Log::INFO, "Started logging");
                
                //quick log
                LOG_WARNING("Memory allocated");
                LOG_ERROR("File not found");
                LOG_INFO("updated log level");
        }

        //
        // game render loop
        // this is called by the main render loop in main.cpp
        //
        void RenderGame() 
        {
            ImGui::DockSpaceOverViewport();
//            ImGui::ShowDemoWindow();

                static float f = 0.0f;
                static int counter = 0;
                bool show_demo_window = true;
                bool show_another_window = false;
                ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

                ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

                ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
                ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
                ImGui::Checkbox("Another Window", &show_another_window);

                ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
                ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

                if (ImGui::Button("Button")) {
                        counter++;
                        //logs tests
                        Log::log(Log::INFO, "Button clicked, counter = " + std::to_string(counter));
                        Log::log(Log::WARNING, "Button clicked, counter = " + std::to_string(counter));
                        Log::log(Log::ERROR, "Button clicked, counter = " + std::to_string(counter));
                }
                ImGui::SameLine();
                ImGui::Text("counter = %d", counter);
                Log::DrawImGui("LOGS"); //draw log window
                ImGui::End();
        }

        //
        // end turn is called by the game code at the end of each turn
        // this is where we check for a winner
        //
        void EndOfTurn() 
        {
        }
}

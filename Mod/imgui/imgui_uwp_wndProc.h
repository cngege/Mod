#pragma once

#include "imgui.h"

ImGuiMouseSource GetMouseSourceFromMessageExtraInfo();

void ImGui_ImplUWP_AddKeyEvent(ImGuiKey key, bool down, int native_keycode, int native_scancode);
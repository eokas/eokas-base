#ifndef _EOKAS_DATAPOT_UI_UTILS_H_
#define _EOKAS_DATAPOT_UI_UTILS_H_

#include "imgui/imgui.h"
#include "native/utils.h"

ImVec2 operator+(const ImVec2& lhs, const ImVec2& rhs);
ImVec2 operator*(const ImVec2& lhs, const ImVec2& rhs);
bool operator==(const ImVec4& lhs, const ImVec4& rhs);
bool operator!=(const ImVec4& lhs, const ImVec4& rhs);

#endif//_EOKAS_DATAPOT_UI_UTILS_H_

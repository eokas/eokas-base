
#include "./utils.h"

namespace eokas::datapot {

}

ImVec2 operator+(const ImVec2& lhs, const ImVec2& rhs)
{
    return ImVec2(lhs.x + rhs.x, lhs.y + rhs.y);
}

ImVec2 operator*(const ImVec2& lhs, const ImVec2& rhs)
{
    return ImVec2(lhs.x * rhs.x, lhs.y * rhs.y);
}

bool operator==(const ImVec4& lhs, const ImVec4& rhs)
{
    return memcmp(&lhs, &rhs, sizeof(ImVec4)) == 0;
}

bool operator!=(const ImVec4& lhs, const ImVec4& rhs)
{
    return memcmp(&lhs, &rhs, sizeof(ImVec4)) != 0;
}


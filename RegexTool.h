#pragma once

#include <regex.h>
#include <cstddef>

class RegexTool
{
public:
    static int Match(const char *string, const char *pattern);
    static int MatchIgnoreCase(const char *string, const char *pattern);
};

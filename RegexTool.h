#pragma once

#include <regex.h>

class RegexTool
{
public:
    static int Match(const char *string, const char *pattern);
    static int MatchIgnoreCase(const char *string, const char *pattern);
};

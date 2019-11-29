#include "RegexTool.h"

int RegexTool::Match(const char* string, const char* pattern)
{
    regex_t re;
    if (regcomp(&re, pattern, REG_EXTENDED|REG_NOSUB) != 0)
    {
        return -1;
    }

    int status = regexec(&re, string, 0, NULL, 0);
    regfree(&re);

    if (status != 0)
    {
        return 0;
    }

    return 1;
}

int RegexTool::MatchIgnoreCase(const char* string, const char* pattern)
{
    regex_t re;
    if (regcomp(&re, pattern, REG_EXTENDED|REG_ICASE|REG_NOSUB) != 0)
    {
        return -1;
    }

    int status = regexec(&re, string, 0, NULL, 0);
    regfree(&re);

    if (status != 0)
    {
        return 0;
    }

    return 1;
}

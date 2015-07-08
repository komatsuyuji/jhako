/////////////////////////////////////////////////////////////////////////////////
//
// jHako
// Copyright (C) 2014-2015 Komatsu Yuji(Zheng Chuyu)
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
/////////////////////////////////////////////////////////////////////////////////

#include <regex.h>
#include <csv.h>
#include "common.h"

/////////////////////////////////////////////////////////////////////////////////
//
// Function:
//
// Purpose:
//
// Parameters:
//
// Return value:
//
// Author: Komatsu Yuji(Zheng Chuyu)
//
/////////////////////////////////////////////////////////////////////////////////
void jhk_free(void *p)
{
    if (p != NULL)
        free(p);
    p = NULL;
}

/////////////////////////////////////////////////////////////////////////////////
//
// Function:
//
// Purpose:
//
// Parameters:
//
// Return value:
//
// Author: Komatsu Yuji(Zheng Chuyu)
//
/////////////////////////////////////////////////////////////////////////////////
apr_size_t jhk_strncat(char *desc, char *src, apr_size_t s)
{
    apr_size_t len, n;

    if (desc == NULL || src == NULL)
        return -1;

    len = strlen(src);
    n = s - 1 - strlen(desc);
    if (len < n)
        n = len;
    strncat(desc, src, n);

    return n;
}

/////////////////////////////////////////////////////////////////////////////////
//
// Function:
//
// Purpose:
//
// Parameters:
//
// Return value:
//
// Author: Komatsu Yuji(Zheng Chuyu)
//
/////////////////////////////////////////////////////////////////////////////////
char *jhk_timestamp(void)
{
    // YYYY-mm-dd HH:MM:SS.sss
    static char str[24];
    apr_time_t now;
    apr_time_exp_t xt;

    now = apr_time_now();
    apr_time_exp_lt(&xt, now);
    sprintf(str,
            "%04d-%02d-%02d %02d:%02d:%02d.%03d",
            xt.tm_year + 1900,
            xt.tm_mon + 1,
            xt.tm_mday,
            xt.tm_hour, xt.tm_min, xt.tm_sec, xt.tm_usec / 1000);

    return str;
}

/////////////////////////////////////////////////////////////////////////////////
//
// Function:
//
// Purpose:
//
// Parameters:
//
// Return value:
//
// Author: Komatsu Yuji(Zheng Chuyu)
//
/////////////////////////////////////////////////////////////////////////////////
char *jhk_time(void)
{
    // YYYY-mm-dd HH:MM:SS
    static char str[9];
    time_t now;
    struct tm *tm = NULL;

    now = time(NULL);
    tm = localtime(&now);
    sprintf(str, "%04d-%02d-%02d %02d:%02d:%02d", tm->tm_year + 1900,
            tm->tm_mon + 1, tm->tm_mday, tm->tm_hour, tm->tm_min,
            tm->tm_sec);

    return str;
}

/////////////////////////////////////////////////////////////////////////////////
//
// Function:
//
// Purpose:
//
// Parameters:
//
// Return value:
//
// Author: Komatsu Yuji(Zheng Chuyu)
//
/////////////////////////////////////////////////////////////////////////////////
char *jhk_today(void)
{
    // YYYYmmdd
    static char str[9];
    time_t now;
    struct tm *tm = NULL;

    now = time(NULL);
    tm = localtime(&now);
    sprintf(str, "%04d%02d%02d", tm->tm_year + 1900, tm->tm_mon + 1,
            tm->tm_mday);

    return str;
}

/////////////////////////////////////////////////////////////////////////////////
//
// Function:
//
// Purpose:
//
// Parameters:
//
// Return value:
//
// Author: Komatsu Yuji(Zheng Chuyu)
//
/////////////////////////////////////////////////////////////////////////////////
char *jhk_time2str(const time_t t)
{
    // YYYY-mm-dd HH:MM:SS
    char *str;
    struct tm *tm = NULL;

    str = (char *) malloc(20);
    tm = localtime(&t);
    snprintf(str, 20, "%04d-%02d-%02d %02d:%02d:%02d", tm->tm_year + 1900,
             tm->tm_mon + 1, tm->tm_mday, tm->tm_hour, tm->tm_min,
             tm->tm_sec);

    return str;
}

/////////////////////////////////////////////////////////////////////////////////
//
// Function:
//
// Purpose:
//
// Parameters:
//
// Return value:
//
// Author: Komatsu Yuji(Zheng Chuyu)
//
/////////////////////////////////////////////////////////////////////////////////
time_t jhk_midnight(time_t t)
{
    struct tm *tm = NULL;

    tm = localtime(&t);
    tm->tm_hour = 0;
    tm->tm_min = 0;
    tm->tm_sec = 0;

    return mktime(tm);
}

/////////////////////////////////////////////////////////////////////////////////
//
// Function:
//
// Purpose:
//
// Parameters:
//
// Return value:
//
// Author: Komatsu Yuji(Zheng Chuyu)
//
/////////////////////////////////////////////////////////////////////////////////
int jhk_time2day(time_t t)
{
    int day;
    struct tm *tm = NULL;

    tm = localtime(&t);
    day =
        (tm->tm_year + 1900) * 10000 + (tm->tm_mon + 1) * 100 +
        tm->tm_mday;

    return day;
}

/////////////////////////////////////////////////////////////////////////////////
//
// Function:
//
// Purpose:
//
// Parameters:
//
// Return value:
//
// Author: Komatsu Yuji(Zheng Chuyu)
//
/////////////////////////////////////////////////////////////////////////////////
char *jhk_csv(const char *str)
{
    char *csv;
    apr_size_t len;

    if (str == NULL) {
        csv = strdup("");
        return csv;
    }

    len = strlen(str);
    csv = (char *) malloc(2 * len + 3);
    memset(csv, 0, 2 * len + 3);
    csv_write(csv, 2 * len + 2, str, strlen(str));

    return csv;
}

/////////////////////////////////////////////////////////////////////////////////
//
// Function:
//
// Purpose:
//
// Parameters:
//
// Return value:
//
// Author: Komatsu Yuji(Zheng Chuyu)
//
/////////////////////////////////////////////////////////////////////////////////
char *jhk_dformat(const char *fmt, va_list va)
{
    char *str = NULL;
    int n, size;
    va_list va_tmp;

    size = MAX_STRING_LEN;
    while (1) {
        str = (char *) malloc(size * sizeof(char));
        va_copy(va_tmp, va);
        n = vsnprintf(str, size, fmt, va_tmp);
        va_end(va_tmp);
        if (n < 0) {
            jhk_free(str);
            return NULL;
        } else if (n < size) {
            break;
        } else if (n == size) {
            size = n + 1;
        } else {
            size = size + MAX_STRING_LEN;
        }

        jhk_free(str);
    }

    return str;
}

/////////////////////////////////////////////////////////////////////////////////
//
// Function:
//
// Purpose:
//
// Parameters:
//
// Return value:
//
// Author: Komatsu Yuji(Zheng Chuyu)
//
/////////////////////////////////////////////////////////////////////////////////
char *jhk_trim(const char *str)
{
    int len;
    const char *p;
    char *nstr, *q;

    if (str == NULL)
        return NULL;

    len = strlen(str);
    nstr = (char *) malloc(len + 1);

    p = str;
    q = nstr;
    while (*p != '\0') {
        if (*p == ' ') {
            p++;
            continue;
        }
        *q++ = *p++;
    }
    *q = '\0';

    return nstr;
}

/////////////////////////////////////////////////////////////////////////////////
//
// Function:
//
// Purpose:
//
// Parameters:
//
// Return value:
//
// Author: Komatsu Yuji(Zheng Chuyu)
//
/////////////////////////////////////////////////////////////////////////////////
int jhk_aoti(const char *str)
{
    int num;
    char nstr[20];

    num = atoi(str);
    snprintf(nstr, sizeof(nstr), "%d", num);

    if (strcmp(str, nstr) == 0) {
        return num;
    }

    return -1;
}

/////////////////////////////////////////////////////////////////////////////////
//
// Function:
//
// Purpose:
//
// Parameters:
//
// Return value:
//
// Author: Komatsu Yuji(Zheng Chuyu)
//
/////////////////////////////////////////////////////////////////////////////////
int jhk_on_range(const int num, const char *range)
{
    int rv;
    char *fmt_range, *pos;
    int min, max;

    jhklog_trace("In %s()", __func__);
    if (range == NULL)
        return JHAKO_ERROR;

    // init
    rv = JHAKO_ERROR;
    min = -1;
    max = -1;

    // remove space from range
    fmt_range = jhk_trim(range);
    pos = strstr(fmt_range, "-");
    if (pos == NULL) {
        // range: max
        max = jhk_aoti(fmt_range);
        if (max < 0) {
            jhklog_warn("In %s() incorrect range: %s", __func__, range);
            goto error;
        }
        if (num == max) {
            jhklog_trace("In %s() %d == %d", __func__, num, max);
            rv = JHAKO_TRUE;
        } else {
            jhklog_trace("In %s() %d is not == %d", __func__, num, max);
            rv = JHAKO_FALSE;
        }
    } else if (pos == fmt_range) {
        // range: - max
        max = jhk_aoti(pos + 1);
        if (max < 0) {
            jhklog_warn("In %s() incorrect range: %s", __func__, range);
            goto error;
        }
        if (num <= max) {
            jhklog_trace("In %s() %d <= %d", __func__, num, max);
            rv = JHAKO_TRUE;
        } else {
            jhklog_trace("In %s() %d is not <= %d", __func__, num, max);
            rv = JHAKO_FALSE;
        }
    } else if (pos == fmt_range + strlen(fmt_range) - 1) {
        // range: min -
        *pos = '\0';
        min = jhk_aoti(fmt_range);
        if (min < 0) {
            jhklog_warn("In %s() incorrect range: %s", __func__, range);
            goto error;
        }
        if (num >= min) {
            jhklog_trace("In %s() %d >= %d", __func__, num, min);
            rv = JHAKO_TRUE;
        } else {
            jhklog_trace("In %s() %d is not >= %d", __func__, num, min);
            rv = JHAKO_FALSE;
        }
    } else {
        // range: min - max
        *pos = '\0';
        min = jhk_aoti(fmt_range);
        max = jhk_aoti(pos + 1);
        if (min < 0 || max < 0) {
            jhklog_warn("In %s() incorrect range: %s", __func__, range);
            goto error;
        }
        if (num >= min && num <= max) {
            jhklog_trace("In %s() num %d is in the range: %d - %d",
                         __func__, num, min, max);
            rv = JHAKO_TRUE;
        } else {
            jhklog_trace("In %s() num %d is not in the range: %d - %d",
                         __func__, num, min, max);
            rv = JHAKO_FALSE;
        }
    }

  error:
    jhk_free(fmt_range);
    return rv;
}

/////////////////////////////////////////////////////////////////////////////////
//
// Function:
//
// Purpose:
//
// Parameters:
//
// Return value:
//
// Author: Komatsu Yuji(Zheng Chuyu)
//
/////////////////////////////////////////////////////////////////////////////////
int jhk_on_cron(const int value, const char *cron)
{
    int rv, cmp;
    char *cstr, *tp;

    jhklog_trace("In %s()", __func__);
    if (cron == NULL)
        return JHAKO_ERROR;

    jhklog_trace("In %s() value: %d, cron: %s", __func__, value, cron);
    if (strlen(cron) == 0) {
        jhklog_warn("In %s() cron is unset. %s", __func__, cron);
        return JHAKO_FALSE;
    }

    if (strcmp(cron, "*") == 0) {
        jhklog_trace("In %s() matched the pattern. value: %d, cron: %s",
                     __func__, value, cron);
        return JHAKO_TRUE;
    }

    rv = JHAKO_FALSE;
    cstr = strdup(cron);
    tp = strtok(cstr, ",");
    while (tp != NULL) {
        cmp = jhk_on_range(value, tp);
        if (cmp == JHAKO_FALSE) {
            tp = strtok(NULL, ",");
            continue;
        }

        if (cmp == JHAKO_ERROR) {
            jhklog_warn("In %s() unknown format: %s", __func__, tp);
        } else if (cmp == JHAKO_TRUE) {
            jhklog_trace
                ("In %s() matched the pattern. value: %d, format: %s",
                 __func__, value, tp);
        }
        rv = cmp;
        break;
    }

    jhk_free(cstr);
    return rv;
}

/////////////////////////////////////////////////////////////////////////////////
//
// Function:
//
// Purpose:
//
// Parameters:
//
// Return value:
//
// Author: Komatsu Yuji(Zheng Chuyu)
//
/////////////////////////////////////////////////////////////////////////////////
int jhk_on_regexp(const char *str, const char *pattern)
{
    int rv;
    regex_t re;
    regmatch_t match;

    if (str == NULL || pattern == NULL)
        return JHAKO_ERROR;

    jhklog_trace("In %s() str: %s, pattern: %s", __func__, str, pattern);

    if (regcomp(&re, pattern, REG_EXTENDED | REG_NEWLINE) != 0) {
        jhklog_warn("In %s() regcomp failed. str: %s, pattern: %s",
                    __func__, str, pattern);
        return JHAKO_ERROR;
    }

    rv = JHAKO_FALSE;
    if (regexec(&re, str, (apr_size_t) 1, &match, 0) == 0)
        rv = JHAKO_TRUE;

    regfree(&re);
    return rv;
}

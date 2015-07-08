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

#include <iconv.h>
#include "common.h"
#include "charset.h"

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
char *charset_convert(char *str, const char *from_charset,
                      const char *to_charset)
{
    apr_size_t inlen, outlen;
    iconv_t cd;
    char *buf, *pbuf;

    if (str == NULL || from_charset == NULL || to_charset == NULL)
        return NULL;

    // init
    inlen = strlen(str);
    outlen = inlen * 4 + 1;
    buf = (char *) malloc(outlen);
    memset(buf, 0, outlen);
    pbuf = buf;

    // iconv
    cd = iconv_open(to_charset, from_charset);
    if (cd == (iconv_t) - 1) {
        strncpy(buf, str, inlen);
        return buf;
    }

    if (iconv(cd, &str, &inlen, &pbuf, &outlen) == -1) {
        strncpy(buf, str, inlen);
    }

    iconv_close(cd);
    return buf;
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
int charset_get_code(const int codepage, char *code, const int n)
{
    switch (codepage) {
    case 65001:
        snprintf(code, n, "%s", "UTF-8");
        break;
    default:
        snprintf(code, n, "CP%d", codepage);
        break;
    }
    return 0;
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
char *charset_from_utf8(char *str, const int codepage)
{
    char code[64];

    charset_get_code(codepage, code, 64);
    return charset_convert(str, "UTF-8", code);
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
char *charset_to_utf8(char *str, const int codepage)
{
    char code[64];

    charset_get_code(codepage, code, 64);
    return charset_convert(str, code, "UTF-8");
}

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

#ifndef JHAKO_VERSION_H
#define JHAKO_VERSION_H

#define STR(str)  #str
#define XSTR(str) STR(str)

#define APPLICATION_NAME        "jHako"
#define JHAKO_VERSION_MAJOR    0
#define JHAKO_VERSION_MINOR    1
#define JHAKO_VERSION_PATCH    0
#define JHAKO_VERSION_RC       ""

#define JHAKO_VERSION XSTR(JHAKO_VERSION_MAJOR) "." XSTR(JHAKO_VERSION_MINOR) "." XSTR(JHAKO_VERSION_PATCH)

#endif

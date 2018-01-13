/*  This file is part of Sharkive
>	Copyright (C) 2018 Bernardo Giordano
>
>   This program is free software: you can redistribute it and/or modify
>   it under the terms of the GNU General Public License as published by
>   the Free Software Foundation, either version 3 of the License, or
>   (at your option) any later version.
>
>   This program is distributed in the hope that it will be useful,
>   but WITHOUT ANY WARRANTY; without even the implied warranty of
>   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
>   GNU General Public License for more details.
>
>   You should have received a copy of the GNU General Public License
>   along with this program.  If not, see <http://www.gnu.org/licenses/>.
>   See LICENSE for information.
*/

#include "stringutil.h"

std::u16string u8tou16(const char* src)
{
    char16_t tmp[256] = {0};
    utf8_to_utf16((uint16_t *)tmp, (uint8_t *)src, 256);
    return std::u16string(tmp);
}

std::string u16tou8(std::u16string src)
{
	static std::wstring_convert<std::codecvt_utf8_utf16<char16_t>,char16_t> convert;
	std::string dst = convert.to_bytes(src);
	return dst;
}

// https://stackoverflow.com/questions/2342162/stdstring-formatting-like-sprintf
std::string string_format(const std::string fmt_str, ...)
{
	int final_n, n = ((int)fmt_str.size()) * 2; /* Reserve two times as much as the length of the fmt_str */
	std::unique_ptr<char[]> formatted;
	va_list ap;
	while(1)
	{
		formatted.reset(new char[n]); /* Wrap the plain char array into the unique_ptr */
		strcpy(&formatted[0], fmt_str.c_str());
		va_start(ap, fmt_str);
		final_n = vsnprintf(&formatted[0], n, fmt_str.c_str(), ap);
		va_end(ap);
		if (final_n < 0 || final_n >= n)
			n += abs(final_n - n + 1);
		else
			break;
	}
	return std::string(formatted.get());
}

std::string getSizeString(double size)
{
	int i = 0;
	static const char* units[] = {"B", "kB", "MB", "GB"};
	while (size > 1024)
	{
		size /= 1024;
		i++;
	}
	return string_format("%.*f %s", i, size, units[i]);
}
#ifndef CONSOLE_IO_COLOR_HPP_
#define CONSOLE_IO_COLOR_HPP_

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

namespace cio
{

// 色
enum class Color
{
	Default,
	Red = FOREGROUND_INTENSITY | FOREGROUND_RED,
	Green = FOREGROUND_INTENSITY | FOREGROUND_GREEN,
	Blue = FOREGROUND_INTENSITY | FOREGROUND_BLUE
};

}

#endif // CONSOLE_IO_COLOR_HPP_

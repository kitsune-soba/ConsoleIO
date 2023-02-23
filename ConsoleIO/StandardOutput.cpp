#include <iostream>
#include "ConsoleIO.hpp"
#include "StandardOutput.hpp"

namespace cio
{

// Singleton なインスタンスを返す
StandardOutput& StandardOutput::getInstance(void)
{
	static StandardOutput instance;
	return instance;
}

// コンストラクタ
StandardOutput::StandardOutput()
	: stdOutHandle(GetStdHandle(STD_OUTPUT_HANDLE))
{
	CONSOLE_SCREEN_BUFFER_INFO info;
	GetConsoleScreenBufferInfo(stdOutHandle, &info);
	defaultConsoleAttributes = info.wAttributes;
}

// 文字列を標準出力に出力する
void StandardOutput::print(const std::string& string, const Color color)
{
	const bool colored = color != Color::Default;
	if (colored) { SetConsoleTextAttribute(stdOutHandle, static_cast<WORD>(color)); }
	std::cout << string;
	if (colored) { SetConsoleTextAttribute(stdOutHandle, defaultConsoleAttributes); }
}

// 文字列を標準エラー出力に出力する
void StandardOutput::printError(const std::string& string, const Color color)
{
	const bool colored = color != Color::Default;
	if (colored) { SetConsoleTextAttribute(stdOutHandle, static_cast<WORD>(color)); }
	std::cerr << string;
	if (colored) { SetConsoleTextAttribute(stdOutHandle, defaultConsoleAttributes); }
}

// カーソルを戻す
void StandardOutput::back(const std::size_t count) const
{
	std::cout << std::string(count, '\b');
}

// バッファを流す
void StandardOutput::flush(void) const
{
	std::cout << std::flush;
}

}

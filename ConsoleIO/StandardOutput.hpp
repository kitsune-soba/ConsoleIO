#ifndef CONSOLE_STANDARD_OUTPUT_HPP_
#define CONSOLE_STANDARD_OUTPUT_HPP_

#include <string>
#include "Color.hpp"

namespace cio
{

// 標準出力（標準エラー出力も含む）のヘルパー
// Singleton パターン
class StandardOutput
{
public:
	static StandardOutput& getInstance(void);

	void print(const std::string& string, Color color = Color::Default);
	void printError(const std::string& string, Color color = Color::Default);
	void back(std::size_t count) const;
	void flush(void) const;

private:
	StandardOutput();
	StandardOutput(const StandardOutput&) = delete;
	StandardOutput& operator=(const StandardOutput&) = delete;

	HANDLE stdOutHandle;
	WORD defaultConsoleAttributes;
};

}

#endif // CONSOLE_STANDARD_OUTPUT_HPP_

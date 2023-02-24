#include <conio.h>
#include <ctype.h>
#include <format>
#include <iostream>
#include "ConsoleIO.hpp"
#include "Logger.hpp"
#include "StandardOutput.hpp"

namespace cio
{

// 標準（エラー）出力をログファイルにも出力するようにする
void enableLogMirroring(const bool errorOnly, const std::string& path, const bool append)
{
	Logger::getInstance().enable(errorOnly, path, append);
}

// ログファイルへの出力のモードを変更する
void changeLogMirroringMode(const bool errorOnly)
{
	Logger::getInstance().changeMode(errorOnly);
}

// ログファイルへの出力をやめる
void disableLogMirroring(void)
{
	Logger::getInstance().disable();
}

// 標準出力に出力する
void print(const std::string& string, const Color color, const bool flush)
{
	StandardOutput& so = StandardOutput::getInstance();
	Logger& logger = Logger::getInstance();

	so.print(string, color);
	logger.write(string);

	if (flush)
	{
		so.flush();
		logger.flush();
	}
}

// 標準エラー出力に出力する
void printError(const std::string& string, const Color color, const bool flush)
{
	StandardOutput& so = StandardOutput::getInstance();
	Logger& logger = Logger::getInstance();

	so.printError(string, color);
	logger.writeError(string);

	if (flush)
	{
		so.flush();
		logger.flush();
	}
}

// カーソルの位置を戻す
void back(const std::size_t count)
{
	StandardOutput::getInstance().back(count);
}

// コンソールで質問する
char question(
	const std::string& question,
	const char defaultChoice,
	const std::initializer_list<char> followingChoices)
{
	// 選択肢が2個未満の場合はエラー
	const std::size_t choiceCount = ((defaultChoice == '\0') ? 0 : 1) + followingChoices.size();
	if (choiceCount < 2)
	{
		throw TooFewChicesException("The total of default choice and following choices must be 2 or more.");
	}

	// 選択肢を表示
	print(std::format("{}[", question));
	if (defaultChoice != '\0')
	{
		print(std::format("{}/", toUpper(defaultChoice)));
	}

	int count = 0;
	for (const char choice : followingChoices)
	{
		++count;
		print(std::string(1, toLower(choice)));
		if (count < followingChoices.size())
		{
			print("/");
		}
	}
	print("]", Color::Default, true);

	// 入力されたキーに一致する選択肢を探す（見つからない場合はヌル文字を戻り値にする）
	const char key = waitAnyKey();
	print("\n", Color::Default, true);
	const char enterKey = 0x0d;
	char answer;
	if (ignoreCaseEqual(key, defaultChoice) ||
		((defaultChoice != '\0') && (key == enterKey)))
	{
		answer = defaultChoice;
	}
	else
	{
		const bool found = std::find_if(std::cbegin(followingChoices), std::cend(followingChoices),
			[key](const char choice)
			{
				return ignoreCaseEqual(choice, key);
			}) != std::cend(followingChoices);

			answer = found ? key : '\0';
	}

	return toLower(answer);
}

// 何らかのキーが押されるのを待つ
char waitAnyKey(void)
{
	while (_kbhit()) { (void)_getch(); } // バッファのごみを掃除
	return static_cast<char>(_getch());
}

// 大文字と小文字の区別を無視して、同じ文字かどうか判定する
bool ignoreCaseEqual(const char lhs, const char rhs)
{
	return tolower(lhs) == tolower(rhs);
}

// 小文字にする
// 引数に対応する小文字が無ければ、引数をそのまま返す
char toLower(const char alphabet)
{
	return static_cast<char>(tolower(alphabet));
}

// 大文字にする
// 引数に対応する大文字が無ければ、引数をそのまま返す
char toUpper(const char alphabet)
{
	return static_cast<char>(toupper(alphabet));
}

}

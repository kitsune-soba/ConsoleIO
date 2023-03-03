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
	Logger::getInstance().setErrorOnlyMode(errorOnly);
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
	Logger::getInstance().back(count);
}

// コンソールからの入力を1行分受け付ける（スペースで切らない）
std::string getLine(void)
{
	std::string string;
	std::getline(std::cin, string);
	Logger::getInstance().write(string + '\n');
	return std::move(string);
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
	print(std::format("{} [", question));
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
	print("] ", Color::Default, true);

	// 入力されたキーに一致する選択肢を探す
	const std::string input = trimSpace(getLine());
	if (input.empty())
	{
		// 空文字が入力された場合、標準の選択肢があればそれが選択されたことにする
		return (defaultChoice != '\0') ? defaultChoice : '\0';
	}
	else if (input.length() == 1)
	{
		// 1文字だけ入力された場合、選択肢の中から合致するものを探す
		const char singleInput = input.at(0);

		if (ignoreCaseEqual(singleInput, defaultChoice))
		{
			return defaultChoice;
		}

		const char* const selected = std::find_if(std::cbegin(followingChoices), std::cend(followingChoices),
		[singleInput](const char choice)
		{
			return ignoreCaseEqual(choice, singleInput);
		});
		return (selected != std::cend(followingChoices)) ? *selected : '\0';
	}
	else
	{
		// この関数は1文字の選択肢しか扱わないため、2文字以上の入力は常に選択肢と合致しない
		return '\0';
	}
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

// 前後の空白文字を取り除く
std::string trimSpace(const std::string& string)
{
	const char* spaces = " \t\v\r\n";
	const std::size_t start = string.find_first_not_of(spaces);
	if (start != std::string::npos)
	{
		const std::size_t end = string.find_last_not_of(spaces);
		return string.substr(start, end - start + 1);
	}
	else
	{
		return "";
	}
}

}

// ProgressPrinter クラスのテスト

#include "pch.h"
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include "../ConsoleIO/ConsoleIO.hpp"
#include "../ConsoleIO/ProgressPrinter.hpp"

// 指定されたファイルの内容を全て読み取る
std::string readFile(const std::string& path);

TEST(ProgressPrinter, ConstructorAndDestructor)
{
	const std::string logPath = "ProgressPrinter_update.log";

	// 表示を増やしていくテスト
	cio::enableLogMirroring(false, logPath);
	{
		size_t count = 0;
		cio::ProgressPrinter pp([&count] { return std::string(count, '#'); }, 1, false);
		for (count = 0; count < 40; ++count) { Sleep(1); } // 何かしらの作業の進捗
	}
	cio::print("\n");
	cio::disableLogMirroring();
	EXPECT_EQ(readFile(logPath), std::string(40, '#') + '\n');

	// 表示を減らしていくテスト
	cio::enableLogMirroring(false, logPath);
	{
		size_t count = 0;
		cio::ProgressPrinter pp([&count] { return std::string(40 - count, '#'); }, 1, false);
		for (count = 0; count < 40; ++count) { Sleep(1); } // 何かしらの作業の進捗
	}
	cio::disableLogMirroring();
	EXPECT_EQ(readFile(logPath), std::string(40, ' '));

	// 終了時に表示を消すテスト
	cio::enableLogMirroring(false, logPath);
	{
		size_t count = 0;
		cio::ProgressPrinter pp([&count] { return std::string(count, '#'); }, 100, true);
		for (count = 0; count < 40; ++count) { Sleep(1); } // 何かしらの作業の進捗
	}
	cio::disableLogMirroring();
	const std::string log = readFile(logPath);
	EXPECT_LE(log.size(), 40); // clearWhenFinish が有効な場合、最後の更新後（の sleep 中）に更新された進捗は表示されないため、ここでは40文字まで表示しきらない可能性がある
	EXPECT_EQ(log.find_first_not_of(' '), std::string::npos); // 全ての表示が空白文字で上書きされているはず
}

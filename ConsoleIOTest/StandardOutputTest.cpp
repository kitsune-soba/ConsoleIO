// ConsoleIO クラスのテスト

#include "pch.h"
#include "../ConsoleIO/ConsoleIO.hpp"
#include "../ConsoleIO/StandardOutput.hpp"

TEST(StandardOutput, getInstance)
{
	EXPECT_NO_THROW(cio::StandardOutput::getInstance());
}

TEST(StandardOutput, print)
{
	cio::StandardOutput& so = cio::StandardOutput::getInstance();

	// 普通に出力するテスト
	so.print("########################################");
	so.print(" ← 標準の色\n");

	// 色付きの文字を出力するテスト
	so.print("########################################", cio::Color::Red);
	so.print(" ← 赤色\n");
	so.print("########################################", cio::Color::Green);
	so.print(" ← 緑色\n");
	so.print("########################################", cio::Color::Blue);
	so.print(" ← 青色\n");
}

TEST(StandardOutput, printError)
{
	cio::StandardOutput& so = cio::StandardOutput::getInstance();

	// 普通に出力するテスト
	so.printError("########################################");
	so.printError(" ← 標準の色\n");

	// 色付きの文字を出力するテスト
	so.printError("########################################", cio::Color::Red);
	so.printError(" ← 赤色\n");
	so.printError("########################################", cio::Color::Green);
	so.printError(" ← 緑色\n");
	so.printError("########################################", cio::Color::Blue);
	so.printError(" ← 青色\n");
	EXPECT_EQ(cio::question("上の8列の色は正しいですか？", 'y', { 'n' }), 'y');
}

TEST(StandardOutput, back)
{
	cio::StandardOutput& so = cio::StandardOutput::getInstance();

	// 1バイト文字を出力した後にカーソルを戻すテスト
	{
		std::string output("");
		for (int i = 1; i <= 40; ++i)
		{
			so.back(output.size());
			output += '#';
			so.print(output);
			Sleep(1);
		}
		so.print("\n");
	}

	// マルチバイト文字を出力した後にカーソルを戻すテスト
	{
		std::string output("");
		for (int i = 1; i <= 20; ++i)
		{
			so.back(output.size());
			output += "＃";
			so.print(output);
			Sleep(1);
		}
		so.print("\n");
	}
}

TEST(StandardOutput, flush)
{
	cio::StandardOutput& so = cio::StandardOutput::getInstance();

	for (int i = 0; i < 40; ++i)
	{
		so.print("#");
		so.flush();
		Sleep(1);
	}
	so.print("\n");
	EXPECT_EQ(cio::question("上の3列の \'#\'（および \'＃\'）は1文字ずつ左から順に表示されましたか？", 'y', { 'n' }), 'y');
}

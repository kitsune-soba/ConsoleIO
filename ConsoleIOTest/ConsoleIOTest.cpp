// ConsoleIO の関数のテスト

#include "pch.h"
#include <thread>
#include "../ConsoleIO/ConsoleIO.hpp"

// デストラクタで join するスレッドのラッパー
class ScopedThread
{
public:
	ScopedThread(std::thread&& t) : thread(std::move(t)) {}
	~ScopedThread() { thread.join(); }

private:
	std::thread thread;
};

// 指定したキーを遅延して自動入力する
void delayInput(const BYTE keyCode, const bool withShift)
{
	Sleep(10);

	// キーを押す
	if (withShift) { keybd_event(VK_SHIFT, 0, 0, NULL); }
	keybd_event(keyCode, 0, 0, NULL);

	// キーを離す
	keybd_event(keyCode, 0, KEYEVENTF_KEYUP, NULL);
	if (withShift) { keybd_event(VK_SHIFT, 0, KEYEVENTF_KEYUP, NULL); }
}

TEST(ConsoleIO, StandardOutputWrap)
{
	// 中身は殆ど cio::StandardOutput と cio::Logger のメンバを呼び出しているだけなので、詳細なテストはそれらのテストに任せる
	EXPECT_NO_THROW(cio::enableLogMirroring(true, "foobar.log"));
	EXPECT_NO_THROW(cio::disableLogMirroring());
	EXPECT_NO_THROW(cio::print(""));
	EXPECT_NO_THROW(cio::printError(""));
	EXPECT_NO_THROW(cio::back(0));
}

TEST(ConsoleIO, question)
{
	// 選択肢にあるキーを入力するテスト
	{
		ScopedThread thread(std::thread(delayInput, 'A', false));
		EXPECT_EQ(cio::question("標準の選択肢を小文字で選択するテスト", 'A', { 'B', 'C' }), 'a');
	}
	{
		ScopedThread thread(std::thread(delayInput, 'A', true));
		EXPECT_EQ(cio::question("標準の選択肢を大文字で選択するテスト", 'a', { 'b', 'c' }), 'a');
	}
	{
		ScopedThread thread(std::thread(delayInput, 'B', false));
		EXPECT_EQ(cio::question("追加の選択肢を小文字で選択するテスト", 'A', { 'B', 'C' }), 'b');
	}
	{
		ScopedThread thread(std::thread(delayInput, 'B', true));
		EXPECT_EQ(cio::question("追加の選択肢を大文字で選択するテスト", 'a', { 'b', 'c' }), 'b');
	}

	// 選択肢に無いキーを入力するテスト
	{
		ScopedThread thread(std::thread(delayInput, 'A', false));
		EXPECT_EQ(cio::question("選択肢に無いキーを入力するテスト", 'x', { 'y', 'z' }), '\0');
	}

	// エンターキーを押して標準の選択肢を選ぶテスト
	{
		ScopedThread thread(std::thread(delayInput, static_cast<BYTE>(VK_RETURN), false));
		EXPECT_EQ(cio::question("エンターキーで標準の選択肢を選ぶテスト", 'y', { 'n' }), 'y');
	}
	{
		ScopedThread thread(std::thread(delayInput, static_cast<BYTE>(VK_RETURN), false));
		EXPECT_EQ(cio::question("標準の選択肢が無い状態でエンターキーを押すテスト", '\0', { 'y', 'n' }), '\0');
	}

	// 選択肢が2個未満の場合に例外を投げるテスト
	EXPECT_THROW(cio::question("", 'A', {}), cio::TooFewChicesException);
	EXPECT_THROW(cio::question("", '\0', { 'A' }), cio::TooFewChicesException);
	EXPECT_THROW(cio::question("", '\0', {}), cio::TooFewChicesException);
}

TEST(ConsoleIO, waitAnyKey)
{
	// 押されたキーを受け取るテスト
	const BYTE enterKey = static_cast<BYTE>(VK_RETURN);
	ScopedThread thread(std::thread(delayInput, enterKey, false));
	EXPECT_EQ(cio::waitAnyKey(), enterKey);
}

TEST(ConsoleIO, ignoreCaseEqual)
{
	// 一致のテスト
	EXPECT_TRUE(cio::ignoreCaseEqual('a', 'a'));
	EXPECT_TRUE(cio::ignoreCaseEqual('a', 'A'));
	EXPECT_TRUE(cio::ignoreCaseEqual('A', 'a'));
	EXPECT_TRUE(cio::ignoreCaseEqual('A', 'A'));

	// 不一致のテスト
	EXPECT_FALSE(cio::ignoreCaseEqual('A', 'B'));
}

TEST(ConsoleIO, toLower)
{
	// 小文字に変換するテスト
	EXPECT_EQ(cio::toLower('A'), 'a');
	EXPECT_EQ(cio::toLower('a'), 'a');

	// 対応する小文字が無い場合は引数をそのまま返す
	EXPECT_EQ(cio::toLower('!'), '!');
}

TEST(ConsoleIO, toUpper)
{
	// 大文字に変換するテスト
	EXPECT_EQ(cio::toUpper('A'), 'A');
	EXPECT_EQ(cio::toUpper('a'), 'A');

	// 対応する大文字が無い場合は引数をそのまま返す
	EXPECT_EQ(cio::toUpper('!'), '!');
}

// ConsoleIO の関数のテスト

#include "pch.h"
#include <thread>
#include "../ConsoleIO/ConsoleIO.hpp"

// 指定されたファイルの内容を全て読み取る
std::string readFile(const std::string& path);

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
std::thread delayInput(const std::initializer_list<BYTE> keys)
{
	return std::thread ([keys]()
	{
		Sleep(10);

		for (const BYTE key : keys)
		{
			const BYTE keyCode = (islower(key) != 0) ? cio::toUpper(key) : key; // 小文字のアルファベットであればキーコード（大文字）に直す
			const bool shift = isupper(key) != 0; // 大文字を入力する場合はシフトキーを使う

			// キーを押す
			if (shift) { keybd_event(VK_SHIFT, 0, 0, NULL); }
			keybd_event(keyCode, 0, 0, NULL);

			// キーを離す
			keybd_event(keyCode, 0, KEYEVENTF_KEYUP, NULL);
			if (shift) { keybd_event(VK_SHIFT, 0, KEYEVENTF_KEYUP, NULL); }
		}
	});
}

TEST(ConsoleIO, misc)
{
	// 中身は殆ど cio::StandardOutput と cio::Logger のメンバを呼び出しているだけなので、詳細なテストはそれらのテストに任せる
	EXPECT_NO_THROW(cio::enableLogMirroring(true, "ConsoleIO_misc.log"));
	EXPECT_NO_THROW(cio::changeLogMirroringMode(false));
	EXPECT_NO_THROW(cio::disableLogMirroring());
	EXPECT_NO_THROW(cio::print(""));
	EXPECT_NO_THROW(cio::printError(""));
	EXPECT_NO_THROW(cio::back(0));
}

TEST(ConsoleIO, getLine)
{
	const std::string path("ConsoleIO_getLine.log");
	cio::enableLogMirroring(false, path);

	// コンソールから入力を1行受け取るテスト
	{
		ScopedThread thread(delayInput({ 'a', VK_SPACE, 'b', VK_SPACE, 'c', VK_RETURN }));
		EXPECT_EQ(cio::getLine(), "a b c");
	}

	// コンソールからの入力がログファイルに書き込まているか確認
	cio::disableLogMirroring();
	EXPECT_EQ(readFile(path), "a b c\n");
}

TEST(ConsoleIO, question)
{
	// 選択肢にあるキーを入力するテスト
	{
		ScopedThread thread(delayInput({ 'a', VK_RETURN }));
		EXPECT_EQ(cio::question("標準の選択肢を小文字で選択するテスト", 'A', { 'B', 'C' }), 'A');
	}
	{
		ScopedThread thread(delayInput({ 'A', VK_RETURN }));
		EXPECT_EQ(cio::question("標準の選択肢を大文字で選択するテスト", 'a', { 'b', 'c' }), 'a');
	}
	{
		ScopedThread thread(delayInput({ 'b', VK_RETURN }));
		EXPECT_EQ(cio::question("追加の選択肢を小文字で選択するテスト", 'A', { 'B', 'C' }), 'B');
	}
	{
		ScopedThread thread(delayInput({ 'B', VK_RETURN }));
		EXPECT_EQ(cio::question("追加の選択肢を大文字で選択するテスト", 'a', { 'b', 'c' }), 'b');
	}

	// 選択肢に無いキーを入力するテスト
	{
		ScopedThread thread(delayInput({ 'a', VK_RETURN }));
		EXPECT_EQ(cio::question("選択肢に無いキーを入力するテスト", 'x', { 'y', 'z' }), '\0');
	}

	// エンターキーを押して標準の選択肢を選ぶテスト
	{
		ScopedThread thread(delayInput({ VK_RETURN }));
		EXPECT_EQ(cio::question("エンターキーで標準の選択肢を選ぶテスト", 'y', { 'n' }), 'y');
	}
	{
		ScopedThread thread(delayInput({ VK_RETURN }));
		EXPECT_EQ(cio::question("標準の選択肢が無い状態でエンターキーを押すテスト", '\0', { 'y', 'n' }), '\0');
	}

	// 二文字以上入力するテスト
	{
		ScopedThread thread(delayInput({ 'a', 'b', VK_RETURN}));
		EXPECT_EQ(cio::question("二文字以上入力するテスト", 'y', { 'n' }), '\0');
	}

	// 選択肢が2個未満の場合に例外を投げるテスト
	EXPECT_THROW(cio::question("", 'A', {}), cio::TooFewChicesException);
	EXPECT_THROW(cio::question("", '\0', { 'A' }), cio::TooFewChicesException);
	EXPECT_THROW(cio::question("", '\0', {}), cio::TooFewChicesException);

	// ログファイルに書き込まれているか確認
	const std::string log = "ConsoleIO_question.log";
	cio::enableLogMirroring(false, log);
	{
		ScopedThread thread(delayInput({ 'y', VK_RETURN }));
		cio::question("ログファイルに記録するテスト", 'y', { 'n' });
	}
	cio::disableLogMirroring();
	EXPECT_EQ(readFile(log), "ログファイルに記録するテスト [Y/n] y\n");
}

TEST(ConsoleIO, waitAnyKey)
{
	// 押されたキーを受け取るテスト
	ScopedThread thread(delayInput({ VK_RETURN }));
	EXPECT_EQ(cio::waitAnyKey(), VK_RETURN);
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

TEST(ConsoleIO, trimSpace)
{
	// 前後に空白文字が無い場合のテスト
	EXPECT_EQ(cio::trimSpace("not trimmed"), "not trimmed");

	// 前後の空白文字をトリムするテスト
	EXPECT_EQ(cio::trimSpace(" trimmed "), "trimmed");
	EXPECT_EQ(cio::trimSpace("\ttrimmed\t"), "trimmed");
	EXPECT_EQ(cio::trimSpace("\vtrimmed\v"), "trimmed");
	EXPECT_EQ(cio::trimSpace("\rtrimmed\r"), "trimmed");
	EXPECT_EQ(cio::trimSpace("\ntrimmed\n"), "trimmed");
	EXPECT_EQ(cio::trimSpace("  \n\r \t\v trimmed \t   \v\r\n     "), "trimmed");

	// 空白文字のみを渡すテスト
	EXPECT_EQ(cio::trimSpace("\n\r  \t\v"), "");

	// 空文字列を渡すテスト
	EXPECT_EQ(cio::trimSpace(""), "");
}

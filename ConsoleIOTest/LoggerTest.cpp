// Logger クラスのテスト

#include "pch.h"
#include <fstream>
#include <sys/stat.h>
#include "../ConsoleIO/Logger.hpp"

// 指定されたパスのファイルが存在しているか確認する
bool exists(const std::string& path)
{
	struct stat s;
	return stat(path.c_str(), &s) == 0;
}

// 指定されたファイルの内容を全て読み取る
std::string readFile(const std::string& path)
{
	std::ifstream stream(path);
	return std::string(std::istreambuf_iterator<char>(stream), std::istreambuf_iterator<char>());
}

TEST(Logger, getInstance)
{
	EXPECT_NO_THROW(cio::Logger::getInstance());
}

TEST(Logger, enable)
{
	cio::Logger& logger = cio::Logger::getInstance();
	const std::string path("Logger_enable.log");

	// 不正なパスを渡すテスト
	EXPECT_THROW(logger.enable(true, R"(\/:*?"<>|)", false), cio::FileOpenException);

	// 指定されたパスのログファイルが生成されるテスト（errorOnly = true）
	if (exists(path)) { remove(path.c_str()); };
	EXPECT_FALSE(exists(path));
	EXPECT_NO_THROW(logger.enable(true, path, false));
	EXPECT_TRUE(exists(path));
	logger.disable();

	// 指定されたパスのログファイルが生成されるテスト（errorOnly = false）
	if (exists(path)) { remove(path.c_str()); };
	EXPECT_FALSE(exists(path));
	EXPECT_NO_THROW(logger.enable(false, path, false));
	EXPECT_TRUE(exists(path));
	logger.disable();

	// 上書きモードのテスト
	logger.enable(false, path, false);
	logger.write("This line will be overwritten.\n");
	logger.disable();
	logger.enable(false, path, false); // 上書きモードで開きなおす
	EXPECT_EQ(readFile(path), ""); // ファイルの内容は消えているはず
	logger.write("This line will be kept.\n");
	logger.disable();

	// 追記モードのテスト
	logger.enable(false, path, true); // 上書きモードで開きなおす
	EXPECT_EQ(readFile(path), "This line will be kept.\n"); // ファイルの内容は残っているはず
	logger.disable();

	// 多重に enable しても何も起きないテスト
	EXPECT_NO_THROW(logger.enable(true, path, false));
	EXPECT_NO_THROW(logger.enable(true, path, false));
	logger.disable();
}

TEST(Logger, changeMode)
{
	cio::Logger& logger = cio::Logger::getInstance();
	const std::string path("Logger_changeMode.log");

	logger.enable(true, path, false);
	logger.write("StandardOutput_1\n"); // errorOnly モードなので書き込まれないはず
	logger.writeError("StandardError_1\n");
	logger.changeMode(false);
	logger.write("StandardOutput_2\n");
	logger.writeError("StandardError_2\n");
	logger.changeMode(true);
	logger.write("StandardOutput_3\n"); // errorOnly モードなので書き込まれないはず
	logger.writeError("StandardError_3\n");
	logger.flush();
	EXPECT_EQ(readFile(path),
		"StandardError_1\nStandardOutput_2\nStandardError_2\nStandardError_3\n");
	logger.disable();

	// 現在のモードと同じモードに設定しようとしても問題ないことを確認するテスト
	logger.enable(true, path, false);
	EXPECT_NO_THROW(logger.changeMode(true));
	EXPECT_NO_THROW(logger.changeMode(true));
	logger.disable();
	logger.enable(false, path, false);
	EXPECT_NO_THROW(logger.changeMode(false));
	EXPECT_NO_THROW(logger.changeMode(false));
	logger.disable();
}

TEST(Logger, disable)
{
	cio::Logger& logger = cio::Logger::getInstance();
	const std::string path("Logger_disable.log");

	// disable した後は書き込まれなくなるテスト
	if (exists(path)) { remove(path.c_str()); };
	logger.enable(false, path, false);
	logger.write("Standard Output 1\n");
	logger.write("Error Output 1\n");
	logger.disable();
	logger.write("Standard Output 2\n");
	logger.write("Error Output 2\n");
	EXPECT_EQ(readFile(path), "Standard Output 1\nError Output 1\n"); // disable より後は書き込まれていないはず

	// 多重に disable しても何も起きないテスト
	EXPECT_NO_THROW(logger.disable());
	EXPECT_NO_THROW(logger.disable());
}

TEST(Logger, write)
{
	cio::Logger& logger = cio::Logger::getInstance();
	const std::string path("Logger_write.log");

	// ログが無効な状態でも問題は発生しない
	EXPECT_NO_THROW(logger.write(""));

	// errorOnly = true のテスト
	if (exists(path)) { remove(path.c_str()); };
	logger.enable(true, path, false);
	logger.write("Standard Output 1\n");
	logger.write("Standard Output 2\n");
	logger.flush();
	EXPECT_EQ(readFile(path), ""); // 書き込まれていないはず
	logger.disable();

	// errorOnly = false のテスト
	if (exists(path)) { remove(path.c_str()); };
	logger.enable(false, path, true);
	logger.write("Standard Output 1\n");
	logger.write("Standard Output 2\n");
	logger.flush();
	EXPECT_EQ(readFile(path), "Standard Output 1\nStandard Output 2\n"); // 書き込まれているはず
	logger.disable();
}

TEST(Logger, writeError)
{
	cio::Logger& logger = cio::Logger::getInstance();
	const std::string path("Logger_writeError.log");

	// ログが無効な状態でも問題は発生しない
	EXPECT_NO_THROW(logger.writeError(""));

	// errorOnly = true のテスト
	if (exists(path)) { remove(path.c_str()); };
	logger.enable(true, path, false);
	logger.writeError("Error Output 1\n");
	logger.writeError("Error Output 2\n");
	logger.flush();
	EXPECT_EQ(readFile(path), "Error Output 1\nError Output 2\n"); // 書き込まれているはず
	logger.disable();

	// errorOnly = false のテスト
	if (exists(path)) { remove(path.c_str()); };
	logger.enable(false, path, false);
	logger.writeError("Error Output 1\n");
	logger.writeError("Error Output 2\n");
	logger.flush();
	EXPECT_EQ(readFile(path), "Error Output 1\nError Output 2\n"); // 書き込まれているはず
	logger.disable();
}

TEST(Logger, flush)
{
	cio::Logger& logger = cio::Logger::getInstance();
	const std::string path("Logger_flush.log");

	// バッファを流すことでファイルに確実に書き込まれるはず
	logger.enable(false, path, false);
	logger.write("If not flush, this line may not be written immediately.");
	logger.flush();
	EXPECT_EQ(readFile(path), "If not flush, this line may not be written immediately.");
	logger.disable();

	// ログが無効な状態でも問題は発生しない
	EXPECT_NO_THROW(logger.flush());
}

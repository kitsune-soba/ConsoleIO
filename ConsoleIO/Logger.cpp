#include <format>
#include "Logger.hpp"

namespace cio
{

// Singleton なインスタンスを返す
Logger& Logger::getInstance(void)
{
	static Logger instance;
	return instance;
}

// ログの書き込みを有効化する
// 既にログが開かれている場合は何もしない（errorOnly の変更も不可）
void Logger::enable(const bool errOnly, const std::string& path, const bool append)
{
	if (stream.is_open()) { return; }

	if (append) { stream.open(path, std::ios_base::app); }
	else { stream.open(path); }

	if (!stream)
	{
		throw FileOpenException(std::format("Failed to open or create log file: {}", path));
	}

	enabled = true;
	errorOnly = errOnly;
}

// ログ出力のモードを変更する
void Logger::changeMode(const bool errOnly)
{
	errorOnly = errOnly;
}

// ログファイルを閉じて書き込みを無効化する
void Logger::disable(void)
{
	stream.close();
	enabled = false;
}

// ログに書き込む
void Logger::write(const std::string& string)
{
	if (enabled && !errorOnly)
	{
		stream << string;
	}
}

// ログにエラーを書き込む
void Logger::writeError(const std::string& string)
{
	if (enabled)
	{
		stream << string;
	}
}

// 指定された分だけ戻る
void Logger::back(const std::size_t count)
{
	if (enabled && !errorOnly)
	{
		stream.seekp(-static_cast<std::streamoff>(count), std::ios::cur);
	}
}

// バッファを流す
void Logger::flush(void)
{
	if (enabled)
	{
		stream << std::flush;
	}
}

}

#ifndef CONSOLE_IO_LOGGER_HPP_
#define CONSOLE_IO_LOGGER_HPP_

#include <fstream>
#include <stdexcept>
#include <string>

namespace cio
{

// ファイルを開けなかった時に投げられる例外
class FileOpenException : public std::runtime_error
{
public:
	FileOpenException(const std::string& message)
		: runtime_error(message) {}
};

// ログを記録する
// Singleton パターン
class Logger
{
public:
	static Logger& getInstance(void);

	void enable(bool errorOnly, const std::string& path, bool append);
	void setErrorOnlyMode(bool errorOnly);
	bool getErrorOnlyMode(void) const;
	void disable(void);
	void write(const std::string& string);
	void writeError(const std::string& string);
	void back(std::size_t count);
	void flush(void);

private:
	Logger() = default;
	Logger(const Logger&) = delete;
	Logger& operator=(const Logger&) = delete;

	bool enabled = false;
	bool errorOnly = true;
	std::ofstream stream;
};

}

#endif // CONSOLE_IO_LOGGER_HPP_

﻿#ifndef CONSOLE_IO_PROGRESS_PRINTER_HPP_
#define CONSOLE_IO_PROGRESS_PRINTER_HPP_

#include <functional>
#include <string>
#include <thread>

namespace cio
{

// 進捗状況などを更新して表示するクラス
// 表示の更新中に他の出力（cio::print など）を行うと表示が崩れるので注意
class ProgressPrinter
{
public:
	ProgressPrinter(const std::function<std::string(void)>& progress, size_t sleep, bool clearWhenFinish);
	~ProgressPrinter();

private:
	void update(void);
	void printProgress(void);

	const std::function<std::string(void)> progress;
	const std::size_t sleep;
	const bool clearWhenFinish;
	std::thread thread;
	bool finishRequired = false;
	std::size_t lastLength = 0;
};

}

#endif // CONSOLE_IO_PROGRESS_PRINTER_HPP_

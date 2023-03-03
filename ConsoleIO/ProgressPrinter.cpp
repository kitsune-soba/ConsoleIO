#include "ConsoleIO.hpp"
#include "Logger.hpp"
#include "ProgressPrinter.hpp"

namespace cio
{

// コンストラクタ
ProgressPrinter::ProgressPrinter(
	const std::function<std::string(void)>& progress,
	const size_t pollingInterval,
	const bool clearWhenFinish,
	const bool suppressLogMirroring)
	: progress(progress)
	, pollingInterval(pollingInterval)
	, clearWhenFinish(clearWhenFinish)
	, suppressLogMirroring(suppressLogMirroring)
	, originalLogErrorOnlyMode(Logger::getInstance().getErrorOnlyMode())
	, thread([this] { update(); })
{
	if (suppressLogMirroring)
	{
		changeLogMirroringMode(true);
	}
}

// デストラクタ
ProgressPrinter::~ProgressPrinter()
{
	finishRequired = true;
	thread.join();
}

// 表示を更新する
void ProgressPrinter::update(void)
{
	// インスタンスの破棄が始まるまで表示を更新し続ける
	while (!finishRequired)
	{
		printProgress();
		std::this_thread::sleep_for(std::chrono::milliseconds(pollingInterval));
	}

	if (clearWhenFinish)
	{
		// 表示した内容を消す
		const std::size_t lastLength = lastWrote.size();
		back(lastLength);
		print(std::string(lastLength, ' '));
		back(lastLength);
	}
	else
	{
		// 最後の表示の後にスリープを挟んでいるので、最後にもう一度表示を更新する
		printProgress();
	}

	// ログの書き込みが抑制されていれば、これを解除する
	if (suppressLogMirroring)
	{
		Logger& logger = Logger::getInstance();
		logger.setErrorOnlyMode(originalLogErrorOnlyMode);
		if (!clearWhenFinish)
		{
			logger.write(lastWrote);
		}
	}
}

// 表示する
void ProgressPrinter::printProgress(void)
{
	std::string newProgress = progress();
	if (newProgress == lastWrote) { return; } // 前回の表示と内容が変わらなければ何もしない

	// 前回表示した分カーソルを戻す
	const std::size_t lastLength = lastWrote.size();
	back(lastLength);

	// 今回の表示
	std::size_t newLength = newProgress.size();
	print(newProgress);

	// 前回の表示を今回の表示で上書きしきれなかった分を空白文字で埋める
	if (lastLength > newLength)
	{
		const std::size_t spaceLength = lastLength - newLength;
		print(std::string(spaceLength, ' '));
		back(spaceLength);
	}

	lastWrote = std::move(newProgress);
}

}

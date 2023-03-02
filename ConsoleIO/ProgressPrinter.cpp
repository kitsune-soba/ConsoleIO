#include "ConsoleIO.hpp"
#include "ProgressPrinter.hpp"

namespace cio
{

// コンストラクタ
ProgressPrinter::ProgressPrinter(
	const std::function<std::string(void)>& progress,
	const size_t sleep,
	const bool clearWhenFinish)
	: progress(progress)
	, sleep(sleep)
	, clearWhenFinish(clearWhenFinish)
	, thread([this] { update(); })
{}

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
		std::this_thread::sleep_for(std::chrono::milliseconds(sleep));
	}

	if (clearWhenFinish)
	{
		// 表示した内容を消す
		back(lastLength);
		print(std::string(lastLength, ' '));
		back(lastLength);
	}
	else
	{
		// 最後の表示の後にスリープを挟んでいるので、最後にもう一度表示を更新する
		printProgress();
	}
}

// 表示する
void ProgressPrinter::printProgress(void)
{
	// 前回表示した分カーソルを戻す
	back(lastLength);

	// 今回の表示
	const std::string newProgress = progress();
	std::size_t newLength = newProgress.size();
	print(newProgress);

	// 前回の表示を今回の表示で上書きしきれなかった分を空白文字で埋める
	if (lastLength > newLength)
	{
		const std::size_t spaceLength = lastLength - newLength;
		print(std::string(spaceLength, ' '));
		back(spaceLength);
	}

	lastLength = newLength;
}

}

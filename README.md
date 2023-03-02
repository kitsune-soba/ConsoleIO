# ConsoleIO
コンソール入出力のヘルパー。
文字を色付きで表示したり、<question> [Y/n] のような入力待ちを提供するなど。  
最新版：https://github.com/kitsune-soba/ConsoleIO

## 開発環境
- Windows
- Visual Studio Community 2022

## 動作環境
- Windows

## 使い方
1. ConsoleIO をプロジェクトに導入
	- ConsoleIO.lib をリンク
	- ヘッダファイルへのインクルードパスを通す
1. コードを書く
	- 下記のサンプルコードを参照

サンプルコード
```
#include <thread>

// ConsoleIO ヘッダやライブラリの場所はお好みで
#include <ConsoleIO/ConsoleIO.hpp>
#include <ConsoleIO/ProgressPrinter.hpp>
#pragma comment(lib, "../../ConsoleIO/x64/Debug/ConsoleIO.lib")

// 何かしらの仕事をするクラス
class SomeWork
{
public:
	void run(void)
	{
		for (progress = 0; progress < 100; ++progress)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(50));
		}
	}

	std::string getProgressString(void) const
	{
		return std::to_string(progress) + '%';
	}

private:
	int progress = 0;
};

int main(int, char* [])
{
	// cio::print などによる出力をログファイルにも書き出すようにする
	cio::enableLogMirroring(false, "test.log");

	// 標準（エラー）出力へ出力する
	cio::print("標準出力するテスト\n");
	cio::printError("標準エラー出力するテスト\n");

	// 進捗状況を表示する
	// pp のデストラクタが呼び出されるまで、別スレッドで
	// 「処理中...0%」から「処理中...100%」まで 1000 ms 間隔で表示を更新する
	SomeWork work;
	cio::print("処理中...");
	{
		cio::ProgressPrinter pp([&work] { return work.getProgressString(); }, 1000, true);
		work.run();
	}
	cio::print("完了\n", cio::Color::Green); // 緑色で表示

	// ユーザーに質問する
	// 「終了しますか？ [Y/n/?]」と表示し、ユーザからの入力を受け付ける
	char answer;
	while ((answer = cio::question("終了しますか？", 'y', { 'n', '?' })) != 'y')
	{
		cio::print("終了するには Y（または y ）を入力してください。Y が標準の選択肢なので単にエンターキーを押しても終了できます。\n");
	}

	return 0;
}
```

## 弱点
- スレッドセーフではない
- ワイド文字（wchar_t）には非対応
- 他の標準出力（std::cout, std::printf, etc.）と混ぜて使われることは想定していない

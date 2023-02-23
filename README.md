# ConsoleIO
コンソール入出力のヘルパー。
文字を色付きで表示したり、<question> [Y/n] のような入力待ちを提供するなど。  
最新版：https://github.com/kitsune-soba/ConsoleIO

## 開発環境
- Windows 10
- Visual Studio Community 2022

## 動作環境
- Windows

## 使い方
1. ConsoleIO をプロジェクトに導入
	- [ConsoleIO.hpp](ConsoleIO/ConsoleIO.hpp) をインクルード
	- ConsoleIO.lib をリンク
1. 用途に応じて [ConsoleIO.hpp](ConsoleIO/ConsoleIO.hpp) で宣言されている関数を呼ぶ

サンプルコード
```
// ライブラリやヘッダの場所はお好みで
#pragma comment(lib, "../ConsoleIO/lib/ConsoleIO.lib")
#include "../ConsoleIO/include/ConsoleIO.hpp"

int main(int, char* [])
{
	cio::print("標準出力するテスト\n");
	cio::printError("標準エラー出力するテスト\n");

	char answer;
	while ((answer = cio::question("終了しますか？", 'y', { 'n', '?' })) != 'y') // 終了しますか？[Y/n/?] と表示される
	{
		switch (answer)
		{
		case '?':
			cio::print("Y(y)", cio::Color::Green);
			cio::print(" キーを押すと終了できます。\nまた、Y は標準の選択肢（第二引数）に設定されているので ");
			cio::print("Enter", cio::Color::Green);
			cio::print(" を押しても同様に終了できます。\n");
			break;
		case '\0': // 選択肢に無いキーが押されたらヌル文字が返る
			cio::printError("そのキーは選択肢にありませんね…。\n", cio::Color::Red);
			break;
		}
	}

	return 0;
}
```

## 弱点
- スレッドセーフではない
- ワイド文字（wchar_t）には非対応
- 他の標準出力（std::cout, std::printf, etc.）と混ぜて使われることは想定していない

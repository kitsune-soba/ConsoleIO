#ifndef CONSOLE_IO_CONSOLE_IO_HPP_
#define CONSOLE_IO_CONSOLE_IO_HPP_

#include <stdexcept>
#include <string>
#include "Color.hpp"

namespace cio
{

// question 関数に指定した選択肢が2個未満だった場合に投げられる例外
class TooFewChicesException : public std::logic_error
{
public:
	TooFewChicesException(const std::string& message)
		: logic_error(message) {}
};

// 標準（エラー）出力をログファイルにも出力するようにする
void enableLogMirroring(bool errorOnly, const std::string& path, bool append = false);

// ログファイルへの出力のモードを変更する
void changeLogMirroringMode(bool errorOnly);

// ログファイルへの出力をやめる
void disableLogMirroring(void);

// 標準出力に出力する
void print(const std::string& string, Color color = Color::Default, bool flush = false);

// 標準エラー出力に出力する
void printError(const std::string& string, Color color = Color::Default, bool flush = true);

// 指定された文字数分、出力の現在位置を戻す
// 戻った分の文字は削除しない（消したければ別途空白文字で上書きする）
// FIXME コンソールへの出力とログファイルへの出力に差がある
//       コンソールでは現在の行の先頭より前には戻らず、ログファイルでは現在の行の先頭より前へも（ファイル先頭より前へすらも）戻る
void back(std::size_t count);

// コンソールからの入力を1行分受け付ける（スペースで切らない）
// 標準出力のログファイルへのミラーリングが有効かつ errorOnly でなければ、コンソールへの入力もログに書き込まれる
// 但し、maxSize を超過した分の入力はコンソールには表示されるもののログ（や戻り値）では切り捨てられる
std::string getLine(void);

// <question> [Y/n] のような質問文をコンソールに表示し、キー入力による回答を受け取る
// 空文字が入力された場合は標準の選択肢（defaultChoice）が選択される
// 標準の選択肢を設定したくない場合は defaultChoice にヌル文字を指定する
// 戻り値は選択された選択肢（大文字・小文字は実引数に準拠）で、選択肢以外の回答があった場合はヌル文字を返す
char question(const std::string& question, char defaultChoice, std::initializer_list<char> followingChoices);

// いずれかのキーが入力されるまで待機する
// 戻り値は入力されたキー
char waitAnyKey(void);

// 大文字・小文字の違いを無視して文字の一致を判定する
bool ignoreCaseEqual(char lhs, char rhs);

// 小文字に変換する
// 対応する小文字が無い場合は引数をそのまま返す
char toLower(char alphabet);

// 大文字に変換する
// 対応する大文字が無い場合は引数をそのまま返す
char toUpper(char alphabet);

// 前後の空白文字を取り除く
std::string trimSpace(const std::string& string);

}

#endif // CONSOLE_IO_CONSOLE_IO_HPP_

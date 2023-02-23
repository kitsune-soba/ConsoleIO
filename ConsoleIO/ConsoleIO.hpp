﻿#ifndef CONSOLE_IO_CONSOLE_IO_HPP_
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

// ログファイルへの出力をやめる
void disableLogMirroring(void);

// 標準出力に出力する
void print(const std::string& string, Color color = Color::Default, bool flush = false);

// 標準エラー出力に出力する
void printError(const std::string& string, Color color = Color::Default, bool flush = true);

// カーソルを指定された文字数分戻す
// 戻った分の文字は削除しない
// 現在の行の先頭よりも前へは戻らない
void back(std::size_t count);

// <question> [Y/n] のような質問文をコンソールに表示し、キー入力による回答を受け取る
// エンターキーが押された場合は標準の選択肢（defaultChoice）が選択されたとみなす
// 標準の選択肢を設定したくない場合は defaultChoice にヌル文字を指定する
// 戻り値は選択された選択肢（常に小文字）で、選択肢以外の回答があった場合はヌル文字を返す
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

}

#endif // CONSOLE_IO_CONSOLE_IO_HPP_

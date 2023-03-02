// ProgressPrinter �N���X�̃e�X�g

#include "pch.h"
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include "../ConsoleIO/ConsoleIO.hpp"
#include "../ConsoleIO/ProgressPrinter.hpp"

// �w�肳�ꂽ�t�@�C���̓��e��S�ēǂݎ��
std::string readFile(const std::string& path);

TEST(ProgressPrinter, ConstructorAndDestructor)
{
	const std::string logPath = "ProgressPrinter_update.log";

	// �\���𑝂₵�Ă����e�X�g
	cio::enableLogMirroring(false, logPath);
	{
		size_t count = 0;
		cio::ProgressPrinter pp([&count] { return std::string(count, '#'); }, 1, false);
		for (count = 0; count < 40; ++count) { Sleep(1); } // ��������̍�Ƃ̐i��
	}
	cio::print("\n");
	cio::disableLogMirroring();
	EXPECT_EQ(readFile(logPath), std::string(40, '#') + '\n');

	// �\�������炵�Ă����e�X�g
	cio::enableLogMirroring(false, logPath);
	{
		size_t count = 0;
		cio::ProgressPrinter pp([&count] { return std::string(40 - count, '#'); }, 1, false);
		for (count = 0; count < 40; ++count) { Sleep(1); } // ��������̍�Ƃ̐i��
	}
	cio::disableLogMirroring();
	EXPECT_EQ(readFile(logPath), std::string(40, ' '));

	// �I�����ɕ\���������e�X�g
	cio::enableLogMirroring(false, logPath);
	{
		size_t count = 0;
		cio::ProgressPrinter pp([&count] { return std::string(count, '#'); }, 100, true);
		for (count = 0; count < 40; ++count) { Sleep(1); } // ��������̍�Ƃ̐i��
	}
	cio::disableLogMirroring();
	const std::string log = readFile(logPath);
	EXPECT_LE(log.size(), 40); // clearWhenFinish ���L���ȏꍇ�A�Ō�̍X�V��i�� sleep ���j�ɍX�V���ꂽ�i���͕\������Ȃ����߁A�����ł�40�����܂ŕ\��������Ȃ��\��������
	EXPECT_EQ(log.find_first_not_of(' '), std::string::npos); // �S�Ă̕\�����󔒕����ŏ㏑������Ă���͂�
}

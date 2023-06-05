#pragma once
#include <stdio.h>

class MyFile
{
public:
	MyFile() = default;
	~MyFile();

	// ストリーム
	FILE* streem = nullptr;


	/// <summary>
	/// ファイルオープン
	/// </summary>
	/// <param name="path">ファイルパス</param>
	/// <param name="mode">開くモード</param>
	/// <returns>true : 正常 , false : エラー</returns>
	bool Open(const char* path, const char* mode);
	/// <summary>
	/// ファイルクローズ
	/// </summary>
	void Close();

};


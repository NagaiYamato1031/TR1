#pragma once
#include <stdio.h>

class MyFile
{
public:
	MyFile() = default;
	~MyFile();
	// ファイルポインタ
	FILE* fp = nullptr;
	// ファイルのバッファ
	//char buffer[];

	/// <summary>
	/// ファイルオープン
	/// </summary>
	/// <param name="path">ファイルパス</param>
	/// <param name="mode">開くモード</param>
	/// <returns>true : 正常 , false : エラー</returns>
	bool Open(const char* path, const char* mode);

};


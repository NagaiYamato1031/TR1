#include "MyFile.h"

MyFile::~MyFile() {

}

bool MyFile::Open(const char* path, const char* mode) {
	// ファイルを開く
	fp = nullptr;
	errno_t errorCode =	fopen_s(&fp, path, mode);

	// 開けなかったら
	if (0 != errorCode) {
		return false;
	}

	// ファイルを閉じる
	//file.close();
	return true;
	//return fopen_s(&streem, path, mode) ? true : false;
}

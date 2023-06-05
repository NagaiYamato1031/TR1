#include "MyFile.h"
#include <cassert>

MyFile::~MyFile() {
	delete streem;
}

bool MyFile::Open(const char* path, const char* mode) {
	// ファイルを開く
	return fopen_s(&streem, path, mode) ? true : false;
}

void MyFile::Close() {
	if (streem) {
		fclose(streem);
	}
}
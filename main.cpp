#include <Novice.h>
#include "Mymath.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix4x4.h"

#include "MyCurve.h"

const char kWindowTitle[] = "LE2A_12_ナガイ_ヤマト_TR1";
const int kWindowWidth = 1280;
const int kWindowHeight = 720;

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, kWindowWidth, kWindowHeight);

	// キー入力結果を受け取る箱
	char keys[256] = {0};
	char preKeys[256] = {0};

	MyCurve straight;
	straight.Initialize();

	straight.type = LineType::Straight;
	straight.controlPoint.push_back({ 100,150 });
	straight.controlPoint.push_back({ 200,200 });
	straight.controlPoint.push_back({ 300,100 });
	straight.controlPoint.push_back({ 400,60 });
	straight.controlPoint.push_back({ 500,220 });
	straight.controlPoint.push_back({ 600,250 });

	MyCurve cSpline;
	cSpline.Initialize();

	cSpline.type = LineType::CSpline;
	cSpline.controlPoint.push_back({ 100,350 });
	cSpline.controlPoint.push_back({ 200,400 });
	cSpline.controlPoint.push_back({ 300,300 });
	cSpline.controlPoint.push_back({ 400,250 });
	cSpline.controlPoint.push_back({ 500,420 });
	cSpline.controlPoint.push_back({ 600,450 });

	MyCurve bezier;
	bezier.Initialize();

	bezier.type = LineType::Bezier;
	bezier.controlPoint.push_back({ 100,550 });
	bezier.controlPoint.push_back({ 200,600 });
	bezier.controlPoint.push_back({ 300,500 });
	bezier.controlPoint.push_back({ 400,450 });
	bezier.controlPoint.push_back({ 500,620 });
	bezier.controlPoint.push_back({ 600,650 });


	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();

		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		///
		/// ↓更新処理ここから
		///



		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///


		straight.Draw();
		cSpline.Draw();
		bezier.Draw();

		///
		/// ↑描画処理ここまで
		///

		// フレームの終了
		Novice::EndFrame();

		// ESCキーが押されたらループを抜ける
		if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
			break;
		}
	}

	// ライブラリの終了
	Novice::Finalize();
	return 0;
}

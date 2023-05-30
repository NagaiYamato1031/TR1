#include <Novice.h>
#include "Mymath.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix4x4.h"

#include "MyCurve.h"

#include <chrono>

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

	straight.type_ = LineType::Straight;
	straight.interpolate_ = 4;
	straight.controlPoint_.push_back({ 100,150 });
	straight.controlPoint_.push_back({ 200,200 });
	straight.controlPoint_.push_back({ 300,100 });
	straight.controlPoint_.push_back({ 400,60 });
	straight.controlPoint_.push_back({ 500,220 });
	straight.controlPoint_.push_back({ 600,250 });

	MyCurve cSpline;
	cSpline.Initialize();

	cSpline.type_ = LineType::CSpline;
	cSpline.interpolate_ = 8;
	cSpline.controlPoint_.push_back({ 100,350 });
	cSpline.controlPoint_.push_back({ 200,400 });
	cSpline.controlPoint_.push_back({ 300,300 });
	cSpline.controlPoint_.push_back({ 400,250 });
	cSpline.controlPoint_.push_back({ 500,420 });
	cSpline.controlPoint_.push_back({ 600,450 });

	MyCurve bezier;
	bezier.Initialize();

	bezier.type_ = LineType::Bezier;
	bezier.interpolate_ = 32;
	bezier.controlPoint_.push_back({ 100,550 });
	bezier.controlPoint_.push_back({ 200,600 });
	bezier.controlPoint_.push_back({ 300,500 });
	bezier.controlPoint_.push_back({ 400,450 });
	bezier.controlPoint_.push_back({ 500,620 });
	bezier.controlPoint_.push_back({ 600,650 });
	bezier.SetInterp();

	MyCurve bezierToCSpline;
	bezierToCSpline.Initialize();
	bezierToCSpline = bezier.ConvertCSpline(6);
	bezierToCSpline.interpolate_ = 4;
	bezierToCSpline.startPositon_ = { 600.0f,0.0f };

	MyCurve test;
	test.Initialize();

	test.type_ = LineType::Bezier;
	test.interpolate_ = 4;
	test.controlPoint_.push_back({ 800,100 });
	test.controlPoint_.push_back({ 900,100 });

	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();

		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		auto start = std::chrono::system_clock::now();

		///
		/// ↓更新処理ここから
		///

		if (preKeys[DIK_C] == 0 && keys[DIK_C] != 0) {
			isDrawControl = !isDrawControl;
		}
		if (preKeys[DIK_I] == 0 && keys[DIK_I] != 0) {
			isDrawInterp = !isDrawInterp;
		}

		straight.SetInterp();
		cSpline.SetInterp();
		bezier.SetInterp();
		bezierToCSpline.SetInterp();
		test.SetInterp();

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		Novice::ScreenPrintf(10, 60, "Straight");
		straight.Draw();
		Novice::ScreenPrintf(10, 260, "Catmull-rom-Spline");
		cSpline.Draw();
		Novice::ScreenPrintf(10, 460, "Bezier");
		bezier.Draw();
		Novice::ScreenPrintf(700, 460, "BezierToCSpline");
		bezierToCSpline.Draw();
		Novice::ScreenPrintf(800, 60, "test");
		test.Draw();


		///
		/// ↑描画処理ここまで
		///

		// フレームの終了
		Novice::EndFrame();


		auto end = std::chrono::system_clock::now();
		auto time = end - start;
		auto fps = std::chrono::duration_cast<std::chrono::milliseconds>(time).count();
		Novice::ScreenPrintf(10, 10, "%d", ( 1000 / fps));

		// ESCキーが押されたらループを抜ける
		if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
			break;
		}
	}

	// ライブラリの終了
	Novice::Finalize();
	return 0;
}

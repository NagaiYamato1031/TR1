#include <Novice.h>
#include <stdio.h>
#include <fstream>

#include <chrono>
#include "imgui.h"

#include "Mymath.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix4x4.h"

#include "MyCurve.h"
#include "MyCurveManager.h"


const char kWindowTitle[] = "LE2A_12_ナガイ_ヤマト_TR1";
const int kWindowWidth = 1280;
const int kWindowHeight = 720;



// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, kWindowWidth, kWindowHeight);

	// キー入力結果を受け取る箱
	char keys[256] = { 0 };
	char preKeys[256] = { 0 };

	MyCurveManager curveManager;
	curveManager.Initialize();

#pragma region Curve

	//MyCurve straight;
	//straight.Initialize();

	//straight.startPositon_ = { 100,150 };
	//straight.type_ = LineType::Straight;
	//straight.interpolate_ = 4;
	//straight.anchorPoint_.clear();
	//straight.anchorPoint_.push_back({ 0,0 });
	//straight.anchorPoint_.push_back({ 100,50 });
	//straight.anchorPoint_.push_back({ 200,-50 });
	//straight.anchorPoint_.push_back({ 300,-90 });
	//straight.anchorPoint_.push_back({ 400,80 });
	//straight.anchorPoint_.push_back({ 500,100 });

	//curveManager.AddCurve(straight);
//
//#ifdef DEBUG
//
//
//
//	MyCurve cSpline;
//	cSpline.Initialize();
//
//	cSpline.startPositon_ = { 100,350 };
//	cSpline.type_ = LineType::CSpline;
//	cSpline.interpolate_ = 8;
//	cSpline.anchorPoint_.clear();
//	cSpline.anchorPoint_.push_back({ 0,0 });
//	cSpline.anchorPoint_.push_back({ 100,50 });
//	cSpline.anchorPoint_.push_back({ 200,-50 });
//	cSpline.anchorPoint_.push_back({ 300,-90 });
//	cSpline.anchorPoint_.push_back({ 400,80 });
//	cSpline.anchorPoint_.push_back({ 500,100 });
//
//	curveManager.AddCurve(cSpline);
//
//	MyCurve bezier;
//	bezier.Initialize();
//
//	bezier.startPositon_ = { 100,550 };
//	bezier.type_ = LineType::Bezier;
//	bezier.interpolate_ = 32;
//	bezier.anchorPoint_.clear();
//	bezier.anchorPoint_.push_back({ 0,0 });
//	bezier.anchorPoint_.push_back({ 100,50 });
//	bezier.anchorPoint_.push_back({ 200,-50 });
//	bezier.anchorPoint_.push_back({ 300,-90 });
//	bezier.anchorPoint_.push_back({ 400,80 });
//	bezier.anchorPoint_.push_back({ 500,100 });
//	bezier.SetInterp();
//
//	curveManager.AddCurve(bezier);
//
//	int ConvertCSplineInterp = 6;
//
//
//	MyCurve bezierToCSpline;
//	bezierToCSpline.Initialize();
//	bezierToCSpline.anchorPoint_.clear();
//	bezierToCSpline = bezier.ConvertCSpline(ConvertCSplineInterp);
//	bezierToCSpline.interpolate_ = 4;
//	bezierToCSpline.startPositon_ = { 700.0f,550.0f };
//
//	curveManager.AddCurve(bezierToCSpline);
//
//#endif // DEBUG

#pragma endregion

#pragma region FPS 計測用

	const int kAvarageMax = 30;

	int avarageCount = 0;
	double avarages[kAvarageMax] = { 0 };
	double avarage{ 0 };
	double minimum{ 0 };

#pragma endregion

	curveManager.SetInterp();

#pragma region 実用化用の変数達

	bool isActive = false;
	bool canActive = false;
	int	useIndex = 0;

	float t = 0.0f;
	float variation = 0.005f;

	// 四角の色を変えてく
	const uint32_t kMaxColor = 0xFF;
	uint32_t boxXAlphaColor = 0xFF;
	uint32_t boxYAlphaColor = 0xFF;

	Vector2 boxPosition0{ 100,600 };
	Vector2 boxPosition1{ 250,600 };
	Vector2 boxPosition2{ 400,600 };
	Vector2 boxPosition3{ 550,600 };
	Vector2 boxSize{ 50,50 };
	Vector2 boxChangedSize{ 50,50 };

	Vector2 maxValue{ 1,1 };
	Vector2 minValue{ 0,0 };
	Vector2 value{ 0,0 };
	Vector2 normalizeValue{ 0,0 };
	Vector2 changedT = { 0,0 };

#pragma endregion



	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {
		auto start = std::chrono::steady_clock::now();

		// フレームの開始
		Novice::BeginFrame();

		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		///
		/// ↓更新処理ここから
		///

#pragma region ImGui

		ImGui::Begin("infomation");

		ImGui::Text("fps : %.02lf(%.02lf)", avarage, minimum);

		ImGui::End();

		curveManager.ImGuiControls();

		ImGui::Begin("Example");

		ImGui::Checkbox("isActive", &isActive);

		std::string items = "";
		std::vector<MyCurve> curves = curveManager.GetCurves();
		for (MyCurve curve : curves) {
			for (int i = 0; i < curves.size(); i++) {
				items += std::to_string(i);
				items += '\0';
			}
			items += '\0';
		}
		ImGui::Combo("useIndex", &useIndex, items.c_str());

		ImGui::SliderFloat("t", &t, 0.0f, 1.0f);
		ImGui::SliderFloat("variation", &variation, 0.001f, 0.01f);

#pragma endregion

		//if (preKeys[DIK_C] == 0 && keys[DIK_C] != 0) {
		//	isDrawControl = !isDrawControl;
		//}
		//if (preKeys[DIK_I] == 0 && keys[DIK_I] != 0) {
		//	isDrawInterp = !isDrawInterp;
		//}

		curveManager.SetInterp();
		///straight.SetInterp();
		//cSpline.SetInterp();
		//bezier.SetInterp();
		//bezierToCSpline = bezier.ConvertCSpline(ConvertCSplineInterp);
		//bezierToCSpline.interpolate_ = 4;
		//bezierToCSpline.startPositon_ = { 700.0f,550.0f };
		//bezierToCSpline.SetInterp();
		//test.SetInterp();
		if (useIndex < 0 || curveManager.curves_.size() <= 1) {
			canActive = false;
			isActive = false;
		}
		else {
			canActive = true;
		}
		if (canActive) {
			if (isActive) {
				t += variation;
				if (1.0f < t) {
					t = 0.0f;
				}
			}
			maxValue = curveManager.curves_[useIndex].GetMax();
			minValue = curveManager.curves_[useIndex].GetMin();

			value = curveManager.curves_[useIndex].GetValueT(t);
			normalizeValue.x = (value.x - minValue.x) / (maxValue.x - minValue.x);
			// スクリーン座標だから逆にしたい感はある
			normalizeValue.y = (value.y - minValue.y) / (maxValue.y - minValue.y);
			changedT.x = max(0, min(normalizeValue.x, 1));
			//changedT.y = Mymath::Clamp(changedT.y, 0.0f, 1.0f);
			changedT.y = normalizeValue.y;
		}
		ImGui::Text("maxValue : %.02f , %0.2f", maxValue.x, maxValue.y);
		ImGui::Text("minValue : %.02f , %0.2f", minValue.x, minValue.y);
		ImGui::Text("chandedT : %.02f , %0.2f", changedT.x, changedT.y);

		ImGui::End();

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		//Novice::ScreenPrintf(10, 60, "Straight");
		//straight.Draw();
		//Novice::ScreenPrintf(10, 260, "Catmull-rom-Spline");
		//cSpline.Draw();
		//Novice::ScreenPrintf(10, 460, "Bezier");
		//bezier.Draw();
		//Novice::ScreenPrintf(700, 460, "ConvertCSpline");
		//bezierToCSpline.Draw();
		//test.Draw();

		curveManager.Draw();

		static Vector2 pos{ 0,0 };
		float radius = 10;
		radius;
		if (canActive) {
			boxXAlphaColor = (uint32_t)(kMaxColor * changedT.x);
			boxYAlphaColor = (uint32_t)(kMaxColor * changedT.y);

			boxChangedSize.x = boxSize.x + boxSize.x * changedT.x;
			boxChangedSize.y = boxSize.y + boxSize.y * changedT.y;

			pos = curveManager.curves_[useIndex].startPositon_;

			pos.x += value.x;
			pos.y -= value.y;
		}
		int padd = 50;
		// 線上の値
		Novice::DrawBox(int(pos.x - radius), int(pos.y - radius), (int)radius * 2, (int)radius * 2, 0.0f, 0x00FFFFFFFF, kFillModeSolid);


		Novice::ScreenPrintf((int)boxPosition0.x - padd, (int)boxPosition0.y - padd, "No Change");
		Novice::DrawBox(int(boxPosition0.x - boxSize.x / 2), int(boxPosition0.y - boxSize.y / 2), (int)boxSize.x, (int)boxSize.y, 0.0f, 0xFFFFFFFF, kFillModeSolid);
		Novice::ScreenPrintf((int)boxPosition1.x - padd, (int)boxPosition1.y - padd, "Alpha to tX");
		Novice::DrawBox(int(boxPosition1.x - boxSize.x / 2), int(boxPosition1.y - boxSize.y / 2), (int)boxSize.x, (int)boxSize.y, 0.0f, 0xFFFFFF00 + boxXAlphaColor, kFillModeSolid);
		Novice::ScreenPrintf((int)boxPosition2.x - padd, (int)boxPosition2.y - padd, "Alpha to tY");
		Novice::DrawBox(int(boxPosition2.x - boxSize.x / 2), int(boxPosition2.y - boxSize.y / 2), (int)boxSize.x, (int)boxSize.y, 0.0f, 0xFFFFFF00 + boxYAlphaColor, kFillModeSolid);
		Novice::ScreenPrintf((int)boxPosition3.x - padd, (int)boxPosition3.y - padd - 20, "Scale to tX , tY");
		Novice::DrawBox(int(boxPosition3.x - boxChangedSize.x / 2), int(boxPosition3.y - boxChangedSize.y / 2), (int)boxChangedSize.x, (int)boxChangedSize.y, 0.0f, 0xFFFFFFFF, kFillModeSolid);

		//ImGui::Text("color : %X , %X", boxXAlphaColor, boxYAlphaColor);

		///
		/// ↑描画処理ここまで
		///

		// フレームの終了
		Novice::EndFrame();

#pragma region FPS 計測

		auto end = std::chrono::steady_clock::now();
		auto time = end - start;
		auto fps = 1000000.0 / static_cast<double>(std::chrono::duration_cast<std::chrono::microseconds>(time).count());

		avarages[avarageCount] = fps;
		avarageCount++;
		if (kAvarageMax <= avarageCount) {
			avarageCount = 0;
			minimum = avarages[0];
			double temp{ 0 };
			for (int i = 0; i < kAvarageMax; i++) {
				temp += avarages[i];
				if (avarages[i] < minimum) {
					minimum = avarages[i];
				}
			}
			avarage = temp / (double)kAvarageMax;
		}

#pragma endregion


		// ESCキーが押されたらループを抜ける
		if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
			break;
		}
	}

	// ライブラリの終了
	Novice::Finalize();
	return 0;
}

// 正規のスプライン曲線だとバグらないから、ConvertCSplineでバグってる可能性大
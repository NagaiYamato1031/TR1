#include <Novice.h>

#include <chrono>
#include "imgui.h"

#include "Mymath.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix4x4.h"

#include "MyCurve.h"

const char kWindowTitle[] = "LE2A_12_ナガイ_ヤマト_TR1";
const int kWindowWidth = 1280;
const int kWindowHeight = 720;

void ImGuiTree(MyCurve& curve, const char* name) {
	if (ImGui::TreeNode(name)) {
		if (ImGui::RadioButton("Straight", curve.type_ == LineType::Straight)) {
			curve.type_ = LineType::Straight;
		}
		ImGui::SameLine();
		if (ImGui::RadioButton("CSpline", curve.type_ == LineType::CSpline)) {
			curve.type_ = LineType::CSpline;
		}
		ImGui::SameLine();
		if (ImGui::RadioButton("Bezier", curve.type_ == LineType::Bezier)) {
			curve.type_ = LineType::Bezier;
		}

		ImGui::SliderInt("interpolate", &curve.interpolate_, 1, MyCurve::kMaxInterPolation);


		ImGui::BeginChild(ImGui::GetID((void*)0), ImVec2(300, 150), ImGuiWindowFlags_NoTitleBar);
		for (int i = 0; i < curve.anchorPoint_.size(); i++) {
			char str[16];
			sprintf_s(str, "Index:%d", i);
			ImGui::DragFloat2(str, &curve.anchorPoint_[i].x, 1.0f);
		}
		ImGui::EndChild();
		ImGui::TreePop();
	}
}


// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, kWindowWidth, kWindowHeight);

	// キー入力結果を受け取る箱
	char keys[256] = { 0 };
	char preKeys[256] = { 0 };

	MyCurve straight;
	straight.Initialize();

	straight.startPositon_ = { 100,150 };
	straight.type_ = LineType::Straight;
	straight.interpolate_ = 4;
	straight.anchorPoint_.push_back({ 0,0 });
	straight.anchorPoint_.push_back({ 100,50 });
	straight.anchorPoint_.push_back({ 200,-50 });
	straight.anchorPoint_.push_back({ 300,-90 });
	straight.anchorPoint_.push_back({ 400,80 });
	straight.anchorPoint_.push_back({ 500,100 });

	MyCurve cSpline;
	cSpline.Initialize();

	cSpline.startPositon_ = { 100,350 };
	cSpline.type_ = LineType::CSpline;
	cSpline.interpolate_ = 8;
	cSpline.anchorPoint_.push_back({ 0,0 });
	cSpline.anchorPoint_.push_back({ 100,50 });
	cSpline.anchorPoint_.push_back({ 200,-50 });
	cSpline.anchorPoint_.push_back({ 300,-90 });
	cSpline.anchorPoint_.push_back({ 400,80 });
	cSpline.anchorPoint_.push_back({ 500,100 });

	MyCurve bezier;
	bezier.Initialize();

	bezier.startPositon_ = { 100,550 };
	bezier.type_ = LineType::Bezier;
	bezier.interpolate_ = 32;
	bezier.anchorPoint_.push_back({ 0,0 });
	bezier.anchorPoint_.push_back({ 100,50 });
	bezier.anchorPoint_.push_back({ 200,-50 });
	bezier.anchorPoint_.push_back({ 300,-90 });
	bezier.anchorPoint_.push_back({ 400,80 });
	bezier.anchorPoint_.push_back({ 500,100 });
	bezier.SetInterp();

	int ConvertCSplineInterp = 6;

	MyCurve bezierToCSpline;
	bezierToCSpline.Initialize();
	bezierToCSpline = bezier.ConvertCSpline(6);
	bezierToCSpline.interpolate_ = 4;
	bezierToCSpline.startPositon_ = { 700.0f,550.0f };

	MyCurve test;
	test.Initialize();

	test.startPositon_ = { 700,150 };
	test.type_ = LineType::CSpline;
	test.interpolate_ = 4;
	test.anchorPoint_.push_back({ 0,0 });
	test.anchorPoint_.push_back({ 100,50 });
	test.anchorPoint_.push_back({ 200,-50 });

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

#pragma region ImGui

		ImGui::Begin("LineControll");

		ImGui::Checkbox("AnchorDraw", &isDrawControl);
		ImGui::Checkbox("InterpDraw", &isDrawInterp);

		ImGuiTree(straight, "straight");
		ImGuiTree(cSpline, "cspline");
#pragma region Bezier
		if (ImGui::TreeNode("bezier")) {
			if (ImGui::RadioButton("Straight", bezier.type_ == LineType::Straight)) {
				bezier.type_ = LineType::Straight;
			}
			ImGui::SameLine();
			if (ImGui::RadioButton("CSpline", bezier.type_ == LineType::CSpline)) {
				bezier.type_ = LineType::CSpline;
			}
			ImGui::SameLine();
			if (ImGui::RadioButton("Bezier", bezier.type_ == LineType::Bezier)) {
				bezier.type_ = LineType::Bezier;
			}

			ImGui::SliderInt("interpolate", &bezier.interpolate_, 1, MyCurve::kMaxInterPolation);
			ImGui::SliderInt("ConvertInterp", &ConvertCSplineInterp, 1, 8);


			ImGui::BeginChild(ImGui::GetID((void*)0), ImVec2(300, 150), ImGuiWindowFlags_NoTitleBar);
			for (int i = 0; i < bezier.anchorPoint_.size(); i++) {
				char str[16];
				sprintf_s(str, "Index:%d", i);
				ImGui::DragFloat2(str, &bezier.anchorPoint_[i].x, 1.0f);
			}
			ImGui::EndChild();
			ImGui::TreePop();
		}
#pragma endregion
		ImGuiTree(test, "test");

		ImGui::End();

#pragma endregion

		if (preKeys[DIK_C] == 0 && keys[DIK_C] != 0) {
			isDrawControl = !isDrawControl;
		}
		if (preKeys[DIK_I] == 0 && keys[DIK_I] != 0) {
			isDrawInterp = !isDrawInterp;
		}

		straight.SetInterp();
		cSpline.SetInterp();
		bezier.SetInterp();
		bezierToCSpline = bezier.ConvertCSpline(ConvertCSplineInterp);
		bezierToCSpline.interpolate_ = 4;
		bezierToCSpline.startPositon_ = { 700.0f,550.0f };
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
		Novice::ScreenPrintf(700, 460, "ConvertCSpline");
		bezierToCSpline.Draw();
		test.Draw();

		///
		/// ↑描画処理ここまで
		///

		// フレームの終了
		Novice::EndFrame();


		auto end = std::chrono::system_clock::now();
		auto time = end - start;
		auto fps = std::chrono::duration_cast<std::chrono::milliseconds>(time).count();
		Novice::ScreenPrintf(10, 10, "%d", (1000 / fps));

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
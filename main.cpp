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

bool OpenFile(FILE*& fp, const char* path, const char* mode) {
	errno_t errorCode = fopen_s(&fp, path, mode);
	if (errorCode != 0) {
		return false;
	}
	return true;
}
void SaveFileF(MyCurveManager& curveManager) {
	std::ofstream file;
	file.open("file.csv", std::ios::trunc);
	if (!file.is_open()) {
		return;
	}

	std::vector<MyCurve> curves = curveManager.GetCurves();
	for (MyCurve curve : curves) {
		std::string str = "SP,";
		// StartPosition
		//file.write(str.c_str(), str.size());
		// StartPosition
		//str = "%.2f,%.2f\n", curve.startPositon_.x, curve.startPositon_.y;
		str += std::to_string(curve.startPositon_.x);
		str += ',';
		str += std::to_string(curve.startPositon_.y);
		str += '\n';
		file.write(str.c_str(), str.size());
		//file.write(str.c_str(), str.size());
		for (Vector2 ancher : curve.anchorPoint_) {
			// AncherPoint
			str = "AP,";
			//str = "%.2f,%.2f\n", ancher.x, ancher.y;
			str += std::to_string(ancher.x);
			str += ',';
			str += std::to_string(ancher.y);
			str += '\n';

			file.write(str.c_str(), str.size());
		}
		// 曲線の終わりの合図
		str = "TYPE,";
		switch (curve.type_)
		{
		case LineType::Straight:
			str += "Straight";
			break;
		case LineType::CSpline:
			str += "CSpline";
			break;
		case LineType::Bezier:
			str += "Bezier";
			break;
		default:
			str += "Straight";
			break;
		}
		str += '\n';
		file.write(str.c_str(), str.size());

		str = "INTP,";
		str += std::to_string(curve.interpolate_);
		str += '\n';

		file.write(str.c_str(), str.size());
	}
	file.close();
}

void SaveFile(MyCurveManager& curveManager) {
	FILE* fp = nullptr;
	if (!OpenFile(fp, "file.csv", "w")) {
		return;
	}
	std::vector<MyCurve> curves = curveManager.GetCurves();
	for (MyCurve curve : curves) {
		std::string str = "SP,";
		// StartPosition
		//fprintf_s(fp, str.c_str());
		// StartPosition
		//str = "%.2f,%.2f\n", curve.startPositon_.x, curve.startPositon_.y;
		str += std::to_string(curve.startPositon_.x);
		str += ',';
		str += std::to_string(curve.startPositon_.y);
		str += '\n';
		fprintf_s(fp, str.c_str());
		//fprintf_s(fp, str.c_str());
		for (Vector2 ancher : curve.anchorPoint_) {
			//str = "%.2f,%.2f\n", ancher.x, ancher.y;
			// AncherPoint
			str = "AP,";
			str += std::to_string(ancher.x);
			str += ',';
			str += std::to_string(ancher.y);
			str += '\n';

			fprintf_s(fp, str.c_str());
		}

	}
	fclose(fp);
}

void LoadFileF(MyCurveManager& curveManager) {
	std::stringstream ss;
	std::ifstream file;
	file.open("file.csv", std::ios::in);
	if (!file.is_open()) {
		return;
	}
	ss << file.rdbuf();
	file.close();
	curveManager.Initialize();

	std::string line;
	MyCurve curve;
	// ファイルが終わるまで
	while (std::getline(ss, line)) {
		// ストリーム変換
		std::stringstream line_stream(line);

		std::string word;
		// , 区切りで行の先頭文字列を取得
		std::getline(line_stream, word, ',');

		// StartPosition だったら
		if (word.find("SP") == 0) {
			std::getline(line_stream, word, ',');
			float x = (float)std::atof(word.c_str());
			std::getline(line_stream, word, ',');
			float y = (float)std::atof(word.c_str());
			curve.startPositon_ = Vector2(x, y);
		}
		// AncherPosition だったら
		if (word.find("AP") == 0) {
			std::getline(line_stream, word, ',');
			float x = (float)std::atof(word.c_str());
			std::getline(line_stream, word, ',');
			float y = (float)std::atof(word.c_str());
			curve.anchorPoint_.push_back(Vector2(x, y));
		}
		// 曲線の情報が終わったら
		if (word.find("TYPE") == 0) {
			std::getline(line_stream, word, ',');
			if (word.find("Straight") == 0) {
				curve.type_ = LineType::Straight;
			}
			else if (word.find("CSpline") == 0) {
				curve.type_ = LineType::CSpline;
			}
			else if (word.find("Bezier") == 0) {
				curve.type_ = LineType::Bezier;
			}
		}
		if (word.find("INTP") == 0) {
			std::getline(line_stream, word, ',');
			curve.interpolate_ = std::atoi(word.c_str());
			curveManager.AddCurve(curve);
			curve.Initialize();
		}
	}
	curveManager.SetInterp();
}

void LoadFile(MyCurveManager& curveManager) {
	FILE* fp = nullptr;
	if (!OpenFile(fp, "file.csv", "r")) {
		return;
	}
	curveManager.Initialize();
	std::string str;
	//fgets(str.c_str(), 256, fp);

	fclose(fp);
}

void ImGuiMenuFunc(MyCurveManager& curveManager) {
	if (ImGui::BeginMenuBar()) {
		if (ImGui::BeginMenu("File")) {
			if (ImGui::MenuItem("Save")) {
				SaveFileF(curveManager);
			}
			if (ImGui::MenuItem("Load")) {
				LoadFileF(curveManager);
			}
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}
}

void ImGuiTreeFunc(MyCurve& curve, const char* name) {
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


		ImGui::BeginChild(ImGui::GetID((void*)0), ImVec2(0, 0), ImGuiWindowFlags_NoTitleBar);
		for (int i = 0; i < curve.anchorPoint_.size(); i++) {
			char str[64];
			if (i != curve.anchorPoint_.size()) {
				sprintf_s(str, "L:%.2f , I:%d", curve.ancherLength_[i], i);
			}
			else {
				sprintf_s(str, "I:%d", i);
			}
			ImGui::DragFloat2(str, &curve.anchorPoint_[i].x, 1.0f);
		}
		ImGui::EndChild();


		std::string items = "";
		for (int i = 0; i < curve.anchorPoint_.size(); i++) {
			items += std::to_string(i);
			items += '\0';
		}
		items += '\0';
		ImGui::Combo("SelectAdd", &curve.addNumber, items.c_str());
		// 変更されたかどうか
		bool isChanged = false;
		if (ImGui::Button("Add")) {
			curve.AddAncher(curve.addNumber);
			curve.addNumber++;
			isChanged = true;
		}
		ImGui::Combo("SelectResume", &curve.resumeNumber, items.c_str());
		if (ImGui::Button("Resume")) {
			curve.ResumeAncher(curve.resumeNumber);
			isChanged = true;
			if (curve.resumeNumber != 0) {
				curve.resumeNumber--;
			}
		}
		if (isChanged) {
			curve.addNumber = Mymath::Clamp(curve.addNumber, 0, (int)curve.anchorPoint_.size() - 1);
		}

		ImGui::TreePop();
	}
}

void ImGuiTreeFunc(MyCurveManager& curveManager) {
	int num = 0;
	std::vector<MyCurve>& curves = curveManager.GetCurves();
	for (MyCurve& curve : curves) {
		num++;
		std::string name = "curve";
		name += std::to_string(num);
		if (ImGui::TreeNode(name.c_str())) {
			ImGui::DragFloat2("StartPosition", &curve.startPositon_.x, 1.0f);

			if (ImGui::RadioButton("Straight", curve.type_ == LineType::Straight)) {
				curve.type_ = LineType::Straight;
				ImGui::TreePop();
				return;
			}
			ImGui::SameLine();
			if (ImGui::RadioButton("CSpline", curve.type_ == LineType::CSpline)) {
				curve.type_ = LineType::CSpline;
				ImGui::TreePop();
				return;
			}
			ImGui::SameLine();
			if (ImGui::RadioButton("Bezier", curve.type_ == LineType::Bezier)) {
				curve.type_ = LineType::Bezier;
				ImGui::TreePop();
				return;
			}

			ImGui::SliderInt("interpolate", &curve.interpolate_, 1, MyCurve::kMaxInterPolation);
			static float t = 0;
			int radius = 15;
			static Vector2 vec{ 0,0 };
			ImGui::SliderFloat("t", &t, 0.0f, 1.0f);
			vec = curve.GetValueT(t);
			ImGui::Text("%.2f,%.2f", vec.x, vec.y);
			Novice::DrawBox(int(vec.x - radius), int(vec.y - radius), radius * 2, radius * 2, 0.0f, 0x00FFFFFFFF, kFillModeSolid);

			ImGui::BeginChild(ImGui::GetID((void*)0), ImVec2(0, 0), ImGuiWindowFlags_NoTitleBar);
			for (int i = 0; i < curve.anchorPoint_.size(); i++) {
				char str[64];
				sprintf_s(str, "I:%d", i);
				ImGui::DragFloat2(str, &curve.anchorPoint_[i].x, 1.0f);
				/*if (curve.type_ != LineType::Bezier && i != curve.anchorPoint_.size()) {
					sprintf_s(str, "L:%.2f", curve.ancherLength_[i]);
				}*/
				ImGui::SameLine();
				ImGui::Text(str);
			}
			ImGui::EndChild();


			std::string items = "";
			for (int i = 0; i < curve.anchorPoint_.size(); i++) {
				items += std::to_string(i);
				items += '\0';
			}
			items += '\0';
			ImGui::Combo("SelectAdd", &curve.addNumber, items.c_str());
			// 変更されたかどうか
			bool isChanged = false;
			if (ImGui::Button("Add")) {
				curve.AddAncher(curve.addNumber);
				curve.addNumber++;
				isChanged = true;
			}
			ImGui::Combo("SelectResume", &curve.resumeNumber, items.c_str());
			if (ImGui::Button("Resume")) {
				curve.ResumeAncher(curve.resumeNumber);
				isChanged = true;
				if (curve.resumeNumber != 0) {
					curve.resumeNumber--;
				}
			}
			if (isChanged) {
				curve.addNumber = Mymath::Clamp(curve.addNumber, 0, (int)curve.anchorPoint_.size() - 1);
			}

			ImGui::TreePop();
		}
	}
}



// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, kWindowWidth, kWindowHeight);

	// キー入力結果を受け取る箱
	char keys[256] = { 0 };
	char preKeys[256] = { 0 };

	MyCurveManager curveManager;

#pragma region Curve

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

	curveManager.AddCurve(straight);

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

	curveManager.AddCurve(cSpline);

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

	curveManager.AddCurve(bezier);

	int ConvertCSplineInterp = 6;


	MyCurve bezierToCSpline;
	bezierToCSpline.Initialize();
	bezierToCSpline = bezier.ConvertCSpline(ConvertCSplineInterp);
	bezierToCSpline.interpolate_ = 4;
	bezierToCSpline.startPositon_ = { 700.0f,550.0f };

	curveManager.AddCurve(bezierToCSpline);

#pragma endregion

#pragma region FPS 計測用

	const int kAvarageMax = 30;

	int avarageCount = 0;
	double avarages[kAvarageMax] = { 0 };
	double avarage{ 0 };
	double minimum{ 0 };

#pragma endregion

	//// ファイル読み込み、書き込み
	//std::stringstream bufferStream;
	//std::ifstream file;
	//file.open("file.txt");
	//assert(file.is_open());

	//// ファイルの内容を文字列ストリームにコピー
	//bufferStream << file.rdbuf();

	//// ファイルを閉じる
	//file.close();

	curveManager.SetInterp();

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


		ImGui::Begin("LineControll", nullptr, ImGuiWindowFlags_MenuBar);

		ImGuiMenuFunc(curveManager);

		ImGui::Checkbox("AnchorDraw", &isDrawControl);
		ImGui::Checkbox("InterpDraw", &isDrawInterp);

		ImGuiTreeFunc(curveManager);

		//
		//		ImGuiTreeFunc(straight, "straight");
		//		ImGuiTreeFunc(cSpline, "cspline");
		//#pragma region Bezier
		//		if (ImGui::TreeNode("bezier")) {
		//			if (ImGui::RadioButton("Straight", bezier.type_ == LineType::Straight)) {
		//				bezier.type_ = LineType::Straight;
		//			}
		//			ImGui::SameLine();
		//			if (ImGui::RadioButton("CSpline", bezier.type_ == LineType::CSpline)) {
		//				bezier.type_ = LineType::CSpline;
		//			}
		//			ImGui::SameLine();
		//			if (ImGui::RadioButton("Bezier", bezier.type_ == LineType::Bezier)) {
		//				bezier.type_ = LineType::Bezier;
		//			}
		//
		//			ImGui::SliderInt("interpolate", &bezier.interpolate_, 1, MyCurve::kMaxInterPolation);
		//			ImGui::SliderInt("ConvertInterp", &ConvertCSplineInterp, 1, 16);
		//
		//
		//			ImGui::BeginChild(ImGui::GetID((void*)0), ImVec2(300, 150), ImGuiWindowFlags_NoTitleBar);
		//			for (int i = 0; i < bezier.anchorPoint_.size(); i++) {
		//				char str[16];
		//				sprintf_s(str, "Index:%d", i);
		//				ImGui::DragFloat2(str, &bezier.anchorPoint_[i].x, 1.0f);
		//			}
		//			ImGui::EndChild();
		//			ImGui::TreePop();
		//		}
		//#pragma endregion
		//		ImGuiTreeFunc(test, "test");

		ImGui::End();

#pragma endregion

		if (preKeys[DIK_C] == 0 && keys[DIK_C] != 0) {
			isDrawControl = !isDrawControl;
		}
		if (preKeys[DIK_I] == 0 && keys[DIK_I] != 0) {
			isDrawInterp = !isDrawInterp;
		}

		curveManager.SetInterp();
		///straight.SetInterp();
		//cSpline.SetInterp();
		//bezier.SetInterp();
		//bezierToCSpline = bezier.ConvertCSpline(ConvertCSplineInterp);
		//bezierToCSpline.interpolate_ = 4;
		//bezierToCSpline.startPositon_ = { 700.0f,550.0f };
		//bezierToCSpline.SetInterp();
		//test.SetInterp();

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		Novice::ScreenPrintf(10, 60, "Straight");
		//straight.Draw();
		Novice::ScreenPrintf(10, 260, "Catmull-rom-Spline");
		//cSpline.Draw();
		Novice::ScreenPrintf(10, 460, "Bezier");
		//bezier.Draw();
		Novice::ScreenPrintf(700, 460, "ConvertCSpline");
		//bezierToCSpline.Draw();
		//test.Draw();
		curveManager.Draw();


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
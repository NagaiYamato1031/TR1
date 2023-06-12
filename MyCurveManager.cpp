#include "MyCurveManager.h"
#include <Novice.h>
#include <stdio.h>
#include <fstream>
#include <sstream>
#include <string>
#include <sys/stat.h>
#include <direct.h>

#include "imgui.h"

#pragma region ImGui

const char* folderPath = "curveOutput/";

void MakeFolder() {
	struct stat st;
	int result = stat(folderPath, &st);
	if (result == 0) {
		// 既にある
		return;
	}
	else {
		result = _mkdir(folderPath); // フォルダ作成の試行

		if (result == 0) {
			return;
		}
		else {
			perror("_mkdir");
		}
	}
}

bool OpenFile(FILE*& fp, const char* path, const char* mode) {
	errno_t errorCode = fopen_s(&fp, path, mode);
	if (errorCode != 0) {
		return false;
	}
	return true;
}

void SaveFileF(MyCurveManager& curveManager, const char* path) {
	MakeFolder();
	std::ofstream file;
	std::string name = folderPath;
	name += path;
	file.open(name + ".csv", std::ios::trunc);
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
		for (Vector2 anchor : curve.anchorPoint_) {
			// AncherPoint
			str = "AP,";
			//str = "%.2f,%.2f\n", ancher.x, ancher.y;
			str += std::to_string(anchor.x);
			str += ',';
			str += std::to_string(anchor.y);
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

void LoadFileF(MyCurveManager& curveManager, const char* path) {
	std::stringstream ss;
	std::ifstream file;
	std::string name = folderPath;
	name += path;
	file.open(name + ".csv", std::ios::in);
	if (!file.is_open()) {
		return;
	}
	ss << file.rdbuf();
	file.close();
	curveManager.Initialize();

	std::string line;
	MyCurve curve;
	curve.Initialize();
	curve.anchorPoint_.clear();
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
			curve.anchorPoint_.clear();
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

bool ImGuiSaveWindow(MyCurveManager& curveManager, char* path) {
	bool pushButton = false;
	char* name = path;
	ImGui::Begin("SaveConfig");
	ImGui::InputText(".csv", name, sizeof(path));
	path = name;
	if (ImGui::Button("OK")) {
		SaveFileF(curveManager, path);
		pushButton = true;
	}
	ImGui::SameLine();
	if (ImGui::Button("Cancel")) {
		pushButton = true;
	}
	if (pushButton) {
		ImGui::End();
		return false;
	}
	else {
		ImGui::End();
		return true;
	}
}

bool ImGuiLoadWindow(MyCurveManager& curveManager, char* path) {
	bool pushButton = false;
	char* name = path;
	ImGui::Begin("LoadConfig");
	ImGui::InputText(".csv", name, sizeof(path));
	path = name;
	if (ImGui::Button("OK")) {
		LoadFileF(curveManager, path);
		pushButton = true;
	}
	ImGui::SameLine();
	if (ImGui::Button("Cancel")) {
		pushButton = true;
	}
	if (pushButton) {
		ImGui::End();
		return false;
	}
	else {
		ImGui::End();
		return true;
	}
}

void ImGuiMenuFunc(MyCurveManager& curveManager) {
	static bool isSave = false;
	static bool isLoad = false;
	static char path[32] = "file";
	if (ImGui::BeginMenuBar()) {
		if (ImGui::BeginMenu("File")) {
			if (ImGui::MenuItem("Save") || isSave) {
				isSave = true;
				isLoad = false;
			}
			if (ImGui::MenuItem("Load") || isLoad) {
				isLoad = true;
				isSave = false;
			}
			ImGui::EndMenu();

		}
		ImGui::EndMenuBar();
	}
	if (isSave) {
		isSave = ImGuiSaveWindow(curveManager, path);
	}
	else if (isLoad) {
		isLoad = ImGuiLoadWindow(curveManager, path);
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
				//sprintf_s(str, "L:%.2f , I:%d", curve.ancherLength_[i], i);
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


			//static float t = 0;
			//int radius = 15;
			//static Vector2 vec{ 0,0 };
			//ImGui::SliderFloat("t", &t, 0.0f, 1.0f);
			//vec = curve.GetValueT(t);
			////ImGui::Text("%.2f,%.2f", vec.x, vec.y);
			//Novice::DrawBox(int(vec.x - radius), int(vec.y - radius), radius * 2, radius * 2, 0.0f, 0x00FFFFFFFF, kFillModeSolid);

			ImGui::BeginChild(ImGui::GetID((void*)0), ImVec2(0, 200), ImGuiWindowFlags_NoTitleBar);
			for (int i = 0; i < curve.anchorPoint_.size(); i++) {
				char str[64];
				sprintf_s(str, "I:%d", i);
				ImGui::DragFloat2(str, &curve.anchorPoint_[i].x, 1.0f);
				/*if (curve.type_ != LineType::Bezier && i != curve.anchorPoint_.size()) {
					sprintf_s(str, "L:%.2f", curve.ancherLength_[i]);
				}*/
				//ImGui::SameLine();
				//ImGui::Text(str);
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

			// CSpline 曲線への変換をするか
			if (ImGui::TreeNode("Convert")) {
				items = "";
				for (int i = 0; i < curves.size(); i++) {
					items += std::to_string(i);
					items += '\0';
				}
				items += '\0';
				static int index = 0;
				ImGui::Combo("FromCurve", &index, items.c_str());
				static int interp = 8;
				ImGui::SliderInt("ConvertInterp", &interp, 1, MyCurve::kMaxInterPolation);
				if (ImGui::Button("CSpline")) {
					curve = curves[index].ConvertCSpline(interp);
				}
				ImGui::TreePop();
			}

			ImGui::TreePop();
		}
		ImGui::Separator();
	}
}

void MyCurveManager::ImGuiControls() {


	ImGui::Begin("LineControll", nullptr, ImGuiWindowFlags_MenuBar);

	ImGuiMenuFunc(*this);

	ImGui::Checkbox("CurveDraw", &isDraw);
	if (isDraw) {
		ImGui::Bullet();
		ImGui::Checkbox("AnchorDraw", &isDrawControl);
		ImGui::Bullet();
		ImGui::Checkbox("InterpDraw", &isDrawInterp);
		ImGui::Bullet();
		ImGui::Checkbox("LineDraw", &isDrawLine);
	}
	ImGui::Separator();

	ImGuiTreeFunc(*this);

	//resumeNumberImGui::Separator();

	if (ImGui::Button("AddCurve")) {
		MyCurve curve = MyCurve();
		curve.Initialize();
		this->AddCurve(curve);
		if (this->resumeNumber == -1) {
			this->resumeNumber = 0;
		}
	}
	std::string items = "";
	std::vector<MyCurve> curves = this->GetCurves();
	for (MyCurve curve : curves) {
		for (int i = 0; i < curves.size(); i++) {
			items += std::to_string(i);
			items += '\0';
		}
		items += '\0';
	}
	ImGui::Combo("ResumeNumber", &this->resumeNumber, items.c_str());
	if (ImGui::Button("ResumeCurve") && this->resumeNumber != -1) {
		this->ResumeCurve();

	}

	ImGui::End();

}

#pragma endregion

MyCurveManager::MyCurveManager() {
	Initialize();
}
MyCurveManager::~MyCurveManager() {

}

void MyCurveManager::Initialize() {
	curves_.clear();
	resumeNumber = 0;
}


void MyCurveManager::Draw() {
	if (!isDraw)
		return;
	for (MyCurve curve : curves_) {
		curve.Draw();
	}
}
void MyCurveManager::AddCurve(const MyCurve& curve) {
	curves_.push_back(curve);
}

void MyCurveManager::ResumeCurve(int index) {
	curves_.erase(curves_.begin() + index);
}

void MyCurveManager::ResumeCurve() {
	resumeNumber = Mymath::Clamp(resumeNumber, 0, (int)curves_.size() - 1);
	curves_.erase(curves_.begin() + resumeNumber);
	resumeNumber = Mymath::Clamp(resumeNumber, 0, (int)curves_.size() - 1);
}

void MyCurveManager::SetInterp() {
	for (MyCurve& curve : curves_) {
		curve.SetInterp();
	}
}

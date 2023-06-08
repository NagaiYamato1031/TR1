#include "MyCurveManager.h"

MyCurveManager::MyCurveManager() {
	Initialize();
}
MyCurveManager::~MyCurveManager() {

}

void MyCurveManager::Initialize() {
	curves_.clear();
}


void MyCurveManager::Draw() {
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

void MyCurveManager::SetInterp() {
	for (MyCurve& curve : curves_) {
		curve.SetInterp();
	}
}
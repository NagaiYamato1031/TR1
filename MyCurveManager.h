#pragma once
#include <vector>

#include "MyCurve.h"
#include "Mymath.h"

class MyCurveManager
{
public:
	MyCurveManager();
	~MyCurveManager();

	void Initialize();

	void Draw();

	void AddCurve(const MyCurve& curve);

	void ResumeCurve(int index);

	void SetInterp();

	MyCurve& GetCurve(int index) { return curves_[index]; }

	std::vector<MyCurve>& GetCurves() { return curves_; }

	// カーブを管理
	std::vector<MyCurve> curves_;



};


#pragma once
#include <list>

#include "Mymath.h"

enum class LineType {
	Straight,	// 直線
	CSpline,	// Catmull-romスプライン
	Bezier,		// ベジエ
};

extern bool isDrawControl;
extern bool isDrawInterp;

class MyCurve
{
public:

	// 最大補間点数
	const static int kMaxInterPolation = 64;

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 補間点を作成する
	/// </summary>
	void SetInterp();
	/// <summary>
	/// 補間点を作成する
	/// </summary>
	/// <param name="type">曲線のタイプを指定</param>
	void SetInterp(LineType type);


	/// <summary>
	/// 描画
	/// 絶対書き直して自由度高くする
	/// </summary>
	void Draw();

	// 制御点
	std::list<Vector2> controlPoint_;

	// 補間点
	std::list<Vector2> interpPoint_;

	// 線のタイプ
	LineType type_ = LineType::Straight;

	// 点と点の補間数(1 ～ 64)
	int interpolate_ = 8;

	/// <summary>
	/// リストの要素数を確認する
	/// </summary>
	/// <returns>線が描けるか: true, false</returns>
	bool CheckElements();

	/// <summary>
	/// 値を消去する
	/// </summary>
	/// <param name="index">添え字</param>
	///void Remove(int index);

private:

	void InterpStraight();
	void InterpCSpline();
	void InterpBezier();

	void DrawStraight();
	void DrawCSpline();
	void DrawBezier();



};


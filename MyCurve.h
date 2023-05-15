#pragma once
#include <list>

#include "Mymath.h"

enum class LineType {
	Straight,	// 直線
	CSpline,	// Catmull-romスプライン
	Bezier,		// ベジエ
};

class MyCurve
{
public:

	// 最大補間点数
	const static int kMaxInterPolation = 1024;

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 描画
	/// 絶対書き直して自由度高くする
	/// </summary>
	void Draw();
	
	// 制御点
	std::list<Vector2> controlPoint;

	// 線のタイプ
	LineType type = LineType::Straight;

	// 補間率(0 ～ 100 : 0 の場合点と点を通る直線とする)
	//int Interpolate = 100;


	/// <summary>
	/// 値を消去する
	/// </summary>
	/// <param name="index">添え字</param>
	///void Remove(int index);

	/// <summary>
	/// リスト内の要素数を返す
	/// </summary>
	/// <returns>リスト内の要素数</returns>
	int GetCount();

private:

	void DrawStraight();
	void DrawCSpline();
	void DrawBezier();

};


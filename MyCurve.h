#pragma once
#include <vector>

#include "Mymath.h"

enum class LineType {
	Straight,	// 直線
	CSpline,	// Catmull-romスプライン
	Bezier,		// ベジエ
};

// 制御点を描画するか
extern bool isDrawControl;
// 補間点を描画するか
extern bool isDrawInterp;
// 線を描画するか
extern bool isDrawLine;

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
	/// 補間点を作成する(変更したときに呼び出す)
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

	/// <summary>
	/// 曲線を C スプライン曲線に変換する
	/// </summary>
	/// <param name="interpolate">分割数</param>
	/// <returns>C スプライン曲線</returns>
	MyCurve ConvertCSpline(int interpolate);

	/// <summary>
	/// 全体の中での t に近い点の場所を返す、不完全な値参照
	/// </summary>
	/// <param name="t">0 ～ 1</param>
	/// <returns>0 ～ 1 にある t の近似値</returns>
	Vector2 GetValueNearTPoint(float t);
	
	/// <summary>
	/// t の位置を返す(線形補間とか)
	/// </summary>
	/// <param name="t">0 ～ 1</param>
	/// <returns>全体の中での t の値</returns>
	Vector2 GetValueT(float t);


	Vector2 GetMax();
	Vector2 GetMin();

	/// <summary>
	/// 曲線全体の長さを求める
	/// </summary>
	void SetLengthAll();

	// 始点の位置
	Vector2 startPositon_;


	// 制御点
	std::vector<Vector2> anchorPoint_;

	// 補間点
	std::vector<Vector2> interpPoint_;

	// 制御点の間の長さ(ベジェ曲線の時に使えない)
	//std::vector<float> ancherLength_;
	// 補間点間の長さ
	std::vector<float> interpLength_;

	// 曲線全体の長さ
	float curveLength_ = 0;

	// 線のタイプ
	LineType type_ = LineType::Straight;

	// 点と点の補間数(1 ～ 64)
	int interpolate_ = 8;

	int addNumber = 0;
	int resumeNumber = 0;

	/// <summary>
	/// リストの要素数を確認する
	/// </summary>
	/// <returns>線が描けるか: true, false</returns>
	bool CheckElements();

	/// <summary>
	/// 値を消去する
	/// </summary>
	/// <param name="index">添え字</param>
	void ResumeAncher(int index);

	/// <summary>
	/// 値を追加する
	/// </summary>
	void AddAncher();
	/// <summary>
	/// 値を指定した場所に追加する
	/// </summary>
	/// <param name="index">添え字</param>
	void AddAncher(int index);

	void SetStartPosition(const Vector2& pos) {
		startPositon_ = pos;
	}

private:

	float GetInterpLength(int index);

	void InterpStraight();
	void InterpCSpline();
	void InterpBezier();

	//void DrawStraight();
	//void DrawCSpline();
	//void DrawBezier();



};


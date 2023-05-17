#include "MyCurve.h"

#include <Novice.h>

#include "Mymath.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix4x4.h"

bool isDrawControl = true;
bool isDrawInterp = true;

void MyCurve::Initialize() {

}

void MyCurve::SetInterp() {
	// 補間数を制限
	interpolate_ = min(interpolate_, kMaxInterPolation);

	// 補間点を初期化
	interpPoint_.clear();

	// ここは Type によって関数分けしたい
	switch (type_)
	{
	case LineType::Straight:
	{
		// 現在の点と次の点を参照して補間点を決める
		InterpStraight();
	}
	break;
	case LineType::CSpline:
	{
		// 前後四つの点を参照して補間点を決める
		InterpCSpline();
	}
	break;
	case LineType::Bezier:
	{
		// すべての点を参照して補間点を決める
		InterpBezier();
	}
	break;
	default:
		break;
	}
}
void MyCurve::SetInterp(LineType type) {

	type_ = type;
	SetInterp();
	return;

	//// 補間数を制限
	//interpolate_ = max(interpolate_, kMaxInterPolation);
	//
	//// 補間点を初期化
	//interpPoint_.clear();

	//// 補間点を追加
	//switch (type)
	//{
	//case LineType::Straight:
	//	InterpStraight();
	//	break;
	//case LineType::CSpline:
	//	InterpCSpline();
	//	break;
	//case LineType::Bezier:
	//	InterpStraight();
	//	break;
	//default:
	//	break;
	//}

}
// この中ですべての点を参照して描画
void MyCurve::Draw() {
	// 線が描けない場合は終了
	if (!CheckElements()) {
		return;
	}

	if (isDrawControl) {
		// 制御点を描画する
		for (Vector2 p : controlPoint_) {
			Novice::DrawEllipse(int(p.x), int(p.y), 10, 10, 0.0f, RED, kFillModeSolid);
		}
	}
	if (isDrawInterp) {
		// 補間点を描画する
		for (Vector2 p : interpPoint_) {
			Novice::DrawEllipse(int(p.x), int(p.y), 5, 5, 0.0f, 0x00FF0077, kFillModeSolid);
		}
	}

	// 現在の参照する点を基準にして線を描画する
	std::list<Vector2>::iterator current = interpPoint_.begin();
	// 次の点
	std::list<Vector2>::iterator next = current;
	// 次の点に移動させる
	next++;
	// ここから描画
	for (; next != interpPoint_.end(); current++, next++) {
		Novice::DrawLine(int(current->x), int(current->y), int(next->x), int(next->y), WHITE);
	}
	//// ここは Type によって関数分けしたい
	//switch (type_)
	//{
	//case LineType::Straight:
	//{
	//	// 現在の点と次の点を参照して線を描く
	//	DrawStraight();
	//}
	//break;
	//case LineType::CSpline:
	//{
	//	// 前後四つの点を参照して線を描く
	//	DrawCSpline();
	//}
	//break;
	//case LineType::Bezier:
	//{
	//	// すべての点を参照して線を描く
	//	DrawBezier();
	//}
	//break;
	//default:
	//	break;
	//}
}


bool MyCurve::CheckElements() {
	// 点が 2 以下の場合線を描けない
	if (controlPoint_.size() < 2) {
		return false;
	}
	else if (interpPoint_.size() < 2) {
		return false;
	}
	// 点が 2 つ以上あるので線が描ける
	return true;
}


void MyCurve::InterpStraight() {
	// 制御点が存在しない場合処理しない
	if (controlPoint_.size() == 0) {
		return;
	}
	// リスト内の数が 2 以下の時は始点のみ
	if (controlPoint_.size() < 2) {
		interpPoint_ = controlPoint_;
		return;
	}
	///////////////////////////////////////////////
	// 制御点が 2 つ以上あるのでイテレータを用意 //
	///////////////////////////////////////////////

	// 現在の参照する点を基準にして線を描画する
	std::list<Vector2>::iterator current = controlPoint_.begin();
	// 次の点
	std::list<Vector2>::iterator next = current;
	// 次の点に移動させる
	next++;

	//////////
	// 補間 //
	//////////

	// リスト内すべてを参照する
	for (; next != controlPoint_.end(); current++, next++) {
		// 指定した補間数で補間
		interpPoint_.push_back(Mymath::Lerp(*current, *next, 0.0f));
		for (int i = 1; i <= interpolate_; i++) {
			// t を計算
			float t = static_cast<float>(i) / static_cast<float>(interpolate_);
			// t の値で分割
			interpPoint_.push_back(Mymath::Lerp(*current, *next, t));
		}
	}
}
void MyCurve::InterpCSpline() {
	// 要素数が 3 以下なら
	if (controlPoint_.size() < 3) {
		// 直線なので直線で補間
		InterpStraight();
		return;
	}
	///////////////////////////////////////////////
	// 制御点が 3 つ以上あるのでイテレータを用意 //
	///////////////////////////////////////////////

	// 現在の参照する点を基準にして線を描画する
	std::list<Vector2>::iterator current = controlPoint_.begin();
	// 次の点
	std::list<Vector2>::iterator next = current;
	// 次の点に移動させる
	next++;
	// 三つ目の点
	std::list<Vector2>::iterator two_next = next;
	// 移動
	two_next++;

	//////////
	// 補間 //
	//////////

	// 始点
	interpPoint_.push_back(Mymath::CatmullRom(*current, *current, *next, *two_next, 0.0f));
	for (int i = 1; i <= interpolate_; i++) {
		float t = static_cast<float>(i) / static_cast<float>(interpolate_);
		interpPoint_.push_back(Mymath::CatmullRom(*current, *current, *next, *two_next, t));
	}
	// 制御点が 3 の場合補間を終了
	if (controlPoint_.size() == 3) {
		return;
	}
	// 4 つ目の点
	std::list<Vector2>::iterator three_next = two_next;
	// 移動
	three_next++;

	// 要素数が 4 以上の場合繰り返す
	for (; three_next != controlPoint_.end(); current++, next++, two_next++, three_next++) {
		//Novice::DrawLine(int(current->x), int(current->y), int(next->x), int(next->y), WHITE);
		// 補間点
		interpPoint_.push_back(Mymath::CatmullRom(*current, *next, *two_next, *three_next, 0.0f));
		for (int i = 1; i <= interpolate_; i++) {
			float t = static_cast<float>(i) / static_cast<float>(interpolate_);
			interpPoint_.push_back(Mymath::CatmullRom(*current, *next, *two_next, *three_next, t));
		}
	}
	// 終点
	interpPoint_.push_back(Mymath::CatmullRom(*current, *next, *two_next, *two_next, 0.0f));
	for (int i = 1; i <= interpolate_; i++) {
		float t = static_cast<float>(i) / static_cast<float>(interpolate_);
		interpPoint_.push_back(Mymath::CatmullRom(*current, *next, *two_next, *two_next, t));
	}
}
void MyCurve::InterpBezier() {
	// 要素数が 3 以下なら
	if (controlPoint_.size() < 3) {
		// 直線なので直線で補間
		InterpStraight();
		return;
	}
	//////////
	// 補間 //
	//////////

	{
		float t = 0.0f;
		// 補間するための中間点
		std::list<Vector2> preParameters = controlPoint_;
		// 補間した後の中間点
		std::list<Vector2> parameters;
		// 現在の参照する点を基準にして線を描画する
		std::list<Vector2>::iterator current = preParameters.begin();
		// 次の点
		std::list<Vector2>::iterator next = current;
		// 次の点に移動させる
		next++;
		// 一つの線になるまで中間点を作る
		for (;;) {
			// 最初から最後まで線形補間させる
			for (; next != preParameters.end(); current++, next++) {
				parameters.push_back(Mymath::Lerp(*current, *next, t));
			}
			// 線が 1 本だけ描けるようになったら終了
			if (parameters.size() == 2) {
				break;
			}
			// 点が 3 個以上あるなら続ける
			else {
				// リストを更新する
				preParameters = parameters;
				// 使わないので初期化
				parameters.clear();
				// イテレータを初期位置に戻す
				current = preParameters.begin();
				next = current;
				next++;
			}
		}
		// イテレータを初期位置に戻す
		current = parameters.begin();
		next = current;
		next++;
		//interpPoint_.push_back(Mymath::Lerp(*current, *next, t));
		interpPoint_.push_back(Mymath::Lerp(parameters.front(), parameters.back(), t)); 
	}
	// 補間数分計算する
	// 処理の関係で補間点が 1 / 2 になるため二倍にする
	for (int i = 1; i <= interpolate_ * 2; i++) {
		float t = static_cast<float>(i) / static_cast<float>(interpolate_ * 2);
		// 補間するための中間点
		std::list<Vector2> preParameters = controlPoint_;
		// 補間した後の中間点
		std::list<Vector2> parameters;
		// 現在の参照する点を基準にして線を描画する
		std::list<Vector2>::iterator current = preParameters.begin();
		// 次の点
		std::list<Vector2>::iterator next = current;
		// 次の点に移動させる
		next++;
		// 一つの線になるまで中間点を作る
		for (;;) {
			// 最初から最後まで線形補間させる
			for (; next != preParameters.end(); current++, next++) {
				parameters.push_back(Mymath::Lerp(*current, *next, t));
			}
			// 線が 1 本だけ描けるようになったら終了
			if (parameters.size() == 2) {
				break;
			}
			// 点が 3 個以上あるなら続ける
			else {
				// リストを更新する
				preParameters = parameters;
				// 使わないので初期化
				parameters.clear();
				// イテレータを初期位置に戻す
				current = preParameters.begin();
				next = current;
				next++;
			}
		}
		// イテレータを初期位置に戻す
		current = parameters.begin();
		next = current;
		next++;
		//interpPoint_.push_back(Mymath::Lerp(*current, *next, t));
		interpPoint_.push_back(Mymath::Lerp(parameters.front(), parameters.back(), t));
	}
	//Novice::ScreenPrintf(10, 30, "ControlEndPoint x %.2f : y %.2f", controlPoint_.back().x, controlPoint_.back().y);
	//Novice::ScreenPrintf(10, 50, "InterpEndPoint  x %.2f : y %.2f", interpPoint_.back().x, interpPoint_.back().y);
}

void MyCurve::DrawStraight() {
	// 現在の参照する点を基準にして線を描画する
	std::list<Vector2>::iterator current = controlPoint_.begin();
	// 次の点
	std::list<Vector2>::iterator next = current;
	// 次の点に移動させる
	next++;
	// ここから描画
	for (next; next != controlPoint_.end(); next++, current++) {
		Novice::DrawLine(int(current->x), int(current->y), int(next->x), int(next->y), WHITE);
	}
}
void MyCurve::DrawCSpline() {
	// 現在の参照する点を基準にして線を描画する
	std::list<Vector2>::iterator current = controlPoint_.begin();
	// 次の点
	std::list<Vector2>::iterator next = current;
	// 次の点に移動させる
	next++;
	// ここから描画
	// 要素数が 3 以下なら
	if (controlPoint_.size() < 3) {
		for (next; next != controlPoint_.end(); next++, current++) {
			Novice::DrawLine(int(current->x), int(current->y), int(next->x), int(next->y), WHITE);
		}
		return;
	}
	// 要素数が 3 なら
	else if (controlPoint_.size() == 3) {
		// 補間点
		std::list<Vector2> interpList;
		for (int i = 0; i < kMaxInterPolation; i++) {
			float t = static_cast<float>(i) / static_cast<float>(kMaxInterPolation);
			interpList.push_back(Mymath::CatmullRom(*current, *next, *next, *(next++), t));
		}
		std::list<Vector2>::iterator lCurrent = interpList.begin();
		std::list<Vector2>::iterator lNext = lCurrent;
		lNext++;
		for (lNext; lNext != interpList.end(); lNext++, lCurrent++) {
			Novice::DrawLine(int(current->x), int(current->y), int(next->x), int(next->y), WHITE);
		}
	}
	std::list<Vector2>::iterator two_next = next;
	two_next++;
	std::list<Vector2>::iterator three_next = two_next;
	three_next++;

	// 始点
	std::list<Vector2> InterpList;
	for (int i = 0; i < kMaxInterPolation; i++) {
		float t = static_cast<float>(i) / static_cast<float>(kMaxInterPolation);
		InterpList.push_back(Mymath::CatmullRom(*current, *current, *next, *two_next, t));
	}
	// 要素数が 4 以上の場合
	for (; three_next != controlPoint_.end(); current++, next++, two_next++, three_next++) {
		//Novice::DrawLine(int(current->x), int(current->y), int(next->x), int(next->y), WHITE);
		// 補間点
		for (int i = 0; i < kMaxInterPolation; i++) {
			float t = static_cast<float>(i) / static_cast<float>(kMaxInterPolation);
			InterpList.push_back(Mymath::CatmullRom(*current, *next, *two_next, *three_next, t));
		}
		std::list<Vector2>::iterator lCurrent = InterpList.begin();
		std::list<Vector2>::iterator lNext = lCurrent;
		lNext++;
		for (lNext; lNext != InterpList.end(); lNext++, lCurrent++) {
			Novice::DrawLine(int(lCurrent->x), int(lCurrent->y), int(lNext->x), int(lNext->y), WHITE);
		}
		Novice::DrawLine(int(lCurrent->x), int(lCurrent->y), int(two_next->x), int(two_next->y), WHITE);
		InterpList.clear();
	}
	// 終点
	InterpList;
	for (int i = 0; i < kMaxInterPolation; i++) {
		float t = static_cast<float>(i) / static_cast<float>(kMaxInterPolation);
		InterpList.push_back(Mymath::CatmullRom(*current, *next, *two_next, *two_next, t));
	}
	std::list<Vector2>::iterator lCurrent = InterpList.begin();
	std::list<Vector2>::iterator lNext = lCurrent;
	lNext++;
	for (lNext; lNext != InterpList.end(); lNext++, lCurrent++) {
		Novice::DrawLine(int(lCurrent->x), int(lCurrent->y), int(lNext->x), int(lNext->y), WHITE);
	}
	InterpList.clear();
}
void MyCurve::DrawBezier() {
	// 現在の参照する点を基準にして線を描画する
	std::list<Vector2>::iterator current = controlPoint_.begin();
	// 次の点
	std::list<Vector2>::iterator next = current;
	// 次の点に移動させる
	next++;
	// ここから描画
	for (next; next != controlPoint_.end(); next++, current++) {
		Novice::DrawLine(int(current->x), int(current->y), int(next->x), int(next->y), WHITE);
	}
}

//void MyCurve::Remove(int index) {
//	std::list<Vector2>::iterator itr = controlPoint.begin();
//	for (int i = 0; i < index; i++) {
//		itr++;
//	}
//	controlPoint.remove(*itr);
//}
/*

void addInterpolatedPoints(std::list<Point>& pointList, std::list<Point>::iterator start, std::list<Point>::iterator end, int numInterpolationPoints) {
	if (numInterpolationPoints < 1) {
		return;
	}

	std::list<Point> interpolatedPoints;
	for (int i = 0; i <= numInterpolationPoints; i++) {
		double t = (double)i / (double)numInterpolationPoints;
		double u = 1 - t;
		double uuu = u * u * u;
		double uu = u * u;
		double tt = t * t;
		double ttt = tt * t;

		Point interpolatedPoint;
		interpolatedPoint.x =
			uuu * start->x +
			3 *	uu * t * (start->x +
			(end->x - start->x) / 3) +
			3 * u * tt * (start->x +
			2 * (end->x - start->x) / 3) +
			ttt * end->x;

		interpolatedPoint.y = uuu * start->y + 3 * uu * t * (start->y + (end->y - start->y) / 3) + 3 * u * tt * (start->y + 2 * (end->y - start->y) / 3) + ttt * end->y;

		interpolatedPoints.push_back(interpolatedPoint);
	}

	pointList.insert(std::next(start), interpolatedPoints.begin(), interpolatedPoints.end());
}
*/
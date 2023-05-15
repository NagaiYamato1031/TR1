#include "MyCurve.h"

#include <Novice.h>

#include "Mymath.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix4x4.h"

void MyCurve::Initialize() {

}

// この中ですべての点を参照して描画
void MyCurve::Draw() {

	// 制御点を描画する
	for (Vector2 p : controlPoint) {
		Novice::DrawEllipse(int(p.x), int(p.y), 10, 10, 0.0f, RED, kFillModeSolid);
	}

	// ここは Type によって関数分けしたい
	switch (type)
	{
	case LineType::Straight:
	{
		// 現在の点と次の点を参照して線を描く
		DrawStraight();
	}
	break;
	case LineType::CSpline:
	{
		// 前後四つの点を参照して線を描く
		DrawCSpline();
	}
	break;
	case LineType::Bezier:
	{
		// すべての点を参照して線を描く
		DrawBezier();
	}
	break;
	default:
		break;
	}
}

void MyCurve::DrawStraight() {
	// 現在の参照する点を基準にして線を描画する
	std::list<Vector2>::iterator current = controlPoint.begin();
	// 次の点
	std::list<Vector2>::iterator next = current;
	// 次の点に移動させる
	next++;
	// ここから描画
	for (next; next != controlPoint.end(); next++, current++) {
		Novice::DrawLine(int(current->x), int(current->y), int(next->x), int(next->y), WHITE);
	}
}
void MyCurve::DrawCSpline() {
	// 現在の参照する点を基準にして線を描画する
	std::list<Vector2>::iterator current = controlPoint.begin();
	// 次の点
	std::list<Vector2>::iterator next = current;
	// 次の点に移動させる
	next++;
	// ここから描画
	// 要素数が三以下なら
	if (GetCount() < 3) {
		for (next; next != controlPoint.end(); next++, current++) {
			Novice::DrawLine(int(current->x), int(current->y), int(next->x), int(next->y), WHITE);
		}
		return;
	}
	// 要素数が三なら
	else if (GetCount() == 3) {

	}
	// 要素数が 4 以上の場合
	for (next; next != controlPoint.end(); next++, current++) {
		Novice::DrawLine(int(current->x), int(current->y), int(next->x), int(next->y), WHITE);
	}
}
void MyCurve::DrawBezier() {
	// 現在の参照する点を基準にして線を描画する
	std::list<Vector2>::iterator current = controlPoint.begin();
	// 次の点
	std::list<Vector2>::iterator next = current;
	// 次の点に移動させる
	next++;
	// ここから描画
	for (next; next != controlPoint.end(); next++, current++) {
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

int MyCurve::GetCount() {
	int cnt = 0;
	std::list<Vector2>::iterator itr = controlPoint.begin();
	for (itr; itr != controlPoint.end(); itr++) {
		cnt++;
	}
	return cnt;
}
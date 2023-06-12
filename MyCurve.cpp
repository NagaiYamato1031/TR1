#include "MyCurve.h"

#include <Novice.h>

#include "Mymath.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix4x4.h"

bool isDrawControl = true;
bool isDrawInterp = true;
bool isDrawLine = true;

void MyCurve::Initialize() {
	startPositon_ = { 100.0f,100.0f };
	ancherLength_.clear();
	anchorPoint_.clear();
	interpPoint_.clear();
	interpolate_ = 8;
	type_ = LineType::Straight;
	curveLength_ = 0;

	anchorPoint_.push_back({ 0,0 });
	anchorPoint_.push_back({ 100,0 });

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
	SetLengthAll();
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
		int radius = 10;
		// 制御点を描画する
		for (Vector2 p : anchorPoint_) {
			Novice::DrawBox(int(p.x + startPositon_.x - radius), int(-p.y + startPositon_.y - radius), radius * 2, radius * 2, 0.0f, RED, kFillModeSolid);
			//Novice::DrawBox(int(p.x + startPositon_.x - radius), int(p.y + startPositon_.y - radius), radius * 2, radius * 2, 0.0f, RED, kFillModeSolid);
		}
	}
	if (isDrawInterp) {
		int radius = 5;
		// 補間点を描画する
		for (Vector2 p : interpPoint_) {
			Novice::DrawBox(int(p.x + startPositon_.x - radius), int(-p.y + startPositon_.y - radius), radius * 2, radius * 2, 0.0f, 0x00FF0077, kFillModeSolid);
			//Novice::DrawBox(int(p.x + startPositon_.x - radius), int(p.y + startPositon_.y - radius), radius * 2, radius * 2, 0.0f, 0x00FF0077, kFillModeSolid);
		}
	}

	// 現在の参照する点を基準にして線を描画する
	//std::list<Vector2>::iterator current = interpPoint_.begin();
	// 次の点
	//std::list<Vector2>::iterator next = current;
	// 次の点に移動させる
	//next++;
	// ここから描画
	if (isDrawLine) {
		for (int i = 1; i < interpPoint_.size(); i++) {
			Novice::DrawLine(int(interpPoint_[i - 1].x + startPositon_.x), int(-interpPoint_[i - 1].y + startPositon_.y), int(interpPoint_[i].x + startPositon_.x), int(-interpPoint_[i].y + startPositon_.y), WHITE);
			//Novice::DrawLine(int(interpPoint_[i - 1].x + startPositon_.x), int(interpPoint_[i - 1].y + startPositon_.y), int(interpPoint_[i].x + startPositon_.x), int(interpPoint_[i].y + startPositon_.y), WHITE);
		}
	}
	/*for (; next != interpPoint_.end(); current++, next++) {
		Novice::DrawLine(int(current->x + startPositon_.x), int(current->y + startPositon_.y), int(next->x + startPositon_.x), int(next->y + startPositon_.y), WHITE);
	}*/
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

MyCurve MyCurve::ConvertCSpline(int interpolate = 8) {
	// 変換後の曲線
	MyCurve result;
	result.type_ = LineType::CSpline;
	interpolate = Mymath::Clamp(interpolate, 1, (int)interpPoint_.size() - 1);
	//interpolate = (int)Mymath::Clamp((float)interpolate, 1.0f, (float)kMaxInterPolation);
	// 補間数分計算する
	//for (int i = 0; i <= interpolate; i++) {
	//	float t = static_cast<float>(i) / static_cast<float>(interpolate);
	//	// 補間するための中間点
	//	std::vector<Vector2> preParameters = anchorPoint_;
	//	// 補間した後の中間点
	//	std::vector<Vector2> parameters;
	//	// 一つの線になるまで中間点を作る
	//	for (;;) {
	//		// 最初から最後まで線形補間させる
	//		for (int j = 1; j < preParameters.size(); j++) {
	//			parameters.push_back(Mymath::Lerp(preParameters[j - 1], preParameters[j], t));
	//		}
	//		// 線が 1 本だけ描けるようになったら終了
	//		if (parameters.size() == 2) {
	//			break;
	//		}
	//		// 点が 3 個以上あるなら続ける
	//		else {
	//			// リストを更新する
	//			preParameters = parameters;
	//			// 使わないので初期化
	//			parameters.clear();
	//		}
	//	}
	//	resurt.anchorPoint_.push_back(Mymath::Lerp(parameters.front(), parameters.back(), t));
	//}
	// 分割数に合わせて曲線を分割
	result.startPositon_ = startPositon_;
	for (int i = 0; i < interpolate; i++) {
		float t = i / (float)interpolate;
		result.anchorPoint_.push_back(GetValueT(t));
	}
	result.anchorPoint_.push_back(GetValueT(1.0f));
	result.SetInterp();
	return result;
}


Vector2 MyCurve::GetValueNearTPoint(float t) {
	// 値を丸める
	t = Mymath::Clamp(t, 0.0f, 1.0f);
	// 補間点の最大数を取り出す
	int size = (int)interpPoint_.size();
	// t の値を最大数のうちの近い整数に戻す
	int nearIndex = (int)Mymath::Clamp(roundf(size * t) - 1, 0.0f, (float)interpPoint_.size());
	// イテレーター
	/*std::list<Vector2>::iterator index = interpPoint_.begin();
	for (int i = 0; i < nearIndex; i++) {
		index++;
	}*/
	return interpPoint_[nearIndex];
}

Vector2 MyCurve::GetValueT(float t) {
	// t が 0 または 1 なら
	if (t <= interpLength_.front()) {
		//return startPositon_ + anchorPoint_[0];
		// 区間の始まりと終わりを t 表記で取り出してみる
		float startT = GetInterpLength(0) / curveLength_;
		float endT = GetInterpLength(1) / curveLength_;
		// 始まりと終わりの間での t の値を取り出す
		float midT = (t - startT) / (endT - startT);
		//Vector2 result = startPositon_ + interpPoint_[0] + Mymath::Lerp({ 0,0 }, interpPoint_[1], midT);
		Vector2 result = interpPoint_[0] + Mymath::Lerp({ 0,0 }, interpPoint_[1], midT);
		return result;
	}
	else if (interpLength_.back() <= t) {
		int index = (int)interpPoint_.size() - 1;
		float startT = GetInterpLength(index - 1) / curveLength_;
		float endT = GetInterpLength(index) / curveLength_;
		// 始まりと終わりの間での t の値を取り出す
		float midT = (t - startT) / (endT - startT);
		//Vector2 result = startPositon_ + interpPoint_[index - 1] + Mymath::Lerp({ 0,0 }, interpPoint_[index] - interpPoint_[index - 1], midT);
		Vector2 result = interpPoint_[index - 1] + Mymath::Lerp({ 0,0 }, interpPoint_[index] - interpPoint_[index - 1], midT);
		return result;
		//return startPositon_ + interpPoint_.back();
	}

	// 補間点単位で長さを割り出す
	int index = 0;
	float lengthAll = 0;
	for (float length : interpLength_) {
		lengthAll += length;
		// 最後の値(最大の長さ)を取り出す
		if (t * curveLength_ <= lengthAll) {
			break;
		}
		index++;
	}
	// 補間点で区切られてる区間がわかったので、そこから t の値で取り出す
	// 区間の始まりと終わりを t 表記で取り出してみる
	float startT = GetInterpLength(index - 1) / curveLength_;
	float endT = GetInterpLength(index) / curveLength_;
	// 始まりと終わりの間での t の値を取り出す
	float midT = (t - startT) / (endT - startT);
	// 画面上の値を返す
	//Vector2 result = startPositon_ + interpPoint_[index - 1] + Mymath::Lerp({ 0,0 }, interpPoint_[index] - interpPoint_[index - 1], midT);
	// 0 を原点とした値を変えす
	Vector2 result = interpPoint_[index - 1] + Mymath::Lerp({ 0,0 }, interpPoint_[index] - interpPoint_[index - 1], midT);

	return result;
}

Vector2 MyCurve::GetMax() {
	Vector2 max{ 0,0 };
	for (Vector2 value : interpPoint_) {
		if (max.x < value.x) {
			max.x = value.x;
		}
		if (max.y < value.y) {
			max.y = value.y;
		}
	}
	return max;
}

Vector2 MyCurve::GetMin() {
	Vector2 min{ 0,0 };
	for (Vector2 value : interpPoint_) {
		if (value.x < min.x) {
			min.x = value.x;
		}
		if (value.y < min.y) {
			min.y = value.y;
		}
	}
	return min;
}


void MyCurve::SetLengthAll() {
	// 一区間の長さ
	//float length = 0;
	curveLength_ = 0;
	//ancherLength_.clear();
	//ancherLength_.push_back(0);
	interpLength_.clear();
	interpLength_.push_back(0);
	// 補間点すべて
	//int ancher = 1;
	for (int i = 1; i < interpPoint_.size(); i++) {
		// ベジェ曲線の時の対処を書く

		// 次の補間点までの距離を求める
		//length += Mymath::Length(interpPoint_[i] - interpPoint_[i - 1]);
		interpLength_.push_back(Mymath::Length(interpPoint_[i] - interpPoint_[i - 1]));
		curveLength_ += Mymath::Length(interpPoint_[i] - interpPoint_[i - 1]);

		// 補間点と制御点が同じなら
		//if (anchorPoint_[ancher].x == interpPoint_[i].x && anchorPoint_[ancher].y == interpPoint_[i].y) {
		//	ancherLength_.push_back(length);
		//	curveLength_ += length;
		//	// 一つの区間の計算を終わらせる
		//	length = 0;
		//	ancher++;
		//}
	}
	//ancherLength_.erase(ancherLength_.begin());
}

bool MyCurve::CheckElements() {
	// 点が 2 以下の場合線を描けない
	if (anchorPoint_.size() < 2) {
		return false;
	}
	else if (interpPoint_.size() < 2) {
		return false;
	}
	// 点が 2 つ以上あるので線が描ける
	return true;
}


void MyCurve::ResumeAncher(int index) {
	if (anchorPoint_.size() == 2) {
		return;
	}
	anchorPoint_.erase(anchorPoint_.begin() + index);
}

void MyCurve::AddAncher() {
	Vector2 temp = *(anchorPoint_.end() - 1);
	temp.x += 20;
	temp.y += 20;
	anchorPoint_.push_back(temp);
}

void MyCurve::AddAncher(int index) {
	Vector2 temp = *(anchorPoint_.begin() + index);
	temp.x += 20;
	temp.y += 20;
	anchorPoint_.insert(anchorPoint_.begin() + index + 1, temp);
	//anchorPoint_.push_back(temp);
}

// メンバ関数


float MyCurve::GetInterpLength(int index) {
	float result = 0;
	// 添え字まで
	for (int i = 0; i <= index; i++) {
		result += interpLength_[i];
	}
	return result;
}


void MyCurve::InterpStraight() {
	// 制御点が存在しない場合処理しない
	if (anchorPoint_.size() == 0) {
		return;
	}
	// リスト内の数が 2 以下の時は始点のみ
	if (anchorPoint_.size() < 2) {
		interpPoint_ = anchorPoint_;
		return;
	}
	///////////////////////////////////////////////
	// 制御点が 2 つ以上あるのでイテレータを用意 //
	///////////////////////////////////////////////

	// 現在の参照する点を基準にして線を描画する
	//std::list<Vector2>::iterator current = anchorPoint_.begin();
	// 次の点
	//std::list<Vector2>::iterator next = current;
	// 次の点に移動させる
	//next++;

	//////////
	// 補間 //
	//////////

	// リスト内すべてを参照する
	for (int i = 1; i < anchorPoint_.size(); i++) {
		// 指定した補間数で補間
		//interpPoint_.push_back(Mymath::Lerp(anchorPoint_[i - 1], anchorPoint_[i], 0.0f));
		for (int j = 0; j <= interpolate_; j++) {
			// t を計算
			float t = static_cast<float>(j) / static_cast<float>(interpolate_);
			// t の値で分割
			interpPoint_.push_back(Mymath::Lerp(anchorPoint_[i - 1], anchorPoint_[i], t));
		}
	}
	//for (; next != anchorPoint_.end(); current++, next++) {
	//	// 指定した補間数で補間
	//	interpPoint_.push_back(Mymath::Lerp(*current, *next, 0.0f));
	//	for (int i = 1; i <= interpolate_; i++) {
	//		// t を計算
	//		float t = static_cast<float>(i) / static_cast<float>(interpolate_);
	//		// t の値で分割
	//		interpPoint_.push_back(Mymath::Lerp(*current, *next, t));
	//	}
	//}
}
void MyCurve::InterpCSpline() {
	// 要素数が 3 以下なら
	if (anchorPoint_.size() < 3) {
		// 直線なので直線で補間
		InterpStraight();
		return;
	}
	///////////////////////////////////////////////
	// 制御点が 3 つ以上あるのでイテレータを用意 //
	///////////////////////////////////////////////

	// 現在の参照する点を基準にして線を描画する
	//std::list<Vector2>::iterator current = anchorPoint_.begin();
	// 次の点
	//std::list<Vector2>::iterator next = current;
	// 次の点に移動させる
	//next++;
	// 三つ目の点
	//std::list<Vector2>::iterator two_next = next;
	// 移動
	//two_next++;

	//////////
	// 補間 //
	//////////

	// 始点
	//interpPoint_.push_back(Mymath::CatmullRom(anchorPoint_[0], anchorPoint_[0], anchorPoint_[1], anchorPoint_[2], 0.0f));
	for (int i = 0; i <= interpolate_; i++) {
		float t = static_cast<float>(i) / static_cast<float>(interpolate_);
		//interpPoint_.push_back(Mymath::CatmullRom(*current, *current, *next, *two_next, t));
		interpPoint_.push_back(Mymath::CatmullRom(anchorPoint_[0], anchorPoint_[0], anchorPoint_[1], anchorPoint_[2], t));
	}
	// 制御点が 3 の場合補間を終了
	/*if (controlPoint_.size() == 3) {
		return;
	}*/
	// 4 つ目の点
	//std::list<Vector2>::iterator three_next = two_next;
	// 移動
	//three_next++;
	int i = 3;
	// 要素数が 4 以上の場合繰り返す
	for (i = 3; i < anchorPoint_.size(); i++) {
		// 補間点
		//interpPoint_.push_back(Mymath::CatmullRom(anchorPoint_[i - 3], anchorPoint_[i - 2], anchorPoint_[i - 1], anchorPoint_[i], 0.0f));
		for (int j = 0; j <= interpolate_; j++) {
			float t = static_cast<float>(j) / static_cast<float>(interpolate_);
			interpPoint_.push_back(Mymath::CatmullRom(anchorPoint_[i - 3], anchorPoint_[i - 2], anchorPoint_[i - 1], anchorPoint_[i], t));
		}
	}
	//for (; three_next != anchorPoint_.end(); current++, next++, two_next++, three_next++) {
	//	//Novice::DrawLine(int(current->x), int(current->y), int(next->x), int(next->y), WHITE);
	//	// 補間点
	//	interpPoint_.push_back(Mymath::CatmullRom(*current, *next, *two_next, *three_next, 0.0f));
	//	for (int i = 1; i <= interpolate_; i++) {
	//		float t = static_cast<float>(i) / static_cast<float>(interpolate_);
	//		interpPoint_.push_back(Mymath::CatmullRom(*current, *next, *two_next, *three_next, t));
	//	}
	//}
	// 終点
	//interpPoint_.push_back(Mymath::CatmullRom(*current, *next, *two_next, *two_next, 0.0f));
	for (int j = 0; j <= interpolate_; j++) {
		float t = static_cast<float>(j) / static_cast<float>(interpolate_);
		interpPoint_.push_back(Mymath::CatmullRom(anchorPoint_[i - 3], anchorPoint_[i - 2], anchorPoint_[i - 1], anchorPoint_[i - 1], t));
	}
}
void MyCurve::InterpBezier() {
	// 要素数が 3 以下なら
	if (anchorPoint_.size() < 3) {
		// 直線なので直線で補間
		InterpStraight();
		return;
	}
	//////////
	// 補間 //
	//////////

	// 補間数分計算する
	// 処理の関係で補間点が 1 / 2 になるため二倍にする
	for (int i = 0; i <= interpolate_ * 2; i++) {
		float t = static_cast<float>(i) / static_cast<float>(interpolate_ * 2);
		// 補間するための中間点
		std::vector<Vector2> preParameters = anchorPoint_;
		// 補間した後の中間点
		std::vector<Vector2> parameters;
		// 一つの線になるまで中間点を作る
		for (;;) {
			// 最初から最後まで線形補間させる
			for (int j = 1; j < preParameters.size(); j++) {
				parameters.push_back(Mymath::Lerp(preParameters[j - 1], preParameters[j], t));
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
			}
		}
		interpPoint_.push_back(Mymath::Lerp(parameters.front(), parameters.back(), t));
	}


	// 補間数分計算する
	// 処理の関係で補間点が 1 / 2 になるため二倍にする
	//for (int i = 1; i <= interpolate_ * 2; i++) {
	//	float t = static_cast<float>(i) / static_cast<float>(interpolate_ * 2);
	//	// 補間するための中間点
	//	std::list<Vector2> preParameters = anchorPoint_;
	//	// 補間した後の中間点
	//	std::list<Vector2> parameters;
	//	// 現在の参照する点を基準にして線を描画する
	//	std::list<Vector2>::iterator current = preParameters.begin();
	//	// 次の点
	//	std::list<Vector2>::iterator next = current;
	//	// 次の点に移動させる
	//	next++;
	//	// 一つの線になるまで中間点を作る
	//	for (;;) {
	//		// 最初から最後まで線形補間させる
	//		for (; next != preParameters.end(); current++, next++) {
	//			parameters.push_back(Mymath::Lerp(*current, *next, t));
	//		}
	//		// 線が 1 本だけ描けるようになったら終了
	//		if (parameters.size() == 2) {
	//			break;
	//		}
	//		// 点が 3 個以上あるなら続ける
	//		else {
	//			// リストを更新する
	//			preParameters = parameters;
	//			// 使わないので初期化
	//			parameters.clear();
	//			// イテレータを初期位置に戻す
	//			current = preParameters.begin();
	//			next = current;
	//			next++;
	//		}
	//	}
	//	// イテレータを初期位置に戻す
	//	current = parameters.begin();
	//	next = current;
	//	next++;
	//	//interpPoint_.push_back(Mymath::Lerp(*current, *next, t));
	//	interpPoint_.push_back(Mymath::Lerp(parameters.front(), parameters.back(), t));
	//}
	//Novice::ScreenPrintf(10, 30, "ControlEndPoint x %.2f : y %.2f", controlPoint_.back().x, controlPoint_.back().y);
	//Novice::ScreenPrintf(10, 50, "InterpEndPoint  x %.2f : y %.2f", interpPoint_.back().x, interpPoint_.back().y);
}
//
//void MyCurve::DrawStraight() {
//	// 現在の参照する点を基準にして線を描画する
//	std::list<Vector2>::iterator current = anchorPoint_.begin();
//	// 次の点
//	std::list<Vector2>::iterator next = current;
//	// 次の点に移動させる
//	next++;
//	// ここから描画
//	for (next; next != anchorPoint_.end(); next++, current++) {
//		Novice::DrawLine(int(current->x), int(current->y), int(next->x), int(next->y), WHITE);
//	}
//}
//void MyCurve::DrawCSpline() {
//	// 現在の参照する点を基準にして線を描画する
//	std::list<Vector2>::iterator current = anchorPoint_.begin();
//	// 次の点
//	std::list<Vector2>::iterator next = current;
//	// 次の点に移動させる
//	next++;
//	// ここから描画
//	// 要素数が 3 以下なら
//	if (anchorPoint_.size() < 3) {
//		for (next; next != anchorPoint_.end(); next++, current++) {
//			Novice::DrawLine(int(current->x), int(current->y), int(next->x), int(next->y), WHITE);
//		}
//		return;
//	}
//	// 要素数が 3 なら
//	else if (anchorPoint_.size() == 3) {
//		// 補間点
//		std::list<Vector2> interpList;
//		for (int i = 0; i < kMaxInterPolation; i++) {
//			float t = static_cast<float>(i) / static_cast<float>(kMaxInterPolation);
//			interpList.push_back(Mymath::CatmullRom(*current, *next, *next, *(next++), t));
//		}
//		std::list<Vector2>::iterator lCurrent = interpList.begin();
//		std::list<Vector2>::iterator lNext = lCurrent;
//		lNext++;
//		for (lNext; lNext != interpList.end(); lNext++, lCurrent++) {
//			Novice::DrawLine(int(current->x), int(current->y), int(next->x), int(next->y), WHITE);
//		}
//	}
//	std::list<Vector2>::iterator two_next = next;
//	two_next++;
//	std::list<Vector2>::iterator three_next = two_next;
//	three_next++;
//
//	// 始点
//	std::list<Vector2> InterpList;
//	for (int i = 0; i < kMaxInterPolation; i++) {
//		float t = static_cast<float>(i) / static_cast<float>(kMaxInterPolation);
//		InterpList.push_back(Mymath::CatmullRom(*current, *current, *next, *two_next, t));
//	}
//	// 要素数が 4 以上の場合
//	for (; three_next != anchorPoint_.end(); current++, next++, two_next++, three_next++) {
//		//Novice::DrawLine(int(current->x), int(current->y), int(next->x), int(next->y), WHITE);
//		// 補間点
//		for (int i = 0; i < kMaxInterPolation; i++) {
//			float t = static_cast<float>(i) / static_cast<float>(kMaxInterPolation);
//			InterpList.push_back(Mymath::CatmullRom(*current, *next, *two_next, *three_next, t));
//		}
//		std::list<Vector2>::iterator lCurrent = InterpList.begin();
//		std::list<Vector2>::iterator lNext = lCurrent;
//		lNext++;
//		for (lNext; lNext != InterpList.end(); lNext++, lCurrent++) {
//			Novice::DrawLine(int(lCurrent->x), int(lCurrent->y), int(lNext->x), int(lNext->y), WHITE);
//		}
//		Novice::DrawLine(int(lCurrent->x), int(lCurrent->y), int(two_next->x), int(two_next->y), WHITE);
//		InterpList.clear();
//	}
//	// 終点
//	InterpList;
//	for (int i = 0; i < kMaxInterPolation; i++) {
//		float t = static_cast<float>(i) / static_cast<float>(kMaxInterPolation);
//		InterpList.push_back(Mymath::CatmullRom(*current, *next, *two_next, *two_next, t));
//	}
//	std::list<Vector2>::iterator lCurrent = InterpList.begin();
//	std::list<Vector2>::iterator lNext = lCurrent;
//	lNext++;
//	for (lNext; lNext != InterpList.end(); lNext++, lCurrent++) {
//		Novice::DrawLine(int(lCurrent->x), int(lCurrent->y), int(lNext->x), int(lNext->y), WHITE);
//	}
//	InterpList.clear();
//}
//void MyCurve::DrawBezier() {
//	// 現在の参照する点を基準にして線を描画する
//	std::list<Vector2>::iterator current = anchorPoint_.begin();
//	// 次の点
//	std::list<Vector2>::iterator next = current;
//	// 次の点に移動させる
//	next++;
//	// ここから描画
//	for (next; next != anchorPoint_.end(); next++, current++) {
//		Novice::DrawLine(int(current->x), int(current->y), int(next->x), int(next->y), WHITE);
//	}
//}

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
#include "ShapeCollider.h"
#include "ObjectBase.h"
#include "ActorBase.h"

void ShapeCollider::Initialize(Vector2D init_location, Vector2D init_size, float init_radius)
{
	location = init_location;
	local_location = 0;
	box_size = init_size;
	radius = init_radius;
}

bool ShapeCollider::CheckHit(ShapeCollider* object)
{
	//自身も相手も四角なら
	if (this->radius <= 0.f && object->radius <= 0.f)
	{
		return CollisionCheckBtoB(object->GetLocation(), object->GetSize());
	}
	//自身が四角で、相手が円なら
	if (this->radius <= 0.f && object->radius > 0.f)
	{
		return CollisionCheckBtoC(this->location,this->box_size,object->GetLocation(), object->GetRadius());
	}
	//自身も相手も円なら
	if (this->radius > 0.f && object->radius > 0.f)
	{
		return CollisionCheckCtoC(object->GetLocation(), object->GetRadius());
	}
	//自身が円で、相手が四角なら
	if (this->radius > 0.f && object->radius <= 0.f)
	{
		return CollisionCheckBtoC(object->GetLocation(), object->GetSize(), this->location, this->radius);
	}
}


bool ShapeCollider::CollisionCheckBtoB(Vector2D _loc2, Vector2D _size2)const
{
	Vector2D loc1 = this->location;
	Vector2D size1 = this->box_size;
	Vector2D loc2 = _loc2;
	Vector2D size2 = _size2;
	//当たり判定確認
	if (loc1.x < loc2.x + size2.x &&
		loc2.x < loc1.x + size1.x &&
		loc1.y < loc2.y + size2.y &&
		loc2.y < loc1.y + size1.y)
	{
		return true;
	}
	return false;
}

bool ShapeCollider::CollisionCheckBtoC(Vector2D _loc1, Vector2D _size1, Vector2D _loc2, float _radius2)const
{
	bool nResult = false;

	float box_left = _loc1.x - (_size1.x / 2);
	float box_right = _loc1.x + (_size1.x / 2);
	float box_up = _loc1.y + (_size1.y / 2);
	float box_down = _loc1.y - (_size1.y / 2);

	// 四角形の四辺に対して円の半径分だけ足したとき円が重なっていたら
	if ((_loc2.x > box_left - _radius2) &&
		(_loc2.x < box_right + _radius2) &&
		(_loc2.y > box_down - _radius2) &&
		(_loc2.y < box_up + _radius2))
	{
		nResult = true;
		float fl = _radius2 * _radius2;


		// 左
		if (_loc2.x < box_left)
		{
			// 左上
			if ((_loc2.y < box_down))
			{
				if ((DistanceSqrf(box_left, box_down, _loc2.x, _loc2.y) >= fl))
				{
					nResult = false;
				}
			}
			else
			{
				// 左下
				if (_loc2.y > box_up)
				{
					if ((DistanceSqrf(box_left, box_up, _loc2.x, _loc2.y) >= fl))
					{
						nResult = false;
					}
				}
			}
		}
		else
		{
			// 右
			if (_loc2.x > box_right)
			{
				// 右上
				if ((_loc2.y < box_down))
				{
					if ((DistanceSqrf(box_right, box_down, _loc2.x, _loc2.y) >= fl))
					{
						nResult = false;
					}
				}
				else
				{
					// 右下
					if (_loc2.y > box_up)
					{
						if ((DistanceSqrf(box_right, box_up, _loc2.x, _loc2.y) >= fl))
						{
							nResult = false;
						}
					}
				}
			}
		}
	}

	return nResult;
}

bool ShapeCollider::CollisionCheckCtoC(Vector2D _loc, float _radius)const
{
	return this->radius + _radius > sqrt(powf(location.x - _loc.x, 2) + pow(location.y - _loc.y, 2));
}

float ShapeCollider::DistanceSqrf(const float t_x1, const float t_y1, const float t_x2, const float t_y2)const
{
	float dx = t_x2 - t_x1;
	float dy = t_y2 - t_y1;

	return (dx * dx) + (dy * dy);
}

void ShapeCollider::Push(ShapeCollider* hit_object)
{
	Vector2D saveloc = this->location;
	Vector2D savesize = this->box_size;

	Vector2D loc = saveloc;
	Vector2D size = savesize;

	bool stageHitFlg[2][4] = { false };
	float move[4] = { 0 };

	//上下判定用に座標とエリアの調整
	loc = { saveloc.x + 10.f,saveloc.y };
	size = { savesize.x - 20.f, 1.f };

	//プレイヤー上方向の判定
	if (CheckHit(hit_object) && !stageHitFlg[1][top]) {
		stageHitFlg[0][top] = true;
		stageHitFlg[1][top] = true;
	}
	else {
		stageHitFlg[0][top] = false;
	}

	//プレイヤー下方向の判定
	loc.y = saveloc.y + savesize.y + 1;

	if (CheckHit(hit_object) && !stageHitFlg[1][bottom]) {
		stageHitFlg[0][bottom] = true;
		stageHitFlg[1][bottom] = true;
	}
	else {
		stageHitFlg[0][bottom] = false;
	}

	//最初の値に戻す
	loc = saveloc;
	size = savesize;

	//上方向に埋まらないようにする
	if (stageHitFlg[0][top]) {//上方向に埋まっていたら
		float t = (hit_object->GetLocation().y + hit_object->GetSize().y) - loc.y;
		if ((int)t != 0) {
			move[top] = t;
		}
	}

	//下方向に埋まらないようにする
	if (stageHitFlg[0][bottom]) {//下方向に埋まっていたら
		float t = hit_object->GetLocation().y - (loc.y + size.y);
		if ((int)t != 0) {
			move[bottom] = t;
		}
	}

	//左右判定用に座標とエリアの調整
	size = { 1.f,savesize.y - 20.f };

	//プレイヤー左方向の判定
	if (CheckHit(hit_object) && !stageHitFlg[1][left]) {
		stageHitFlg[0][left] = true;
		stageHitFlg[1][left] = true;
	}
	else {
		stageHitFlg[0][left] = false;
	}


	//プレイヤー右方向の判定
	loc.x = saveloc.x + savesize.x + 1;

	if (CheckHit(hit_object) && !stageHitFlg[1][right]) {
		stageHitFlg[0][right] = true;
		stageHitFlg[1][right] = true;
	}
	else {
		stageHitFlg[0][right] = false;
	}

	//最初の値に戻す
	loc = saveloc;
	size = savesize;

	//左方向に埋まらないようにする
	if (stageHitFlg[0][left]) {//左方向に埋まっていたら
		float t = (hit_object->GetLocation().x + hit_object->GetSize().x) - loc.x;
		if ((int)t != 0) {
			move[left] = t;
		}
	}

	//右方向に埋まらないようにする
	if (stageHitFlg[0][right]) {//右方向に埋まっていたら
		float t = hit_object->GetLocation().x - (loc.x + size.x);
		if ((int)t != 0) {
			move[right] = t;
		}
	}


	//上下左右の移動量から移動後も埋まってるか調べる
	//左右移動させてみてまだ埋まってたら戻す
		//上下の座標更新
	loc.y += (move[top] + move[bottom]);
	loc.x += (move[left] + move[right]);

	if (loc.x +
		size.x <
		hit_object->GetLocation().x ||
		loc.x >
		hit_object->GetLocation().x +
		hit_object->GetSize().x) {
		if (stageHitFlg[1][top] || stageHitFlg[1][bottom]) {
			loc.x -= (move[left] + move[right]);
		}
	}

	//移動量を実際のオブジェクトに反映させる
	this->location = loc;

	////当たった辺を保存する
	//for (int i = 0; i < 4; i++)
	//{
	//	myself->hit_flg[i] = stageHitFlg[0][i];
	//}
}
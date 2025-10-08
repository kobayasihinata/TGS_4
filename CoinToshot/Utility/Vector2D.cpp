#include "Vector2D.h"

Vector2D::Vector2D() : x(0.0f), y(0.0f)
{
}
Vector2D::Vector2D(float scalar) : x(scalar), y(scalar)
{
}
Vector2D::Vector2D(float mx, float my) : x(mx), y(my)
{
}
Vector2D::Vector2D(int mx, int my) : x((float)mx),y((float)my)
{
}
Vector2D::Vector2D(float mx, int my) : x(mx), y((float)my)
{
}
Vector2D::Vector2D(int mx, float my) : x((float)mx), y(my)
{
}
Vector2D::~Vector2D()
{
}

Vector2D& Vector2D::operator = (const Vector2D& location)
{
	this->x = location.x;
	this->y = location.y;

	return (*this);
}

bool Vector2D::operator == (const Vector2D& location)
{
	return (this->x == location.x && this->y == location.y);
}

bool Vector2D::operator != (const Vector2D& location)
{
	return (this->x != location.x || this->y != location.y);
}
bool Vector2D::operator > (const Vector2D& location)
{
	return (this->x > location.x && this->y > location.y);
}
bool Vector2D::operator >= (const Vector2D& location)
{
	return (this->x >= location.x && this->y >= location.y);
}
bool Vector2D::operator < (const Vector2D& location)
{
	return (this->x < location.x && this->y < location.y);
}
bool Vector2D::operator <= (const Vector2D& location)
{
	return (this->x <= location.x && this->y <= location.y);
}


const Vector2D Vector2D::operator + (const Vector2D& location)const
{
	return Vector2D((this->x + location.x), this->y + location.y);
}

Vector2D& Vector2D::operator += (const Vector2D& location)
{
	this->x += location.x;
	this->y += location.y;

	return (*this);
}

const Vector2D Vector2D::operator - (const Vector2D& location)const
{
	return Vector2D((this->x - location.x), (this->y - location.y));
}

Vector2D& Vector2D::operator -= (const Vector2D& location)
{
	this->x -= location.x;
	this->y -= location.y;

	return (*this);
}

const Vector2D Vector2D::operator * (const float& scalar)const
{
	return Vector2D((this->x * scalar), (this->y * scalar));
}

const Vector2D Vector2D::operator * (const Vector2D& location)const
{
	return Vector2D((this->x * location.x), (this->y * location.y));
}

Vector2D& Vector2D::operator *= (const float& scalar)
{
	this->x *= scalar;
	this->y *= scalar;

	return (*this);
}

Vector2D& Vector2D::operator *= (const Vector2D& location)
{
	this->x *= location.x;
	this->y *= location.y;

	return (*this);
}

const Vector2D Vector2D::operator / (const float& scalar)const
{
	return Vector2D((this->x / scalar), (this->y / scalar));
}

const Vector2D Vector2D::operator / (const Vector2D& location)const
{
	return Vector2D((this->x / location.x), (this->y / location.y));
}

Vector2D& Vector2D::operator /= (const float& scalar)
{
	this->x /= scalar;
	this->y /= scalar;

	return (*this);
}

Vector2D& Vector2D::operator /= (const Vector2D& location)
{
	this->x /= location.x;
	this->y /= location.y;

	return (*this);
}

void Vector2D::ToInt(int* x, int* y)const
{
	(*x) = static_cast<int>(this->x);
	(*y) = static_cast<int>(this->y);
}

//void Vector2D::DrawBoxV2(Vector2D _loc1, Vector2D _loc2, unsigned int _color, bool _fill_flg)const
//{
//	DrawBoxAA(_loc1.x, _loc1.y, _loc2.x, _loc2.y, _color, _fill_flg);
//}




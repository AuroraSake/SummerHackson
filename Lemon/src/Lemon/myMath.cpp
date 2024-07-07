#pragma  once
#include "myMath.h"
//_____________________________________________________________________//
//
// myMath
// 
// WIP版
// 
// 数学タイプの包み的なもの
// vec2 ~ vec4 float の並びだけ
//
// Vector3:
//  演算機能を含み
//  DirectXMathタイプの抽象化としてやり取りが出来る
//_____________________________________________________________________//


DirectX::XMFLOAT3 Vector3::XMFLOAT3() const
{
    return DirectX::XMFLOAT3(this->x, this->y, this->z);
}
DirectX::XMVECTOR Vector3::XMVECTOR() const
{
    return DirectX::XMLoadFloat3(&this->XMFLOAT3());
}

Vector3::Vector3(vec2 other)
    : x(other.x), y(other.y), z(0.0f) {}
Vector3::Vector3(const DirectX::XMFLOAT3 other)
    : x(other.x), y(other.y), z(other.z) {}
Vector3::Vector3(const DirectX::XMVECTOR& other)
{
    DirectX::XMFLOAT3 temp; DirectX::XMStoreFloat3(&temp, other);
    this->x = temp.x;
    this->y = temp.y;
    this->z = temp.z;
}
Vector3::Vector3(float _x, float _y, float _z)
    : x(_x), y(_y), z(_z) {}
Vector3::Vector3(float _value)
    : x(_value), y(_value), z(_value) {}
Vector3::Vector3(const Vector3& other)
    : x(other.x), y(other.y), z(other.z) {}







Vector3 Vector3::operator-() const
{
    Vector3 v;
    v.x = -x;
    v.y = -y;
    v.z = -z;
    return v;
}
Vector3& Vector3::operator+=(const Vector3& rhs)
{
    this->x += rhs.x;
    this->y += rhs.y;
    this->z += rhs.z;
    return *this;
}
Vector3& Vector3::operator-=(const Vector3& rhs)
{
    return *this += -rhs;
}
Vector3& Vector3::operator*=(const Vector3& rhs)
{
    this->x *= rhs.x;
    this->y *= rhs.y;
    this->z *= rhs.z;
    return *this;
}
Vector3& Vector3::operator/=(const Vector3& rhs)
{
    this->x /= rhs.x;
    this->y /= rhs.y;
    this->z /= rhs.z;
    return *this;
}
Vector3 operator+(Vector3 lhs, const Vector3& rhs)
{
    return lhs += rhs;
}
Vector3 operator-(Vector3 lhs, const Vector3& rhs)
{
    return lhs -= rhs;
}
Vector3 operator*(Vector3 lhs, const Vector3& rhs)
{
    return lhs *= rhs;
}
Vector3 operator/(Vector3 lhs, const Vector3& rhs)
{
    return lhs /= rhs;
}








float Vector3::MagnitudeSq() const
{
    return x * x + y * y + z * z;
}
float Vector3::Magnitude() const
{
    return sqrtf(MagnitudeSq());
}

Vector3 Vector3::Normalize() const
{
    return *this / Magnitude();
}

Vector3 Dot(Vector3& lhs, Vector3& rhs)
{
    Vector3 temp = lhs * rhs;
    return  temp.x + temp.y + temp.z;
}

Vector3 Cross(Vector3& lhs, Vector3& rhs)
{
    return Vector3(lhs.y * rhs.z - lhs.z * rhs.y, lhs.z * rhs.x - lhs.x * rhs.z, lhs.x * rhs.y - lhs.y * rhs.x);
}

Vector3 Vector3::Rotate(const Vector3& other)
{
    DirectX::XMMATRIX rotateMtx = DirectX::XMMatrixRotationRollPitchYawFromVector(other.XMVECTOR());
    DirectX::XMFLOAT3 temp;
    XMStoreFloat3( &temp, DirectX::XMVector3TransformCoord(this->XMVECTOR(), rotateMtx));
    return Vector3(temp.x, temp.y, temp.z);
}
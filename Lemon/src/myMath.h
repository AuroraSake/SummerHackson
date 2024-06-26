#pragma once

#include <math.h>
#include <DirectXMath.h>

//_____________________________________________________________________//
//
// myMath.h
// 
// WIP��
// 
// ���w�^�C�v�̕�ݓI�Ȃ���
// vec2 ~ vec4 float �̕��т���
//
// Vector3:
//  ���Z�@�\���܂�
//  DirectXMath�^�C�v�̒��ۉ��Ƃ��Ă���肪�o����
//_____________________________________________________________________//

//vec2 ~ vec4 float �̕��т���
//�ꎞ�΍�
//vec2���� ���Z�q���g����
struct vec2
{
    float x;
    float y;

    vec2() : x(0.0f), y(0.0f)
    {}
    vec2(float _x, float _y) : x(_x), y(_y)
    {}

    vec2 operator-() const
    {
        vec2 opposite;
        opposite.x = -x;
        opposite.y = -y;
        return opposite;
    }

    vec2& operator+=(const vec2& rhs)
    {
        this->x = x + rhs.x;
        this->y = y + rhs.y;
        return *this;
    }

    vec2& operator/(const float rhs)
    {
        this->x = x / rhs;
        this->y = y / rhs;
        return *this;
    }

    friend vec2 operator+(vec2 lhs, const vec2& rhs)
    {
        return lhs += rhs;
    }

    friend vec2 operator-(vec2 lhs, const vec2& rhs)
    {
        return lhs += -rhs;
    }
};
struct vec3
{
    float x;
    float y;
    float z;
};
struct vec4
{
    float x;
    float y;
    float z;
    float w;
};


//Vector3
//�R�[�h��streamline���̂��߂̃f�[�^�^�C�v
//Have operations, use as interface for XM types
//to do: 
//  SIMP �œK���̂���
//  �v�Z��DirectX�֐��̕�݂ɂ���
struct Vector3
{
public:
    float x = 0.0f, y = 0.0f, z = 0.0f;

public:
    Vector3() {}
    Vector3(vec2 other);
    Vector3(const DirectX::XMFLOAT3 other);
    Vector3(const DirectX::XMVECTOR& other);
    Vector3(float _x, float _y, float _z);
    Vector3(float _value);
    Vector3(const Vector3&);

    ~Vector3() {}


    float MagnitudeSq() const;
    float Magnitude() const;
    vec3  raw() const;
    Vector3 Normalize() const;

    Vector3 operator-() const;
    Vector3& operator+=(const Vector3& rhs);
    Vector3& operator-=(const Vector3& rhs);
    Vector3& operator*=(const Vector3& rhs);
    Vector3& operator/=(const Vector3& rhs);
    friend Vector3 operator+(Vector3 lhs, const Vector3& rhs);
    friend Vector3 operator-(Vector3 lhs, const Vector3& rhs);
    friend Vector3 operator*(Vector3 lhs, const Vector3& rhs);
    friend Vector3 operator/(Vector3 lhs, const Vector3& rhs);



    //DirectX::XMFLOAT3 XMFLOAT3(const Vector3& other);
    //DirectX::XMVECTOR XMVECTOR(const Vector3& other);

    DirectX::XMFLOAT3 XMFLOAT3() const;
    DirectX::XMVECTOR XMVECTOR() const;


    Vector3 Rotate(const Vector3& other);
};

Vector3 Dot(Vector3& lhs, Vector3& rhs);
Vector3 Cross(Vector3& lhs, Vector3& rhs);
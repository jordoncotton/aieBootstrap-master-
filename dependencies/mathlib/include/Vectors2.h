#pragma once

class Vector2
{
	float xPos;
	float yPos;

public:
	Vector2();
	~Vector2();
	Vector2(float x, float y);

	float GetX();
	float GetY();
	float magnitude();
	float Distance(Vector2& other);
	float DotProduct(Vector2& other);

	Vector2 Normalize();
	Vector2 operator+(Vector2 &rhs);
	Vector2 operator-(Vector2 &rhs);
	Vector2 operator*(float &rhs);


	bool operator==(Vector2 &rhs);
	bool operator!=(Vector2 &rhs);
};

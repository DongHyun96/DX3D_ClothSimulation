#pragma once

struct Vector3
{
	Vector3()
	{
	}

	Vector3(float x, float y, float z)
		:x(x), y(y), z(z)
	{
	}

	Vector3(XMFLOAT3 value)
		:x(value.x), y(value.y), z(value.z)
	{
	}

	Vector3(XMVECTOR value)
	{
		x = XMVectorGetX(value);
		y = XMVectorGetY(value);
		z = XMVectorGetZ(value);
	}

	operator XMFLOAT3()
	{
		return XMFLOAT3(x, y, z);
	}

	operator XMVECTOR()
	{
		return XMVectorSet(x, y, z, 0.f);
	}

	void operator+=(const Vector3& other)
	{
		this->x += other.x;
		this->y += other.y;
		this->z += other.z;
	}

	void operator-=(const Vector3& other)
	{
		this->x -= other.x;
		this->y -= other.y;
		this->z -= other.z;
	}

	float operator[](const UINT& index)
	{
		switch (index)
		{
		case 0:	 return x;
		case 1:	 return y;
		case 2:	 return z;
		default: return 0.f;
		}
	}


	Vector3 operator+(const Vector3& other) const
	{
		return Vector3
		(
			this->x + other.x,
			this->y + other.y,
			this->z + other.z
		);
	}

	Vector3 operator-(const Vector3& other) const
	{
		return Vector3
		(
			this->x - other.x,
			this->y - other.y,
			this->z - other.z
		);
	}


	Vector3 operator*(const float& value) const
	{
		return Vector3
		(
			this->x * value,
			this->y * value,
			this->z * value
		);
	}

	friend Vector3 operator*(const float& scalar, const Vector3& other) 
	{
		return other * scalar;
	}

	Vector3 operator/(const float& value) const
	{
		return Vector3
		(
			this->x / value,
			this->y / value,
			this->z / value
		);
	}

	void operator*=(const float& value)
	{
		this->x *= value;
		this->y *= value;
		this->z *= value;
	}

	void operator/=(const float& value)
	{
		this->x /= value;
		this->y /= value;
		this->z /= value;
	}

	bool operator==(const Vector3& other)
	{
		return (this->x == other.x) && (this->y == other.y) && (this->z == other.z);
	}

	float Length()
	{
		return Vector3(XMVector3Length(*this)).x;
	}

	Vector3 GetNormalized()
	{
		return XMVector3Normalize(*this);
	}

	void Normalize()
	{
		*this = XMVector3Normalize(*this);
	}

	static float Dot(const Vector3& v1, const Vector3& v2)
	{
		return Vector3(XMVector3Dot(const_cast<Vector3&>(v1), const_cast<Vector3&>(v2))).x;
	}

	static Vector3 Cross(const Vector3& v1, const Vector3& v2)
	{
		return XMVector3Cross(const_cast<Vector3&>(v1), const_cast<Vector3&>(v2));
	}

	float x{};
	float y{};
	float z{};
};

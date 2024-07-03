#include "Vector3.h"

namespace dw
{
	// Constructors
	Vector3::Vector3() : x(0.0), y(0.0), z(0.0) {}
	Vector3::Vector3(float x, float y, float z) : x(x), y(y), z(z) {}
	Vector3::Vector3(const Vector3& otherVector) : x(otherVector.x), y(otherVector.y), z(otherVector.z) {}
	Vector3::Vector3(Vector3&& rhs) noexcept
	{
		x = std::move(rhs.x);
		y = std::move(rhs.y);
		z = std::move(rhs.z);

		rhs.reset();
	}

	// Destructor (no extra functionality needed as of now)
	Vector3::~Vector3() {}

	// Reinitializes the fields
	void Vector3::reset()
	{
		x = 0.0;
		y = 0.0;
		z = 0.0;
	}

	// Swaps pointers between values (or values between pointers..?)
	void Vector3::swap(Vector3& v)
	{
		std::swap(x, v.x);
		std::swap(y, v.y);
		std::swap(z, v.z);
	}

	// Copy & Swap
	Vector3& Vector3::operator = (Vector3 rhs)
	{
		swap(rhs);
		return *this;
	}

	// C string getter
	const char* Vector3::c_str()
	{
		char buffer[100];
		snprintf(buffer, sizeof(buffer), "<%f, %f, %f>", x, y, z);

		return buffer;
	}

	// Class version getter
	const char* Vector3::version()
	{
		return DW_VECTOR_3_VERSION;
	}

	// Bool equality operator
	bool Vector3::operator == (const Vector3& rhs) const
	{
		return this->x == rhs.x && this->y == rhs.y && this->z == rhs.z;
	}

	// Vector3 on Vector3 operators
	Vector3 Vector3::operator + (const Vector3& rhs) const
	{
		return Vector3(x + rhs.x, y + rhs.y, z + rhs.z);
	}

	Vector3 Vector3::operator - (const Vector3& rhs) const
	{
		return Vector3(x - rhs.x, y - rhs.y, z - rhs.z);
	}

	Vector3 Vector3::operator * (const Vector3& rhs) const
	{
		return Vector3(x * rhs.x, y * rhs.y, z * rhs.z);
	}

	Vector3 Vector3::operator / (const Vector3& rhs) const
	{
		return Vector3(x / rhs.x, y / rhs.y, z / rhs.z);
	}

	// Extra assignment operators
	Vector3 Vector3::operator += (const float d)
	{
		x = x + d;
		y = y + d;
		z = z + d;
		return *this;
	}

	Vector3 Vector3::operator -= (const float d)
	{
		x = x - d;
		y = y - d;
		z = z - d;
		return *this;
	}

	Vector3 Vector3::operator *= (const float d)
	{
		x = x * d;
		y = y * d;
		z = z * d;
		return *this;
	}

	Vector3 Vector3::operator /= (const float d)
	{
		x = x / d;
		y = y / d;
		z = z / d;
		return *this;
	}

	// Assignment with Vector3 operators
	Vector3 Vector3::operator += (const Vector3& v)
	{
		x = x + v.x;
		y = y + v.y;
		z = z + v.z;
		return *this;
	}

	Vector3 Vector3::operator -= (const Vector3& v)
	{
		x = x - v.x;
		y = y - v.y;
		z = z - v.z;
		return *this;
	}

	// Vector access operator
	float Vector3::operator [] (int i) const
	{
		if (i == 0)
		{
			return this->x;
		}
		else if (i == 1)
		{
			return this->y;
		}
		else if (i == 2)
		{
			return this->z;
		}
		else
		{
			throw std::runtime_error("Error accessing vector value. Invalid index.");
		}
	}

	float& Vector3::operator [] (int i)
	{
		if (i == 0)
		{
			return x;
		}
		else if (i == 1)
		{
			return y;
		}
		else if (i == 2)
		{
			return z;
		}
		else
		{
			throw std::runtime_error("Error accessing vector value. Invalid index.");
		}
	}

	// Vector operations
	float Vector3::dot(const Vector3& rhs) const
	{
		Vector3 multiplied = *this * rhs;
		return multiplied.x + multiplied.y + multiplied.z;
	}

	Vector3 Vector3::cross(const Vector3& rhs) const
	{
		return Vector3((this->y * rhs.z) - (this->z * rhs.y),
			(this->z * rhs.x) - (this->x * rhs.z),
			(this->x * rhs.y) - (this->y * rhs.x));
	}

	float Vector3::magnitude()
	{
		return sqrt((this->x * this->x) + (this->y * this->y) + (this->z * this->z));
	}

	Vector3 Vector3::unit()
	{
		return *this / this->magnitude();
	}

	void Vector3::setByIndex(int i, float value)
	{
		if (i == 0)
		{
			x = value;
		}
		else if (i == 1)
		{
			y = value;
		}
		else if (i == 2)
		{
			z = value;
		}
		else
		{
			throw std::runtime_error("Error accessing vector value. Invalid index.");
		}
	}

	Vector3 Vector3::rotateByQuaternion(float i, float j, float k, float s) const
	{
		Vector3 u = Vector3(i, j, k);
		return (2 * dot(u) * u) + ((2 * s * s - 1) * (*this)) + (2 * s * u.cross(*this));
	}

	// Float on Vector3 operators
	Vector3 operator + (Vector3 v, const float& d)
	{
		v += d;
		return v;
	}

	Vector3 operator - (Vector3 v, const float& d)
	{
		v -= d;
		return v;
	}

	Vector3 operator * (Vector3 v, const float& d)
	{
		v *= d;
		return v;
	}

	Vector3 operator / (Vector3 v, const float& d)
	{
		v /= d;
		return v;
	}

	// Direction irrelevant for + and *, disallowing dividing or subtracting vector3 from a float
	Vector3 operator + (const float& d, Vector3 v)
	{
		v += d;
		return v;
	}
	Vector3 operator * (const float& d, Vector3 v)
	{
		v *= d;
		return v;
	}
}

// Left bit shift overload for use with std::cout
std::ostream& operator << (std::ostream& os, const dw::Vector3& rhs)
{
	os << "<" << rhs.x << ", " << rhs.y << ", " << rhs.z << ">";
	return os;
}

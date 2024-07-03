#ifndef DW_VECTOR_3
#define DW_VECTOR_3

// Version number macro
#define DW_VECTOR_3_VERSION "1.0.6"

#include <memory>
#include <cstdio>
#include <iostream>

namespace dw
{
	class Vector3
	{
	public:
		float x = 0.0;
		float y = 0.0;
		float z = 0.0;

		Vector3();
		Vector3(float x, float y = 0, float z = 0);
		Vector3(const Vector3&);
		Vector3(Vector3&&) noexcept;
		~Vector3();

		Vector3& operator = (Vector3);
		bool operator == (const Vector3&) const;
		Vector3 operator + (const Vector3&) const;
		Vector3 operator - (const Vector3&) const;
		Vector3 operator * (const Vector3&) const;
		Vector3 operator / (const Vector3&) const;

		Vector3 operator += (const float);
		Vector3 operator -= (const float);
		Vector3 operator *= (const float);
		Vector3 operator /= (const float);

		Vector3 operator += (const Vector3&);
		Vector3 operator -= (const Vector3&);

		float operator [] (int) const;

		float& operator [](int);


		void reset();
		void swap(Vector3&);
		const char* c_str();
		const char* version();

		float dot(const Vector3&) const;
		Vector3 cross(const Vector3&) const;
		float magnitude();
		Vector3 unit();

		void setByIndex(int, float);

		Vector3 rotateByQuaternion(float, float, float, float) const;
	};

	Vector3 operator + (Vector3, const float&);
	Vector3 operator - (Vector3, const float&);
	Vector3 operator * (Vector3, const float&);
	Vector3 operator / (Vector3, const float&);

	Vector3 operator + (const float&, Vector3);
	Vector3 operator * (const float&, Vector3);

	class Ray
	{
	public:
		dw::Vector3 origin;
		dw::Vector3 direction;

		Ray(dw::Vector3 origin, dw::Vector3 direction) : origin(origin), direction(direction) {}
	};
}


// Left bit shift overload for use with std::cout
std::ostream& operator << (std::ostream& os, const dw::Vector3& rhs);


#endif
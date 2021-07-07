#pragma once
#include <array>
#include <iostream>

#include "Core.h"

namespace jmk {

#define DIM2 2
#define DIM3 3

	template <typename coordinate_type, size_t dimensions = DIM3 >
	class Vector 
	{
		static_assert(std::is_arithmetic_v<coordinate_type>, "Vector class can only store integral or floating points values");

		std::array<coordinate_type, dimensions> coords;
		bool is_normalized = false;

	public:
		Vector() {}

		Vector(std::array<coordinate_type, dimensions> _coords) : coords(_coords) {}

		// TODO this looks bad in 2D case. Need to change based on template parameter dimension
		Vector(coordinate_type _x, coordinate_type _y, coordinate_type _z = 0.0) : coords({_x,_y,_z}) {}

		// Eqality check
		bool operator==(const Vector<coordinate_type, dimensions>&);

		// Not equal check operator
		bool operator!=(const Vector<coordinate_type, dimensions>&);

		// Less than operator for comparison
		bool operator<(Vector<coordinate_type, dimensions>&);

		// Greater than operator for comparison
		bool operator>(const Vector<coordinate_type, dimensions>&);

		// Substraction operator. Substract x and y component separately.
		Vector<coordinate_type, dimensions> operator-(const Vector<coordinate_type, dimensions>&);

		// Greater than operator for comparison
		Vector<coordinate_type, dimensions> operator+(const Vector<coordinate_type, dimensions>&);

		coordinate_type operator[](int);

		// Dot product
		float dot(Vector<coordinate_type, dimensions>& v1, Vector<coordinate_type, dimensions>& v2);

		// Cross product
		Vector<coordinate_type, dimensions> cross(const Vector<coordinate_type, dimensions>&);

		// Return the magnitude of the the vector (mod(A) / |A|)
		float magnitude();

		// Normalize the vector
		void normalize();

		// Returns the underline array object 
		std::array<coordinate_type, dimensions> data();
	};

	typedef Vector<float, DIM2>		Vector2f;
	typedef Vector<float, DIM3>		Vector3f;


	template<typename coordinate_type, size_t dimensions>
	inline bool Vector<coordinate_type, dimensions>::operator==(const Vector<coordinate_type, dimensions>&)
	{
		return false;
	}
	

	template<typename coordinate_type, size_t dimensions>
	inline bool Vector<coordinate_type, dimensions>::operator!=(const Vector<coordinate_type, dimensions>&)
	{
		return false;
	}
	

	template<typename coordinate_type, size_t dimensions>
	inline bool Vector<coordinate_type, dimensions>::operator<(Vector<coordinate_type, dimensions>&)
	{
		return false;
	}

	
	template<typename coordinate_type, size_t dimensions>
	inline bool Vector<coordinate_type, dimensions>::operator>(const Vector<coordinate_type, dimensions>&)
	{
		return false;
	}

	
	template<typename coordinate_type, size_t dimensions>
	inline Vector<coordinate_type, dimensions> Vector<coordinate_type, dimensions>::operator-(const Vector<coordinate_type, dimensions>&)
	{
		return Vector<coordinate_type, dimensions>();
	}

	
	template<typename coordinate_type, size_t dimensions>
	inline Vector<coordinate_type, dimensions> Vector<coordinate_type, dimensions>::operator+(const Vector<coordinate_type, dimensions>&)
	{
		return Vector<coordinate_type, dimensions>();
	}

	template<typename coordinate_type, size_t dimensions>
	inline coordinate_type Vector<coordinate_type, dimensions>::operator[](int _index)
	{
		if (_index >= coords.size()) {
			std::cout << "Index out of bounds";
			return coordinate_type();
		}

		return coords[_index];
	}


	template<typename coordinate_type, size_t dimensions>
	inline float Vector<coordinate_type, dimensions>::dot(Vector<coordinate_type, dimensions>& v1, Vector<coordinate_type, dimensions>& v2)
	{
		return 0.0f;
	}


	template<typename coordinate_type, size_t dimensions>
	inline Vector<coordinate_type, dimensions> Vector<coordinate_type, dimensions>::cross(const Vector<coordinate_type, dimensions>&)
	{
		return Vector<coordinate_type, dimensions>();
	}

	
	template<typename coordinate_type, size_t dimensions>
	inline float Vector<coordinate_type, dimensions>::magnitude()
	{
		return 0.0f;
	}


	template<typename coordinate_type, size_t dimensions>
	inline void Vector<coordinate_type, dimensions>::normalize()
	{
	}


	template<typename coordinate_type, size_t dimensions>
	inline std::array<coordinate_type, dimensions> Vector<coordinate_type, dimensions>::data()
	{
		return std::array<coordinate_type, dimensions>();
	}

	
	template<typename coordinate_type, size_t dimensions>
	float dotProduct(Vector<coordinate_type, dimensions>& v1, Vector<coordinate_type, dimensions>& v2);


	template<typename coordinate_type, size_t dimensions>
	Vector<coordinate_type, dimensions> crossProduct(Vector<coordinate_type, dimensions>& v1, Vector<coordinate_type, dimensions>& v2);

}
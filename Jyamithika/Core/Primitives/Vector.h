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

		template<typename coordinate_type, size_t dimensions>
		friend float dotProduct(Vector<coordinate_type, dimensions>& v1, Vector<coordinate_type, dimensions>& v2);

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

		coordinate_type operator[](int) const;

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
	inline bool Vector<coordinate_type, dimensions>::operator==(const Vector<coordinate_type, dimensions>& _other)
	{
		bool is_equal = false;

		for (int i = 0; i < dimensions; i++)
		{
			if (!isEqualD(coords[i], _other.coords[i]))
				return false;
		}
		return true;
	}
	

	template<typename coordinate_type, size_t dimensions>
	inline bool Vector<coordinate_type, dimensions>::operator!=(const Vector<coordinate_type, dimensions>& _other)
	{
		return !(*this == _other);
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
	inline Vector<coordinate_type, dimensions> Vector<coordinate_type, dimensions>::operator-(const Vector<coordinate_type, dimensions>& _other)
	{
		std::array<coordinate_type, dimensions> temp_array;

		for (int i = 0; i < dimensions; i++)
			temp_array[i] = coords[i] - _other.coords[i];

		return Vector<coordinate_type, dimensions>(temp_array);
	}

	
	template<typename coordinate_type, size_t dimensions>
	inline Vector<coordinate_type, dimensions> Vector<coordinate_type, dimensions>::operator+(const Vector<coordinate_type, dimensions>& _other)
	{
		std::array<coordinate_type, dimensions> temp_array;

		for (int i = 0; i < dimensions; i++)
			temp_array[i] = coords[i] + _other.coords[i];

		return Vector<coordinate_type, dimensions>(temp_array);
	}

	
	template<typename coordinate_type, size_t dimensions>
	inline coordinate_type Vector<coordinate_type, dimensions>::operator[](int _index) const
	{
		if (_index >= coords.size()) {
			std::cout << "Index out of bounds";
			return coordinate_type();
		}

		return coords[_index];
	}
	

	template<typename coordinate_type, size_t dimensions>
	inline float Vector<coordinate_type, dimensions>::magnitude()
	{
		float value = 0.0f;
		for (int i = 0; i < dimensions; i++)
			value += pow(coords[i], 2.0);

		return sqrt(value);
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
	float dotProduct(Vector<coordinate_type, dimensions>& v1, Vector<coordinate_type, dimensions>& v2)
	{
		if (v1.coords.size() != v2.coords.size())
			return FLT_MIN;

		float product = 0;
		for (int i = 0; i < v1.coords.size(); i++)
			product = product + v1[i] * v2[i];
		return product;
	}


	template<typename coordinate_type, size_t dimensions>
	Vector<coordinate_type, dimensions> crossProduct(Vector<coordinate_type, dimensions>& v1, Vector<coordinate_type, dimensions>& v2);

}
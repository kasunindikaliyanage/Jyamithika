#pragma once
#include <array>
#include <iostream>

// TODO : Vector should move to ../Core

#include "../Base/Core.h"

namespace jmk {

#define DIM2 2
#define DIM3 3

#define X 0
#define Y 1
#define Z 2

	template <typename coordinate_type, size_t dimensions = DIM3 >
	class Vector 
	{
		static_assert(std::is_arithmetic_v<coordinate_type>, "Vector class can only store integral or floating points values");
		static_assert(dimensions >= DIM2, "Vector dimension atleast should be 2D");

		std::array<coordinate_type, dimensions> coords = {};
		bool is_normalized = false;

		template<typename coordinate_type, size_t dimensions>
		friend float dotProduct(const Vector<coordinate_type, dimensions>& v1, const Vector<coordinate_type, dimensions>& v2);

	public:
		Vector() {}

		Vector(std::array<coordinate_type, dimensions> _coords) : coords(_coords) {}

		Vector(coordinate_type _x, coordinate_type _y, coordinate_type _z) : coords({_x,_y,_z}) {}

		Vector(coordinate_type _x, coordinate_type _y) : coords({ _x,_y}) {}

		//Vector(const Vector<coordinate_type, other_dimensions>&);

		// Eqality check
		bool operator==(const Vector<coordinate_type, dimensions>&) const;

		// Not equal check operator
		bool operator!=(const Vector<coordinate_type, dimensions>&) const;

		// Less than operator for comparison
		bool operator<(const Vector<coordinate_type, dimensions>&) const;

		// Greater than operator for comparison
		bool operator>(const Vector<coordinate_type, dimensions>&) const;

		Vector<coordinate_type, dimensions> operator*(coordinate_type value);

		// Substraction operator. Substract x and y component separately.
		Vector<coordinate_type, dimensions> operator-(const Vector<coordinate_type, dimensions>&) const;

		// Greater than operator for comparison
		Vector<coordinate_type, dimensions> operator+(const Vector<coordinate_type, dimensions>&) const;

		//TODO make this to modifiable
		coordinate_type operator[](const unsigned int) const;

		// Dot product
		float dot(Vector<coordinate_type, dimensions>& v1, Vector<coordinate_type, dimensions>& v2);

		// Cross product
		Vector<coordinate_type, dimensions> cross(const Vector<coordinate_type, dimensions>&);

		void assign(const unsigned int dim, coordinate_type value);

		// Return the magnitude of the the vector (mod(A) / |A|)
		float magnitude() const;

		// Normalize the vector
		void normalize();

		// Returns the underline array object 
		std::array<coordinate_type, dimensions> data();
	};

	typedef Vector<float, DIM2>		Vector2f;
	typedef Vector<float, DIM3>		Vector3f;


	template<typename coordinate_type, size_t dimensions>
	inline bool Vector<coordinate_type, dimensions>::operator==(const Vector<coordinate_type, dimensions>& _other) const
	{
		for (int i = 0; i < dimensions; i++)
		{
			if (!isEqualD(coords[i], _other.coords[i]))
				return false;
		}
		return true;
	}
	

	template<typename coordinate_type, size_t dimensions>
	inline bool Vector<coordinate_type, dimensions>::operator!=(const Vector<coordinate_type, dimensions>& _other) const
	{
		return !(*this == _other);
	}
	

	template<typename coordinate_type, size_t dimensions>
	inline bool Vector<coordinate_type, dimensions>::operator<(const Vector<coordinate_type, dimensions>& _other) const
	{
		for (size_t i = 0; i < dimensions; i++)
		{
			if (this->coords[i] < _other.coords[i])
				return true;
			else if (this->coords[i] > _other.coords[i])
				return false;
		}

		return false;
	}

	
	template<typename coordinate_type, size_t dimensions>
	inline bool Vector<coordinate_type, dimensions>::operator>(const Vector<coordinate_type, dimensions>& _other) const
	{
		if (*this == _other)
			return false;
		return !(*this < _other);
	}

	template<typename coordinate_type, size_t dimensions>
	inline Vector<coordinate_type, dimensions> Vector<coordinate_type, dimensions>::operator*(coordinate_type value)
	{
		std::array<coordinate_type, dimensions> temp_array;

		for (int i = 0; i < dimensions; i++)
			temp_array[i] = coords[i] * value;

		return Vector<coordinate_type, dimensions>(temp_array);
	}

	
	template<typename coordinate_type, size_t dimensions>
	inline Vector<coordinate_type, dimensions> Vector<coordinate_type, dimensions>::operator-( const Vector<coordinate_type, dimensions>& _other) const
	{
		std::array<coordinate_type, dimensions> temp_array;

		for (int i = 0; i < dimensions; i++)
			temp_array[i] = coords[i] - _other.coords[i];

		return Vector<coordinate_type, dimensions>(temp_array);
	}

	
	template<typename coordinate_type, size_t dimensions>
	inline Vector<coordinate_type, dimensions> Vector<coordinate_type, dimensions>::operator+(const Vector<coordinate_type, dimensions>& _other) const
	{
		std::array<coordinate_type, dimensions> temp_array;

		for (int i = 0; i < dimensions; i++)
			temp_array[i] = coords[i] + _other.coords[i];

		return Vector<coordinate_type, dimensions>(temp_array);
	}

	
	template<typename coordinate_type, size_t dimensions>
	inline coordinate_type Vector<coordinate_type, dimensions>::operator[](const unsigned int _index) const
	{
		if (_index >= coords.size()) {
			std::cout << "Index out of bounds";
			return coordinate_type();
		}

		return coords[_index];
	}
	

	template<typename coordinate_type, size_t dimensions>
	inline void Vector<coordinate_type, dimensions>::assign(const unsigned int _index, coordinate_type value)
	{
		if (_index >= coords.size()) {
			std::cout << "Index out of bounds";
		}

		coords[_index] = value;
	}

	template<typename coordinate_type, size_t dimensions>
	inline float Vector<coordinate_type, dimensions>::magnitude() const
	{
		float value = 0.0f;
		for (int i = 0; i < dimensions; i++)
			value += pow(coords[i], 2.0);

		return sqrt(value);
	}


	template<typename coordinate_type, size_t dimensions>
	inline void Vector<coordinate_type, dimensions>::normalize()
	{
		auto mag = magnitude();
		for (int i = 0; i < dimensions; i++)
			assign(i, coords[i] / mag);
	}


	template<typename coordinate_type, size_t dimensions>
	inline std::array<coordinate_type, dimensions> Vector<coordinate_type, dimensions>::data()
	{
		return std::array<coordinate_type, dimensions>();
	}

	
	template<typename coordinate_type, size_t dimensions>
	float dotProduct(const Vector<coordinate_type, dimensions>& v1, const Vector<coordinate_type, dimensions>& v2)
	{
		if (v1.coords.size() != v2.coords.size())
			return FLT_MIN;

		float product = 0;
		for (size_t i = 0; i < v1.coords.size(); i++)
			product = product + v1[i] * v2[i];
		return product;
	}

	Vector3f crossProduct3d(Vector3f a, Vector3f b);

	float corssProduct2d(Vector2f a, Vector2f b);

	Vector2f prependicluar(Vector2f&);

	float scalerTripleProduct(Vector3f a, Vector3f b, Vector3f c);
	
	bool orthogonal(Vector3f a, Vector3f b);
}
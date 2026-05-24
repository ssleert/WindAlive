module;
#include <cmath>
#include <windalive.hpp>
export module Math.Vector;

export namespace Math {
  struct Vector2 {
    float x, y;

    fn operator+(Vector2 other) const -> Vector2 {
      return {x + other.x, y + other.y};
    }

    fn operator-(Vector2 other) const -> Vector2 {
      return {x - other.x, y - other.y};
    }

    fn operator-() const -> Vector2 {
      return {-x, -y};
    }

    fn operator*(float scalar) const -> Vector2 {
      return {x * scalar, y * scalar};
    }

    fn operator/(float scalar) const -> Vector2 {
      return {x / scalar, y / scalar};
    }

    fn operator+=(Vector2 other) -> Vector2& {
      x += other.x;
      y += other.y;
      return *this;
    }

    fn operator-=(Vector2 other) -> Vector2& {
      x -= other.x;
      y -= other.y;
      return *this;
    }

    fn operator*=(float scalar) -> Vector2& {
      x *= scalar;
      y *= scalar;
      return *this;
    }

    fn operator/=(float scalar) -> Vector2& {
      x /= scalar;
      y /= scalar;
      return *this;
    }

    fn dot(Vector2 other) const -> float {
      return x * other.x + y * other.y;
    }

    fn lengthSquared() const -> float {
      return x * x + y * y;
    }

    fn length() const -> float {
      return std::sqrtf(lengthSquared());
    }

    fn normalize() -> Vector2 {
      float len = length();
      if (len == 0.0f) return {0.0f, 0.0f};
      return *this /= len;
    }
  };

  struct Vector3 {
    float x, y, z;

    fn operator+(Vector3 other) const -> Vector3 {
      return {x + other.x, y + other.y, z + other.z};
    }

    fn operator-(Vector3 other) const -> Vector3 {
      return {x - other.x, y - other.y, z - other.z};
    }

    fn operator-() const -> Vector3 {
      return {-x, -y, -z};
    }

    fn operator*(float scalar) const -> Vector3 {
      return {x * scalar, y * scalar, z * scalar};
    }

    fn operator/(float scalar) const -> Vector3 {
      return {x / scalar, y / scalar, z / scalar};
    }

    fn operator+=(Vector3 other) -> Vector3& {
      x += other.x;
      y += other.y;
      z += other.z;
      return *this;
    }

    fn operator-=(Vector3 other) -> Vector3& {
      x -= other.x;
      y -= other.y;
      z -= other.z;
      return *this;
    }

    fn operator*=(float scalar) -> Vector3& {
      x *= scalar;
      y *= scalar;
      z *= scalar;
      return *this;
    }

    fn operator/=(float scalar) -> Vector3& {
      x /= scalar;
      y /= scalar;
      z /= scalar;
      return *this;
    }

    fn dot(Vector3 other) const -> float {
      return x * other.x + y * other.y + z * other.z;
    }

    fn cross(Vector3 other) const -> Vector3 {
      return {
        y * other.z - z * other.y,
        z * other.x - x * other.z,
        x * other.y - y * other.x
      };
    }

    fn lengthSquared() const -> float {
      return x * x + y * y + z * z;
    }

    fn length() const -> float {
      return std::sqrtf(lengthSquared());
    }

    fn normalize() -> Vector3 {
      float len = length();
      if (len == 0.0f) return {0.0f, 0.0f, 0.0f};
      return *this /= len;
    }
  };
}

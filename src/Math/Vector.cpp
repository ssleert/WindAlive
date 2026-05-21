module;
#include <cmath>
#include <windalive.hpp>
export module Math.Vector;

export namespace Math {
  struct Vector2 {
    float x, y;

    auto operator+(Vector2 other) const -> Vector2 {
      return {x + other.x, y + other.y};
    }

    auto operator-(Vector2 other) const -> Vector2 {
      return {x - other.x, y - other.y};
    }

    auto operator-() const -> Vector2 {
      return {-x, -y};
    }

    auto operator*(float scalar) const -> Vector2 {
      return {x * scalar, y * scalar};
    }

    auto operator/(float scalar) const -> Vector2 {
      return {x / scalar, y / scalar};
    }

    auto operator+=(Vector2 other) -> Vector2& {
      x += other.x;
      y += other.y;
      return *this;
    }

    auto operator-=(Vector2 other) -> Vector2& {
      x -= other.x;
      y -= other.y;
      return *this;
    }

    auto operator*=(float scalar) -> Vector2& {
      x *= scalar;
      y *= scalar;
      return *this;
    }

    auto operator/=(float scalar) -> Vector2& {
      x /= scalar;
      y /= scalar;
      return *this;
    }

    auto dot(Vector2 other) const -> float {
      return x * other.x + y * other.y;
    }

    auto lengthSquared() const -> float {
      return x * x + y * y;
    }

    auto length() const -> float {
      return std::sqrtf(lengthSquared());
    }

    auto normalize() -> Vector2 {
      float len = length();
      if (len == 0.0f) return {0.0f, 0.0f};
      return *this /= len;
    }
  };

  struct Vector3 {
    float x, y, z;

    auto operator+(Vector3 other) const -> Vector3 {
      return {x + other.x, y + other.y, z + other.z};
    }

    auto operator-(Vector3 other) const -> Vector3 {
      return {x - other.x, y - other.y, z - other.z};
    }

    auto operator-() const -> Vector3 {
      return {-x, -y, -z};
    }

    auto operator*(float scalar) const -> Vector3 {
      return {x * scalar, y * scalar, z * scalar};
    }

    auto operator/(float scalar) const -> Vector3 {
      return {x / scalar, y / scalar, z / scalar};
    }

    auto operator+=(Vector3 other) -> Vector3& {
      x += other.x;
      y += other.y;
      z += other.z;
      return *this;
    }

    auto operator-=(Vector3 other) -> Vector3& {
      x -= other.x;
      y -= other.y;
      z -= other.z;
      return *this;
    }

    auto operator*=(float scalar) -> Vector3& {
      x *= scalar;
      y *= scalar;
      z *= scalar;
      return *this;
    }

    auto operator/=(float scalar) -> Vector3& {
      x /= scalar;
      y /= scalar;
      z /= scalar;
      return *this;
    }

    auto dot(Vector3 other) const -> float {
      return x * other.x + y * other.y + z * other.z;
    }

    auto cross(Vector3 other) const -> Vector3 {
      return {
        y * other.z - z * other.y,
        z * other.x - x * other.z,
        x * other.y - y * other.x
      };
    }

    auto lengthSquared() const -> float {
      return x * x + y * y + z * z;
    }

    auto length() const -> float {
      return std::sqrtf(lengthSquared());
    }

    auto normalize() -> Vector3 {
      float len = length();
      if (len == 0.0f) return {0.0f, 0.0f, 0.0f};
      return *this /= len;
    }
  };
}

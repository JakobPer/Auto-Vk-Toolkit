#include <exekutor.hpp>

namespace xk
{
	glm::vec2 get_angles_from_direction_yaw_pitch(const glm::vec3& direction)
	{
		glm::vec3 floorProjection(direction.x, 0.0f, direction.z);
		float directionLength = glm::length(floorProjection);
		float updownAngle = glm::atan(direction.y, directionLength);
		float leftrightAngle = -glm::atan(direction.x, -direction.z);
		return glm::vec2(leftrightAngle, updownAngle);
	}

	glm::vec2 get_angles_from_direction_roll_pitch(const glm::vec3& direction)
	{
		glm::vec3 wallProjection(direction.x, direction.y, 0.0f);
		float directionLength = glm::length(wallProjection);
		float rollAngle = glm::atan(direction.z, directionLength);
		float leftrightAngle = -glm::atan(direction.x, direction.y);
		return glm::vec2(leftrightAngle, rollAngle);
	}

	glm::vec3 get_translation_from_matrix(const glm::mat4& m)
	{
		return glm::vec3(m[3][0], m[3][1], m[3][2]);
	}

	bool solve_system_of_equations(const glm::dmat3& A, const glm::dvec3& c, glm::dvec3& outX)
	{
		if (glm::abs(glm::determinant(A)) < glm::epsilon<double>()) {
			return false;
		}

		// Determinant is not 0 => we can aply the Cramer rule
		outX = glm::inverse(glm::transpose(A)) * c;
		return true;
	}

	bool solve_system_of_equations(const glm::dmat2& A, const glm::dvec2& c, glm::dvec2& outX)
	{
		if (glm::abs(glm::determinant(A)) < glm::epsilon<double>()) {
			return false;
		}

		// Determinant is not 0 => we can aply the Cramer rule
		outX = glm::inverse(glm::transpose(A)) * c;
		return true;
	}

	bool points_in_same_direction(const glm::dvec2& a, const glm::dvec2& b)
	{
		return glm::dot(a, b) > 0.0f;
	}

	bool points_in_same_direction(const glm::dvec3& a, const glm::dvec3& b)
	{
		return glm::dot(a, b) > 0.0f;
	}

	bool almost_same_as(const glm::dvec2& a, const glm::dvec2& b, const double epsilon)
	{
		return glm::abs(a.x - b.x) < epsilon && glm::abs(a.y - b.y) < epsilon;
	}

	bool almost_same_as(const glm::dvec3& a, const glm::dvec3& b, const double epsilon)
	{
		return glm::abs(a.x - b.x) < epsilon && glm::abs(a.y - b.y) < epsilon && glm::abs(a.z - b.z) < epsilon;
	}

	bool same_as(const glm::dvec2& a, const glm::dvec2& b)
	{
		return almost_same_as(a, b, glm::epsilon<double>());
	}


	bool same_as(const glm::dvec3& a, const glm::dvec3& b)
	{
		return almost_same_as(a, b, glm::epsilon<double>());
	}

	glm::mat4 rotate_vector_a_to_vector_b(glm::vec3 a, glm::vec3 b)
	{
		glm::vec3 v = glm::cross(a, b);
		float e = glm::dot(a, b);
		float h = 1.0f / (1.0f + e);
		glm::mat4 R(e + h * v.x * v.x, h * v.x * v.y - v.z, h * v.x * v.z + v.y, 0.0f,
					h * v.x * v.y + v.z, e + h * v.y * v.y, h * v.y * v.z - v.x, 0.0f,
					h * v.x * v.z - v.y, h * v.y * v.z + v.x, e + h * v.z * v.z, 0.0f,
					0.0f, 0.0f, 0.0f, 1.0f);
		return glm::transpose(R);
	}

	std::optional<float> solve_linear_equation(float constant_coeff, float linear_coeff)
	{
		if (linear_coeff == 0.0f)
			return std::nullopt;

		float solution = -constant_coeff / linear_coeff;
		return solution;
	}

	std::optional<std::tuple<float, float>> solve_quadratic_equation(float constant_coeff, float linear_coeff, float quadratic_coeff)
	{
		if (quadratic_coeff == 0.0f) {
			if (auto lin_result = solve_linear_equation(constant_coeff, linear_coeff))
				return std::make_tuple(*lin_result, *lin_result);
			else
				return std::nullopt;
		}

		float beta = linear_coeff / 2.0f;
		float discriminant = beta * beta - quadratic_coeff * constant_coeff;
		if (discriminant < 0.0f)
			return std::nullopt;

		float root = glm::sqrt(discriminant);
		float s = glm::sign(quadratic_coeff);
		return std::make_tuple(
			(-beta + root * s) / quadratic_coeff,
			(-beta - root * s) / quadratic_coeff
		);
	}
}

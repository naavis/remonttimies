#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

inline glm::vec2 toUnitDisk(const glm::vec2& squarePoint) {
	/* Shirley-Chiu mapping from square to unit circle.
	Input point assumed to be on a [0, 1]^2 square. */
	float phi, r;
	float a = 2.0f * squarePoint.x - 1.0f;
	float b = 2.0f * squarePoint.y - 1.0f;

	if (a > -b) {
		if (a > b) {
			r = a;
			phi = (glm::pi<float>() / 4.0f) * (b / a);
		}
		else {
			r = b;
			phi = (glm::pi<float>() / 4.0f) * (2.0f - a / b);
		}
	}
	else {
		if (a < b) {
			r = -a;
			phi = (glm::pi<float>() / 4.0f) * (4.0f + b / a);
		}
		else {
			r = -b;
			if (b != 0.0f)
				phi = (glm::pi<float>() / 4.0f) * (6.0f - a / b);
			else
				phi = 0.0f;
		}
	}
	return glm::vec2(r * glm::cos(phi), r * glm::sin(phi));
}

glm::mat3 formBasis(const glm::vec3& n) {
	glm::vec3 q = n;

	/* Replace smallest element (by absolute value) of q with 1.0f */
	int minIndex = -1;
	float minValue = std::numeric_limits<float>::max();
	for (int i = 0; i < 3; ++i) {
		if (glm::abs(q[i]) < minValue) {
			minValue = glm::abs(q[i]);
			minIndex = i;
		}
	}
	q[minIndex] = 1.0f;

	auto t = glm::normalize(glm::cross(q, n));
	auto b = glm::normalize(glm::cross(n, t));

	auto basis = glm::mat3(t, b, n);

	return basis;
}
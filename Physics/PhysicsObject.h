#pragma once
#include "glm/vec2.hpp"

enum ShapeType {PLANE = 0, SPHERE, BOX};
const int SHAPE_COUNT = 3;

class PhysicsObject {
public:
	PhysicsObject(ShapeType a_shapeID) : m_shapeID(a_shapeID) {};

	virtual void FixedUpdate(glm::vec2 gravity, float timeStep) = 0;
	virtual void MakeGizmo() = 0;
	virtual void ResetPosition() {};
	ShapeType GetSphapeID() { return m_shapeID; };
	virtual float GetKineticEnergy() { return 0; };

	glm::vec2 Cross(const float i, const glm::vec2 &v) {
		return { -i * v.y, i * v.x };
	}

	float Cross(const glm::vec2 &v1, const glm::vec2 &v2) {
		return  v1.x * v2.y - v1.y * v2.x;
	}

	glm::vec2 lastCollisionPoint = glm::vec2();
	glm::vec2 lastCollisionNormal = glm::vec2();
protected:

	ShapeType m_shapeID;
	
private:
};
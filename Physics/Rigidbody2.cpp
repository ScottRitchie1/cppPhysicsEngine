#include "Rigidbody.h"
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/matrix_transform_2d.hpp>
#include <glm/mat3x3.hpp>
#include <iostream>
#include <Gizmos.h>
#define MIN_LINEAR_THRESHOLD 0.1f
#define MIN_ROTATION_THRESHOLD 0.01f

Rigidbody::Rigidbody(ShapeType shapeID, glm::vec2 position, glm::vec2 velocity, float rotation, float mass, float elasticity, bool isKinematic) : PhysicsObject(shapeID){
	m_position = position;
	m_velocity = velocity;
	m_angularVelocity = 0;
	m_rotation = rotation;
	SetMass(mass);
	m_isKinematic = isKinematic;
	SetDrag(0.1f, 1.1f);
	SetElasticity(elasticity);
}

Rigidbody::Rigidbody(ShapeType shapeID, glm::vec2 position, float angle, float speed, float rotation, float mass, float elasticity, bool isKinematic) : PhysicsObject(shapeID) {
	m_position = position;
	m_velocity.x = speed * glm::cos(angle);
	m_velocity.y = speed * glm::sin(angle);
	m_angularVelocity = 0;

	m_rotation = rotation;
	SetMass(mass);
	m_isKinematic = isKinematic;
	SetDrag(0.1f, 1.1f);
	SetElasticity(elasticity);
}

Rigidbody::~Rigidbody() {

}

void Rigidbody::FixedUpdate(glm::vec2 gravity, float timeStep) {
	if (m_isKinematic) {
		m_angularVelocity = 0;
		m_velocity = glm::vec2(0,0);
		return;
	}

	
	m_velocity += gravity * timeStep;
	m_position += m_velocity * timeStep;


	m_velocity -= m_velocity * m_linearDrag * timeStep;
	m_rotation += m_angularVelocity * timeStep;
	m_angularVelocity -= m_angularVelocity * m_angularDrag * timeStep;


	if (glm::length(m_velocity) < MIN_LINEAR_THRESHOLD) {
		m_velocity = glm::vec2(0, 0);
	}

	if (abs(m_angularVelocity) < MIN_ROTATION_THRESHOLD) {
		m_angularVelocity = 0;
	}
}
void Rigidbody::Debug() {
	std::cout << "position: " << m_position.x << ", " << m_position.y << std::endl;
}

void Rigidbody::CalculateKinematic(glm::vec2 gravity, float timeStep, float time)
{
	float t = 0;
	float angle = atan2f(m_velocity.y, m_velocity.x);
	float speed = glm::length(m_velocity);
	while (t <= time) {


		aie::Gizmos::add2DCircle(
			glm::vec2(
				m_position.x + (speed * t * cosf(angle)) - (0.5f * gravity.x * pow(t, 2)),
				m_position.y + (speed * t * sinf(angle)) - (0.5f * -gravity.y * pow(t, 2))
			), 
			0.5f, 6, glm::vec4(1, 1, 0, 1));
		t += timeStep;
	}
}

void Rigidbody::ApplyForce(glm::vec2 force, glm::vec2 pos) {
	m_velocity += force / m_mass;
	//glm::vec2 momentArm = pos - m_position;
	//glm::vec2 parallelComponent = momentArm * (glm::dot(force, momentArm) / glm::dot(momentArm, momentArm));
	//glm::vec2 angularForce = force - parallelComponent;
	//glm::vec2 angularAcceleration = (angularForce * glm::length(momentArm)) / m_moment;
	//m_angularVelocity += atan2f(angularAcceleration.y, angularAcceleration.x);
	m_angularVelocity += ((force.y/10) * pos.x - (force.x/10) * pos.y) / (m_moment);
}



void Rigidbody::ResolvedCollision(Rigidbody * actor, glm::vec2 contact, glm::vec2* collisionNormal)
{
	glm::vec2 normal = collisionNormal ? *collisionNormal : glm::normalize(actor->GetPosition() - m_position);

	glm::vec2 vp = m_velocity + Cross(m_angularVelocity, contact - m_position)*(float)(1-m_isKinematic) - actor->m_velocity - Cross(actor->m_angularVelocity, contact - actor->m_position)*(float)(1 - m_isKinematic);
	
	float vp_p = glm::dot(vp, normal);
	if (vp_p < 0) {
	
		float elasicity = (m_elasticity + actor->GetElasticity()) / 2.0f;
	
		float j = -(1 + elasicity) * vp_p / (
			(1 / m_mass + powf(Cross(contact - m_position, normal), 2) / m_moment)*(float)(1-m_isKinematic) + 
			(1 / actor->m_mass + powf(Cross(contact - actor->m_position, normal), 2) / actor->m_moment)*(float)(1 - actor->m_isKinematic)
			);
		glm::vec2 force = j * normal;
		ApplyForce(force, contact - m_position);
		actor->ApplyForce(-force, contact - actor->m_position);
	}
}	

float Rigidbody::GetKineticEnergy()
{
	return 0.5f * (m_mass * glm::dot(m_velocity, m_velocity) + m_moment * m_angularVelocity * m_angularVelocity);
}

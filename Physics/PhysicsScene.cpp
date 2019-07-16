#include "PhysicsScene.h"
#include <iostream>
#include <list>
#include <glm/glm.hpp>
#include "Circle.h"
#include "Line.h"
#include "Rect.h"

typedef bool(*fn)(PhysicsObject*, PhysicsObject*);

static fn collisionsFunctionArray[] =
{
	PhysicsScene::Line2Line, PhysicsScene::Line2Circle, PhysicsScene::Line2Rect,
	PhysicsScene::Circle2Line, PhysicsScene::Circle2Circle, PhysicsScene::Circle2Rect,
	PhysicsScene::Rect2Line, PhysicsScene::Rect2Circle, PhysicsScene::Rect2Rect
};

PhysicsScene::PhysicsScene() : m_timeStep(0.01f), m_gravity(glm::vec2(0, 0)) {};

PhysicsScene::~PhysicsScene() {
	for (auto pActor : m_actors) {
		delete pActor;
	}
};

void PhysicsScene::AddActor(PhysicsObject* actor) {
	m_actors.push_back(actor);
}
void PhysicsScene::RemovedActor(PhysicsObject* actor) {
	auto item = std::find(m_actors.begin(), m_actors.end(), actor);
	if (item < m_actors.end()) {
		m_actors.erase(item);
	}
}
void PhysicsScene::Update(float deltatime) {

	static float timer = 0;
	timer += deltatime;
	if (timer < m_timeStep) {
		return;
	}
	timer -= m_timeStep;

	for (auto actor : m_actors) {
		actor->FixedUpdate(m_gravity, m_timeStep);
	}

	CheckForCollisions();
}
void PhysicsScene::UpdateGizmos() {
	for (auto pActor : m_actors)
	{
		pActor->MakeGizmo();
	}
}

void PhysicsScene::CheckForCollisions()
{
	int actorCount = m_actors.size();
	for (int outer = 0; outer < actorCount - 1; outer++)
	{
		for (int inner = outer + 1; inner < actorCount; inner++) {
			PhysicsObject* obj1 = m_actors[outer];
			PhysicsObject* obj2 = m_actors[inner];

			int shapeId1 = obj1->GetSphapeID();
			int shapeId2 = obj2->GetSphapeID();

			int functionIdx = (shapeId1 * SHAPE_COUNT) + shapeId2;
			fn collisionFunctionPtr = collisionsFunctionArray[functionIdx];
			if (collisionFunctionPtr != nullptr) {
				
				if (collisionFunctionPtr(obj1, obj2)) {
					//collision occured
					//printf("%i ", functionIdx);
				}
				//float kePre = obj1->GetKineticEnergy() + obj2->GetKineticEnergy();
				//float kePost = obj1->GetKineticEnergy() + obj2->GetKineticEnergy();
				//
				//float deltaKe = kePost - kePre;
				//if (deltaKe < 0.01f || deltaKe > 0.01f) {
				//	//std::cout << "Energy change detected!" << std::endl;
				//}
			}
		}
	}
}

bool PhysicsScene::Line2Line(PhysicsObject *, PhysicsObject *)
{
	return false;
}

bool PhysicsScene::Line2Circle(PhysicsObject * plane, PhysicsObject * circle)
{
	return Circle2Line(circle, plane);
}

bool PhysicsScene::Line2Rect(PhysicsObject * line, PhysicsObject * rect)
{
	return Rect2Line(rect, line);
}

bool PhysicsScene::Circle2Line(PhysicsObject * obj1, PhysicsObject * obj2)
{
	Circle* circle = dynamic_cast<Circle*>(obj1);
	Line* line = dynamic_cast<Line*>(obj2);

	if (circle != nullptr && line != nullptr) {
		glm::vec2 collisionNormal = line->GetNormal();

		float circleToLine = glm::dot(circle->GetPosition(), line->GetNormal()) - line->GetDistToOrigin();

		if (circleToLine < 0) {
			collisionNormal *= -1;
			circleToLine *= -1;
		}

		float intersection = circle->GetRadius() - circleToLine;
		if (intersection > 0) {
			glm::vec2 contact = circle->GetPosition() + (collisionNormal * -circle->GetRadius());

			circle->SetPosition(circle->GetPosition() + line->GetNormal() * (circle->GetRadius() - circleToLine));
			line->ResolvedCollision(circle, contact);
			if (circle->OnCollision != nullptr)
				circle->OnCollision(circle, nullptr, contact, collisionNormal);
			return true;
		}
	}

	return false;
}

bool PhysicsScene::Circle2Circle(PhysicsObject * obj1, PhysicsObject *obj2)
{
	Circle* circle1 = dynamic_cast<Circle*>(obj1);
	Circle* circle2 = dynamic_cast<Circle*>(obj2);
	glm::vec2 delta = circle2->GetPosition() - circle1->GetPosition();
	float dist = glm::length(delta);
	if (circle1->GetRadius() + circle2->GetRadius() > dist) {
		glm::vec2 constantForce1 = (circle2->isKinematic() ? 1 : 0.5f) * (dist - (circle1->GetRadius() + circle2->GetRadius()))*delta / dist;
		glm::vec2 constantForce2 = (circle1->isKinematic() ? 1 : 0.5f) * (dist - (circle1->GetRadius() + circle2->GetRadius()))*delta / dist;
		circle1->MovePosition(circle1->GetPosition() + constantForce1);
		circle2->MovePosition(circle2->GetPosition() - constantForce2);
		circle1->ResolvedCollision(circle2, 0.5f * (circle1->GetPosition() + circle2->GetPosition()));
		if (circle1->OnCollision != nullptr)
			circle1->OnCollision(circle1, circle2, (0.5f * (circle1->GetPosition() + circle2->GetPosition())), circle1->GetPosition() - circle2->GetPosition());
		if (circle2->OnCollision != nullptr)
			circle2->OnCollision(circle2, circle1, (0.5f * (circle1->GetPosition() + circle2->GetPosition())), circle2->GetPosition() - circle1->GetPosition());
		return true;
	}
	return false;
}

bool PhysicsScene::Circle2Rect(PhysicsObject * circle, PhysicsObject * rect)
{
	return Rect2Circle(rect, circle);
}

bool PhysicsScene::Rect2Line(PhysicsObject * obj1, PhysicsObject * obj2)
{
	
	Rect* rect = dynamic_cast<Rect*>(obj1);
	Line* line = dynamic_cast<Line*>(obj2);

	if (rect != nullptr && line != nullptr) {
		int numContacts = 0;
		glm::vec2 contact(0, 0);
		float contactV = 0;
		float penetration = 0;

		glm::vec2 lineOrigin = line->GetNormal() * line->GetDistToOrigin();
		float comFromPlane = glm::dot(rect->GetPosition() - lineOrigin, line->GetNormal());
		for (float x = -rect->GetExtents().x; x < rect->GetExtents().x * 2; x += rect->GetExtents().x * 2)
		{
			for (float y = -rect->GetExtents().y; y < rect->GetExtents().y * 2; y += rect->GetExtents().y * 2)
			{
				glm::vec2 p = rect->GetPosition() + x * rect->GetLocalX() + y * rect->GetLocalY();
				float distFromLine = glm::dot(p - lineOrigin, line->GetNormal());

				float velIntoLine = glm::dot(rect->GetVelocity() + 
					rect->GetVelocityAngular() * 
					(-y * rect->GetLocalX() + x * rect->GetLocalY()), line->GetNormal());

				if ((distFromLine > 0 && comFromPlane < 0 && velIntoLine > 0) || 
					(distFromLine < 0 && comFromPlane > 0 && velIntoLine < 0)) {
					numContacts ++;
					contact += p;
					contactV += velIntoLine;

					if (comFromPlane >= 0) {
						if (penetration > distFromLine)
							penetration = distFromLine;
					}
					else {
						if (penetration < distFromLine)
							penetration = distFromLine;
					}
				}
			}
		}

		if (numContacts > 0) {
			rect->SetPosition(rect->GetPosition() - line->GetNormal() * penetration);
			if (rect->OnCollision != nullptr)
				rect->OnCollision(rect, nullptr, contact, line->GetNormal());
			line->ResolvedCollision(rect, contact / (float)numContacts);
			return true;
		}
	}
	return false;
}

bool PhysicsScene::Rect2Circle(PhysicsObject *obj1, PhysicsObject *obj2)
{
	Rect* rect = dynamic_cast<Rect*>(obj1);
	Circle* circle = dynamic_cast<Circle*>(obj2);
	
	if (rect != nullptr && circle != nullptr) {
		glm::vec2 circleDirectionDelta = circle->GetPosition() - rect->GetPosition();
		float w2 = rect->GetExtents().x, h2 = rect->GetExtents().y;
	
		int numContacts = 0;
		float penetration = 0;
		glm::vec2 contact(0, 0);
		glm::vec2* edgeNormal = nullptr;

		for (float x = -w2; x <= w2; x += w2 * 2)
		{
			for (float y = -h2; y <= h2; y += h2 * 2)
			{
				glm::vec2 p = x * rect->GetLocalX() + y * rect->GetLocalY();//delta position to corner
				glm::vec2 dp = p - circleDirectionDelta;//delta position from corner to rect
				if (dp.x*dp.x + dp.y*dp.y <= circle->GetRadius()*circle->GetRadius()) {
					numContacts++;
					contact += glm::vec2(x, y);
					edgeNormal = new glm::vec2(-glm::normalize(-dp));
					penetration = circle->GetRadius() - glm::length(dp);
				}
			}
		}
	
		glm::vec2 p0(glm::dot(rect->GetLocalX(), circleDirectionDelta), glm::dot(rect->GetLocalY(), circleDirectionDelta));
	
		if (p0.y < h2 && p0.y > -h2) {
			if (p0.x > 0 && p0.x < w2 + circle->GetRadius()) {
				numContacts++;
				contact += glm::vec2(w2, p0.y);
				float pen = w2 + circle->GetRadius() - p0.x;
				if (pen <= penetration || penetration == 0){
					edgeNormal = new glm::vec2(-rect->GetLocalX());
					penetration = pen;
				}
			}
			if (p0.x < 0 && p0.x > -(w2 + circle->GetRadius())) {
				numContacts++;
				contact += glm::vec2(-w2, p0.y);
				float pen = w2 + circle->GetRadius() + p0.x;
				if (pen <= penetration || penetration == 0){
					edgeNormal = new glm::vec2(rect->GetLocalX());
					penetration = w2 + circle->GetRadius() + p0.x;
				}
			}
		}
		if (p0.x < w2 && p0.x > -w2) {
			if (p0.y > 0 && p0.y < h2 + circle->GetRadius()) {
				//top
				numContacts++;
				contact += glm::vec2(p0.x, h2);
 				float pen = h2 + circle->GetRadius() - p0.y;
				if (pen <= penetration || penetration == 0){
					edgeNormal = new glm::vec2(-rect->GetLocalY());
					penetration = pen;
				}
			}
			if (p0.y < 0 && p0.y > -(h2 + circle->GetRadius())) {
				numContacts++;
				contact += glm::vec2(p0.x, -h2);
				float pen = h2 + circle->GetRadius() + p0.y;
				if (pen <= penetration || penetration == 0) {
					edgeNormal = new glm::vec2(rect->GetLocalY());
					penetration = pen;
				}

			}
		}
		
	
		if (numContacts > 0) {
			contact = rect->GetPosition() + (1.0f / numContacts) * (rect->GetLocalX() * contact.x + rect->GetLocalY()*contact.y);
			glm::vec2 constantForce = (penetration) * -(*edgeNormal);
			circle->MovePosition(circle->GetPosition() + (rect->isKinematic() ? 1 : 0.5f) * constantForce);
			rect->MovePosition(rect->GetPosition() - (circle->isKinematic() ? 1 : 0.5f) * constantForce);
			circle->ResolvedCollision(rect, contact, edgeNormal);
			if (circle->OnCollision != nullptr)
				circle->OnCollision(circle, rect, contact, (*edgeNormal));
			if (rect->OnCollision != nullptr)
				rect->OnCollision(rect, circle, contact, -(*edgeNormal));
			delete edgeNormal;
			return true;
		}
		delete edgeNormal;
	}
	
	return false;
}

bool PhysicsScene::Rect2Rect(PhysicsObject * obj1, PhysicsObject *obj2)
{
	Rect* rect1 = dynamic_cast<Rect*>(obj1);
	Rect* rect2 = dynamic_cast<Rect*>(obj2);
	if (rect1 != nullptr && rect2 != nullptr) {
		glm::vec2 rectPos = rect2->GetPosition() - rect1->GetPosition();

		glm::vec2 normal(0,0);
		glm::vec2 contactForce1(0,0), contactForce2(0,0);
		glm::vec2 contact(0, 0);
		glm::vec2 contactOther(0, 0);
		int numContacts = 0;

		rect1->CheckRectCorners(*rect2, contact, numContacts, normal, contactForce1);
		
		if (rect2->CheckRectCorners(*rect1, contactOther, numContacts, normal, contactForce2)) {
			normal = -normal;
			contact = contactOther;
		}
		
		if (numContacts > 0) {

			rect1->MovePosition(rect1->GetPosition() - ((rect2->isKinematic() ? 1 : 0.5f) * (contactForce1 - contactForce2)));
			rect2->MovePosition(rect2->GetPosition() + ((rect1->isKinematic() ? 1 : 0.5f) * (contactForce1 - contactForce2)));
			rect1->ResolvedCollision(rect2, contact, &-normal);
			if (rect1->OnCollision != nullptr)
				rect1->OnCollision(rect1, rect2, contact, normal);
			if (rect2->OnCollision != nullptr)
				rect2->OnCollision(rect2, rect1, contact, -normal);
			return true;
		}
		
	}
	return false;
}

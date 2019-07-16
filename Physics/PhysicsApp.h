#pragma once

#include "Application.h"
#include "Renderer2D.h"

class PhysicsScene;
class Circle;
class Line;
class Rect;

class PhysicsApp : public aie::Application {
public:

	PhysicsApp();
	virtual ~PhysicsApp();

	virtual bool startup();
	virtual void shutdown();

	virtual void update(float deltaTime);
	virtual void draw();

protected:

	aie::Renderer2D*	m_2dRenderer;
	aie::Font*			m_font;

	PhysicsScene*		m_physicsScene;
	Circle*				m_ball;
	Rect*				m_rotator;

	Rect*				m_paddleRight;
	Rect*				m_paddleLeft;
};
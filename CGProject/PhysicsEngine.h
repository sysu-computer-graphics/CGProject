#pragma once

#include "Bullet.h"
#include "Model.h"
#include "Target.h"
#include <list>
#include <math.h>
#include "Controler.h"
#include "SceneController.h"

class PhysicsEngine
{
public:
	// Singleton
	static PhysicsEngine* getInstance();

	static double getDistance(const glm::vec3 &p0, const glm::vec3 &p1);
	static void collisionDetect(std::list<Target> &targetsList);
	static bool hasCollision(SceneController * sceneController);
	static bool hasCollisionBarrier(SceneController * sceneController);

private:
	static const double thresholdDistance;
	static const double thresholdBarrierDistance;
	static PhysicsEngine* instance;

	PhysicsEngine();
	~PhysicsEngine();
	// Singleton: disallow the copy constructor and assign operator
	PhysicsEngine(const PhysicsEngine &other) = delete;
	PhysicsEngine & operator=(const PhysicsEngine &other) = delete;
};


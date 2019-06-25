#include "PhysicsEngine.h"

// æ≤Ã¨±‰¡ø
PhysicsEngine* PhysicsEngine::instance = nullptr;
const double PhysicsEngine::thresholdDistance = 1.7;
const double PhysicsEngine::thresholdBarrierDistance = 1.5;


PhysicsEngine::PhysicsEngine()
{
}

PhysicsEngine::~PhysicsEngine()
{
}

PhysicsEngine * PhysicsEngine::getInstance()
{
	if (instance == nullptr) {
		instance = new PhysicsEngine();
	}
	return instance;
}

double PhysicsEngine::getDistance(const glm::vec3 & p0, const glm::vec3 & p1)
{
	return sqrt(pow(p0.x - p1.x, 2) + pow(p0.y - p1.y, 2) + pow(p0.z - p1.z, 2));
}

void PhysicsEngine::collisionDetect(std::list<Target>& targetsList)
{
	for (auto it = targetsList.begin(); it != targetsList.end();) {
		bool isErase = false;
		for (auto it_bullet : Controler::getInstance()->bulletManager->getBulletLists()) {
			if (getDistance((*it).getPosition(), it_bullet->getPosition()) < PhysicsEngine::thresholdDistance) {
				targetsList.erase(it);
				Controler::getInstance()->bulletManager->deleteBullet(it_bullet->id);
				isErase = true;
				break;
			}
		}
		if (!isErase) {
			it++;
		}
	}

}

bool PhysicsEngine::hasCollision(SceneController * sceneController)
{
	for (auto it_bullet : Controler::getInstance()->bulletManager->getBulletLists()) {
		if (getDistance(sceneController->getTargetPosition(), it_bullet->getPosition()) < PhysicsEngine::thresholdDistance) {
			Controler::getInstance()->bulletManager->deleteBullet(it_bullet->id);
			return true;
		}
	}
	return false;
}

bool PhysicsEngine::hasCollisionBarrier(SceneController * sceneController)
{
	for (auto it_bullet : Controler::getInstance()->bulletManager->getBulletLists()) {
		//std::vector<glm::vec3> barrierPostions = sceneController->getBarrierPositions();
		std::vector<glm::vec3> newBarrierPositions = sceneController->getBarrierNewPositions();
		std::vector<float> barrierSizes = sceneController->getBarrierSizeSet();
		for (int i = 0; i < newBarrierPositions.size(); i++) {
			glm::vec3 barrierPosition = newBarrierPositions[i];
			if (getDistance(barrierPosition, it_bullet->getPosition()) < barrierSizes[i] * PhysicsEngine::thresholdBarrierDistance) {
				Controler::getInstance()->bulletManager->deleteBullet(it_bullet->id);
				return true;
			}
		}
		
	}
	return false;
}

#include "BulletManager.h"

BulletManager::BulletManager()
{
}

BulletManager::~BulletManager()
{
	this->bulletLists.clear();
}


void BulletManager::newBullet(glm::vec3 front, glm::vec3 position) {
	this->bulletLists.push_back(new Bullet(front, position));
}

void BulletManager::render(Model &bulletModel, const glm::vec3 &_lightPos) {
	for (int i = 0; i < (int)this->bulletLists.size(); i++) {
		bulletLists[i]->render(bulletModel, _lightPos);
	}
}

void BulletManager::deleteBullet(std::string bulletId) {
	for (auto it = bulletLists.begin(); it != bulletLists.end();) {
		if ((*it)->id == bulletId) {
			it = bulletLists.erase(it);
		}
		else {
			it++;
		}
	}
}
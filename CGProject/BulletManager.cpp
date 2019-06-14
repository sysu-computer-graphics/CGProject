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

void BulletManager::render(Model &bulletModel) {
	for (int i = 0; i < this->bulletLists.size(); i++) {
		bulletLists[i]->render(bulletModel);
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
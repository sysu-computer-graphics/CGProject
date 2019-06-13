#pragma once
#include <vector>

#include "Bullet.h"

class BulletManager
{
public:
	BulletManager();
	~BulletManager();
	void newBullet(glm::vec3 front, glm::vec3 position);
	void render();
	void deleteBullet(std::string bulletId);
private:
	std::vector<Bullet*> bulletLists;
};


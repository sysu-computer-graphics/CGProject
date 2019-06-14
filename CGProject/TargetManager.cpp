#include "TargetManager.h"



TargetManager::TargetManager(int _targetNum)
{
	this->targetNum = _targetNum;
	this->positions.push_back(glm::vec3(0.0f));
	for (int i = 1; i < this->targetNum; i++) {
		this->positions.push_back(this->positions.back() + glm::vec3(1.0f, 0.0f, 0.0f));
	}
	for (int i = 0; i < this->targetNum; i++) {
		this->targets.push_back(new Target(this->positions[i]));
	}
}


TargetManager::~TargetManager()
{
}

void TargetManager::render() {
	for (int i = 0; i < targetNum; i++) {
		this->targets[i]->render();
	}
}

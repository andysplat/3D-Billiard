#pragma once
#include <Component/SimpleScene.h>
#include <Component/Transform/Transform.h>
#include <Core/GPU/Mesh.h>
#include "C:\Users\andrei.mihu2809\Desktop\Framework-EGC-master\Source\Laboratoare\Laborator5\LabCamera.h"

using namespace std;

class Tema : public SimpleScene
{
public:
	Tema();
	~Tema();

	void Init() override;

private:
	void FrameStart() override;
	void Update(float deltaTimeSeconds) override;
	void FrameEnd() override;

	void RenderSimpleMesh(Mesh *mesh, Shader *shader, const glm::mat4 &modelMatrix, const glm::vec3 &color = glm::vec3(1));

	void OnInputUpdate(float deltaTime, int mods) override;
	void OnKeyPress(int key, int mods) override;
	void OnKeyRelease(int key, int mods) override;
	void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
	void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
	void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
	void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
	void OnWindowResize(int width, int height) override;
protected:
	glm::vec3 lightPosition; 
	Laborator::Camera *camera;
	glm::mat4 projectionMatrix;
	bool renderCameraTarget;
	bool balls[15];
	pair<float, float> pos[15];
	pair<float, float> vel[15];
	bool start;
	bool choose;
	pair<bool, int> player1;
	pair<bool, int> player2;
	float rotate;
	float ballX;
	float ballZ;
	float cueX;
	float cueZ;
	float power;
	float friction;
	bool shoot;
	bool foult;
	pair<int, int> foults;
	float X;
	float Z;
	float sgnX = 1;
	float sgnZ = 1;
	float pwr[15];
};

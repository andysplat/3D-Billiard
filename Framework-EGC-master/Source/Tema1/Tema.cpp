#include "Tema.h"
#include "Laboratoare/Laborator3/Object2D.h"
#include "Laboratoare/Laborator3/Transform2D.h"

#include <vector>
#include <iostream>

#include <Core/Engine.h>

using namespace std;

Tema1::Tema1()
{
}

Tema1::~Tema1()
{
}

void Tema1::Init()
{
	glm::ivec2 resolution = window->GetResolution();
	auto camera = GetSceneCamera();
	camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0.01f, 400);
	camera->SetPosition(glm::vec3(0, 0, 50));
	camera->SetRotation(glm::vec3(0, 0, 0));
	camera->Update();
	GetCameraInput()->SetActive(false);

	glm::vec3 corner = glm::vec3(0, 0, 0);
	float length = 20;
	float width = 660;
	float length1 = 1240;
	float width1 = 20;
	float length2 = 200;
	float width2 = 10;
	float length3 = 60;
	float width3 = 30;
	int n, m;

	translateX = 0;
	translateY = 0;
	
	scaleX = 1;
	scaleY = 1;

	angularStep = 0;
	
	for (n = 1; n <= 10; n++)
		for (m = 1; m <= 12; m++) {
			caramizi[n][m] = true;
			scalare[n][m] = 1.0f;
			nivel[n][m] = nivele;
		}

	Mesh* wall1 = Object2D::CreateRectangle("perete stanga-dreapta", corner, length, width, glm::vec3(1, 0, 0), true);
	AddMeshToList(wall1);

	Mesh* wall2 = Object2D::CreateRectangle("perete sus", corner, length1, width1, glm::vec3(1, 0, 0), true);
	AddMeshToList(wall2);

	Mesh* wall3 = Object2D::CreateRectangle("perete jos", corner, length1 + 40, width1, glm::vec3(1, 0, 0), true);
	AddMeshToList(wall3);

	Mesh* platform = Object2D::CreateRectangle("platforma", corner, length2, width2, glm::vec3(0, 1, 0), true);
	AddMeshToList(platform);

	Mesh* brick = Object2D::CreateRectangle("caramida", corner, length3, width3, glm::vec3(1, 0, 0), true);
	AddMeshToList(brick);

	Mesh* circle = Object2D::CreateCircle("bila", corner, 360, glm::vec3(1, 1, 1));
	AddMeshToList(circle);

	Mesh* life = Object2D::CreateCircle("viata", corner, 360, glm::vec3(1, 1, 1));
	AddMeshToList(life);

	Mesh* powerup1 = Object2D::CreateSquare("powerup1", corner, length, glm::vec3(0, 0, 1), true);
	AddMeshToList(powerup1);

	Mesh* powerup2 = Object2D::CreateSquare("powerup2", corner, length, glm::vec3(0, 1, 1), true);
	AddMeshToList(powerup2);

}

void Tema1::FrameStart()
{
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::ivec2 resolution = window->GetResolution();
	glViewport(0, 0, resolution.x, resolution.y);
}

void Tema1::Update(float deltaTimeSeconds)
{
	static float sgn = 1;
	static float sgnx = 1;
	static float sgny = -1;
	static int tx1 = 0;
	static int ty1 = 60;
	static int tx2 = 1260;
	static int ty2 = 60;
	static int tx3 = 20;
	static int ty3 = 700;
	static float platformax;
	static float platformay = 20;
	static float bilax;
	static float bilay = 40;
	static int viata1x = 80;
	static int viata1y = 30;
	static int viata2x = 50;
	static int viata2y = 30;
	static int viata3x = 20;
	static int viata3y = 30;
	static float unghi = 0;
	static int n, m;
	static int x, y;
	static float min;
	static int powerup = 0;
	static bool powerup_on = false;
	static float powerupx = 0;
	static float powerupy = 0;
	static float rotation = 0;
	static float powerup_time = 0;
	static int powerups;
	static float stronger = false;
	static int destroyed = 0;
	static bool hit = false;
	
	glm::ivec2 resolution = window->GetResolution();
	
	modelMatrix = glm::mat3(1);
	modelMatrix *= Transform2D::Translate(tx1, ty1);
	RenderMesh2D(meshes["perete stanga-dreapta"], shaders["VertexColor"], modelMatrix);
	
	modelMatrix = glm::mat3(1);
	modelMatrix *= Transform2D::Translate(tx2, ty2); 
	RenderMesh2D(meshes["perete stanga-dreapta"], shaders["VertexColor"], modelMatrix);

	modelMatrix = glm::mat3(1);
	modelMatrix *= Transform2D::Translate(tx3, ty3); 
	RenderMesh2D(meshes["perete sus"], shaders["VertexColor"], modelMatrix);

	if (powerup_on) {
		if ((powerup_time > 0) && (powerup_time <= 30)) {
			powerup_time += deltaTimeSeconds;
			if (powerups == 1)
				stronger = true;
			else 
				RenderMesh2D(meshes["perete jos"], shaders["VertexColor"], glm::mat3(1));
		}
		else {
			if (powerupy > 0 && (powerup_time == 0)) {
				if ((powerupy <= 40) && (powerupy >= 20) && (powerupx > platformax) && (powerupx < platformax + 200) && (powerup_time == 0))
					powerup_time += deltaTimeSeconds;
				else {
					rotation += 0.05f;
					powerupy -= 100 * deltaTimeSeconds;
					modelMatrix = glm::mat3(1);
					modelMatrix *= Transform2D::Translate(powerupx, powerupy) * Transform2D::Rotate(rotation) * Transform2D::Translate(-10, -10);
					if (powerups == 1)
						RenderMesh2D(meshes["powerup2"], shaders["VertexColor"], modelMatrix);
					else 
						RenderMesh2D(meshes["powerup1"], shaders["VertexColor"], modelMatrix);
				}
			}
			else {
				powerup_on = false;
				powerup_time = 0;
				if (powerup >= 5)
					powerup = 4;
				if (powerups == 1)
					stronger = false;
			}
		}
	}

	for (n = 1; n <= 10; n++)
		for (m = 1; m <= 12; m++)
			if (caramizi[n][m]) {
				x = 0;
				y = 0;
				min = 5;
				if ((170 + (m - 1) * 60 + (m - 1) * 20 - 5 <= bilax) && (170 + m * 60 + (m - 1) * 20 + 5 >= bilax) && (fabs(bilay - (220 + (n - 1) * 30 + (n - 1) * 10) + 5) < 5) && (sgny > 0) && !hit) {
					if (fabs(bilay - (220 + (n - 1) * 30 + (n - 1) * 10) + 5) < min) {
						min = fabs(bilay - (220 + (n - 1) * 30 + (n - 1) * 10) + 5);
						y = 1;
					}
					
				}
				if ((170 + (m - 1) * 60 + (m - 1) * 20 - 5 <= bilax) && (170 + m * 60 + (m - 1) * 20 + 5 >= bilax) && (fabs(bilay - (220 + n * 30 + (n - 1) * 10) - 5) < 5) && (sgny < 0) && !hit) {
					if (fabs(bilay - (220 + n * 30 + (n - 1) * 10) - 5) < min) {
						min = fabs(bilay - (220 + n * 30 + (n - 1) * 10) - 5);
						y = 1;
					}
				}
				if ((220 + (n - 1) * 30 + (n - 1) * 10 - 5 <= bilay) && (220 + n * 30 + (n - 1) * 10 + 5 >= bilay) && (fabs(bilax - (170 + (m - 1) * 60 + (m - 1) * 20) + 5) < 5) && (sgnx * cos(unghi) > 0) && !hit) {
					if (fabs(bilax - (170 + (m - 1) * 60 + (m - 1) * 20) + 5) < min) {
						min = fabs(bilax - (170 + (m - 1) * 60 + (m - 1) * 20) + 5);
						y = 0;
						x = 1;
					}
				}
				if ((220 + (n - 1) * 30 + (n - 1) * 10 - 5 <= bilay) && (220 + n * 30 + (n - 1) * 10 + 5 >= bilay) && (fabs(bilax - (170 + m * 60 + (m - 1) * 20) - 5) < 5) && (sgnx * cos(unghi) < 0) && !hit) {
					if (fabs(bilax - (170 + m * 60 + (m - 1) * 20) - 5) < min) {
						min = fabs(bilax - (170 + m * 60 + (m - 1) * 20) - 5);
						y = 0;
						x = 1;
					}
				}
				
				if (x == 1) {
					if(!stronger)
						sgnx *= -1;
					nivel[n][m]--;
					if (nivel[n][m] == 0 || stronger) {
						caramizi[n][m] = false;
						destroyed++;
						powerup++;
						hit = true;
					}
				}
				if (y == 1) {
					if (!stronger) 
						sgny *= -1;
					nivel[n][m]--;
					if (nivel[n][m] == 0 || stronger) {
						caramizi[n][m] = false;
						destroyed++;
						powerup++;
						hit = true;
					}
				}

				if ((powerup == 20) && (!powerup_on)) {
					rotation = 0;
					powerup = 0;
					powerup_on = true;
					powerups = rand() % 2;
					powerupx = 170 + (m - 1) * 60 + (m - 1) * 20 + 30;
					powerupy = 220 + (n - 1) * 30 + (n - 1) * 10 + 15;
				}
				
				modelMatrix = glm::mat3(1);
				modelMatrix *= Transform2D::Translate(170 + (m - 1) * 60 + (m - 1) * 20, 220 + (n - 1) * 30 + (n - 1) * 10);
				RenderMesh2D(meshes["caramida"], shaders["VertexColor"], modelMatrix);
			}
			else {
				if (scalare[n][m] > 0)
					scalare[n][m] -= 0.05f;
				modelMatrix = glm::mat3(1);
				modelMatrix *= Transform2D::Translate(170 + (m - 1) * 60 + (m - 1) * 20 + 30, 220 + (n - 1) * 30 + (n - 1) * 10 + 15) * Transform2D::Scale(scalare[n][m], scalare[n][m]) * Transform2D::Translate(-30, -15);
				RenderMesh2D(meshes["caramida"], shaders["VertexColor"], modelMatrix);
			}
	hit = false;
	if (start) {
		if ((bilay <= 40) && (bilax > platformax) && (bilax < platformax + 200) && (sgny < 0)) {
			//if (platformax + 100 == bilax)
				//sgn = 0;
			//cout << "sgn " << sgn << endl;
			unghi = M_PI * (platformax + 200 - bilax) / 200;
			sgny *= -1;
			if (cos(unghi) < 0)
				if (sgnx < 0) {
					sgnx *= -1;
			}
			if (cos(unghi) > 0)
				if (sgnx < 0) {
					sgnx *= -1;
			}
			//if (cos(unghi) < 0)
			//	sgn = -1;
			//if (cos(unghi) > 0)
			//	sgn = 1;
			//if (platformax + 100 == bilax)
				//sgn = 0;
		}

		if ((fabs(bilay - 25) < 5) && (sgny < 0) && (powerup_time > 0) && (powerups == 0)) {
			sgny *= -1;
		}

		if ((fabs(bilay - 695) < 5) && (sgny > 0)) {
			sgny *= -1;
		}
		if ((bilay >= 60) && (fabs(bilax - 25) < 5) && (sgnx * cos(unghi) < 0)) {
			sgnx *= -1;
		}
		if ((bilay >= 60) && (fabs(bilax - 1255) < 5) && (sgnx * cos(unghi) > 0)) {
			sgnx *= -1;
		}
		if ((fabs(bilay - 55) < 5) && (bilax >= 1260) && (sgny > 0)) {
			sgny *= -1;
		}
		if ((fabs(bilay - 55) < 5) && (bilax <= 20) && (sgny > 0)) {
			sgny *= -1;
		}
		
		bilax += sgnx * 300 * deltaTimeSeconds * cos(unghi);
		bilay += sgny * 300 * deltaTimeSeconds;
		if (bilay <= 0) {
			vieti -= 1;
			start = false;
			sgny = -1;
			sgnx = 1;
			bilay = 40;
			powerup_on = false;
			powerup_time = 0;
			powerup = 0;
			stronger = false;
		}

	}
	else {
		bilax = mouse;
	}

	modelMatrix = glm::mat3(1);
	modelMatrix *= Transform2D::Translate(bilax, bilay) * Transform2D::Scale(10, 10);
	RenderMesh2D(meshes["bila"], shaders["VertexColor"], modelMatrix);

	platformax = mouse - 100;
	modelMatrix = glm::mat3(1);
	modelMatrix *= Transform2D::Translate(platformax, platformay);
	RenderMesh2D(meshes["platforma"], shaders["VertexColor"], modelMatrix);

	if (vieti > 2) {
		modelMatrix = glm::mat3(1);
		modelMatrix *= Transform2D::Translate(viata1x, viata1y) * Transform2D::Scale(10, 10);
		RenderMesh2D(meshes["viata"], shaders["VertexColor"], modelMatrix);
	}

	if (vieti > 1) {
		modelMatrix = glm::mat3(1);
		modelMatrix *= Transform2D::Translate(viata2x, viata2y) * Transform2D::Scale(10, 10);
		RenderMesh2D(meshes["viata"], shaders["VertexColor"], modelMatrix);
	}

	if (vieti > 0) {
		modelMatrix = glm::mat3(1);
		modelMatrix *= Transform2D::Translate(viata3x, viata3y) * Transform2D::Scale(10, 10);
		RenderMesh2D(meshes["viata"], shaders["VertexColor"], modelMatrix);
	}

	if (vieti < 0 || destroyed == 120) {
		if (vieti >= 0)
			nivele++;
		vieti = 3;
		start = false;
		sgny = -1;
		sgnx = 1;
		bilay = 40;
		powerup_on = false;
		powerup_time = 0;
		powerup = 0;
		stronger = false;
		destroyed = 0;
		for (n = 1; n <= 10; n++)
			for (m = 1; m <= 12; m++) {
				caramizi[n][m] = true;
				scalare[n][m] = 1.0f;
				nivel[n][m] = nivele;
			}
	}
	
}

void Tema1::FrameEnd()
{

}

void Tema1::OnInputUpdate(float deltaTime, int mods)
{

}

void Tema1::OnKeyPress(int key, int mods)
{
	// add key press event
}

void Tema1::OnKeyRelease(int key, int mods)
{
	// add key release event
}

void Tema1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
	// add mouse move event
	mouse = mouseX;
}

void Tema1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button press event
	start = true;
}

void Tema1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button release event
}

void Tema1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}

void Tema1::OnWindowResize(int width, int height)
{
}
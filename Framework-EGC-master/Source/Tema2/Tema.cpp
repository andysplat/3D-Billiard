#include "Tema.h"

#include <vector>
#include <string>
#include <iostream>

#include <Core/Engine.h>

using namespace std;

Tema::Tema()
{
}

Tema::~Tema()
{
}

void Tema::Init()
{
	camera = new Laborator::Camera();
	camera->Set(glm::vec3(0, 4, 0), glm::vec3(0, -1, 0), glm::vec3(0, 0, -1));
	renderCameraTarget = false;
	player1.first = false;
	player2.first = false;
	start = false;
	choose = false;
	foult = false;
	shoot = false;
	power = 0;
	friction = 0.1f;
	pos[0] = { 0, 0 };
	pos[1] = { -1, 0 };
	pos[2] = { 0.5f, 0.9f };
	pos[3] = { 0.5f, -0.9f };
	pos[4] = { -0.5f, -0.9f };
	pos[5] = { 1, -1.8f };
	pos[6] = { 2, -1.8f };
	pos[7] = { -2, -1.8f };
	pos[8] = { -0.5f, 0.9f };
	pos[9] = { 1, 0 };
	pos[10] = { 1.5f, -0.9f };
	pos[11] = { -1.5f, -0.9f };
	pos[12] = { 0, -1.8f };
	pos[13] = { -1, -1.8f };
	pos[14] = { 0, 1.8f };
	for (int o = 0; o < 15; o++) {
		balls[o] = true;
		vel[o] = { 0, 0 };
		pwr[o] = 0;
		pos[o].first = pos[o].first;
		pos[o].second = pos[o].second - 10;
	}
	
	{
		Mesh* mesh = new Mesh("box");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "box.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}

	{
		Mesh* mesh = new Mesh("sphere");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "sphere.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}

	{
		Mesh* mesh = new Mesh("Pool_Cue");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "Pool_Cue.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}

	// Create a shader program for drawing face polygon with the color of the normal
	{
		Shader *shader = new Shader("ShaderLab7");
		shader->AddShader("Source/Laboratoare/Laborator7/Shaders/VertexShader.glsl", GL_VERTEX_SHADER);
		shader->AddShader("Source/Laboratoare/Laborator7/Shaders/FragmentShader.glsl", GL_FRAGMENT_SHADER);
		shader->CreateAndLink();
		shaders[shader->GetName()] = shader;
	}

	//Light & material properties
	{
		lightPosition = glm::vec3(0, 1, 1);
	}
	projectionMatrix = glm::perspective(RADIANS(60), window->props.aspectRatio, 0.01f, 200.0f);
	rotate = RADIANS(180.0f);
	ballX = 0;
	ballZ = 10;
}

void Tema::FrameStart()
{
	// clears the color buffer (using the previously set color) and depth buffer
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::ivec2 resolution = window->GetResolution();
	// sets the screen area where to draw
	glViewport(0, 0, resolution.x, resolution.y);
}

void Tema::Update(float deltaTimeSeconds)
{
	static float distX;
	static float distZ;
	static float velX;
	static float velZ;
	static float dist;
	static float pow;
	static float overlap;
	if (fabs(ballZ) <= 0.05f * 10 && ballX > 0.9f * 10 && X > 0) {
		foult = true;
		ballX = 0;
		ballZ = 10;
		power = 0;
	}
	if (fabs(ballZ) <= 0.05f * 10 && ballX < -0.9f * 10 && X < 0) {
		foult = true;
		ballX = 0;
		ballZ = 10;
		power = 0;
	}
	if (ballZ >= 1.4f * 10 && ballX > 0.9f * 10 && (X >= 0) && (Z >= 0)) {
		foult = true;
		ballX = 0;
		ballZ = 10;
		power = 0;
	}
	if (ballZ >= 1.4f * 10 && ballX < -0.9f * 10 && (X <= 0) && (Z >= 0)) {
		foult = true;
		ballX = 0;
		ballZ = 10;
		power = 0;
	}
	if (ballZ <= -1.4f * 10 && ballX > 0.9f * 10 && (X >= 0) && (Z <= 0)) {
		foult = true;
		ballX = 0;
		ballZ = 10;
		power = 0;
	}
	if (ballZ <= -1.4f * 10 && ballX < -0.9f * 10 && (X <= 0) && (Z <= 0)) {
		foult = true;
		ballX = 0;
		ballZ = 10;
		power = 0;
	}
	if (ballX > 0.94f * 10 && X > 0) 
		X *= -1;
	if (ballX < -0.94f * 10  && X < 0)
		X *= -1;
	if (ballZ < -1.44f * 10 && Z < 0)
		Z *= -1;
	if (ballZ > 1.44f * 10 && Z > 0)
		Z *= -1;

	if(!shoot) {
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.01f));
		modelMatrix = glm::translate(modelMatrix, glm::vec3(ballX * 10, 100, ballZ * 10));
		modelMatrix = glm::rotate(modelMatrix, rotate, glm::vec3(0, 1, 0));
		modelMatrix = glm::rotate(modelMatrix, RADIANS(28.0f), glm::vec3(1, 0, 0));
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 5, -155));
		RenderSimpleMesh(meshes["Pool_Cue"], shaders["ShaderLab7"], modelMatrix, glm::vec3(0.5f, 0.3f, 0));
	}

	{//white ball
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.1f));
		modelMatrix = glm::translate(modelMatrix, glm::vec3(ballX, 10, ballZ));
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0.5f, 0));
		RenderSimpleMesh(meshes["sphere"], shaders["ShaderLab7"], modelMatrix, glm::vec3(1, 1, 1));
	}

	for (int o = 0; o < 15; o++) {
		for (int m = o; m < 15; m++) {

			if (m == o) {
				distX = ballX - pos[o].first;
				distZ = ballZ - pos[o].second;
				velX = vel[o].first * pwr[o] - X * power;
				velZ = vel[o].second * pwr[o] - Z * power;
			}
			else {
				distX = pos[m].first - pos[o].first;
				distZ = pos[m].second - pos[o].second;
				velX = vel[o].first * pwr[o] - vel[m].first * pwr[o];
				velZ = vel[o].second * pwr[o] - vel[m].second * pwr[o];
			}
			
			dist = fabs(distX * distX + distZ * distZ);
			if (dist <= 1) {				
				
				dist = sqrtf(dist);
				distX = distX / dist;
				distZ = distZ / dist;
				overlap = 0.5f * (dist - 1);
				pos[o].first += overlap * distX;
				pos[o].second += overlap * distZ;
				pow = distX * velX + distZ * velZ;
				vel[o].first =  (-1) * distX;
				vel[o].second =  (-1) * distZ;
				pwr[o] = pow;
				if (m == o) {
					ballX -= overlap * distX;
					ballZ -= overlap * distZ;
					power = pow;
				}
				else {
					pos[m].first -= overlap * distX;
					pos[m].second -= overlap * distZ;
					vel[m].first = distX;
					vel[m].second = distZ;
					pwr[m] = pow;
				}
			
			}
		}				
		
		if (balls[o]) {
			glm::mat4 modelMatrix = glm::mat4(1);
			modelMatrix = glm::scale(modelMatrix, glm::vec3(0.1f));
			modelMatrix = glm::translate(modelMatrix, glm::vec3(pos[o].first, 10.5f, pos[o].second));

			if(o == 0)
				RenderSimpleMesh(meshes["sphere"], shaders["ShaderLab7"], modelMatrix, glm::vec3(0, 0, 0));
			else if(o < 8)
				RenderSimpleMesh(meshes["sphere"], shaders["ShaderLab7"], modelMatrix, glm::vec3(1, 0, 0));
			else
				RenderSimpleMesh(meshes["sphere"], shaders["ShaderLab7"], modelMatrix, glm::vec3(1, 1, 0));
		}
	}
	shoot = false;
	if (power > 0) {
		ballX += X * 10 * deltaTimeSeconds;
		ballZ += Z * 10 * deltaTimeSeconds;
		power -= friction;
		shoot = true;
	}
	else 
		power = 0;

	
	for (int o = 0; o < 15; o++) {
		if (balls[o]) {
			if (fabs(pos[o].second) <= 0.05f * 10 && pos[o].first > 0.9f * 10 && vel[o].first > 0) {
				if (o == 0) {
					if (player1.first == true) {
						if (player1.second == 7 || player1.second == 14)
							cout << "Player 1 pot the black ball first. Player 1 won!" << endl;
						else
							cout << "Player 1 pot the black ball. Player 2 won!" << endl;
					}
					else {
						if (player2.second == 7 || player1.second == 14)
							cout << "Player 2 pot the black ball first. Player 2 won!" << endl;
						else
							cout << "Player 2 pot the black ball. Player 1 won!" << endl;
					}
				}
				else if (!choose) {
					if (player1.first == true) {
						if (o >= 8) {
							player1.second = 8;
							player2.second = 1;
						}
						else {
							player1.second = 1;
							player2.second = 8;
						}
						choose = true;
					}
					else {
						if (o >= 8) {
							player2.second = 8;
							player1.second = 1;
						}
						else {
							player2.second = 1;
							player1.second = 8;
						}
						choose = true;
					}
				}
				else {
					if (o >= 8) {
						if (player1.first == true) {
							if (player1.second >= 8) {
								player1.second++;
								player1.first = false;
								player2.first = true;
							}
							else {
								foult = true;
								foults.first++;
							}
						}
						else {
							if (player2.second >= 8) {
								player2.second++;
								player2.first = false;
								player1.first = true;
							}
							else {
								foult = true;
								foults.second++;
							}
						}
					}
					else {
						if (player1.first == true) {
							if (player1.second >= 8) {
								foult = true;
								foults.first++;
							}
							else {
								player1.second++;
								player1.first = false;
								player2.first = true;
							}
						}
						else {
							if (player2.second >= 8) {
								foult = true;
								foults.second++;
							}
							else {
								player2.second++;
								player2.first = false;
								player1.first = true;
							}
						}
					}
				}
				balls[o] = false;
			}
			if (fabs(pos[o].second) <= 0.05f * 10 && pos[o].first < -0.9f * 10 && vel[o].first < 0) {
				if (o == 0) {
					if (player1.first == true) {
						if (player1.second == 7 || player1.second == 14)
							cout << "Player 1 pot the black ball first. Player 1 won!" << endl;
						else
							cout << "Player 1 pot the black ball. Player 2 won!" << endl;
					}
					else {
						if (player2.second == 7 || player1.second == 14)
							cout << "Player 2 pot the black ball first. Player 2 won!" << endl;
						else
							cout << "Player 2 pot the black ball. Player 1 won!" << endl;
					}
				}
				else if (!choose) {
					if (player1.first == true) {
						if (o >= 8) {
							player1.second = 8;
							player2.second = 1;
						}
						else {
							player1.second = 1;
							player2.second = 8;
						}
						choose = true;
					}
					else {
						if (o >= 8) {
							player2.second = 8;
							player1.second = 1;
						}
						else {
							player2.second = 1;
							player1.second = 8;
						}
						choose = true;
					}
				}	
				else {
					if (o >= 8) {
						if (player1.first == true) {
							if (player1.second >= 8) {
								player1.second++;
								player1.first = false;
								player2.first = true;
							}
							else {
								foult = true;
								foults.first++;
							}
						}
						else {
							if (player2.second >= 8) {
								player2.second++;
								player2.first = false;
								player1.first = true;
							}
							else {
								foult = true;
								foults.second++;
							}
						}
					}
					else {
						if (player1.first == true) {
							if (player1.second >= 8) {
								foult = true;
								foults.first++;
							}
							else {
								player1.second++;
								player1.first = false;
								player2.first = true;
							}
						}
						else {
							if (player2.second >= 8) {
								foult = true;
								foults.second++;
							}
							else {
								player2.second++;
								player2.first = false;
								player1.first = true;
							}
						}
					}
				}
				balls[o] = false;
			}
			if (pos[o].second >= 1.4f * 10 && pos[o].first > 0.9f * 10 && (vel[o].first >= 0) && (vel[o].second >= 0)) {
				if (o == 0) {
					if (player1.first == true) {
						if (player1.second == 7 || player1.second == 14)
							cout << "Player 1 pot the black ball first. Player 1 won!" << endl;
						else
							cout << "Player 1 pot the black ball. Player 2 won!" << endl;
					}
					else {
						if (player2.second == 7 || player1.second == 14)
							cout << "Player 2 pot the black ball first. Player 2 won!" << endl;
						else
							cout << "Player 2 pot the black ball. Player 1 won!" << endl;
					}
				}
				else if (!choose) {
					if (player1.first == true) {
						if (o >= 8) {
							player1.second = 8;
							player2.second = 1;
						}
						else {
							player1.second = 1;
							player2.second = 8;
						}
						choose = true;
					}
					else {
						if (o >= 8) {
							player2.second = 8;
							player1.second = 1;
						}
						else {
							player2.second = 1;
							player1.second = 8;
						}
						choose = true;
					}
				}
				else {
					if (o >= 8) {
						if (player1.first == true) {
							if (player1.second >= 8) {
								player1.second++;
								player1.first = false;
								player2.first = true;
							}
							else {
								foult = true;
								foults.first++;
							}
						}
						else {
							if (player2.second >= 8) {
								player2.second++;
								player2.first = false;
								player1.first = true;
							}
							else {
								foult = true;
								foults.second++;
							}
						}
					}
					else {
						if (player1.first == true) {
							if (player1.second >= 8) {
								foult = true;
								foults.first++;
							}
							else {
								player1.second++;
								player1.first = false;
								player2.first = true;
							}
						}
						else {
							if (player2.second >= 8) {
								foult = true;
								foults.second++;
							}
							else {
								player2.second++;
								player2.first = false;
								player1.first = true;
							}
						}
					}
				}
				balls[o] = false;
			}
			if (pos[o].second >= 1.4f * 10 && pos[o].first < -0.9f * 10 && (vel[o].first <= 0) && (vel[o].second >= 0)) {
				if (o == 0) {
					if (player1.first == true) {
						if (player1.second == 7 || player1.second == 14)
							cout << "Player 1 pot the black ball first. Player 1 won!" << endl;
						else
							cout << "Player 1 pot the black ball. Player 2 won!" << endl;
					}
					else {
						if (player2.second == 7 || player1.second == 14)
							cout << "Player 2 pot the black ball first. Player 2 won!" << endl;
						else
							cout << "Player 2 pot the black ball. Player 1 won!" << endl;
					}
				}
				else if (!choose) {
					if (player1.first == true) {
						if (o >= 8) {
							player1.second = 8;
							player2.second = 1;
						}
						else {
							player1.second = 1;
							player2.second = 8;
						}
						choose = true;
					}
					else {
						if (o >= 8) {
							player2.second = 8;
							player1.second = 1;
						}
						else {
							player2.second = 1;
							player1.second = 8;
						}
						choose = true;
					}
				}
				else {
					if (o >= 8) {
						if (player1.first == true) {
							if (player1.second >= 8) {
								player1.second++;
								player1.first = false;
								player2.first = true;
							}
							else {
								foult = true;
								foults.first++;
							}
						}
						else {
							if (player2.second >= 8) {
								player2.second++;
								player2.first = false;
								player1.first = true;
							}
							else {
								foult = true;
								foults.second++;
							}
						}
					}
					else {
						if (player1.first == true) {
							if (player1.second >= 8) {
								foult = true;
								foults.first++;
							}
							else {
								player1.second++;
								player1.first = false;
								player2.first = true;
							}
						}
						else {
							if (player2.second >= 8) {
								foult = true;
								foults.second++;
							}
							else {
								player2.second++;
								player2.first = false;
								player1.first = true;
							}
						}
					}
				}
				balls[o] = false;
			}
			if (pos[o].second <= -1.4f * 10 && pos[o].first > 0.9f * 10 && (vel[o].first >= 0) && (vel[o].second <= 0)) {
				if (o == 0) {
					if (player1.first == true) {
						if (player1.second == 7 || player1.second == 14)
							cout << "Player 1 pot the black ball first. Player 1 won!" << endl;
						else
							cout << "Player 1 pot the black ball. Player 2 won!" << endl;
					}
					else {
						if (player2.second == 7 || player1.second == 14)
							cout << "Player 2 pot the black ball first. Player 2 won!" << endl;
						else
							cout << "Player 2 pot the black ball. Player 1 won!" << endl;
					}
				}
				else if (!choose) {
					if (player1.first == true) {
						if (o >= 8) {
							player1.second = 8;
							player2.second = 1;
						}
						else {
							player1.second = 1;
							player2.second = 8;
						}
						choose = true;
					}
					else {
						if (o >= 8) {
							player2.second = 8;
							player1.second = 1;
						}
						else {
							player2.second = 1;
							player1.second = 8;
						}
						choose = true;
					}
				}
				else {
					if (o >= 8) {
						if (player1.first == true) {
							if (player1.second >= 8) {
								player1.second++;
								player1.first = false;
								player2.first = true;
							}
							else {
								foult = true;
								foults.first++;
							}
						}
						else {
							if (player2.second >= 8) {
								player2.second++;
								player2.first = false;
								player1.first = true;
							}
							else {
								foult = true;
								foults.second++;
							}
						}
					}
					else {
						if (player1.first == true) {
							if (player1.second >= 8) {
								foult = true;
								foults.first++;
							}
							else {
								player1.second++;
								player1.first = false;
								player2.first = true;
							}
						}
						else {
							if (player2.second >= 8) {
								foult = true;
								foults.second++;
							}
							else {
								player2.second++;
								player2.first = false;
								player1.first = true;
							}
						}
					}
				}
				balls[o] = false;
			}
			if (pos[o].second <= -1.4f * 10 && pos[o].first < -0.9f * 10 && (vel[o].first <= 0) && (vel[o].second <= 0)) {
				if (o == 0) {
					if (player1.first == true) {
						if (player1.second == 7 || player1.second == 14)
							cout << "Player 1 pot the black ball first. Player 1 won!" << endl;
						else
							cout << "Player 1 pot the black ball. Player 2 won!" << endl;
					}
					else {
						if (player2.second == 7 || player1.second == 14)
							cout << "Player 2 pot the black ball first. Player 2 won!" << endl;
						else
							cout << "Player 2 pot the black ball. Player 1 won!" << endl;
					}
				}
				else if (!choose) {
					if (player1.first == true) {
						if (o >= 8) {
							player1.second = 8;
							player2.second = 1;
						}
						else {
							player1.second = 1;
							player2.second = 8;
						}
						choose = true;
					}
					else {
						if (o >= 8) {
							player2.second = 8;
							player1.second = 1;
						}
						else {
							player2.second = 1;
							player1.second = 8;
						}
						choose = true;
					}
				}
				else {
					if (o >= 8) {
						if (player1.first == true) {
							if (player1.second >= 8) {
								player1.second++;
								player1.first = false;
								player2.first = true;
							}
							else {
								foult = true;
								foults.first++;
							}
						}
						else {
							if (player2.second >= 8) {
								player2.second++;
								player2.first = false;
								player1.first = true;
							}
							else {
								foult = true;
								foults.second++;
							}
						}
					}
					else {
						if (player1.first == true) {
							if (player1.second >= 8) {
								foult = true;
								foults.first++;
							}
							else {
								player1.second++;
								player1.first = false;
								player2.first = true;
							}
						}
						else {
							if (player2.second >= 8) {
								foult = true;
								foults.second++;
							}
							else {
								player2.second++;
								player2.first = false;
								player1.first = true;
							}
						}
					}
				}
				balls[o] = false;
			}
			if (pos[o].first > 0.94f * 10 && vel[o].first > 0)
				vel[o].first *= -1;
			if (pos[o].first < -0.94f * 10 && vel[o].first < 0)
				vel[o].first *= -1;
			if (pos[o].second < -1.44f * 10 && vel[o].second < 0)
				vel[o].second *= -1;
			if (pos[o].second > 1.44f * 10 && vel[o].second > 0)
				vel[o].second *= -1;
			pos[o].first += vel[o].first * pwr[o] * deltaTimeSeconds;
			pos[o].second += vel[o].second * pwr[o] * deltaTimeSeconds;
			if (pwr[o] != 0) {
				if (fabs(pwr[o]) < friction)
					pwr[o] = 0;
				else if (pwr[o] > friction) {
					pwr[o] -= friction;
					shoot = true;
				}
				else{
					pwr[o] += friction;
					shoot = true;
				}
			}
		}
	}

	{
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(1, 1, 0));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.1f));
		RenderSimpleMesh(meshes["sphere"], shaders["ShaderLab7"], modelMatrix, glm::vec3(0, 0, 0));
	}

	{
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(-1, 1, 0));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.1f));
		RenderSimpleMesh(meshes["sphere"], shaders["ShaderLab7"], modelMatrix, glm::vec3(0, 0, 0));
	}

	{
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(1, 1, 1.5f));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.1f));
		RenderSimpleMesh(meshes["sphere"], shaders["ShaderLab7"], modelMatrix, glm::vec3(0, 0, 0));
	}

	{
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(-1, 1, 1.5f));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.1f));
		RenderSimpleMesh(meshes["sphere"], shaders["ShaderLab7"], modelMatrix, glm::vec3(0, 0, 0));
	}

	{
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(1, 1, -1.5f));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.1f));
		RenderSimpleMesh(meshes["sphere"], shaders["ShaderLab7"], modelMatrix, glm::vec3(0, 0, 0));
	}

	{
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(-1, 1, -1.5f));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.1f));
		RenderSimpleMesh(meshes["sphere"], shaders["ShaderLab7"], modelMatrix, glm::vec3(0, 0, 0));
	}
	
	{
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0.5f, 0.5f, 0));
		RenderSimpleMesh(meshes["box"], shaders["ShaderLab7"], modelMatrix, glm::vec3(0, 0.75, 0));
	}

	{
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(-0.5f, 0.5f, 0));
		RenderSimpleMesh(meshes["box"], shaders["ShaderLab7"], modelMatrix, glm::vec3(0, 0.75, 0));
	}
	
	{
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0.5f, 0.5f, 1));
		RenderSimpleMesh(meshes["box"], shaders["ShaderLab7"], modelMatrix, glm::vec3(0, 0.75, 0));
	}

	{
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(-0.5f, 0.5f, 1));
		RenderSimpleMesh(meshes["box"], shaders["ShaderLab7"], modelMatrix, glm::vec3(0, 0.75, 0));
	}

	{
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0.5f, 0.5f, -1));
		RenderSimpleMesh(meshes["box"], shaders["ShaderLab7"], modelMatrix, glm::vec3(0, 0.75, 0));
	}

	{
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(-0.5f, 0.5f, -1));
		RenderSimpleMesh(meshes["box"], shaders["ShaderLab7"], modelMatrix, glm::vec3(0, 0.75, 0));
	}

	{
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 1, 1.5f));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(2.2f, 0.1f, 0.1f));
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0.5f, 0.5f));
		RenderSimpleMesh(meshes["box"], shaders["ShaderLab7"], modelMatrix, glm::vec3(0, 0.5, 0));
	}
	
	{
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 1, -1.5f));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(2.2f, 0.1f, 0.1f));
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0.5f, -0.5f));
		RenderSimpleMesh(meshes["box"], shaders["ShaderLab7"], modelMatrix, glm::vec3(0, 0.5, 0));
	}

	{
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(1, 1, 0));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.1f, 0.1f, 3));
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0.5f, 0.5f, 0));
		RenderSimpleMesh(meshes["box"], shaders["ShaderLab7"], modelMatrix, glm::vec3(0, 0.5, 0));
	}

	{
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(-1, 1, 0));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.1f, 0.1f, 3));
		modelMatrix = glm::translate(modelMatrix, glm::vec3(-0.5f, 0.5f, 0));
		RenderSimpleMesh(meshes["box"], shaders["ShaderLab7"], modelMatrix, glm::vec3(0, 0.5, 0));
	}

}
void Tema::FrameEnd()
{
	DrawCoordinatSystem(camera->GetViewMatrix(), projectionMatrix);
}

void Tema::RenderSimpleMesh(Mesh *mesh, Shader *shader, const glm::mat4 & modelMatrix, const glm::vec3 &color)
{
	if (!mesh || !shader || !shader->GetProgramID())
		return;

	// render an object using the specified shader and the specified position
	glUseProgram(shader->program);

	GLint material = glGetUniformLocation(shader->program, "object_color");
	glUniform3fv(material, 1, glm::value_ptr(color));

	// Bind model matrix
	GLint loc_model_matrix = glGetUniformLocation(shader->program, "Model");
	glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

	// Bind view matrix
	glm::mat4 viewMatrix = camera->GetViewMatrix();
	int loc_view_matrix = glGetUniformLocation(shader->program, "View");
	glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));

	// Bind projection matrix
	
	int loc_projection_matrix = glGetUniformLocation(shader->program, "Projection");
	glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

	// Draw the object
	glBindVertexArray(mesh->GetBuffers()->VAO);
	glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_SHORT, 0);
}

// Documentation for the input functions can be found in: "/Source/Core/Window/InputController.h" or
// https://github.com/UPB-Graphics/Framework-EGC/blob/master/Source/Core/Window/InputController.h

void Tema::OnInputUpdate(float deltaTime, int mods)
{	
	float cameraSpeed = 1.0f;
	if (!start || foult) {
		if (window->KeyHold(GLFW_KEY_W)) {
			// TODO : translate the camera forward
			if (!renderCameraTarget)
				if (ballZ > 0.75f * 10)
					ballZ -= deltaTime * cameraSpeed * 10;
		}

		if (window->KeyHold(GLFW_KEY_S)) {
			// TODO : translate the camera backward
			if (!renderCameraTarget)
				if (ballZ < 1.42f * 10)
					ballZ += deltaTime * cameraSpeed * 10;
		}

		if (window->KeyHold(GLFW_KEY_D)) {
			// TODO : translate the camera to the right
			if (!renderCameraTarget)
				if (ballX < 0.92f * 10)
					ballX += deltaTime * cameraSpeed * 10;
		}

		if (window->KeyHold(GLFW_KEY_A)) {
			// TODO : translate the camera to the left
			if (!renderCameraTarget)
				if (ballX > -0.92f * 10)
					ballX -= deltaTime * cameraSpeed * 10;
		}
	}
		if (window->KeyHold(GLFW_KEY_Q)) {
			// TODO : translate the camera down
			if (!renderCameraTarget)
				camera->TranslateForward(deltaTime * cameraSpeed);
		}

		if (window->KeyHold(GLFW_KEY_E)) {
			// TODO : translate the camera up
			if (!renderCameraTarget)
				camera->TranslateForward(-deltaTime * cameraSpeed);
		}

}

void Tema::OnKeyPress(int key, int mods)
{
	// add key press event
	if (window->KeyHold(GLFW_KEY_SPACE)) {
		if (foult) {
			if (player1.first == true)
				cout << "Player 1 foult!" << endl;
			else
				cout << "Player 2 foult!" << endl;
		}
		if (!start) {
			start = true;
			player1.first = true;
			cout << "Player 1 turn." << endl;
		}
		else {
			player1.first = !player1.first;
			player2.first = !player2.first;
			if (player1.first)
				cout << "Player 1 turn." << endl;
			else
				cout << "Player 2 turn." << endl;
		}
		if (!choose)
			cout << "Table is free!" << endl;
		else {
			if (player1.first == true) {
				if (player1.second == 7 || player1.second == 14)
					cout << "Player 1 can pot the black ball" << endl;
				else if (player1.second >= 8) {
					cout << "Player 1 is with yellow balls" << endl;
					cout << "Player 1 pot " << player1.second - 7 << " yellow balls" << endl;
				}
				else {
					cout << "Player 1 is with red balls" << endl;
					cout << "Player 1 pot " << player1.second - 0 << " red balls" << endl;
				}
			}
			else {
				if (player2.second == 7 || player2.second == 14)
					cout << "Player 2 can pot the black ball" << endl;
				else if (player2.second >= 8) {
					cout << "Player 2 is with yellow balls" << endl;
					cout << "Player 2 pot " << player2.second - 7 << " yellow balls" << endl;
				}
				else {
					cout << "Player 2 is with red balls" << endl;
					cout << "Player 2 pot " << player2.second - 0 << " red balls" << endl;
				}
			}
		}
		if (player1.first == true)
			cout << "Player 1 made " << foults.first << " faults" << endl;
		else
			cout << "Player 2 made " << foults.second << " faults" << endl;
		foult = false;
		renderCameraTarget = true;
		camera->Set(glm::vec3(ballX / 10, 1.5f, ballZ / 10 + 0.75f), glm::vec3(ballX / 10, 1.05f, ballZ / 10), glm::vec3(0, 1, 0));
	}
}

void Tema::OnKeyRelease(int key, int mods)
{
	// add key release event
}

void Tema::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
	// add mouse move event
		float sensivityOY = 0.001f;
		if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT)) {
			if (renderCameraTarget) {
				// TODO : rotate the camera in Third-person mode around OX and OY using deltaX and deltaY
				// use the sensitivity variables for setting up the rotation speed
				camera->RotateThirdPerson_OY(-4 * sensivityOY * deltaX);
				rotate += -4 * sensivityOY * deltaX;
			}
			else
			{
				camera->RotateFirstPerson_OY(-2 * sensivityOY * deltaX);
			}
		}
}

void Tema::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
	if (window->MouseHold(GLFW_MOUSE_BUTTON_LEFT)) {
		shoot = true;
		power = 30;
		X = camera->forward.x;
		Z = camera->forward.z;
		renderCameraTarget = false;
		camera->Set(glm::vec3(0, 4, 0), glm::vec3(0, -1, 0), glm::vec3(0, 0, -1));
		rotate = RADIANS(180.0f);
	}
	// add mouse button press event
}

void Tema::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button release event
}

void Tema::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}

void Tema::OnWindowResize(int width, int height)
{
}


#include "Object2D.h"

#include <Core/Engine.h>

using namespace std;

Mesh* Object2D::CreateSquare(std::string name, glm::vec3 leftBottomCorner, float length, glm::vec3 color, bool fill)
{
	glm::vec3 corner = leftBottomCorner;

	std::vector<VertexFormat> vertices =
	{
		VertexFormat(corner, color),
		VertexFormat(corner + glm::vec3(length, 0, 0), color),
		VertexFormat(corner + glm::vec3(length, length, 0), color),
		VertexFormat(corner + glm::vec3(0, length, 0), color)
	};

	Mesh* square = new Mesh(name);
	std::vector<unsigned short> indices = { 0, 1, 2, 3 };
	
	if (!fill) {
		square->SetDrawMode(GL_LINE_LOOP);
	}
	else {
		// draw 2 triangles. Add the remaining 2 indices
		indices.push_back(0);
		indices.push_back(2);
	}

	square->InitFromData(vertices, indices);
	return square;
}

Mesh* Object2D::CreateRectangle(std::string name, glm::vec3 leftBottomCorner, float length, float width, glm::vec3 color, bool fill)
{
	glm::vec3 corner = leftBottomCorner;

	std::vector<VertexFormat> vertices =
	{
		VertexFormat(corner, color),
		VertexFormat(corner + glm::vec3(length, 0, 0), color),
		VertexFormat(corner + glm::vec3(length, width, 0), color),
		VertexFormat(corner + glm::vec3(0, width, 0), color)
	};

	Mesh* rectangle = new Mesh(name);
	std::vector<unsigned short> indices = { 0, 1, 2, 3 };

	if (!fill) {
		rectangle->SetDrawMode(GL_LINE_LOOP);
	}
	else {
		// draw 2 triangles. Add the remaining 2 indices
		indices.push_back(0);
		indices.push_back(2);
	}

	rectangle->InitFromData(vertices, indices);
	return rectangle;
}

Mesh* Object2D::CreateCircle(std::string name, glm::vec3 leftBottomCorner, int nr_triangles, glm::vec3 color)
{
	glm::vec3 corner = leftBottomCorner;

	vector<VertexFormat> vertices_circle;

	vertices_circle.push_back(VertexFormat(corner, color)); // center
	for (int i = 0; i < nr_triangles; i++) // add vertices equal to the number of triangles
	{
		float theta = 2.0f * 3.1415926f * float(i) / float(nr_triangles); //get the current angle

		float x = cosf(theta); //calculate the x component
		float y = sinf(theta); //calculate the y component

		vertices_circle.push_back(VertexFormat(corner + glm::vec3(x, y, 0), color));
	}

	vector<unsigned short> indices_circle;

	// add (nr_triangles - 1) triangles
	for (int i = 2; i <= nr_triangles; i++)
	{
		indices_circle.push_back(0);
		indices_circle.push_back(i - 1);
		indices_circle.push_back(i);
	};

	// add the last triangle
	indices_circle.push_back(0);
	indices_circle.push_back(nr_triangles);
	indices_circle.push_back(1);

	Mesh* circle = new Mesh(name);

	circle->InitFromData(vertices_circle, indices_circle);
	return circle;
}

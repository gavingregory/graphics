#include "Mesh.h"

Mesh::Mesh(void)	{
	type			= PRIMITIVE_POINTS;

	numVertices		= 0;

	vertices		= NULL;
	colours			= NULL;
	textureCoords	= NULL;
}

Mesh::~Mesh(void)	{
	delete[] vertices;
	delete[] colours;
	delete[] textureCoords;
}
Mesh*	Mesh::GenerateTriangle() {
  Mesh* m = new Mesh();
  m->type = PRIMITIVE_TRIANGLES;
  m->numVertices = 3;
  m->vertices = new Vector4[m->numVertices];
  m->colours = new Colour[m->numVertices];

  m->vertices[0] = Vector4(0.5f, -0.5f, 0, 1.0f);
  m->vertices[1] = Vector4(0.0f, 0.5f, 0, 1.0f);
  m->vertices[2] = Vector4(-0.5f, -0.5f, 0, 1.0f);

  m->colours[0] = Colour(255, 0, 0, 255);
  m->colours[1] = Colour(0, 255, 0, 255);
  m->colours[2] = Colour(0, 0, 255, 255);

  return m;
}

Mesh* Mesh::GenerateLine(const Vector3& from, const Vector3& to) {
  Mesh* m = new Mesh();
  m->numVertices = 2;
  m->vertices = new Vector4[m->numVertices];
  m->colours = new Colour[m->numVertices];

  m->vertices[0] = Vector4(from.x, from.y, from.z, 1.0f);
  m->vertices[1] = Vector4(to.x, to.y, to.z, 1.0f);
  m->colours[0] = Colour(255, 0, 0, 255);
  m->colours[1] = Colour(0, 0, 255, 255);

  m->type = PRIMITIVE_LINES;
  return m;
}

Mesh* Mesh::GenerateLineLoop(const std::vector<Vector3> points) {
  Mesh* m = new Mesh();
  m->numVertices = points.size();
  m->vertices = new Vector4[m->numVertices];
  for (uint i = 0; i < m->numVertices; ++i) {
    m->vertices[i] = Vector4(points[i].x, points[i].y, points[i].z, 1.0f);
  }
  m->type = PRIMITIVE_LINELOOP;
  return m;
}

Mesh* Mesh::GenerateLineStrip(const std::vector<Vector3> points) {
  Mesh* m = new Mesh();
  m->numVertices = points.size();
  m->vertices = new Vector4[m->numVertices];
  for (uint i = 0; i < m->numVertices; ++i) {
    m->vertices[i] = Vector4(points[i].x, points[i].y, points[i].z, 1.0f);
  }
  m->type = PRIMITIVE_LINESTRIP;
  return m;
}

Mesh*  Mesh::GeneratePoints(const std::vector<Vector3> points) {
  Mesh* m = new Mesh();
  m->numVertices = points.size();
  m->vertices = new Vector4[m->numVertices];
  for (uint i = 0; i < m->numVertices; ++i) {
    m->vertices[i] = Vector4(points[i].x, points[i].y, points[i].z, 1.0f);
  }
  m->type = PRIMITIVE_POINTS;
  return m;
}
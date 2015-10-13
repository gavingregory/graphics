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
  m->vertices[0] = Vector4(0.5f, -0.5f, 0, 1.0f);
  m->vertices[1] = Vector4(0.0f, 0.5f, 0, 1.0f);
  m->vertices[2] = Vector4(-0.5f, -0.5f, 0, 1.0f);

  return m;
}

Mesh* Mesh::GenerateLines(const std::vector<Vector3> points) {
  Mesh* m = new Mesh();
  m->numVertices = points.size();
  m->vertices = new Vector4[m->numVertices];
  for (uint i = 0; i < m->numVertices; ++i) {
    m->vertices[i] = Vector4(points[i].x, points[i].y, points[i].z, 1.0f);
  }
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
  std::cout << "size " << points.size() << std::endl;
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
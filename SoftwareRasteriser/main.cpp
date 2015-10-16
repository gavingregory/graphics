#include "SoftwareRasteriser.h"
#include "RenderObject.h"
#include "Mesh.h"
#include <vector>
#include <ctime>
#include <iostream>
#include <random>

int main() {
	SoftwareRasteriser r(800,600);
  RenderObject* linesObject = new RenderObject();

  int size = 4;
  std::vector<Vector3> lines(size);
  std::vector<Colour> colours(size);

  lines.push_back(Vector3(0.5f, 0.5f, 1.0f));
  lines.push_back(Vector3(-0.5f, -0.5f, 1.0f));
  lines.push_back(Vector3(-0.5f, 0.5f, 1.0f));
  lines.push_back(Vector3(0.5f, -0.5f, 1.0f));
  colours.push_back(Colour(255, 0, 0, 255));
  colours.push_back(Colour(0, 255, 0, 255));
  colours.push_back(Colour(0, 255, 0, 255));
  colours.push_back(Colour(0, 0, 255, 255));
  
  linesObject->mesh = Mesh::GenerateLine(
    Vector3(0.5f, 0.5f, 1.0f),
    Vector3(-0.5f, -0.5f, 1.0f)
  );

  while(r.UpdateWindow()) {

    // OTHER

	  r.ClearBuffers();
    r.DrawObject(linesObject);
	  r.SwapBuffers();
	}

  delete linesObject->mesh;
  delete linesObject;

	return 0;
}
#include "SoftwareRasteriser.h"
#include "RenderObject.h"
#include "Mesh.h"
#include <vector>
#include <ctime>
#include <iostream>
#include <random>

int main() {
	SoftwareRasteriser r(800,600);
  RenderObject* pointsObject = new RenderObject();
  RenderObject* linesObject = new RenderObject();
  RenderObject* triObject = new RenderObject();

  int size = 5;
  std::vector<Vector3> points(size);
  std::vector<Vector3> lines(size*2);

  /*for (int i = 0; i < size; ++i) {
    points[i] = Vector3((int)(i * 1.8f) % 3, (int)(i*1.2f) % 4, (int)(i*1.6f) % 5);
  }*/

  // TRIANGLES
  triObject->mesh = Mesh::GenerateTriangle();
 
  const int pointsSize = 500;
  float p[pointsSize*3];
  srand(static_cast <unsigned> (time(0)));
  for (int i = 0; i < (pointsSize*3); ++i) {
    float f = ((float)rand() / (RAND_MAX));
    p[i] = f;
    std::cout << "random number: " << f << std::endl;
  }

  while(r.UpdateWindow()) {

    static float f = 0.0f;
    f += 0.001f;

    // POINTS

    points.resize(0);
    float scale = 1.5f;
    for (int i = 0; i < pointsSize*3; i+=3) {
      p[i] += 0.001f;
      points.push_back(Vector3(cos(p[i] * scale), p[i + 1] * scale, p[i + 2] * scale));
    }
    /*points.push_back(Vector3(sin(f+4.9)/8, 0.1f, 1));
    points.push_back(Vector3(sin(f+6.4)/8, 0.5f, 1));
    points.push_back(Vector3(sin(f+9.2)/8, 0.2f, 1));
    points.push_back(Vector3(sin(f+15.7)/8, 0.8f, 0));
    points.push_back(Vector3(sin(f)/8, 0.6f, 0));*/
    delete pointsObject->mesh;
    pointsObject->mesh = Mesh::GeneratePoints(points);

    // LINES

    lines.resize(0);
    lines.push_back(Vector3(sin(f) * 0.1f, sin(f)/2 *  0.4f, 0.0f));    //1
    lines.push_back(Vector3(sin(f) * 0.4f, sin(f) / 2 *  0.1f, 0.0f));    //2
    lines.push_back(Vector3(sin(f) * 0.4f, sin(f) / 2 *  -0.1f, 0.0f));   //3 
    lines.push_back(Vector3(sin(f) * 0.1f, sin(f) / 2 *  -0.4f, 0.0f));   //4
    lines.push_back(Vector3(sin(f) * -0.1f, sin(f) / 2 *  -0.4f, 0.0f));  //5
    lines.push_back(Vector3(sin(f) * -0.4f, sin(f) / 2 *  -0.1f, 0.0f));  //6
    lines.push_back(Vector3(sin(f) * -0.4f, sin(f) / 2 *  0.1f, 0.0f));   //7
    lines.push_back(Vector3(sin(f) * -0.1f, sin(f) / 2 *  0.4f, 0.0f));   //8
    /*lines.push_back(Vector3(0.5f, 0.5f, 0.0f));
    lines.push_back(Vector3(0.4f, 0.6f, 0.0f));
    lines.push_back(Vector3(0.3f, 0.7f, 0.0f));
    lines.push_back(Vector3(0.2f, 0.8f, 0.0f));
    lines.push_back(Vector3(0.1f, 0.9f, 0.0f));
    lines.push_back(Vector3(0.0f, 10.0f, 0.0f));*/

    delete linesObject->mesh;
    linesObject->mesh = Mesh::GenerateLines(lines);

    // OTHER

	  r.ClearBuffers();
    r.DrawObject(pointsObject);
    r.DrawObject(linesObject);
    r.DrawObject(triObject);
	  r.SwapBuffers();
	}

  delete triObject->mesh;
  delete triObject;

  delete linesObject->mesh;
  delete linesObject;

  delete pointsObject->mesh;
  delete pointsObject;

	return 0;
}
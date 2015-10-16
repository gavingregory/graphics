#include "SoftwareRasteriser.h"
#include "RenderObject.h"
#include "Mesh.h"
#include <vector>
#include <ctime>
#include <iostream>
#include <random>

int main() {
	SoftwareRasteriser r(800,600);
  Mesh* testTri = Mesh::GenerateTriangle();

  RenderObject* o1 = new RenderObject();
  RenderObject* o2 = new RenderObject();
  RenderObject* o3 = new RenderObject();
  RenderObject* o4 = new RenderObject();

  o1->mesh = testTri;
  o2->mesh = testTri;
  o3->mesh = testTri;
  o4->mesh = testTri;

  o1->modelMatrix = Matrix4::Translation(Vector3(-0.5f, -0.5f, 0));
  o2->modelMatrix = Matrix4::Translation(Vector3(-0.5f, 0.5f, 0));
  o3->modelMatrix = Matrix4::Translation(Vector3(0.5f, -0.5f, 0));
  o4->modelMatrix = Matrix4::Translation(Vector3(0.5f, 0.5f, 0));

  while(r.UpdateWindow()) {

	  r.ClearBuffers();
    r.DrawObject(o1);
    r.DrawObject(o2);
    r.DrawObject(o3);
    r.DrawObject(o4);
	  r.SwapBuffers();
	}

  delete testTri;
  delete o1;
  delete o2;
  delete o3;
  delete o4;

	return 0;
}
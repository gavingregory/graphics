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

  Matrix4 viewMatrix;
  float yaw = 0.0f;

  while(r.UpdateWindow()) {

    yaw += Mouse::GetRelativePosition().x;

    if (Keyboard::KeyDown(KEY_A)) {
      viewMatrix = viewMatrix * Matrix4::Translation(Vector3(-0.001f, 0, 0));
    }

    if (Keyboard::KeyDown(KEY_D)) {
      viewMatrix = viewMatrix * Matrix4::Translation(Vector3(0.001f, 0, 0));
    }

    if (Keyboard::KeyDown(KEY_W)) {
      viewMatrix = viewMatrix * Matrix4::Translation(Vector3(0, 0.001f, 0));
    }

    if (Keyboard::KeyDown(KEY_S)) {
      viewMatrix = viewMatrix * Matrix4::Translation(Vector3(0, -0.001f, 0));
    }

    viewMatrix = viewMatrix * Matrix4::Rotation(yaw, Vector3(0, 1, 0));

    r.SetViewMatrix(viewMatrix);

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
#include "SoftwareRasteriser.h"
#include "RenderObject.h"
#include "Mesh.h"
#include <vector>

int main() {
	SoftwareRasteriser r(800,600);
  RenderObject* object = new RenderObject();
  RenderObject* object2 = new RenderObject();
  
  int size = 5;
  std::vector<Vector3> points(size);

  /*for (int i = 0; i < size; ++i) {
    points[i] = Vector3((int)(i * 1.8f) % 3, (int)(i*1.2f) % 4, (int)(i*1.6f) % 5);
  }*/
  
 



  while(r.UpdateWindow()) {
    static float f = 0.0f;

    points[0] = Vector3(sin(f+4.9), 0.1f, 1);
    points[1] = Vector3(sin(f+6.4), 0.5f, 1);
    points[2] = Vector3(sin(f+9.2), 0.2f, 1);
    points[3] = Vector3(sin(f+0.7), 0.8f, 0);
    points[4] = Vector3(sin(f), 0.6f, 0);
    delete object2->mesh;
    object2->mesh = Mesh::GeneratePoints(points);
    f += 0.001f;
    delete object->mesh;
    object->mesh = Mesh::GenerateLine(Vector3(0, 0.0f, 0), Vector3(sin(f), 0.5f, 0));
	  r.ClearBuffers();
    r.DrawObject(object);
    r.DrawObject(object2);
	  r.SwapBuffers();
	}

  delete object->mesh;
  delete object;

	return 0;
}
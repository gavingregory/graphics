#include "SoftwareRasteriser.h"
#include <cmath>
#include <math.h>
/*
While less 'neat' than just doing a 'new', like in the tutorials, it's usually
possible to render a bit quicker to use direct pointers to the drawing area
that the OS gives you. For a bit of a speedup, you can uncomment the define below
to switch to using this method.

For those of you new to the preprocessor, here's a quick explanation:

Preprocessor definitions like #define allow parts of a file to be selectively enabled
or disabled at compile time. This is useful for hiding parts of the codebase on a
per-platform basis: if you have support for linux and windows in your codebase, obviously
the linux platform won't have the windows platform headers available, so compilation will
fail. So instead you can hide away all the platform specific stuff:

#if PLATFORM_WINDOWS
 DoSomeWindowsStuff();
#elif PLATFORM_LINUX
 DoSomeLinuxStuff();
 #else
 #error Unsupported Platform Specified!
 #endif

 As in our usage, it also allows you to selectively compile in some different functionality
 without any 'run time' cost - if it's not enabled by the preprocessor, it won't make it to
 the compiler, so no assembly will be generated.

Also, I've implemented the Resize method for you, in a manner that'll behave itself
no matter which method you use. I kinda forgot to do that, so there was a chance you'd
get exceptions if you resized to a bigger screen area. Sorry about that.
*/
//#define USE_OS_BUFFERS

SoftwareRasteriser::SoftwareRasteriser(uint width, uint height)	: Window(width, height){
	currentDrawBuffer	= 0;

#ifndef USE_OS_BUFFERS
	//Hi! In the tutorials, it's mentioned that we need to form our front + back buffer like so:
	for (int i = 0; i < 2; ++i) {
		buffers[i] = new Colour[screenWidth * screenHeight];
	}
#else
	//This works, but we can actually save a memcopy by rendering directly into the memory the 
	//windowing system gives us, which I've added to the Window class as the 'bufferData' pointers
	for (int i = 0; i < 2; ++i) {
		buffers[i] = (Colour*)bufferData[i];
	}
#endif

	depthBuffer		=	new unsigned short[screenWidth * screenHeight];

	float zScale	= (pow(2.0f,16) - 1) * 0.5f;

	Vector3 halfScreen = Vector3((screenWidth - 1) * 0.5f, (screenHeight - 1) * 0.5f, zScale);

	portMatrix = Matrix4::Translation(halfScreen) * Matrix4::Scale(halfScreen);
}

SoftwareRasteriser::~SoftwareRasteriser(void)	{
#ifndef USE_OS_BUFFERS
	for(int i = 0; i < 2; ++i) {
		delete[] buffers[i];
	}
#endif
	delete[] depthBuffer;
}

void SoftwareRasteriser::Resize() {
	Window::Resize(); //make sure our base class gets to do anything it needs to

#ifndef USE_OS_BUFFERS
	for (int i = 0; i < 2; ++i) {
		delete[] buffers[i];
		buffers[i] = new Colour[screenWidth * screenHeight];
	}
#else
	for (int i = 0; i < 2; ++i) {
		buffers[i] = (Colour*)bufferData[i];
	}
#endif

	delete[] depthBuffer;
	depthBuffer = new unsigned short[screenWidth * screenHeight];

	float zScale = (pow(2.0f, 16) - 1) * 0.5f;

	Vector3 halfScreen = Vector3((screenWidth - 1) * 0.5f, (screenHeight - 1) * 0.5f, zScale);

	portMatrix = Matrix4::Translation(halfScreen) * Matrix4::Scale(halfScreen);
}

Colour*	SoftwareRasteriser::GetCurrentBuffer() {
	return buffers[currentDrawBuffer];
}

void	SoftwareRasteriser::ClearBuffers() {
	Colour* buffer = GetCurrentBuffer();

	unsigned int clearVal = 0xFF000000;
	unsigned int depthVal = ~0;

	for(uint y = 0; y < screenHeight; ++y) {
		for(uint x = 0; x < screenWidth; ++x) {
			buffer[(y * screenWidth) + x].c  = clearVal;
			depthBuffer[(y * screenWidth) + x] = depthVal;
		}
	}
}

void	SoftwareRasteriser::SwapBuffers() {
	PresentBuffer(buffers[currentDrawBuffer]);
	currentDrawBuffer = !currentDrawBuffer;
}

float SoftwareRasteriser::ScreenAreaOfTri(const Vector4& a, const Vector4& b, const Vector4& c) {
  float area = ((a.x * b.y) + (b.x * c.y) + (c.x*a.y)) -
    ((b.x*a.y) + (c.x*b.y) + (a.x*c.y));
  return area*0.5f;
}

void	SoftwareRasteriser::DrawObject(RenderObject*o) {
  switch (o->GetMesh()->GetType()) {
  case PRIMITIVE_TRIANGLES:
    RasteriseTriMesh(o);
    break;
  case PRIMITIVE_POINTS:
    RasterisePointsMesh(o);
    break;
  case PRIMITIVE_LINES:
    RasteriseLinesMesh(o);
    break;
  case PRIMITIVE_LINELOOP:
    RasteriseLineLoopMesh(o);
    break;
  case PRIMITIVE_LINESTRIP:
    RasteriseLineStripMesh(o);
    break;
  }
}

void SoftwareRasteriser::ShadePixel(uint x, uint y, const Colour& c) {
  if (y >= screenHeight || x >= screenWidth)
    return;
  int index = (y*screenWidth) + x;
  buffers[currentDrawBuffer][index] = c;
}

BoundingBox SoftwareRasteriser::CalculateBoxForTri(const Vector4& a, const Vector4& b, const Vector4& c) {
  BoundingBox box;

  box.topLeft.x = a.x; // Start with the first vertex value
  box.topLeft.x = min(box.topLeft.x, b.x); // swap to second if less
  box.topLeft.x = min(box.topLeft.x, c.x); // swap to third if less
  box.topLeft.x = min(box.topLeft.x, 0.0f); // screen bound 

  box.topLeft.y = a.y;
  box.topLeft.y = min(box.topLeft.y, b.y);
  box.topLeft.y = min(box.topLeft.y, c.y);
  box.topLeft.y = min(box.topLeft.y, 0.0f);

  box.bottomRight.x = a.x;
  box.bottomRight.x = max(box.bottomRight.x, b.x);
  box.bottomRight.x = max(box.bottomRight.x, c.x);
  box.bottomRight.x = max(box.bottomRight.x, screenWidth);

  box.bottomRight.y = a.y;
  box.bottomRight.y = max(box.bottomRight.y, b.y);
  box.bottomRight.y = max(box.bottomRight.y, c.y);
  box.bottomRight.y = max(box.bottomRight.y, screenHeight);

  return box;
}

void	SoftwareRasteriser::RasterisePointsMesh(RenderObject*o) {
  Matrix4 mvp = viewProjMatrix * o->GetModelMatrix();

  for (uint i = 0; i < o->GetMesh()->numVertices; ++i) {
    Vector4 vertexPos = mvp * o->GetMesh()->vertices[i];
    vertexPos.SelfDivisionByW();

    Vector4 screenPos = portMatrix * vertexPos;
    ShadePixel((uint)screenPos.x, (uint)screenPos.y, Colour::White);
  }
}

void SoftwareRasteriser::RasteriseLine(
  const Vector4 &vertA,
  const Vector4 &vertB,
  const Colour &colA,
  const Colour &colB,
  const Vector2 &texA,
  const Vector2 &texB) {
  
  // transform our ndc coordinates to screen coordinates
  Vector4 v0 = portMatrix * vertA;
  Vector4 v1 = portMatrix * vertB;
  Vector4 dir = v1 - v0; // what direction is the line going?

  int xDir = (dir.x < 0.0f) ? -1 : 1; // move left or right?
  int yDir = (dir.y < 0.0f) ? -1 : 1; // move up or down?

  int x = (int)v0.x; // current x axis plot point
  int y = (int)v0.y; // current y axis plot point

  int* target = NULL; // increment on error overflow, x or y?
  int* scan = NULL; // increment on every iteration, x or y?
  int scanVal = 0;
  int targetVal = 0;

  float slope = 0.0f;
  int range = 0; // bound our loop by this value

  // if we are steep, we have to scan over the y axis
  if (abs(dir.y) > abs(dir.x)) {
    slope = (dir.x / dir.y);
    range = (int)abs(dir.y);
    target = &x; // error on x
    scan = &y; // iterate over y;
    scanVal = yDir;
    targetVal = xDir;
  }
  else { // not steep, so scan over the x axis
    slope = (dir.y / dir.x);
    range = (int)abs(dir.x);
    target = &y; // error on y
    scan = &x;
    scanVal = xDir;
    targetVal = yDir;
  }

  float absSlope = abs(slope);
  float error = 0.0f;

  for (int i = 0; i < range; ++i) {
    ShadePixel(x, y, Colour::White);
    error += absSlope;
    if (error > 0.5f) {
      error -= 1.0f;
      (*target) += targetVal; // either advance along x or y
    }
    (*scan) += scanVal; // either iterate along x or y
  }
}

void	SoftwareRasteriser::RasteriseLinesMesh(RenderObject*o) {
  Matrix4 mvp = viewProjMatrix * o->GetModelMatrix();

  for (uint i = 0; i < o->GetMesh()->numVertices; i += 2) {
    Vector4 v0 = mvp * o->GetMesh()->vertices[i];
    Vector4 v1 = mvp * o->GetMesh()->vertices[i + 1];

    v0.SelfDivisionByW();
    v1.SelfDivisionByW();

    RasteriseLine(v0, v1);
  }
}

void SoftwareRasteriser::RasteriseLineLoopMesh(RenderObject*o) {
  if (o->GetMesh()->numVertices > 2) {
    Matrix4 mvp = viewProjMatrix * o->GetModelMatrix();
    Vector4 v0;
    Vector4 v1;
  
    for (uint i = 0; i < o->GetMesh()->numVertices; ++i) {
      v0 = mvp * o->GetMesh()->vertices[i];
      v1 = mvp* o->GetMesh()->vertices[(i+1) % o->GetMesh()->numVertices];
      v0.SelfDivisionByW();
      v1.SelfDivisionByW();
      RasteriseLine(v0, v1);
    }
  }
}

void SoftwareRasteriser::RasteriseLineStripMesh(RenderObject*o) {
  if (o->GetMesh()->numVertices > 2) {
    Matrix4 mvp = viewProjMatrix * o->GetModelMatrix();
    Vector4 v0;
    Vector4 v1;

    for (uint i = 0; i < o->GetMesh()->numVertices-1; ++i) {
      v0 = mvp * o->GetMesh()->vertices[i];
      v1 = mvp* o->GetMesh()->vertices[i + 1];
      v0.SelfDivisionByW();
      v1.SelfDivisionByW();
      RasteriseLine(v0, v1);
    }
  }
}

void	SoftwareRasteriser::RasteriseTriMesh(RenderObject*o) {

}
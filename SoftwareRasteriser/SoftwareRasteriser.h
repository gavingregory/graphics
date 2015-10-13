/******************************************************************************
Class:SoftwareRasteriser
Implements:Window
Author:Rich Davison	<richard.davison4@newcastle.ac.uk>
Description: Class to encapsulate the various rasterisation techniques looked
at in the course material.

This is the class you'll be modifying the most!

-_-_-_-_-_-_-_,------,   
_-_-_-_-_-_-_-|   /\_/\   NYANYANYAN
-_-_-_-_-_-_-~|__( ^ .^) /
_-_-_-_-_-_-_-""  ""   

*//////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Matrix4.h"
#include "Mesh.h"
#include "Texture.h"
#include "RenderObject.h"
#include "Common.h"
#include "Window.h"

#include <vector>

using std::vector;

struct BoundingBox {
	Vector2 topLeft;
	Vector2 bottomRight;
};

class RenderObject;
class Texture;

class SoftwareRasteriser : public Window	{
public:
	SoftwareRasteriser(uint width, uint height);
	~SoftwareRasteriser(void);

	void	DrawObject(RenderObject*o);

	void	ClearBuffers();
	void	SwapBuffers();

	void	SetViewMatrix(const Matrix4 &m) {
		viewMatrix		= m;
		viewProjMatrix	= projectionMatrix * viewMatrix;
	}
	
	void	SetProjectionMatrix(const Matrix4 &m) {
		projectionMatrix	= m;
		viewProjMatrix		= projectionMatrix * viewMatrix;
	}

  static float ScreenAreaOfTri(const Vector4& a, const Vector4& b, const Vector4& c);

protected:
	Colour*	GetCurrentBuffer();

	void	RasterisePointsMesh(RenderObject*o);
  void	RasteriseLinesMesh(RenderObject*o); 
  void  RasteriseLineLoopMesh(RenderObject*o);
  void  RasteriseLineStripMesh(RenderObject*o);
	void	RasteriseTriMesh(RenderObject*o);

	virtual void Resize();

	void	RasteriseLine(
    const Vector4 &vertA,
    const Vector4 &vertB, 
		const Colour &colA = Colour(),
    const Colour &colB = Colour(), 
		const Vector2 &texA = Vector2(),
    const Vector2 &texB = Vector2()
    );

  void	ShadePixel(uint x, uint y, const Colour& c);
 
	void	RasteriseTri(const Vector4 &triA, const Vector4 &triB, const Vector4 &triC, 
		const Colour &colA = Colour(), const Colour &colB = Colour(), const Colour &colC= Colour(),
		const Vector3 &texA = Vector3(), const Vector3 &texB= Vector3(), const Vector3 &texC	= Vector3());
	
  BoundingBox CalculateBoxForTri(const Vector4& a, const Vector4& b, const Vector4& c);

	int		currentDrawBuffer;

	Colour*	buffers[2];

	unsigned short*	depthBuffer;

	Matrix4 viewMatrix;
	Matrix4 projectionMatrix;
	Matrix4 textureMatrix;

	Matrix4	viewProjMatrix;

	Matrix4	portMatrix;
};


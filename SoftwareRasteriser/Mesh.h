/******************************************************************************
Class:Mesh
Implements:
Author:Rich Davison	<richard.davison4@newcastle.ac.uk>
Description: Class to represent the geometric data that makes up the meshes
we render on screen.

-_-_-_-_-_-_-_,------,   
_-_-_-_-_-_-_-|   /\_/\   NYANYANYAN
-_-_-_-_-_-_-~|__( ^ .^) /
_-_-_-_-_-_-_-""  ""   

*//////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Vector4.h"
#include "Colour.h"
#include "Vector3.h"
#include "Vector2.h"
#include "Common.h"

#include <string>
#include <fstream>
#include <vector>

using std::ifstream;
using std::string;

enum PrimitiveType {
	PRIMITIVE_POINTS,
	PRIMITIVE_LINES,
  PRIMITIVE_LINESTRIP,
  PRIMITIVE_LINELOOP,
	PRIMITIVE_TRIANGLES
};

class Mesh	{
	friend class SoftwareRasteriser;
public:
	Mesh(void);
	~Mesh(void);

	static Mesh*	GenerateTriangle();
	static Mesh*	GenerateLine(const Vector3& from, const Vector3& to);
  static Mesh*	GenerateLineLoop(const std::vector<Vector3> points);
  static Mesh*	GenerateLineStrip(const std::vector<Vector3> points);
  static Mesh*  GeneratePoints(const std::vector<Vector3> points);

	PrimitiveType	GetType() { return type;}

protected:
	PrimitiveType	type;

	uint			numVertices;

	Vector4*		vertices;
	Colour*			colours;
	Vector2*		textureCoords;	//We get onto what to do with these later on...
};


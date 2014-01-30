#ifndef __UNIFORM_GRID_H__
#define __UNIFORM_GRID_H__

#include <mio/array.h>
#include <mio/vector3.h>
#include <math.h>
#include <malloc.h>

namespace mio
{

template <typename T>
class UniformGrid
{
public:
	Array<T> **grid;
	int divCount[3];
	Vector3 cellWidth;
	Vector3 bboxWidth;
	Box bbox;

	UniformGrid()
	{
		grid = NULL;
	}

	UniformGrid(Box _bbox, int xCount, int yCount, int zCount)
	{
		bbox = _bbox;
		bbox.min_point = bbox.min_point + (-1)*Vector3(RAY_EPSILON);
		bbox.max_point = bbox.max_point + Vector3(RAY_EPSILON);
		bboxWidth = bbox.max_point - bbox.min_point;

		divCount[0] = xCount;
		divCount[1] = yCount;
		divCount[2] = zCount;

		cellWidth[0] = bboxWidth[0] / divCount[0];
		cellWidth[1] = bboxWidth[1] / divCount[1];
		cellWidth[2] = bboxWidth[2] / divCount[2];

		grid = (Array<T> **)malloc(sizeof(Array<T> *) * xCount * yCount * zCount);
		for(int i=0;i<xCount * yCount * zCount;i++)
			grid[i] = new Array<T>();
	}

	void init_by_size(Box _bbox, float xSize, float ySize, float zSize)
	{
		bbox = _bbox;
		bbox.min_point = bbox.min_point + (-1)*Vector3(RAY_EPSILON);
		bbox.max_point = bbox.max_point + Vector3(RAY_EPSILON);
		bboxWidth = bbox.max_point - bbox.min_point;

		divCount[0] = (int)ceil(bboxWidth[0] / xSize);
		divCount[1] = (int)ceil(bboxWidth[1] / ySize);
		divCount[2] = (int)ceil(bboxWidth[2] / zSize);

		cellWidth[0] = xSize;
		cellWidth[1] = ySize;
		cellWidth[2] = zSize;

		grid = (Array<T> **)malloc(sizeof(Array<T> *) * divCount[0] * divCount[1] * divCount[2]);				
		for(int i=0;i<divCount[0] * divCount[1] * divCount[2];i++)
			grid[i] = new Array<T>();
	}

	UniformGrid(Box _bbox, float xSize, float ySize, float zSize)
	{
		init_by_size(_bbox, xSize, ySize, zSize);
	}

	UniformGrid(Box _bbox, float size)
	{
		init_by_size(_bbox, size, size, size);
	}

	inline int get_cell_count()
	{
		return divCount[0] * divCount[1] * divCount[2];
	}

	inline int get_cell_index(const Point3 &p)
	{
		Vector3 v = p - bbox.min_point;
		int x = (int)floor(v.x / cellWidth[0]);
		int y = (int)floor(v.y / cellWidth[1]);
		int z = (int)floor(v.z / cellWidth[2]);
		x = max(0, min(divCount[0]-1, x));
		y = max(0, min(divCount[1]-1, y));
		z = max(0, min(divCount[2]-1, z));

		return divCount[0] * divCount[1] * z + divCount[0] * y + x;
	}

	inline int get_cell_index(int x, int y, int z)
	{
		x = max(0, min(divCount[0]-1, x));
		y = max(0, min(divCount[1]-1, y));
		z = max(0, min(divCount[2]-1, z));
		return divCount[0] * divCount[1] * z + divCount[0] * y + x;
	}

	inline void get_xyz(const Point3 &p, int &x, int &y, int &z)
	{
		Vector3 v = p - bbox.min_point;
		x = (int)floor(v.x / cellWidth[0]);
		y = (int)floor(v.y / cellWidth[1]);
		z = (int)floor(v.z / cellWidth[2]);
		x = max(0, min(divCount[0]-1, x));
		y = max(0, min(divCount[1]-1, y));
		z = max(0, min(divCount[2]-1, z));
	}

	inline Array<T> *operator[](Point3 p)
	{
		return grid[get_cell_index(p)];
	}

	inline Array<T> *get_list(int x, int y, int z)
	{
		return grid[get_cell_index(x,y,z)];
	}

	inline void add(Point3 p, T value)
	{
		int index = get_cell_index(p);
		grid[index]->push_back(value);
	}

	void clear()
	{
		int n = divCount[0] * divCount[1] * divCount[2];
		for(int i=0;i<n;i++)
			grid[i]->clear();
	}

	~UniformGrid()
	{
		int n = divCount[0] * divCount[1] * divCount[2];
		if (grid != NULL)
		{
			for(int i=0;i<n;i++)
				delete grid[i];
			free((void *)grid);
			//delete grid;
		}
		grid = NULL;
	}
};

}

#endif
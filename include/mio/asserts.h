#ifndef __MIO_ASSERTS_H__
#define __MIO_ASSERTS_H__

#include <gtest/gtest.h>

#define ASSERT_POINT_NEAR(expected, actual, tolerance) \
	do { \
		Point3 _e = (expected); \
		Point3 _a = (actual); \
		float _t = (tolerance); \
		ASSERT_NEAR(_e.x, _a.x, _t); \
		ASSERT_NEAR(_e.y, _a.y, _t); \
		ASSERT_NEAR(_e.z, _a.z, _t); \
	} while(false);

#define ASSERT_VECTOR_NEAR(expected, actual, tolerance) \
	do { \
		Vector3 _e = (expected); \
		Vector3 _a = (actual); \
		float _t = (tolerance); \
		ASSERT_NEAR(_e.x, _a.x, _t); \
		ASSERT_NEAR(_e.y, _a.y, _t); \
		ASSERT_NEAR(_e.z, _a.z, _t); \
	} while(false); 

#define ASSERT_NORMAL_NEAR(expected, actual, tolerance) \
	do { \
		Normal _e = (expected); \
		Normal _a = (actual); \
		float _t = (tolerance); \
		ASSERT_NEAR(_e.x, _a.x, _t); \
		ASSERT_NEAR(_e.y, _a.y, _t); \
		ASSERT_NEAR(_e.z, _a.z, _t); \
	} while(false); 

#define ASSERT_RGB_NEAR(expected, actual, tolerance) \
	do { \
		Rgb _e = (expected); \
		Rgb _a = (actual); \
		float _t = (tolerance); \
		ASSERT_NEAR(_e.r, _a.r, _t); \
		ASSERT_NEAR(_e.g, _a.g, _t); \
		ASSERT_NEAR(_e.b, _a.b, _t); \
	} while(false);

#define ASSERT_MATRIX_NEAR(expected, actual, tolerance) \
	do { \
		Matrix4x4 _e = (expected); \
		Matrix4x4 _a = (actual); \
		float _t = (tolerance); \
		for(int i=0;i<4;i++) \
			for(int j=0;j<4;j++) \
				ASSERT_NEAR(_e[i][j], _a[i][j], _t); \
	} while(false);

#define ASSERT_SCENE_MESH_INDEX_EQUAL(expected, actual) \
	do { \
		SceneMeshIndex _e = (expected); \
		SceneMeshIndex _a = (actual); \
		ASSERT_EQ(_e.vertex, _a.vertex); \
		ASSERT_EQ(_e.triangle, _a.triangle); \
		ASSERT_EQ(_e.color, _a.color); \
		ASSERT_EQ(_e.rho, _a.rho); \
		ASSERT_EQ(_e.Le, _a.Le); \
	} while (false);
	
#define ASSERT_SCENE_MESH_TRIANGLE_EQ(expected, actual) \
	do { \
		SceneVertexIndexOnlyTriangle _e = (expected); \
		SceneVertexIndexOnlyTriangle _a = (actual); \
		\
		ASSERT_EQ(_e.id, _a.id); \
		ASSERT_EQ(_e.v[0], _a.v[0]); \
		ASSERT_EQ(_e.v[1], _a.v[1]); \
		ASSERT_EQ(_e.v[2], _a.v[2]); \
	} while (false);

#define ASSERT_COMPLEX_NEAR(expected, actual, tolerance) \
	do { \
		Complex _e = (expected); \
		Complex _a = (actual); \
		float _t = (tolerance); \
		\
		ASSERT_NEAR(_e.re, _a.re, _t); \
		ASSERT_NEAR(_e.im, _a.im, _t); \
	} while (false);

#define ASSERT_SH16_NEAR(expected, actual, tolerance) \
	do { \
		SH16 _e = (expected); \
		SH16 _a = (actual); \
		float _t = (tolerance); \
		\
		ASSERT_NEAR(_e.c[0],  _a.c[0], _t); \
		ASSERT_NEAR(_e.c[1],  _a.c[1], _t); \
		ASSERT_NEAR(_e.c[2],  _a.c[2], _t); \
		ASSERT_NEAR(_e.c[3],  _a.c[3], _t); \
		ASSERT_NEAR(_e.c[4],  _a.c[4], _t); \
		ASSERT_NEAR(_e.c[5],  _a.c[5], _t); \
		ASSERT_NEAR(_e.c[6],  _a.c[6], _t); \
		ASSERT_NEAR(_e.c[7],  _a.c[7], _t); \
		ASSERT_NEAR(_e.c[8],  _a.c[8], _t); \
		ASSERT_NEAR(_e.c[9],  _a.c[9], _t); \
		ASSERT_NEAR(_e.c[10], _a.c[10], _t); \
		ASSERT_NEAR(_e.c[11], _a.c[11], _t); \
		ASSERT_NEAR(_e.c[12], _a.c[12], _t); \
		ASSERT_NEAR(_e.c[13], _a.c[13], _t); \
		ASSERT_NEAR(_e.c[14], _a.c[14], _t); \
		ASSERT_NEAR(_e.c[15], _a.c[15], _t); \
	} while (false);

#define ASSERT_BOX_NEAR(expected, actual, tolerance) \
	do { \
		Box __e = (expected); \
		Box __a = (actual); \
		float __t = (tolerance); \
		\
		ASSERT_POINT_NEAR(__e.min_point, __a.min_point, __t); \
		ASSERT_POINT_NEAR(__e.max_point, __a.max_point, __t); \
	} while(false);

#endif
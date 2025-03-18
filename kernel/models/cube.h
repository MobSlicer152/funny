/* generated from kernel\models\cube.obj by bmp2c.py */

#pragma once

#ifdef CUBE_DEFINE_DATA
extern const unsigned int CUBE_VERTEX_COUNT;
extern const float CUBE_VERTICES[][4];
extern const unsigned int CUBE_TEXCOORD_COUNT;
extern const float CUBE_TEXCOORDS[][2];
extern const unsigned int CUBE_NORMAL_COUNT;
extern const float CUBE_NORMALS[][3];
extern unsigned int CUBE_FACE_COUNT;
extern int CUBE_FACES[][3][3];
#else
const unsigned int CUBE_VERTEX_COUNT = 8;
const float CUBE_VERTICES[][4] =
{
	{1.0, -1.0, -1.0, 1}, {1.0, -1.0, 1.0, 1}, {-1.0, -1.0, 1.0, 1},
	{-1.0, -1.0, -1.0, 1}, {1.0, 1.0, -1.0, 1}, {1.0, 1.0, 1.0, 1},
	{-1.0, 1.0, 1.0, 1}, {-1.0, 1.0, -1.0, 1},
};

const unsigned int CUBE_TEXCOORD_COUNT = 14;
const float CUBE_TEXCOORDS[][2] =
{
	{1.0, 0.333333}, {1.0, 0.666667}, {0.666667, 0.666667},
	{0.666667, 0.333333}, {0.666667, 0.0}, {0.0, 0.333333},
	{0.0, 0.0}, {0.333333, 0.0}, {0.333333, 1.0},
	{0.0, 1.0}, {0.0, 0.666667}, {0.333333, 0.333333},
	{0.333333, 0.666667}, {1.0, 0.0},
};

const unsigned int CUBE_NORMAL_COUNT = 8;
const float CUBE_NORMALS[][3] =
{
	{1.0, -1.0, -1.0}, {1.0, -1.0, -1.0}, {-1.0, -1.0, -1.0},
	{-1.0, -1.0, -1.0}, {1.0, 1.0, 1.0}, {1.0, 1.0, 1.0},
	{-1.0, 1.0, 1.0}, {-1.0, 1.0, 1.0},
};

const unsigned int CUBE_FACE_COUNT = 12;
const int CUBE_FACES[][3][3] =
{
	{{3, 2, 0}, {2, 1, 0}, {1, 0, 0}}, {{5, 4, 1}, {6, 3, 1}, {7, 0, 1}}, {{1, 7, 2}, {5, 6, 2}, {4, 5, 2}},
	{{2, 3, 3}, {6, 4, 3}, {5, 7, 3}}, {{7, 10, 4}, {6, 9, 4}, {2, 8, 4}}, {{7, 10, 5}, {3, 12, 5}, {0, 11, 5}},
	{{3, 2, 0}, {1, 0, 0}, {0, 3, 0}}, {{5, 4, 1}, {7, 0, 1}, {4, 13, 1}}, {{1, 7, 2}, {4, 5, 2}, {0, 11, 2}},
	{{2, 3, 3}, {5, 7, 3}, {1, 11, 3}}, {{7, 10, 4}, {2, 8, 4}, {3, 12, 4}}, {{7, 10, 5}, {0, 11, 5}, {4, 5, 5}},
};

#endif

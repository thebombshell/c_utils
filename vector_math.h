
/** @file		vector_math.h
 *	@brief		The handler of vector, matrix and quaternion math of the smol framework
 *	@details	
 *	@author		[Scott R Howell (Bombshell)](http://scottrhowell.com/) [\@GIT](https://github.com/thebombshell)
 *	@date		14th July 2018
 */
 
#ifndef VECTOR_MATH_H
#define VECTOR_MATH_H

#define _USE_MATH_DEFINES
#include <math.h>

/**	randf - returns a random float between 0.0 and 1.0
 *
 *	@returns	a random float between 0.0 and 1.0
 */
float randf();

#ifdef __DOXYGEN__

/**	@struct		vec
 *	@brief		not a class, but refers to an array of floats used for vector and matrix math
 */

/**	@struct		vec2
 *	@extends	vec
 *	@brief		not a class, but refers to an array of 2 floats used for 2 dimensional vector math
 */

/**	@struct		vec3
 *	@extends	vec
 *	@brief		not a class, but refers to an array of 3 floats used for 3 dimensional vector math
 */

/**	@struct		vec4
 *	@extends	vec
 *	@brief		not a class, but refers to an array of 4 floats used for 4 dimensional vector math
 */

/**	Copies the given vector to the given target
 *	@memberof	vec
 *	@param		t_target - the target float array to copy to
 *	@param		t_b - the source float array to copy from
 *	@returns	the address of t_target to allow nesting vector operations
 */
float* vec_copy(float* t_target, float* t_b);
 
/**	Sets t_target to the sum vector of t_a and t_b
 *	@memberof	vec
 *	@param		t_target - the vector to receive the results
 *	@param		t_a - the first vector operand to be added to
 *	@param		t_b - the second vector operand to be added
 *	@returns	the address of t_target to allow nesting vector operations
 */
float* vec_add(float* t_target, const float* t_a, const float* t_b);

/**	Sets t_target to the resulting vector of t_a minus t_b
 *	@memberof	vec
 *	@param		t_target - the vector to receive the results
 *	@param		t_a - the first vector operand to be subtracted from
 *	@param		t_b - the second vector operand to be subtracted
 *	@returns	the address of t_target to allow nesting vector operations
 */
float* vec_sub(float* t_target, const float* t_a, const float* t_b);

/**	Sets t_target to the multiple vector of t_a and t_b
 *	@memberof	vec
 *	@param		t_target - the vector to receive the results
 *	@param		t_a - the first vector operand to be multiplied
 *	@param		t_b - the second vector operand to be multiplied
 *	@returns	the address of t_target to allow nesting vector operations
 */
float* vec_mul(float* t_target, const float* t_a, const float* t_b);

/**	Sets t_target to the resulting vector of t_a divided by t_b
 *	@memberof	vec
 *	@param		t_target - the vector to receive the results
 *	@param		t_a - the first vector operand to be divided
 *	@param		t_b - the second scalar vector operand to be divided by
 *	@returns	the address of t_target to allow nesting vector operations
 */
float* vec_div(float* t_target, const float* t_a, const float* t_b);

/**	Sets t_target to the sum vector of t_a and t_b
 *	@memberof	vec
 *	@param		t_target - the vector to receive the results
 *	@param		t_a - the first vector operand to be added to
 *	@param		t_b - the second scalar operand to be added
 *	@returns	the address of t_target to allow nesting vector operations
 */
float* vec_adds(float* t_target, const float* t_a, float t_b);

/**	Sets t_target to the resulting vector of t_a minus t_b
 *	@memberof	vec
 *	@param		t_target - the vector to receive the results
 *	@param		t_a - the first vector operand to be subtracted from
 *	@param		t_b - the second scalar operand to be substracted
 *	@returns	the address of t_target to allow nesting vector operations
 */
float* vec_subs(float* t_target, const float* t_a, float t_b);

/**	Sets t_target to the multiple vector of t_a and t_b
 *	@memberof	vec
 *	@param		t_target - the vector to receive the results
 *	@param		t_a - the first vector operand to be multiplied
 *	@param		t_b - the second scalar operand to be multiplied
 *	@returns	the address of t_target to allow nesting vector operations
 */
float* vec_muls(float* t_target, const float* t_a, float t_b);

/**	Sets t_target to the resulting vector of t_a divided by t_b
 *	@memberof	vec
 *	@param		t_target - the vector to receive the results
 *	@param		t_a - the first vector operand to be divided
 *	@param		t_b - the second scalar operand to be divided by
 *	@returns	the address of t_target to allow nesting vector operations
 */
float* vec_divs(float* t_target, const float* t_a, float t_b);

/**	Returns the dot product of t_a and t_b
 *	@memberof	vec
 *	@param		t_a - the first vector operand of the dot product
 *	@param		t_b - the second vector operand of the dot product
 *	@returns	the dot product of t_a and t_b
 */
float vec_dot(const float* t_a, const float* t_b);

/**	Returns the squared length of the given vector
 *	@memberof	vec
 *	@param		t_a - the vector to find the length of
 *	@returns	the squared length of the given vector
 */
float vec_lensq(const float* t_a);

/**	Returns the length of the given vector
 *	@memberof	vec
 *	@param		t_a - the vector to find the length of
 *	@returns	the length of the given vector
 */
float vec_length(const float* t_a);

/**	Sets t_target to the normalized t_a
 *	@memberof	vec
 *	@param		t_target - the vector to receive the results
 *	@param		t_a - the vector to be normalized
 *	@returns	the address of t_target to allow nesting vector operations
 */
float* vec_normalize(float* t_target, const float* t_a);

#endif

#define DECLARE_VECTOR_METHOD_GROUP( SIZE ) \
	float* vec ## SIZE ## _copy(float* t_target, const float* t_b); \
	float* vec ## SIZE ## _add(float* t_target, const float* t_a, const float* t_b); \
	float* vec ## SIZE ## _sub(float* t_target, const float* t_a, const float* t_b); \
	float* vec ## SIZE ## _mul(float* t_target, const float* t_a, const float* t_b); \
	float* vec ## SIZE ## _div(float* t_target, const float* t_a, const float* t_b); \
	float* vec ## SIZE ## _adds(float* t_target, const float* t_a, float t_b); \
	float* vec ## SIZE ## _subs(float* t_target, const float* t_a, float t_b); \
	float* vec ## SIZE ## _muls(float* t_target, const float* t_a, float t_b); \
	float* vec ## SIZE ## _divs(float* t_target, const float* t_a, float t_b); \
	float vec ## SIZE ## _dot(const float* t_a, const float* t_b); \
	float vec ## SIZE ## _lensq(const float* t_a); \
	float vec ## SIZE ## _length(const float* t_a); \
	float* vec ## SIZE ## _normalize(float* t_target, const float* t_a);

DECLARE_VECTOR_METHOD_GROUP( 2 )
DECLARE_VECTOR_METHOD_GROUP( 3 )
DECLARE_VECTOR_METHOD_GROUP( 4 )

/**	Sets t_target to the product of t_a cross t_b
 *	@memberof	vec3
 *	@param		t_target - the vector to receive the results
 *	@param		t_a - the first vector operand of the cross product
 *	@param		t_b - the second vector operand of the cross product
 *	@returns	the address of t_target to allow nesting vector operations
 */
float* vec3_cross(float* t_target, float* t_a, float* t_b);

/**	Sets t_target to t_a rotated by t_b
 *	@memberof	vec3
 *	@param		t_target - the vector to receive the results
 *	@param		t_a - the vector to be rotated
 *	@param		t_b - the quaternion to rotate by
 *	@returns	the address of t_target to allow nesting vector operations
 */
float* vec3_rotate(float* t_target, float* t_a, float* t_b);

/**	@struct		quat
 *	@extends	vec
 *	@brief		not a class, but refers to an array of 4 floats used for 4 dimensional quaternion math
 */

/**	Sets t_target to an quaternion identity
 *	@memberof	quat
 *	@param		t_target - the quaternion to receive the results
 *	@returns	the address of t_target to allow nesting quaternion operations
 */
float* quat_identity(float* t_target);

/**	Sets t_target to a quaternion formed from axis and angle
 *	@memberof	quat
 *	@param		t_target - the quaternion to receive the results
 *	@param		t_axis - the normal vector axis
 *	@param		t_angle - the scalar rotation around the axis
 *	@returns	the address of t_target to allow nesting quaternion operations
 */
float* quat_axis_angle(float* t_target, float* t_axis, float t_angle);

/**	Sets t_target to the conjugate of t_a
 *	@memberof	quat
 *	@param		t_target - the quaternion to receive the results
 *	@param		t_a - the quaternion to be inversed
 *	@returns	the address of t_target to allow nesting quaternion operations
 */
float* quat_conjugate(float* t_target, float* t_a);

/**	Sets t_target to the multiplication of t_a and t_b
 *	@memberof	quat
 *	@param		t_target - the quaternion to receive the results
 *	@param		t_a - the first quaternion operand to be multiplied
 *	@param		t_b - the second quaternion operand to be multiplied
 *	@returns	the address of t_target to allow nesting quaternion operations
 */
float* quat_mul(float* t_target, float* t_a, float* t_b);

#ifdef __DOXYGEN__

/**	Copies the given quaternion to the given target
 *	@memberof	quat
 *	@param		t_target - the target float array to copy to
 *	@param		t_b - the source float array to copy from
 *	@returns	the address of t_target to allow nesting quaternion operations
 */
float* quat_copy(float* t_target, float* t_b)

/**	Returns the squared length of the given quaternion
 *	@memberof	quat
 *	@param		t_a - the quaternion to find the length of
 *	@returns	the squared length of the given quaternion
 */
float quat_lensq(const float* t_a);

/**	Returns the length of the given quaternion
 *	@memberof	quat
 *	@param		t_a - the quaternion to find the length of
 *	@returns	the length of the given quaternion
 */
float quat_length(const float* t_a);

/**	Sets t_target to the normalized t_a
 *	@memberof	quat
 *	@param		t_target - the quaternion to receive the results
 *	@param		t_a - the quaternion to be normalized
 *	@returns	the address of t_target to allow nesting quaternion operations
 */
float* quat_normalize(float* t_target, const float* t_a);
#endif

#define quat_copy( ... ) vec4_copy( __VA_ARGS__ )
#define quat_lensq( ... ) vec4_lensq( __VA_ARGS__ )
#define quat_length( ... ) vec4_length( __VA_ARGS__ )
#define quat_normalize( ... ) vec4_normalize( __VA_ARGS__ )

#ifdef __DOXYGEN__

/**	@struct		mat
 *	@brief		not a class, but refers to an array of floats used for matrix math
 */
 
/**	@struct		mat2
 *	@extends	mat
 *	@brief		not a class, but refers to an array of 4 floats used for matrix math
 */
 
/**	@struct		mat3
 *	@extends	mat
 *	@brief		not a class, but refers to an array of 9 floats used for matrix math
 */
 
/**	@struct		mat4
 *	@extends	mat
 *	@brief		not a class, but refers to an array of 12 floats used for matrix math
 */

/**	Sets and returns t_target as an identity matrix 
 *	@memberof	mat
 *	@param		t_target - the matrix to receive the results
 *	@returns	the address of t_target to allow nesting matrix operations
 */
float* mat_identity(float* t_target);

/**	Sets and returns t_target as a copy of t_b 
 *	@memberof	mat
 *	@param		t_target - the matrix to receive the results
 *	@param		t_b - the matrix to copy
 *	@returns	the address of t_target to allow nesting matrix operations
 */
float* mat_copy(float* t_target, float* t_b);

/**	Sets and returns t_target as the minor of t_b with the given column and row to ignore 
 *	@memberof	mat
 *	@param		t_target - the matrix to receive the results
 *	@param		t_b - the matrix to find the minor of
 *	@param		t_ignore_c - the column of the matrix to ignore
 *	@param		t_ignore_r - the row of the matrix to ignore
 *	@returns	the address of t_target to allow nesting matrix operations
 */
float* mat_minor(float* t_target, float* t_b, unsigned int t_ignore_c, unsigned int t_ignore_r);

/**	Sets and returns t_target as the multiple matrix of t_a and t_b 
 *	@memberof	mat
 *	@param		t_target - the matrix to receive the results
 *	@param		t_a - the first matrix operand of the operation
 *	@param		t_b - the second matrix operand of the operation
 *	@returns	the address of t_target to allow nesting matrix operations
 */
float* mat_mul(float* t_target, float* t_a, float* t_b);

/**	Sets and returns t_target as the sum matrix of each of t_a's elements and t_b 
 *	@memberof	mat
 *	@param		t_target - the matrix to receive the results
 *	@param		t_a - the first matrix operand of the operation
 *	@param		t_b - the second scalar operand of the operation
 *	@returns	the address of t_target to allow nesting matrix operations
 */
float* mat_adds(float* t_target, float* t_a, float t_b);

/**	Sets and returns t_target as the resulting matrix of each of t_a's elements minus t_b 
 *	@memberof	mat
 *	@param		t_target - the matrix to receive the results
 *	@param		t_a - the first matrix operand of the operation
 *	@param		t_b - the second scalar operand of the operation
 *	@returns	the address of t_target to allow nesting matrix operations
 */
float* mat_subs(float* t_target, float* t_a, float t_b);

/**	Sets and returns t_target as the multiple matrix of each of t_a's elements and t_b 
 *	@memberof	mat
 *	@param		t_target - the matrix to receive the results
 *	@param		t_a - the first matrix operand of the operation
 *	@param		t_b - the second scalar operand of the operation
 *	@returns	the address of t_target to allow nesting matrix operations
 */
float* mat_muls(float* t_target, float* t_a, float t_b);

/**	Sets and returns t_target as the resulting matrix of each of t_a's elements divided by t_b 
 *	@memberof	mat
 *	@param		t_target - the matrix to receive the results
 *	@param		t_a - the first matrix operand of the operation
 *	@param		t_b - the second scalar operand of the operation
 *	@returns	the address of t_target to allow nesting matrix operations
 */
float* mat_divs(float* t_target, float* t_a, float t_b);

/**	Finds and returns the determinant of t_a
 *	@memberof	mat
 *	@param		t_a - the matrix to find the determinant of
 *	@returns	the determinant of t_a
 */
float mat_determinant(float* t_a);

/**	Sets and returns t_target as the matrix of t_a's minors
 *	@memberof	mat
 *	@param		t_target - the matrix to receive the results
 *	@param		t_a - the matrix to find the matrix of minors of
 *	@returns	the address of t_target to allow nesting matrix operations
 */
float* mat_minors(float* t_target, float* t_a);

/**	Sets and returns t_target as the matrix of t_a's cofactors
 *	@memberof	mat
 *	@param		t_target - the matrix to receive the results
 *	@param		t_a - the matrix to find the matrix of cofactors of
 *	@returns	the address of t_target to allow nesting matrix operations
 */
float* mat_cofactors(float* t_target, float* t_a);

/**	Sets and returns t_target as the transposed matrix of t_a
 *	@memberof	mat
 *	@param		t_target - the matrix to receive the results
 *	@param		t_a - the matrix to find the transpose of
 *	@returns	the address of t_target to allow nesting matrix operations
 */
float* mat_transpose(float* t_target, float* t_a);

/**	Sets and returns t_target as the adjugate matrix of t_a
 *	@memberof	mat
 *	@param		t_target - the matrix to receive the results
 *	@param		t_a - the matrix to find the adjugate of
 *	@returns	the address of t_target to allow nesting matrix operations
 */
float* mat_adjugate(float* t_target, float* t_a);

/**	Sets and returns t_target as the inverted matrix of t_a
 *	@memberof	mat
 *	@param		t_target - the matrix to receive the results
 *	@param		t_a - the matrix to find the inverse matrix of
 *	@returns	the address of t_target to allow nesting matrix operations
 */
float* mat_invert(float* t_target, float* t_a);

#endif

#define DECLARE_MATRIX_METHOD_GROUP( SIZE ) \
	float* mat ## SIZE ## _identity(float* t_target); \
	float* mat ## SIZE ## _copy(float* t_target, float* t_b); \
	float* mat ## SIZE ## _minor(float* t_target, float* t_b, unsigned int t_ignore_c, unsigned int t_ignore_r); \
	float* mat ## SIZE ## _mul(float* t_target, float* t_a, float* t_b); \
	float* mat ## SIZE ## _adds(float* t_target, float* t_a, float t_b); \
	float* mat ## SIZE ## _subs(float* t_target, float* t_a, float t_b); \
	float* mat ## SIZE ## _muls(float* t_target, float* t_a, float t_b); \
	float* mat ## SIZE ## _divs(float* t_target, float* t_a, float t_b); \
	float mat ## SIZE ## _determinant(float* t_a); \
	float* mat ## SIZE ## _minors(float* t_target, float* t_a); \
	float* mat ## SIZE ## _cofactors(float* t_target, float* t_a); \
	float* mat ## SIZE ## _transpose(float* t_target, float* t_a); \
	float* mat ## SIZE ## _adjugate(float* t_target, float* t_a); \
	float* mat ## SIZE ## _invert(float* t_target, float* t_a); \

DECLARE_MATRIX_METHOD_GROUP( 2 )
DECLARE_MATRIX_METHOD_GROUP( 3 )
DECLARE_MATRIX_METHOD_GROUP( 4 )

/** Sets and returns t_target as the translation matrix given values t_translation
 *	@memberof	mat4
 *	@param		t_target - the matrix to receive the results
 *	@param		t_translation - the vector coordinates describing the translation
 *	@returns	the address of t_target to allow nesting matrix operations
 */
float* mat4_translation(float* t_target, float* t_translation);

/** Sets and returns t_target as the rotation matrix given values t_rotation
 *	@memberof	mat4
 *	@param		t_target - the matrix to receive the results
 *	@param		t_rotation - the quaternion coordinates describing the rotation
 *	@returns	the address of t_target to allow nesting matrix operations
 */
float* mat4_rotation(float* t_target, float* t_rotation);

/** Sets and returns t_target as the scaling matrix given values t_scale
 *	@memberof	mat4
 *	@param		t_target - the matrix to receive the results
 *	@param		t_scale - the vector coordinates describing the scale
 *	@returns	the address of t_target to allow nesting matrix operations
 */
float* mat4_scaling(float* t_target, float* t_scale);

/** Sets and returns t_target as the affine transformation matrix given t_translation, t_rotation and t_scale
 *	@memberof	mat4
 *	@param		t_target - the matrix to receive the results
 *	@param		t_translation - the vector coordinates describing the translation
 *	@param		t_rotation - the quaternion coordinates describing the rotation
 *	@param		t_scale - the vector coordinates describing the scale
 *	@returns	the address of t_target to allow nesting matrix operations
 */
float* mat4_transformation(float* t_target, float* t_translation, float* t_rotation, float* t_scale);

/** Sets and returns t_target as the view matrix given t_eye, t_look and t_up
 *	@memberof	mat4
 *	@param		t_target - the matrix to receive the results
 *	@param		t_eye - the origin vector of the view matrix
 *	@param		t_look - the target vector of the view matrix
 *	@param		t_up - the up orientation normal vector of the matrix
 *	@returns	the address of t_target to allow nesting matrix operations
 */
float* mat4_lookat(float* t_target, float* t_eye, float* t_look, float* t_up);

/** Sets and returns t_target as the frustum projection matrix given t_l, t_r, t_b, t_t, t_n, t_f
 *	@memberof	mat4
 *	@param		t_target - the matrix to receive the results
 *	@param		t_l - the left coordinate of the frustum
 *	@param		t_r - the right coordinate of the frustum
 *	@param		t_b - the bottom coordinate of the frustum
 *	@param		t_t - the top coordinate of the frustum
 *	@param		t_n - the near coordinate of the frustum
 *	@param		t_f - the far coordinate of the frustum
 *	@returns	the address of t_target to allow nesting matrix operations
 */
float* mat4_frustum(float* t_target, float t_l, float t_r, float t_b, float t_t, float t_n, float t_f);

/** Sets and returns t_target as the orthogonal graphic projection matrix given t_l, t_r, t_b, t_t, t_n, t_f
 *	@memberof	mat4
 *	@param		t_target - the matrix to receive the results
 *	@param		t_l - the left coordinate of the projection
 *	@param		t_r - the right coordinate of the projection
 *	@param		t_b - the bottom coordinate of the projection
 *	@param		t_t - the top coordinate of the projection
 *	@param		t_n - the near coordinate of the projection
 *	@param		t_f - the far coordinate of the projection
 *	@returns	the address of t_target to allow nesting matrix operations
 */
float* mat4_orthographic(float* t_target, float t_l, float t_r, float t_b, float t_t, float t_n, float t_f);

/** Sets and returns t_target as the perspective projection matrix given t_ar, t_fov, t_n and t_f
 *	@memberof	mat4
 *	@param		t_target - the matrix to receive the results
 *	@param		t_ar - the aspect ratio of the projection
 *	@param		t_fov - the field of view in radians of the perspective
 *	@param		t_n - the near coordinate of the projection
 *	@param		t_f - the far coordinate of the projection
 *	@returns	the address of t_target to allow nesting matrix operations
 */
float* mat4_perspective(float* t_target, float t_ar, float t_fov, float t_n, float t_f);

#endif
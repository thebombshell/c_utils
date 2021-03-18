
#include "vector_math.h"
#include <stdlib.h>

float randf() {
	
	return (float)rand() / (float)RAND_MAX;
}

#define DEFINE_VECTOR_METHOD_GROUP( T_SIZE ) \
	float* vec ## T_SIZE ## _copy(float* t_target, const float* t_b) { unsigned int i; for (i = 0; i < T_SIZE; ++i) { t_target[i] = t_b[i]; } return t_target; } \
	float* vec ## T_SIZE ## _add(float* t_target, const float* t_a, const float* t_b) { unsigned int i; for (i = 0; i < T_SIZE; ++i) { t_target[i] = t_a[i] + t_b[i]; } return t_target; } \
	float* vec ## T_SIZE ## _sub(float* t_target, const float* t_a, const float* t_b) { unsigned int i; for (i = 0; i < T_SIZE; ++i) { t_target[i] = t_a[i] - t_b[i]; } return t_target; } \
	float* vec ## T_SIZE ## _mul(float* t_target, const float* t_a, const float* t_b) { unsigned int i; for (i = 0; i < T_SIZE; ++i) { t_target[i] = t_a[i] * t_b[i]; } return t_target; } \
	float* vec ## T_SIZE ## _div(float* t_target, const float* t_a, const float* t_b) { unsigned int i; for (i = 0; i < T_SIZE; ++i) { t_target[i] = t_a[i] / t_b[i]; } return t_target; } \
	float* vec ## T_SIZE ## _adds(float* t_target, const float* t_a, float t_b) { unsigned int i; for (i = 0; i < T_SIZE; ++i) { t_target[i] = t_a[i] + t_b; } return t_target; } \
	float* vec ## T_SIZE ## _subs(float* t_target, const float* t_a, float t_b) { unsigned int i; for (i = 0; i < T_SIZE; ++i) { t_target[i] = t_a[i] - t_b; } return t_target; } \
	float* vec ## T_SIZE ## _muls(float* t_target, const float* t_a, float t_b) { unsigned int i; for (i = 0; i < T_SIZE; ++i) { t_target[i] = t_a[i] * t_b; } return t_target; } \
	float* vec ## T_SIZE ## _divs(float* t_target, const float* t_a, float t_b) { unsigned int i; for (i = 0; i < T_SIZE; ++i) { t_target[i] = t_a[i] / t_b; } return t_target; } \
	float vec ## T_SIZE ## _dot(const float* t_a, const float* t_b) { int i; float output = 0.0f; for (i = 0; i < T_SIZE; ++i) { output += t_a[i] * t_b[i]; } return output; } \
	float vec ## T_SIZE ## _lensq(const float* t_a) { return vec ## T_SIZE ## _dot(t_a, t_a); } \
	float vec ## T_SIZE ## _length(const float* t_a) { return sqrt(vec ## T_SIZE ## _lensq(t_a)); } \
	float* vec ## T_SIZE ## _normalize(float* t_target, const float* t_a) { return vec ## T_SIZE ## _divs(t_target, t_a, vec ## T_SIZE ## _length(t_a)); }

DEFINE_VECTOR_METHOD_GROUP( 2 )
DEFINE_VECTOR_METHOD_GROUP( 3 )
DEFINE_VECTOR_METHOD_GROUP( 4 )

float* vec3_cross(float* t_target, float* t_a, float* t_b) {
	
	float a[3], b[3];
	vec3_copy(a, t_a);
	vec3_copy(b, t_b);
	t_target[0] = a[1] * b[2] - a[2] * b[1];
	t_target[1] = a[2] * b[0] - a[0] * b[2];
	t_target[2] = a[0] * b[1] - a[1] * b[0];
	return t_target;
}

float* vec3_rotate(float* t_target, float* t_a, float* t_b) {
	
	float conj[4];
    float temp[4] = {t_a[0], t_a[1], t_a[2], 0.0f};
	quat_conjugate(conj, t_b);
	quat_mul(temp, t_b, temp);
	quat_mul(temp, temp, conj);
	return vec3_copy(t_target, temp);
}

float* quat_identity(float* t_target) {
	
	t_target[0] = 0.0f;
	t_target[1] = 0.0f;
	t_target[2] = 0.0f;
	t_target[3] = 1.0f;
	return t_target;
}

float* quat_axis_angle(float* t_target, float* t_axis, float t_angle) {
	
	vec3_muls(t_target, t_axis, sin(t_angle * 0.5f));
	t_target[3] = cos(t_angle * 0.5f);
	return t_target;
}

float* quat_conjugate(float* t_target, float* t_a) {
	
	t_target[0] = -t_a[0];
	t_target[1] = -t_a[1];
	t_target[2] = -t_a[2];
	t_target[3] = t_a[3];
	return t_target;
}

float* quat_mul(float* t_target, float* t_a, float* t_b) {
	
	float temp[4];
	temp[0] = t_a[1] * t_b[0] + t_a[0] * t_b[1] - t_a[3] * t_b[2] + t_a[2] * t_b[3];
	temp[1] = t_a[2] * t_b[0] + t_a[3] * t_b[1] + t_a[0] * t_b[2] - t_a[1] * t_b[3];
	temp[2] = t_a[3] * t_b[0] - t_a[2] * t_b[1] + t_a[1] * t_b[2] + t_a[0] * t_b[3];
	temp[3] = t_a[0] * t_b[0] - t_a[1] * t_b[1] - t_a[2] * t_b[2] - t_a[3] * t_b[3];
	return quat_copy(t_target, temp);
}

#define DEFINE_MATRIX_METHOD_GROUP( T_SIZE ) \
	float* mat ## T_SIZE ## _identity(float* t_target) \
	{ unsigned int i, j; for (i = 0; i < T_SIZE; ++i) for (j = 0; j < T_SIZE; ++j) { t_target[i * T_SIZE + j] = i == j ? 1.0f : 0.0f; } return t_target; } \
	float* mat ## T_SIZE ## _copy(float* t_target, float* t_b) \
	{ unsigned int i, j; for (i = 0; i < T_SIZE; ++i) for (j = 0; j < T_SIZE; ++j) { t_target[i * T_SIZE + j] = t_b[i * T_SIZE + j]; } return t_target; } \
	float* mat ## T_SIZE ## _minor(float* t_target, float* t_b, unsigned int t_ignore_r, unsigned int t_ignore_c) \
	{ unsigned int i, j, x, y = 0; for (i = 0; i < T_SIZE; ++i) { if (i == t_ignore_r) continue; x = 0; for (j = 0; j < T_SIZE; ++j) { if (j == t_ignore_c) continue; t_target[y * (T_SIZE - 1) + x] = t_b[i * T_SIZE + j]; ++x; } ++y; } return t_target; } \
	float* mat ## T_SIZE ## _mul(float* t_target, float* t_a, float* t_b) \
	{ unsigned int i, j; float temp[T_SIZE * T_SIZE]; mat ## T_SIZE ## _transpose(temp, t_b); for (i = 0; i < T_SIZE; ++i) for (j = 0; j < T_SIZE; ++j) { t_target[i * T_SIZE + j] = vec ## T_SIZE ## _dot(&t_a[i * T_SIZE], &temp[j * T_SIZE]); } return t_target; } \
	float* mat ## T_SIZE ## _adds(float* t_target, float* t_a, float t_b) \
	{ unsigned int i, j; for (i = 0; i < T_SIZE; ++i) for (j = 0; j < T_SIZE; ++j) { t_target[i * T_SIZE + j] = t_a[i * T_SIZE + j] + t_b; } return t_target; } \
	float* mat ## T_SIZE ## _subs(float* t_target, float* t_a, float t_b) \
	{ unsigned int i, j; for (i = 0; i < T_SIZE; ++i) for (j = 0; j < T_SIZE; ++j) { t_target[i * T_SIZE + j] = t_a[i * T_SIZE + j] - t_b; } return t_target; } \
	float* mat ## T_SIZE ## _muls(float* t_target, float* t_a, float t_b) \
	{ unsigned int i, j; for (i = 0; i < T_SIZE; ++i) for (j = 0; j < T_SIZE; ++j) { t_target[i * T_SIZE + j] = t_a[i * T_SIZE + j] * t_b; } return t_target; } \
	float* mat ## T_SIZE ## _divs(float* t_target, float* t_a, float t_b) \
	{ unsigned int i, j; for (i = 0; i < T_SIZE; ++i) for (j = 0; j < T_SIZE; ++j) { t_target[i * T_SIZE + j] = t_a[i * T_SIZE + j] / t_b; } return t_target; } \
	float* mat ## T_SIZE ## _cofactors(float* t_target, float* t_a) \
	{ unsigned int i, j; mat ## T_SIZE ## _minors(t_target, t_a); for (i = 0; i < T_SIZE; ++i) for (j = 0; j < T_SIZE; ++j) { t_target[i * T_SIZE + j] *= ((j + (i % 2))% 2 == 0) ? 1.0f : -1.0f; } return t_target; } \
	float* mat ## T_SIZE ## _transpose(float* t_target, float* t_a) \
	{ unsigned int i, j; float temp[T_SIZE * T_SIZE]; mat ## T_SIZE ## _copy(temp, t_a); for (i = 0; i < T_SIZE; ++i) for (j = 0; j < T_SIZE; ++j) { t_target[i * T_SIZE + j] = temp[j * T_SIZE + i]; } return t_target; } \
	float* mat ## T_SIZE ## _adjugate(float* t_target, float* t_a) \
	{ mat ## T_SIZE ## _cofactors(t_target, t_a); mat ## T_SIZE ## _transpose(t_target, t_target); return t_target; } \
	float* mat ## T_SIZE ## _invert(float* t_target, float* t_a) \
	{ float determinant = mat ## T_SIZE ## _determinant(t_a); mat ## T_SIZE ## _adjugate(t_target, t_a); mat ## T_SIZE ## _muls(t_target, t_target, 1.0f / determinant); return t_target; }

DEFINE_MATRIX_METHOD_GROUP( 2 )
DEFINE_MATRIX_METHOD_GROUP( 3 )
DEFINE_MATRIX_METHOD_GROUP( 4 )

float mat2_determinant(float* t_a) {
	
	return t_a[0] * t_a[3] - t_a[1] * t_a[2];
}

float mat3_determinant(float* t_a) {
	
	float a[4], b[4], c[4];
	mat3_minor(a, t_a, 0, 0);
	mat3_minor(b, t_a, 0, 1);
	mat3_minor(c, t_a, 0, 2);
	return t_a[0] * mat2_determinant(a) - t_a[1] * mat2_determinant(b) + t_a[2] * mat2_determinant(c);
}

float mat4_determinant(float* t_a) {
	
	float a[9], b[9], c[9], d[9];
	mat4_minor(a, t_a, 0, 0);
	mat4_minor(b, t_a, 0, 1);
	mat4_minor(c, t_a, 0, 2);
	mat4_minor(d, t_a, 0, 3);
	return t_a[0] * mat3_determinant(a) - t_a[1] * mat3_determinant(b) + t_a[2] * mat3_determinant(c) - t_a[3] * mat3_determinant(d);
}

float* mat2_minors(float* t_target, float* t_a) {
	
	float temp[4] = { t_a[3], t_a[2], t_a[1], t_a[0] };
	return mat2_copy(t_target, temp);
}

float* mat3_minors(float* t_target, float* t_a) {
	
	float temp[9];
	mat3_copy(temp, t_a);
	unsigned int i, j;
	for (i = 0; i < 3; ++i) {
		for (j = 0; j < 3; ++j) {
			
			t_target[i * 3 + j] = mat2_determinant(mat3_minor(temp, t_a, i, j));
		}
	}
	return t_target;
}

float* mat4_minors(float* t_target, float* t_a) {
	
	float temp[16];
	mat4_copy(temp, t_a);
	unsigned int i, j;
	for (i = 0; i < 4; ++i) {
		for (j = 0; j < 4; ++j) {
			
			t_target[i * 4 + j] = mat3_determinant(mat4_minor(temp, t_a, i, j));
		}
	}
	return t_target;
}

float* mat4_translation(float* t_target, float* t_translation) {
	
	unsigned int i, j;
	for (i = 0; i < 3; ++i) {
		for (j = 0; j < 4; ++j) {
			
			t_target[i * 4 + j] = i == j ? 1.0f : 0.0f;
		}
	}
	t_target[12] = t_translation[0];
	t_target[13] = t_translation[1];
	t_target[14] = t_translation[2];
	t_target[15] = 1.0f;
	return t_target;
}

float* mat4_rotation(float* t_target, float* t_q) {
	
	float q2[4];
	float qs[3];
	vec4_muls(q2, t_q, 2.0f);
	vec3_mul(qs, q2, t_q);
	t_target[0] = 1.0f - qs[1] - qs[2];
	t_target[1] = q2[0] * t_q[1] - q2[2] * t_q[3];
	t_target[2] = q2[0] * t_q[2] + q2[1] * t_q[3];
	t_target[4] = q2[0] * t_q[1] + q2[2] * t_q[3];
	t_target[5] = 1.0f - qs[0] - qs[2];
	t_target[6] = q2[1] * t_q[2] - q2[0] * t_q[3];
	t_target[8] = q2[0] * t_q[2] - q2[1] * t_q[3];
	t_target[9] = q2[1] * t_q[2] + q2[0] * t_q[3];
	t_target[10] = 1.0f - qs[0] - qs[1];
	t_target[3] = t_target[7] = t_target[11] = t_target[12] = t_target[13] = t_target[14] = 0.0f;
	t_target[15] = 1.0f;
	return t_target;
}

float* mat4_scaling(float* t_target, float* t_scale) {
	
	t_target[0] = t_scale[0];
	t_target[5] = t_scale[1];
	t_target[10] = t_scale[2];
	t_target[15] = 1.0f;
	t_target[1] = t_target[2] = t_target[3] = t_target[4] = 
	t_target[6] = t_target[7] = t_target[8] = t_target[9] = 
	t_target[11] = t_target[12] = t_target[13] = t_target[14] = 0.0f;
	return t_target;
}

float* mat4_transformation(float* t_target, float* t_translation, float* t_q, float* t_scale) {
	
	float q2[4];
	float qs[3];
	vec4_muls(q2, t_q, 2.0f);
	vec3_mul(qs, q2, t_q);
	
	t_target[0] = 1.0f - qs[1] - qs[2];
	t_target[1] = q2[0] * t_q[1] - q2[2] * t_q[3];
	t_target[2] = q2[0] * t_q[2] + q2[1] * t_q[3];
	t_target[3] = 0.0f;
	
	t_target[4] = q2[0] * t_q[1] + q2[2] * t_q[3];
	t_target[5] = 1.0f - qs[0] - qs[2];
	t_target[6] = q2[1] * t_q[2] - q2[0] * t_q[3];
	t_target[7] = 0.0f;
	
	t_target[8] = q2[0] * t_q[2] - q2[1] * t_q[3];
	t_target[9] = q2[1] * t_q[2] + q2[0] * t_q[3];
	t_target[10] = 1.0f - qs[0] - qs[1];
	t_target[11] = 0.0f;
	
	t_target[12] = t_translation[0];
	t_target[13] = t_translation[1];
	t_target[14] = t_translation[2];
	t_target[15] = 1.0f;
	return t_target;
	
}

float* mat4_lookat(float* t_target, float* t_eye, float* t_look, float* t_up) {
	
	float x[3], y[3], z[3];
	vec3_normalize(z, vec3_sub(z, t_look, t_eye));
	vec3_normalize(x, vec3_cross(x, z, t_up));
	vec3_cross(y, x, z);
	
	t_target[0] = x[0];
	t_target[1] = y[0];
	t_target[2] = z[0];
	t_target[3] = 0.0f;
	
	t_target[4] = x[1];
	t_target[5] = y[1];
	t_target[6] = z[1];
	t_target[7] = 0.0f;
	
	t_target[8] = x[2];
	t_target[9] = y[2];
	t_target[10] = z[2];
	t_target[11] = 0.0f;
	
	t_target[12] = -vec3_dot(x, t_eye);
	t_target[13] = -vec3_dot(y, t_eye);
	t_target[14] = -vec3_dot(z, t_eye);
	t_target[15] = 1.0f;
	
	return t_target;
}

float* mat4_frustum(float* t_target, float t_l, float t_r, float t_b, float t_t, float t_n, float t_f) {
	
	float h = t_r - t_l;
	float w = t_t - t_b;
	float d = t_f - t_n;
	float n2 = t_n * 2.0f;
	
	t_target[0] = n2 / w;
	t_target[1] = 0.0f;
	t_target[2] = 0.0f;
	t_target[3] = 0.0f;
	
	t_target[4] = 0.0f;
	t_target[5] = n2 / h;
	t_target[6] = 0.0f;
	t_target[7] = 0.0f;
	
	t_target[8] = (t_r + t_l) / w;
	t_target[9] = (t_t + t_b) / h;
	t_target[10] = (t_f + t_n) / d;
	t_target[11] = 1.0;
	
	t_target[12] = 0.0f;
	t_target[13] = 0.0f;
	t_target[14] = -(n2 * t_f) / d;
	t_target[15] = 0.0f;
	
	return t_target;
}

float* mat4_orthographic(float* t_target, float t_l, float t_r, float t_b, float t_t, float t_n, float t_f) {
	
	float h = t_r - t_l;
	float w = t_t - t_b;
	float d = t_f - t_n;
	
	t_target[0] = 2.0f / w;
	t_target[1] = 0.0f;
	t_target[2] = 0.0f;
	t_target[3] = 0.0f;
	
	t_target[4] = 0.0f;
	t_target[5] = 2.0f / h;
	t_target[6] = 0.0f;
	t_target[7] = 0.0f;
	
	t_target[8] = 0.0f;
	t_target[9] = 0.0f;
	t_target[10] = 2.0f / d;
	t_target[11] = 0.0f;
	
	t_target[8] = -(t_r + t_l) / w;
	t_target[9] = -(t_t + t_b) / h;
	t_target[10] = -(t_f + t_n) / d;
	t_target[15] = 1.0f;
	
	return t_target;
}

float* mat4_perspective(float* t_target, float t_ar, float t_fov, float t_n, float t_f) {
	
	float h = tan(t_fov * 0.5f);
	float w = h * t_ar;
	float d = t_f - t_n;
	
	t_target[0] = 1.0f / w;
	t_target[1] = 0.0f;
	t_target[2] = 0.0f;
	t_target[3] = 0.0f;
	
	t_target[4] = 0.0f;
	t_target[5] = 1.0f / h;
	t_target[6] = 0.0f;
	t_target[7] = 0.0f;
	
	t_target[8] = 0.0f;
	t_target[9] = 0.0f;
	t_target[10] = (t_f + t_n) / d;
	t_target[11] = 1.0;
	
	t_target[12] = 0.0f;
	t_target[13] = 0.0f;
	t_target[14] = -(2.0f * t_n * t_f) / d;
	t_target[15] = 0.0f;
	
	return t_target;
}


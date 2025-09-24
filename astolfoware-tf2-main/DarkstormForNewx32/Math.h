#pragma once
#include "SDK.h"
#include <algorithm>
#include <math.h>
#include <cmath>
#pragma warning(disable : 4305) // HOLY SHIT NIGGER SHUT UP I DONT CARE NIGGGGGGGGGGGGGGGGGGGGGGEEEEEEEEEEEEEEEEEEEEEEEEEERRRR
namespace Math
{
	inline void CreateVector(const Vector& angle, Vector& vector)
	{
		float pitch, yaw, tmp;

		pitch = float(angle[0] * PI / 180);
		yaw = float(angle[1] * PI / 180);
		tmp = float(cos(pitch));

		vector[0] = float(-tmp * -cos(yaw));
		vector[1] = float(sin(yaw) * tmp);
		vector[2] = float(-sin(pitch));
	}
	inline double __declspec (naked) __fastcall FastSqrt(double n)
	{
		_asm fld qword ptr[esp + 4]
			_asm fsqrt
		_asm ret 8
	}
	inline Vector CalcAngle(const Vector& source, const Vector& destination)
	{
		Vector angles = Vector(0.0f, 0.0f, 0.0f);
		Vector delta = (source - destination);
		float fHyp = FastSqrt((delta.x * delta.x) + (delta.y * delta.y));

		angles.x = (atanf(delta.z / fHyp) * RADPI);
		angles.y = (atanf(delta.y / delta.x) * RADPI);
		angles.z = 0.0f;

		if (delta.x >= 0.0f)
			angles.y += 180.0f;

		return angles;
	}

	inline float GetFov(const Vector& angle, const Vector& source, const Vector& destination)
	{
		Vector ang, aim;
		float mag, u_dot_v;
		ang = CalcAngle(source, destination);

		CreateVector(angle, aim);
		CreateVector(ang, ang);

		mag = sqrtf(pow(aim.x, 2) + pow(aim.y, 2) + pow(aim.z, 2));
		u_dot_v = aim.Dot(ang);

		return RAD2DEG(acos(u_dot_v / (pow(mag, 2))));
	}

	inline float RemapValClamped(float val, float A, float B, float C, float D)
	{
		if (A == B)
			return val >= B ? D : C;
		float cVal = (val - A) / (B - A);
		cVal = std::clamp(cVal, 0.0f, 1.0f);

		return C + (D - C) * cVal;
	}
	static float normalizeRad(float a) noexcept
	{
		return std::isfinite(a) ? std::remainder(a, PI * 2) : 0.0f;
	}
	static float angleDiffRad(float a1, float a2) noexcept
	{
		float delta;

		delta = normalizeRad(a1 - a2);
		if (a1 > a2)
		{
			if (delta >= PI)
				delta -= PI * 2;
		}
		else
		{
			if (delta <= -PI)
				delta += PI * 2;
		}
		return delta;
	}
	inline Vector VelocityToAngles(const Vector& direction)
	{
		auto Magnitude = [&](const Vector& v) -> float {
			return sqrtf(v.Dot(v));
		};

		float yaw, pitch;

		if (direction.y == 0.0f && direction.x == 0.0f)
		{
			yaw = 0.0f;

			if (direction.z > 0.0f)
				pitch = 270.0f;

			else pitch = 90.0f;
		}

		else
		{
			yaw = RAD2DEG(atan2f(direction.y, direction.x));
			pitch = RAD2DEG(atan2f(-direction.z, Magnitude(Vector(direction))));

			if (yaw < 0.0f)
				yaw += 360.0f;

			if (pitch < 0.0f)
				pitch += 360.0f;
		}

		return { pitch, yaw, 0.0f };
	}
	inline Vector2 RotateVec2(Vector2& point, const Vector2& origin, float radians)
	{
		float s = sin(radians);
		float c = cos(radians);

		//Translate point back to origin
		point.x -= origin.x;
		point.y -= origin.y;

		//Rotate point
		float xnew = point.x * c - point.y * s;
		float ynew = point.x * s + point.y * c;

		//Translate point back
		point.x = xnew + origin.x;
		point.y = ynew + origin.y;
		return point;
	}
	inline void VectorTransform(const Vector input, const matrix3x4& matrix, Vector& output)
	{
		output[0] = input.Dot({ matrix[0][0], matrix[0][1], matrix[0][2] }) + matrix[0][3];
		output[1] = input.Dot({ matrix[1][0], matrix[1][1], matrix[1][2] }) + matrix[1][3];
		output[2] = input.Dot({ matrix[2][0], matrix[2][1], matrix[2][2] }) + matrix[2][3];
	}
	inline float GetDistance(Vector pos1, Vector pos2)
	{
		return sqrt(((pos1.x - pos2.x) * (pos1.x - pos2.x)) + ((pos1.y - pos2.y) * (pos1.y - pos2.y)) + ((pos1.z - pos2.z) * (pos1.z - pos2.z)));
	}
	inline Vector GetDifference(Vector pos1, Vector pos2)
	{
		Vector difference;
		difference.x = pos1.x - pos2.x;
		difference.y = pos1.y - pos2.y;
		difference.z = pos1.z - pos2.z;
		return difference;
	}
	inline float RandFloatRange(float min, float max)
	{
		return (min + 1) + (((float)rand()) / (float)RAND_MAX) * (max - (min + 1));
	}
	inline float NormalizeAngle(float ang)
	{
		if (!std::isfinite(ang))
			return 0.0f;

		return std::remainder(ang, 360.0f);
	}
	inline float CalcFov(const Vector& src, const Vector& dst)
	{
		Vector v_src = Vector();
		AngleVectors(src, &v_src);

		Vector v_dst = Vector();
		AngleVectors(dst, &v_dst);

		float result = RAD2DEG(acos(v_dst.Dot(v_src) / v_dst.LengthSqr()));

		if (!isfinite(result) || isinf(result) || isnan(result))
			result = 0.0f;

		return result;
	}
	inline float GetDistanceMeters(const Vector& vec1, const Vector& vec2) // chatgpt'd.. I don't know how to math. i mean it looks fairly simple
	{
		constexpr float HU_TO_METERS = 0.01905f; // Conversion factor
		return (vec1 - vec2).Length() * HU_TO_METERS;
	}
	inline auto GetRotatedPosition(Vector start, const float rotation, const float distance)
	{
		const auto rad = DEG2RAD(rotation);
		start.x += cosf(rad) * distance;
		start.y += sinf(rad) * distance;

		return start;
	}
}
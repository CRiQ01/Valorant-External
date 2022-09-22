#ifndef VECTOR_3_H
#define VECTOR_3_H
#include <d3d9.h>
#include <math.h>
#define UCONST_Pi 3.1415926535
#define RadianToURotation 180.0f / UCONST_Pi
namespace VALORANT_EXT {
	class Vector3 {
	public:
		Vector3() : x(0.f), y(0.f), z(0.f) {}
		Vector3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}
		~Vector3() {}

		float x;
		float y;
		float z;
		Vector3& operator+=(const Vector3& v) { x += v.x; y += v.y; z += v.z; return *this; }

		inline float Dot(Vector3 v) {
			return x * v.x + y * v.y + z * v.z;
		}

		inline float Distance(Vector3 v) {
			return float(sqrtf(powf(v.x - x, 2.0) + powf(v.y - y, 2.0) + powf(v.z - z, 2.0)));
		}

		Vector3 operator+(Vector3 v) {
			return Vector3(x + v.x, y + v.y, z + v.z);
		}

		Vector3 operator*(float factor) {
			return Vector3(x * factor, y * factor, z * factor);
		}

		Vector3 operator-(Vector3 v) {
			return Vector3(x - v.x, y - v.y, z - v.z);
		}

		inline float Length() {
			return sqrtf((x * x) + (y * y) + (z * z));
		}
		inline float Length2D() {
			return sqrtf((x * x) + (y * y));
		}

		Vector3 Clamp()
		{
			Vector3 clamped = { x, y, 0 };
			while (clamped.y < -180.0f)
				clamped.y += 360.0f;
			while (clamped.y > 180.0f)
				clamped.y -= 360.0f;
			while (clamped.x < -180.0f)
				clamped.x += 360.0f;
			while (clamped.x > 180.0f)
				clamped.x -= 360.0f;
			return clamped;
		}
		Vector3 ToRotator()
		{
			Vector3 rotator;
			rotator.x = -(float)atan(z / Length2D()) * (float)(180.0f / 3.14159265358979323846);
			rotator.y = (float)atan(y / x) * (float)(180.0f / 3.14159265358979323846);
			rotator.z = (float)0.f;
			if (x >= 0.f)
				rotator.y += 180.0f;
			return rotator;
		}

	};

	struct Vector2 {
	public:
		float x;
		float y;

		inline Vector2() : x(0), y(0) {}
		inline Vector2(float x, float y) : x(x), y(y) {}

		inline float Distance(Vector2 v) {
			return sqrtf(((v.x - x) * (v.x - x) + (v.y - y) * (v.y - y)));
		}

		inline Vector2 operator+(const Vector2& v) const {
			return Vector2(x + v.x, y + v.y);
		}

		inline Vector2 operator-(const Vector2& v) const {
			return Vector2(x - v.x, y - v.y);
		}
	};

	struct FMinimalViewInfo
	{
		Vector3 Location; //+ 0x1260
		Vector3 Rotation; //+ 0x126C
		float FOV;     //+ 0x1278
	};

	class FRotator
	{
	public:
		float Pitch = 0.f;
		float Yaw = 0.f;
		float Roll = 0.f;

		D3DMATRIX GetAxes() {
			auto tempMatrix = Matrix();
			return tempMatrix;
		}

		D3DMATRIX Matrix(Vector3 origin = Vector3(0, 0, 0)) {
			float radPitch = (Pitch * float(UCONST_Pi) / 180.f);
			float radYaw = (Yaw * float(UCONST_Pi) / 180.f);
			float radRoll = (Roll * float(UCONST_Pi) / 180.f);
			float SP = sinf(radPitch);
			float CP = cosf(radPitch);
			float SY = sinf(radYaw);
			float CY = cosf(radYaw);
			float SR = sinf(radRoll);
			float CR = cosf(radRoll);

			D3DMATRIX matrix;
			matrix.m[0][0] = CP * CY;
			matrix.m[0][1] = CP * SY;
			matrix.m[0][2] = SP;
			matrix.m[0][3] = 0.f;

			matrix.m[1][0] = SR * SP * CY - CR * SY;
			matrix.m[1][1] = SR * SP * SY + CR * CY;
			matrix.m[1][2] = -SR * CP;
			matrix.m[1][3] = 0.f;

			matrix.m[2][0] = -(CR * SP * CY + SR * SY);
			matrix.m[2][1] = CY * SR - CR * SP * SY;
			matrix.m[2][2] = CR * CP;
			matrix.m[2][3] = 0.f;

			matrix.m[3][0] = origin.x;
			matrix.m[3][1] = origin.y;
			matrix.m[3][2] = origin.z;
			matrix.m[3][3] = 1.f;

			return matrix;
		}
	};

	struct FVector2D {
	public:
		float x;
		float y;

		inline FVector2D() : x(0), y(0) {}
		inline FVector2D(float x, float y) : x(x), y(y) {}

		inline float Distance(FVector2D v) {
			return sqrtf(((v.x - x) * (v.x - x) + (v.y - y) * (v.y - y)));
		}

		inline FVector2D operator+(const FVector2D& v) const {
			return FVector2D(x + v.x, y + v.y);
		}

		inline FVector2D operator-(const FVector2D& v) const {
			return FVector2D(x - v.x, y - v.y);
		}
	};

	class FVector {
	public:
		FVector() : X(0.f), Y(0.f), Z(0.f) {}
		FVector(float _x, float _y, float _z) : X(_x), Y(_y), Z(_z) {}
		~FVector() {}

		float X;
		float Y;
		float Z;

		inline float Dot(FVector v) {
			return X * v.X + Y * v.Y + Z * v.Z;
		}

		inline float Distance(FVector v) {
			return float(sqrtf(powf(v.X - X, 2.0) + powf(v.Y - Y, 2.0) + powf(v.Z - Z, 2.0)));
		}

		FVector operator+(FVector v) {
			return FVector(X + v.X, Y + v.Y, Z + v.Z);
		}

		FVector operator-(FVector v) {
			return FVector(X - v.X, Y - v.Y, Z - v.Z);
		}
		FVector operator/(float flDiv) {
			return FVector(X / flDiv, Y / flDiv, Z / flDiv);
		}

		FVector operator*(float Scale) {

			return FVector(X * Scale, Y * Scale, Z * Scale);
		}
		inline float Length() {
			return sqrtf((X * X) + (Y * Y) + (Z * Z));
		}
		FVector operator-=(FVector v) {

			X -= v.X;
			Y -= v.Y;
			Z -= v.Z;
			return *this;
		}
	};

	struct alignas(16) FPlane : public FVector
	{
		float                                              W;                                                        // 0x000C(0x0004) (Edit, BlueprintVisible, ZeroConstructor, SaveGame, IsPlainOldData)
	};

	struct FMatrix
	{
		struct FPlane                                      XPlane;                                                   // 0x0000(0x0010) (Edit, BlueprintVisible, SaveGame, IsPlainOldData)
		struct FPlane                                      YPlane;                                                   // 0x0010(0x0010) (Edit, BlueprintVisible, SaveGame, IsPlainOldData)
		struct FPlane                                      ZPlane;                                                   // 0x0020(0x0010) (Edit, BlueprintVisible, SaveGame, IsPlainOldData)
		struct FPlane                                      WPlane;                                                   // 0x0030(0x0010) (Edit, BlueprintVisible, SaveGame, IsPlainOldData)

		FMatrix operator*(const FMatrix& pM2)
		{
			FMatrix pOut;
			pOut.XPlane.X = XPlane.X * pM2.XPlane.X + XPlane.Y * pM2.YPlane.X + XPlane.Z * pM2.ZPlane.X + XPlane.W * pM2.WPlane.X;
			pOut.XPlane.Y = XPlane.X * pM2.XPlane.Y + XPlane.Y * pM2.YPlane.Y + XPlane.Z * pM2.ZPlane.Y + XPlane.W * pM2.WPlane.Y;
			pOut.XPlane.Z = XPlane.X * pM2.XPlane.Z + XPlane.Y * pM2.YPlane.Z + XPlane.Z * pM2.ZPlane.Z + XPlane.W * pM2.WPlane.Z;
			pOut.XPlane.W = XPlane.X * pM2.XPlane.W + XPlane.Y * pM2.YPlane.W + XPlane.Z * pM2.ZPlane.W + XPlane.W * pM2.WPlane.W;
			pOut.YPlane.X = YPlane.X * pM2.XPlane.X + YPlane.Y * pM2.YPlane.X + YPlane.Z * pM2.ZPlane.X + YPlane.W * pM2.WPlane.X;
			pOut.YPlane.Y = YPlane.X * pM2.XPlane.Y + YPlane.Y * pM2.YPlane.Y + YPlane.Z * pM2.ZPlane.Y + YPlane.W * pM2.WPlane.Y;
			pOut.YPlane.Z = YPlane.X * pM2.XPlane.Z + YPlane.Y * pM2.YPlane.Z + YPlane.Z * pM2.ZPlane.Z + YPlane.W * pM2.WPlane.Z;
			pOut.YPlane.W = YPlane.X * pM2.XPlane.W + YPlane.Y * pM2.YPlane.W + YPlane.Z * pM2.ZPlane.W + YPlane.W * pM2.WPlane.W;
			pOut.ZPlane.X = ZPlane.X * pM2.XPlane.X + ZPlane.Y * pM2.YPlane.X + ZPlane.Z * pM2.ZPlane.X + ZPlane.W * pM2.WPlane.X;
			pOut.ZPlane.Y = ZPlane.X * pM2.XPlane.Y + ZPlane.Y * pM2.YPlane.Y + ZPlane.Z * pM2.ZPlane.Y + ZPlane.W * pM2.WPlane.Y;
			pOut.ZPlane.Z = ZPlane.X * pM2.XPlane.Z + ZPlane.Y * pM2.YPlane.Z + ZPlane.Z * pM2.ZPlane.Z + ZPlane.W * pM2.WPlane.Z;
			pOut.ZPlane.W = ZPlane.X * pM2.XPlane.W + ZPlane.Y * pM2.YPlane.W + ZPlane.Z * pM2.ZPlane.W + ZPlane.W * pM2.WPlane.W;
			pOut.WPlane.X = WPlane.X * pM2.XPlane.X + WPlane.Y * pM2.YPlane.X + WPlane.Z * pM2.ZPlane.X + WPlane.W * pM2.WPlane.X;
			pOut.WPlane.Y = WPlane.X * pM2.XPlane.Y + WPlane.Y * pM2.YPlane.Y + WPlane.Z * pM2.ZPlane.Y + WPlane.W * pM2.WPlane.Y;
			pOut.WPlane.Z = WPlane.X * pM2.XPlane.Z + WPlane.Y * pM2.YPlane.Z + WPlane.Z * pM2.ZPlane.Z + WPlane.W * pM2.WPlane.Z;
			pOut.WPlane.W = WPlane.X * pM2.XPlane.W + WPlane.Y * pM2.YPlane.W + WPlane.Z * pM2.ZPlane.W + WPlane.W * pM2.WPlane.W;
			return pOut;
		}
	};


	struct alignas(16) FQuat
	{
		float X, Y, Z, W;
	};

	struct alignas(16) FTransform
	{
		struct FQuat                                       Rotation;                                                 // 0x0000(0x0010) (Edit, BlueprintVisible, SaveGame, IsPlainOldData)
		struct FVector                                     Translation;                                              // 0x0010(0x000C) (Edit, BlueprintVisible, SaveGame, IsPlainOldData)
		unsigned char                                      UnknownData00[0x4];                                       // 0x001C(0x0004) MISSED OFFSET
		struct FVector                                     Scale3D;                                                  // 0x0020(0x000C) (Edit, BlueprintVisible, SaveGame, IsPlainOldData)
		unsigned char                                      UnknownData01[0x4];                                       // 0x002C(0x0004) MISSED OFFSET

		FMatrix ToMatrixWithScale() const
		{
			FMatrix m;
			m.WPlane.X = Translation.X;
			m.WPlane.Y = Translation.Y;
			m.WPlane.Z = Translation.Z;

			float x2 = Rotation.X + Rotation.X;
			float y2 = Rotation.Y + Rotation.Y;
			float z2 = Rotation.Z + Rotation.Z;

			float xx2 = Rotation.X * x2;
			float yy2 = Rotation.Y * y2;
			float zz2 = Rotation.Z * z2;
			m.XPlane.X = (1.0f - (yy2 + zz2)) * Scale3D.X;
			m.YPlane.Y = (1.0f - (xx2 + zz2)) * Scale3D.Y;
			m.ZPlane.Z = (1.0f - (xx2 + yy2)) * Scale3D.Z;

			float yz2 = Rotation.Y * z2;
			float wx2 = Rotation.W * x2;
			m.ZPlane.Y = (yz2 - wx2) * Scale3D.Z;
			m.YPlane.Z = (yz2 + wx2) * Scale3D.Y;

			float xy2 = Rotation.X * y2;
			float wz2 = Rotation.W * z2;
			m.YPlane.X = (xy2 - wz2) * Scale3D.Y;
			m.XPlane.Y = (xy2 + wz2) * Scale3D.X;

			float xz2 = Rotation.X * z2;
			float wy2 = Rotation.W * y2;
			m.ZPlane.X = (xz2 + wy2) * Scale3D.Z;
			m.XPlane.Z = (xz2 - wy2) * Scale3D.X;

			m.XPlane.W = 0.0f;
			m.YPlane.W = 0.0f;
			m.ZPlane.W = 0.0f;
			m.WPlane.W = 1.0f;

			return m;
		}
	};

	static FMatrix ToMatrix(FVector rot)
	{
		FVector origin = FVector{};
		float radPitch = (rot.X * float(3.14159265358979323846) / 180.f);
		float radYaw = (rot.Y * float(3.14159265358979323846) / 180.f);
		float radRoll = (rot.Z * float(3.14159265358979323846) / 180.f);

		float SP = sinf(radPitch);
		float CP = cosf(radPitch);
		float SY = sinf(radYaw);
		float CY = cosf(radYaw);
		float SR = sinf(radRoll);
		float CR = cosf(radRoll);

		FMatrix matrix;
		matrix.XPlane.X = CP * CY;
		matrix.XPlane.Y = CP * SY;
		matrix.XPlane.Z = SP;
		matrix.XPlane.W = 0.f;

		matrix.YPlane.X = SR * SP * CY - CR * SY;
		matrix.YPlane.Y = SR * SP * SY + CR * CY;
		matrix.YPlane.Z = -SR * CP;
		matrix.YPlane.W = 0.f;

		matrix.ZPlane.X = -(CR * SP * CY + SR * SY);
		matrix.ZPlane.Y = CY * SR - CR * SP * SY;
		matrix.ZPlane.Z = CR * CP;
		matrix.ZPlane.W = 0.f;

		matrix.WPlane.X = origin.X;
		matrix.WPlane.Y = origin.Y;
		matrix.WPlane.Z = origin.Z;
		matrix.WPlane.W = 1.f;

		return matrix;
	}
}
#endif
#pragma once
typedef float matrix3x4[3][4];
class VMatrix {
private:
	Vector m[4][4];

public:
	const matrix3x4& As3x4() const {
		return *((const matrix3x4*)this);
	}
};
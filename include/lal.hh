#include <cassert>
#include <cstring>
#include <algorithm>

namespace lal 
{
	template<typename T>
	struct Vec3 {
		T x;
		T y;
		T z;
	};

	using Vec3f = Vec3<float>;
	using Vec3d = Vec3<double>;

	template<typename T>
	class Mat33
	{
	public:

		static Mat33 Identity(){
			Mat33 r{};
			r.identity();
			return r;
		}

		Mat33() { zero(); }

		Mat33(Vec3<T> c0, Vec3<T> c1, Vec3<T> c2) {
			_e[col0][xcoef] = c0.x;
			_e[col0][ycoef] = c0.y;
			_e[col0][zcoef] = c0.z;
			_e[col1][xcoef] = c1.x;
			_e[col1][ycoef] = c1.y;
			_e[col1][zcoef] = c1.z;
			_e[col2][xcoef] = c2.x;
			_e[col2][ycoef] = c2.y;
			_e[col2][zcoef] = c2.z;
		}

		Mat33(const Mat33& b){
			std::memcpy(_e, b._e, sizeof(T) * elementCount);
		}

		void operator=(const Mat33& b){
			std::memcpy(_e, b._e, sizeof(T) * elementCount);
		}

		void zero() {
			std::memset(_e, 0, sizeof(T) * elementCount);
		}

		void identity() {
			zero();
			_e[0][0] = _e[1][1] = _e[2][2] = T{1};
		}

		void transpose(){
			std::swap(_e[col0][ycoef], _e[col1][xcoef]);
			std::swap(_e[col0][zcoef], _e[col2][xcoef]);
			std::swap(_e[col1][zcoef], _e[col2][ycoef]);
		}

		void mul(const Mat33<T>& b){
			Mat33 c{};
			c._e[col0][xcoef] =
				(_e[col0][xcoef] * b._e[col0][xcoef]) +
				(_e[col1][xcoef] * b._e[col0][ycoef]) +
				(_e[col2][xcoef] * b._e[col0][zcoef]);

			c._e[col0][ycoef] =
				(_e[col0][ycoef] * b._e[col0][xcoef]) +
				(_e[col1][ycoef] * b._e[col0][ycoef]) +
				(_e[col2][ycoef] * b._e[col0][zcoef]);

			c._e[col0][zcoef] =
				(_e[col0][zcoef] * b._e[col0][xcoef]) +
				(_e[col1][zcoef] * b._e[col0][ycoef]) +
				(_e[col2][zcoef] * b._e[col0][zcoef]);

			c._e[col1][xcoef] =
				(_e[col0][xcoef] * b._e[col1][xcoef]) +
				(_e[col1][xcoef] * b._e[col1][ycoef]) +
				(_e[col2][xcoef] * b._e[col1][zcoef]);

			c._e[col1][ycoef] =
				(_e[col0][ycoef] * b._e[col1][xcoef]) +
				(_e[col1][ycoef] * b._e[col1][ycoef]) +
				(_e[col2][ycoef] * b._e[col1][zcoef]);

			c._e[col1][zcoef] =
				(_e[col0][zcoef] * b._e[col1][xcoef]) +
				(_e[col1][zcoef] * b._e[col1][ycoef]) +
				(_e[col2][zcoef] * b._e[col1][zcoef]);

			c._e[col2][xcoef] =
				(_e[col0][xcoef] * b._e[col2][xcoef]) +
				(_e[col1][xcoef] * b._e[col2][ycoef]) +
				(_e[col2][xcoef] * b._e[col2][zcoef]);

			c._e[col2][ycoef] =
				(_e[col0][ycoef] * b._e[col2][xcoef]) +
				(_e[col1][ycoef] * b._e[col2][ycoef]) +
				(_e[col2][ycoef] * b._e[col2][zcoef]);

			c._e[col2][zcoef] =
				(_e[col0][zcoef] * b._e[col2][xcoef]) +
				(_e[col1][zcoef] * b._e[col2][ycoef]) +
				(_e[col2][zcoef] * b._e[col2][zcoef]);

			*this = c;
		}

		void add(const Mat33& b) {
			_e[col0][xcoef] += b._e[col0][xcoef];
			_e[col0][ycoef] += b._e[col0][ycoef];
			_e[col0][zcoef] += b._e[col0][zcoef];
			_e[col1][xcoef] += b._e[col1][xcoef];
			_e[col1][ycoef] += b._e[col1][ycoef];
			_e[col1][zcoef] += b._e[col1][zcoef];
			_e[col2][xcoef] += b._e[col2][xcoef];
			_e[col2][ycoef] += b._e[col2][ycoef];
			_e[col2][zcoef] += b._e[col2][zcoef];
		}

		void sub(const Mat33& b) {
			_e[col0][xcoef] -= b._e[col0][xcoef];
			_e[col0][ycoef] -= b._e[col0][ycoef];
			_e[col0][zcoef] -= b._e[col0][zcoef];
			_e[col1][xcoef] -= b._e[col1][xcoef];
			_e[col1][ycoef] -= b._e[col1][ycoef];
			_e[col1][zcoef] -= b._e[col1][zcoef];
			_e[col2][xcoef] -= b._e[col2][xcoef];
			_e[col2][ycoef] -= b._e[col2][ycoef];
			_e[col2][zcoef] -= b._e[col2][zcoef];
		}

		void scale(T scalar){
			_e[col0][xcoef] *= scalar;
			_e[col0][ycoef] *= scalar;
			_e[col0][zcoef] *= scalar;
			_e[col1][xcoef] *= scalar;
			_e[col1][ycoef] *= scalar;
			_e[col1][zcoef] *= scalar;
			_e[col2][xcoef] *= scalar;
			_e[col2][ycoef] *= scalar;
			_e[col2][zcoef] *= scalar;
		}

		T at(int row, int col) const {
			assertFullBounds(row, col);
			return _e[col][row];
		}

		Vec3<T> atCol(int col) const {
			assertColBounds(col);
			return {_e[col][xcoef], _e[col][ycoef], _e[col][zcoef]};
		}

		Vec3<T> atRow(int row) const {
			assertRowBounds(row);
			return {_e[xcoef][row], _e[ycoef][row], _e[zcoef][row]};
		}

		bool operator==(){
		}

	private:
		static constexpr int rowCount {3};
		static constexpr int colCount {3};
		static constexpr int elementCount {rowCount * colCount};

		static constexpr int col0 {0};
		static constexpr int col1 {1};
		static constexpr int col2 {2};

		static constexpr int xcoef {0};
		static constexpr int ycoef {1};
		static constexpr int zcoef {2};

		void assertRowBounds(int row, int col) const {
			assert(0 <= row && row < rowCount);
		}

		void assertColBounds(int row, int col) const {
			assert(0 <= col && col < colCount);
		}

		void assertFullBounds(int row, int col) const {
			assert(0 <= row && row < rowCount);
			assert(0 <= col && col < colCount);
		}

		T _e[colCount][rowCount];
	};

	using Mat33f = Mat33<float>;
	using Mat33d = Mat33<double>;
}



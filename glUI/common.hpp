/**
 * @file common.hpp
 * @brief std::vector<>のかわりと、BoostのVector*n*fの代わりの定義
 * @author 20Niship Nishimiya Tadashi
 * @date 2021-01-17
 * @details 詳細な説明
 */


#pragma once

// #include <Windows.h>
// #include <windowsx.h>

#define MY_ASSERT(A) assert(A)

// #ifndef MAX
// #define MAX(a, b) ((a) > (b) ? (a) : (b))
// #endif
// #ifndef MIN
// #define MIN(a, b) ((a) < (b) ? (a) : (b))
// #endif

#define disp(A) (std::cout << #A << " = " << A << "\n")

// namespace glUI{

struct Vector2f;
struct Vector2d;
struct Vector3d;
struct Vector3f;


template<typename T>
struct uiVector
{    int                 Size;
    int                 Capacity;
    T*                  Data;

    // Provide standard typedefs but we don't use them ourselves.
    typedef T                   value_type;
    typedef value_type*         iterator;
    typedef const value_type*   const_iterator;

    // Constructors, destructor
    inline uiVector()                                       { Size = Capacity = 0; Data = NULL; }
    inline uiVector(const uiVector<T>& src)                 { Size = Capacity = 0; Data = NULL; operator=(src); }
    inline uiVector<T>& operator=(const uiVector<T>& src)   { clear(); resize(src.Size); memcpy(Data, src.Data, (size_t)Size * sizeof(T)); return *this; }
    inline ~uiVector()                                      { if (Data) free(Data); }

    inline bool         empty() const                       { return Size == 0; }
    inline int          size() const                        { return Size; }
    inline int          size_in_bytes() const               { return Size * (int)sizeof(T); }
    inline int          capacity() const                    { return Capacity; }
    inline T&           operator[](int i)                   { MY_ASSERT(i < Size); return Data[i]; }
    inline const T&     operator[](int i) const             { MY_ASSERT(i < Size); return Data[i]; }

    inline void         clear()                             { if (Data) { Size = Capacity = 0; free(Data); Data = NULL; } }
    inline T*           begin()                             { return Data; }
    inline const T*     begin() const                       { return Data; }
    inline T*           end()                               { return Data + Size; }
    inline const T*     end() const                         { return Data + Size; }
    inline T&           front()                             { MY_ASSERT(Size > 0); return Data[0]; }
    inline const T&     front() const                       { MY_ASSERT(Size > 0); return Data[0]; }
    inline T&           back()                              { MY_ASSERT(Size > 0); return Data[Size - 1]; }
    inline const T&     back() const                        { MY_ASSERT(Size > 0); return Data[Size - 1]; }
    inline void         swap(uiVector<T>& rhs)              { int rhs_size = rhs.Size; rhs.Size = Size; Size = rhs_size; int rhs_cap = rhs.Capacity; rhs.Capacity = Capacity; Capacity = rhs_cap; T* rhs_data = rhs.Data; rhs.Data = Data; Data = rhs_data; }

    inline int          _grow_capacity(int sz) const        { int new_capacity = Capacity ? (Capacity + Capacity/2) : 8; return new_capacity > sz ? new_capacity : sz; }
    inline void         resize(int new_size)                { if (new_size > Capacity) reserve(_grow_capacity(new_size)); Size = new_size; }
    inline void         resize(int new_size, const T& v)    { if (new_size > Capacity) reserve(_grow_capacity(new_size)); if (new_size > Size) for (int n = Size; n < new_size; n++) memcpy(&Data[n], &v, sizeof(v)); Size = new_size; }
    inline void         shrink(int new_size)                { MY_ASSERT(new_size <= Size); Size = new_size; } // Resize a vector to a smaller size, guaranteed not to cause a reallocation
    inline void         reserve(int new_capacity)           { if (new_capacity <= Capacity) return; T* new_data = (T*)malloc((size_t)new_capacity * sizeof(T)); if (Data) { memcpy(new_data, Data, (size_t)Size * sizeof(T)); free(Data); } Data = new_data; Capacity = new_capacity; }

    // NB: It is illegal to call push_back/push_front/insert with a reference pointing inside the uiVector data itself! e.g. v.push_back(v[10]) is forbidden.
    inline void         push_back(const T& v)               { if (Size == Capacity) reserve(_grow_capacity(Size + 100)); memcpy(&Data[Size], &v, sizeof(v)); Size++; }
    inline void         push_back(std::initializer_list<T> v){ for (auto it = v.begin(); it != v.end(); ++it) { push_back(*it); }}
    inline void         push_back(std::initializer_list<T> v, int n){ T *it; for(int i=0; i<n; i++){ for ( it = v.begin(); it != v.end(); ++it) { push_back(*it); }}}
    inline void         push_back(const T& v, int n)        { T *it; for(int i=0; i<n; i++){ push_back(v); }}
    inline void         pop_back()                          { MY_ASSERT(Size > 0); Size--; }
    inline void         push_front(const T& v)              { if (Size == 0) push_back(v); else insert(Data, v); }
    inline T*           erase(const T* it)                  { MY_ASSERT(it >= Data && it < Data+Size); const ptrdiff_t off = it - Data; memmove(Data + off, Data + off + 1, ((size_t)Size - (size_t)off - 1) * sizeof(T)); Size--; return Data + off; }
    inline T*           erase(const T* it, const T* it_last){ MY_ASSERT(it >= Data && it < Data+Size && it_last > it && it_last <= Data+Size); const ptrdiff_t count = it_last - it; const ptrdiff_t off = it - Data; memmove(Data + off, Data + off + count, ((size_t)Size - (size_t)off - count) * sizeof(T)); Size -= (int)count; return Data + off; }
    inline T*           erase_unsorted(const T* it)         { MY_ASSERT(it >= Data && it < Data+Size);  const ptrdiff_t off = it - Data; if (it < Data+Size-1) memcpy(Data + off, Data + Size - 1, sizeof(T)); Size--; return Data + off; }
    inline T*           insert(const T* it, const T& v)     { MY_ASSERT(it >= Data && it <= Data+Size); const ptrdiff_t off = it - Data; if (Size == Capacity) reserve(_grow_capacity(Size + 1)); if (off < (int)Size) memmove(Data + off + 1, Data + off, ((size_t)Size - (size_t)off) * sizeof(T)); memcpy(&Data[off], &v, sizeof(v)); Size++; return Data + off; }
    inline bool         contains(const T& v) const          { const T* data = Data;  const T* data_end = Data + Size; while (data < data_end) if (*data++ == v) return true; return false; }
    inline T*           find(const T& v)                    { T* data = Data;  const T* data_end = Data + Size; while (data < data_end) if (*data == v) break; else ++data; return data; }
    inline const T*     find(const T& v) const              { const T* data = Data;  const T* data_end = Data + Size; while (data < data_end) if (*data == v) break; else ++data; return data; }
    inline bool         find_erase(const T& v)              { const T* it = find(v); if (it < Data + Size) { erase(it); return true; } return false; }
    inline bool         find_erase_unsorted(const T& v)     { const T* it = find(v); if (it < Data + Size) { erase_unsorted(it); return true; } return false; }
    inline int          index_from_ptr(const T* it) const   { MY_ASSERT(it >= Data && it < Data + Size); const ptrdiff_t off = it - Data; return (int)off; }
    inline void         fill(const T& v)const               { for(int i=0; i<Size; i++){ Data[i] = v; } }
};


struct Vector2f {
	float x, y;
	Vector2f() { reset(); }
	Vector2f(float x_, float y_) { x = x_; y = y_;}
	void reset() { x = 0.0f; y = 0.0f; }
	Vector2f operator+=(const Vector2f& other) { x += other.x; y += other.y; return *this; }
	Vector2f operator-=(const Vector2f& other) { x -= other.x; y -= other.y; return *this; }
	Vector2f operator*=(const float& mul) { x = x * mul; y = y * mul; return *this; }
	Vector2f operator/=(const float& div) { x = x / div; y = y / div; return *this; }
	Vector2f operator-(const Vector2f& other) { return Vector2f(x - other.x, y - other.y); }
	Vector2f operator+(const Vector2f& other) { return Vector2f(x + other.x, y + other.y); }
	Vector2f operator/(const float k) { return Vector2f(x/k, y/k); }
	Vector2f operator*(const float k) { return Vector2f(x*k, y*k); }
	bool operator==(const Vector2f& other) const { return x==other.x && y==other.y; }
};
std::ostream& operator<<(std::ostream& os, const Vector2f& v);


struct Vector2d {
	int32_t x, y;
	Vector2d() { reset(); }
	Vector2d(int x_, int y_) { x = x_; y = y_; }
	void reset() { x = 0; y = 0; }
	Vector2d operator+=(const Vector2d& other) { x += other.x; y += other.y; return *this; }
	Vector2d operator-=(const Vector2f& other) { x -= other.x; y -= other.y; return *this; }
	Vector2d operator*=(const int& mul) { x = x * mul; y = y * mul; return *this; }
	Vector2d operator/=(const int& div) { x = x / div; y = y / div; return *this; }
	Vector2d operator-(const Vector2d& other) { x -= other.x; y -= other.y; return *this; }
	Vector2d operator+(const Vector2d& other) { return Vector2d(x + other.x, y + other.y); }

  	int operator[](int i){ MY_ASSERT(i < 2); if(i == 0){return x;}else{return y;}}
  	const int32_t& operator[](int i)const{ MY_ASSERT(i < 2); if(i == 0){return x;}else{return y;}}
	bool operator==(const Vector2d& other) const { return x==other.x && y==other.y; }
};


struct Vector2ud {
	uint16_t x, y;
	Vector2ud() { reset(); }
	Vector2ud(uint16_t x_, uint16_t y_) { x = x_; y = y_; }
	void reset() { x = 0; y = 0; }

	Vector2ud operator+=(const Vector2ud& other) { x += other.x; y += other.y; return *this; }
	Vector2ud operator-=(const Vector2ud& other) { x -= other.x; y -= other.y; return *this; }
	Vector2ud operator*=(const int& mul) { x = x * mul; y = y * mul; return *this; }
	Vector2ud operator/=(const int& div) { x = x / div; y = y / div; return *this; }
	Vector2ud operator-(const Vector2ud& other) { x -= other.x; y -= other.y; return *this; }
	Vector2ud operator+(const Vector2ud& other) { return Vector2ud(x + other.x, y + other.y); }

  	uint16_t operator[](int i){ MY_ASSERT(i < 2); if(i == 0){return x;}else{return y;}}
  	const uint16_t& operator[](int i)const{ MY_ASSERT(i < 2); if(i == 0){return x;}else{return y;}}
	bool operator==(const Vector2ud& other) const { return x==other.x && y==other.y; }

};


struct Vector3f {
	float x, y, z;
	Vector3f() { reset(); }
	Vector3f(float x_, float y_, float z_) { x = x_; y = y_; z = z_; }
	void reset() { x = 0; y = 0; z = 0; }
	float operator[](int i) {if (i == 0) { return x; }else if (i == 1) { return y; }else return z;}
};

struct Vector4d{
	int x, y, z, q;
	Vector4d() { reset(); }
	Vector4d(int x_, int y_, int z_, int q_) { x = x_; y = y_; z = z_; q = q_; }
	void reset() { x = 0; y = 0; z = 0; q = 0;  }
};


struct Vector4f{
	int x, y, z, q;
	Vector4f() { reset(); }
	Vector4f(int x_, int y_, int z_, int q_) { x = x_; y = y_; z = z_; q = q_; }
	void reset() { x = 0; y = 0; z = 0; q = 0; }
};


struct uiRect {
	uiRect() {
		posX = 0;
		posY = 0;
		width = 0;
		height = 0;
	}

	uiRect(float posX_, float posY_, float width_, float height_){
		posX = posX_;
		posY = posY_;
		width = width_;
		height = height_;
	}

	float posX, posY, width, height;

	inline float right() { return posX + width; }
	inline float bottom() { return posY + height; }

	inline bool isContains(int x, int y) {
		return (x >= posX) && (x <= posX + width) && (y >= posY) && (y <= posY + height);
	}

	inline bool isNoContains(uiRect outside) {
		return (outside.posX > right()) || (outside.right() < posX) || (outside.posY > bottom()) || (outside.bottom() < posY);
	}

	inline Vector2f getPos() { return Vector2f(posX, posY); }
	inline Vector2f getSize() { return Vector2f(width, height); }
};

template<typename T> static inline T uiMin(T lhs, T rhs)                    { return lhs < rhs ? lhs : rhs; }
template<typename T> static inline T uiMax(T lhs, T rhs)                    { return lhs >= rhs ? lhs : rhs; }
template<typename T> static inline T uiClamp(T v, T mn, T mx)               { return (v < mn) ? mn : (v > mx) ? mx : v; }
inline Vector2f uiMin(const Vector2f& lhs, const Vector2f& rhs)             { return Vector2f(lhs.x < rhs.x ? lhs.x : rhs.x, lhs.y < rhs.y ? lhs.y : rhs.y); }
inline Vector2f uiMax(const Vector2f& lhs, const Vector2f& rhs)               { return Vector2f(lhs.x >= rhs.x ? lhs.x : rhs.x, lhs.y >= rhs.y ? lhs.y : rhs.y); }
inline Vector2f uiClamp(const Vector2f& v, const Vector2f& mn, Vector2f mx) { return Vector2f((v.x < mn.x) ? mn.x : (v.x > mx.x) ? mx.x : v.x, (v.y < mn.y) ? mn.y : (v.y > mx.y) ? mx.y : v.y); }

// }
#pragma once

// -------------------------------------------------------------------------- \\
// ------------------------------- Index ------------------------------------ \\
// -------------------------------------------------------------------------- \\

struct Index { int x, y; };
bool operator==(Index a, Index b);

// -------------------------------------------------------------------------- \\
// ------------------------------- Matrix ----------------------------------- \\
// -------------------------------------------------------------------------- \\

template<typename T, size_t width, size_t height = width>
class Matrix
{
public:
	T& operator [](Index i) { return Get(i); }
	T&         Get(Index i) { return m_Data[Convert(i)]; }
	
	bool Valid(Index i) { return i.x >= 0 && i.x < width && i.y >= 0 && i.y < height; }
	int  Convert(Index i) { return i.x + width * i.y; }

	template<typename F>
	void ForEach(F f)
	{
		for (int _y = 0; _y < height; _y++)
			for (int _x = 0; _x < width; _x++)
				f({ _x, _y }, *(m_Data + _x + width * _y));
	}

	template<typename F>
	void ForNeighbours(Index i, F f)
	{
		i.x--;
		if (Valid(i)) f(i, Get(i));
		i.x++, i.y--;
		if (Valid(i)) f(i, Get(i));
		i.x++, i.y++;
		if (Valid(i)) f(i, Get(i));
		i.x--, i.y++;
		if (Valid(i)) f(i, Get(i));
	}

	Matrix<T, width, height> Copy()
	{
		Matrix<T, width, height> _t;
		ForEach([&](Index i, T& b)
			{ _t[i] = b; }
		);

		return _t;
	}

private:
	T m_Data[width * height];
};
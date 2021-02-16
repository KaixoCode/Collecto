#pragma once
#include "pch.hpp"
#include "collecto/ball.hpp"

// -------------------------------------------------------------------------- \\
// ------------------------------- Enums ------------------------------------ \\
// -------------------------------------------------------------------------- \\

// Directions:
//
//    -2
//  1  0 -1
//     2
//

enum class Direction { px = -1, py = -2, nx = 1, ny = 2 };
enum class Axis { x = 1, y = 2 };

// -------------------------------------------------------------------------- \\
// ------------------------------- Score ------------------------------------ \\
// -------------------------------------------------------------------------- \\

struct Score
{
	Score()
	{ std::fill(std::begin(score), std::end(score), 0); }

	int score[COLORS];

	int TotalScore() { return std::accumulate(std::begin(score), std::end(score), 0); }

	operator bool()  { return TotalScore() > 0; }
	int& operator[](Ball::Value b) { return score[Ball::Index(b)]; }
};

// -------------------------------------------------------------------------- \\
// -------------------------------- Move ------------------------------------ \\
// -------------------------------------------------------------------------- \\

struct Move
{
	int index = -1, direction = 0;

	Axis Axis() { return std::abs(direction) == 1 ? Axis::x : Axis::y; }
	bool Sign() { return direction < 0; }

	template<typename T>
	static void ForEach(T t)
	{
		for (int d = -2; d < 3; d == -1 ? d += 2 : d++)
			for (int i = 0; i < COLORS; i++)
				t(Move{ i, d });
	}

	bool operator==(Move o) { return o.index == index && o.direction == direction; }
};

// -------------------------------------------------------------------------- \\
// ------------------------------- Board ------------------------------------ \\
// -------------------------------------------------------------------------- \\

class Board
{
public:
	const static inline unsigned int SIZE = COLORS;
	const static inline Index CENTER = {SIZE / 2, SIZE / 2};
	static Score RemoveGroups(Matrix<Ball, SIZE>& _matrix);
	static Score MoveOnMatrix(Move m, Matrix<Ball, SIZE>& _matrix);
	
	Board();

	Score TryMove(Move m);
	Score DoMove(Move m) 
	{ 
		Score _s = MoveOnMatrix(m, m_Matrix);
		m_InDoubleMove = !_s;

		return _s;
	}
	void  Reset() { InitBoard(); }
	bool  DoubleMove() { return m_InDoubleMove; }

	template<typename F>
	void ForEach(F f) { m_Matrix.ForEach(f); }

	template<typename F>
	void ForNeighbours(Index i, F f) { m_Matrix.ForNeighbours(i, f); }

private:
	Matrix<Ball, SIZE> m_Matrix;
	bool m_InDoubleMove = false;

	void InitBoard();
};

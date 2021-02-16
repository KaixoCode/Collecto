#include "collecto/board.hpp"

// -------------------------------------------------------------------------- \\
// ------------------------------- Board ------------------------------------ \\
// -------------------------------------------------------------------------- \\

Board::Board()
{
	InitBoard();
}

void Board::InitBoard()
{
	bool _done = true;
	do
	{
		_done = true;
		int _avlbl[COLORS];
		for (int i = 0; i < COLORS; i++)
			_avlbl[i] = i == 0 ? 0 : BALLS;

		// Reset all values to 'none'
		ForEach([&](Index i, Ball& b)
			{ b = Ball::none; }
		);

		// Attempt to create a board
		ForEach([&](Index i, Ball& b)
			{
				if (i == CENTER)
					return;

				// Check which color the neighbours are
				bool _nb[COLORS];
				for (int i = 0; i < COLORS; i++)
					_nb[i] = 0;

				ForNeighbours(i, [&](Index i, Ball& b)
					{ _nb[b.Index()] = true; }
				);

				// Make sure it is possible to find a ball
				bool _possible = false;
				for (int i = 0; i < COLORS; i++)
					if (!_nb[i] && _avlbl[i])
						_possible = true;

				// If it is not possible quit and restart.
				if (!_possible)
				{
					_done = false;
					return;
				}

				// Make sure random color is still available and not a neighbour
				while (!_avlbl[b.Index()] || _nb[b.Index()])
				{
					b = Ball::Random();
					b.Pos(i.x, i.y);
				}
				_avlbl[b.Index()]--;
			}
		);
	} while (!_done);
}

Score Board::TryMove(Move m)
{
	// Make a copy of the board matrix to try the move on
	auto _copy = m_Matrix.Copy();

	// Try the move and check if it results in a score
	Score _score = MoveOnMatrix(m, _copy);
	if (_score)
		return _score;

	// If the previous move was a double, no double move now
	if (m_InDoubleMove)
		return Score{};

	// First make sure there are no other possibilities for single moves
	bool _otherPossible = false;
	Move::ForEach([&](Move v)
		{
			if (_otherPossible)
				return;

			// Make a copy of the board matrix to try the move on
			auto _copy = m_Matrix.Copy();

			// Try the move and check if it results in a score
			Score _score = MoveOnMatrix(v, _copy);
			if (_score)
				_otherPossible = true;
		}
	);

	// If another single move is possible, this double aint
	if (_otherPossible)
		return Score{};

	// Check if a double move is available
	Score _biggest;
	Move::ForEach([&](Move v)
		{
			auto _c = _copy.Copy();
			Score _s = MoveOnMatrix(v, _c);

			if (_s.TotalScore() > _biggest.TotalScore())
				_biggest = _s;
		}
	);
	
	// Return biggest possible score for this double move
	return _biggest;
}

Score Board::RemoveGroups(Matrix<Ball, SIZE>& _matrix)
{
	Score _score;
	Matrix<Ball, SIZE> _temp;

	_matrix.ForEach([&](Index i, Ball& b)
		{
			if (b == Ball::none)
				return;

			bool _found = false;
			_matrix.ForNeighbours(i, [&](Index i1, Ball& b1)
				{
					if (b1 == b)
						_found = true;
				}
			);

			if (_found)
			{
				_temp[i] = b;
				_score[b]++;
			}
		}
	);

	_temp.ForEach([&](Index i, Ball& b)
		{
			if (b == Ball::none)
				return;

			_matrix[i].Explode();
		}
	);

	return _score;
}

Score Board::MoveOnMatrix(Move m, Matrix<Ball, SIZE>& _matrix)
{
	int _clrs[COLORS];
	for (int i = 0; i < COLORS; i++)
		_clrs[i] = 0;

	// Determine which direction and axis the move is
	bool _n = m.Sign();
	bool _xAxis = m.Axis() == Axis::x;
	bool _yAxis = m.Axis() == Axis::y;

	// Collect a list of all the colors in order in that row/col
	for (int i = 0; i < COLORS; i++)
	{
		int _x = _xAxis ? i : m.index;
		int _y = _yAxis ? i : m.index;
		_clrs[i] = _matrix[{ _x, _y }].Index();
		_matrix[{ _x, _y }] = Ball::none; // Also clear that row/col
	}

	// Then append those colors to the same row/col without gaps
	int _index = 0;
	for (int i = 0; i < COLORS; i++)
	{
		int _v = _n ? (COLORS) - i - 1 : i;
		if (_clrs[_v] != 0)
		{
			int _q = _n ? (COLORS) - _index - 1 : _index;
			int _x = _xAxis ? _q : m.index;
			int _y = _yAxis ? _q : m.index;
			_matrix[{ _x, _y }] = Ball::Index(_clrs[_v]);
			_matrix[{ _x, _y }].Pos(_xAxis ? _v : _x, _yAxis ? _v : _y);
			_matrix[{ _x, _y }].PosMorph(_x, _y);
			_index++;
		}
	}

	return RemoveGroups(_matrix);
}
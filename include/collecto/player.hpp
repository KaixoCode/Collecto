#pragma once
#include "collecto/board.hpp"

using MoveCallback = std::function<void(Move)>;

// -------------------------------------------------------------------------- \\
// ----------------------------- Player Base -------------------------------- \\
// -------------------------------------------------------------------------- \\

class Player
{
public:

	static inline int DELAY = 700; //ms

	Player(MoveCallback c) { callback = c; }

	virtual void RequestMove(Board& b) = 0;

	void AddScore(Score& _score)
	{
		for (int i = 0; i < COLORS; i++)
			score[Ball::Index(i)] += _score[Ball::Index(i)];
	}

	void Reset()
	{

		score = Score{};
	}

	int GetScore()
	{
		int _score = 0;
		for (int i = 0; i < COLORS; i++)
			_score += std::floor(score[Ball::Index(i)] / 3.0);
		return _score;
	}

	Score score;
protected:
	MoveCallback callback;
};

// -------------------------------------------------------------------------- \\
// ------------------------- Local Human Player ----------------------------- \\
// -------------------------------------------------------------------------- \\

class LocalPlayer : public Player
{
public:
	using Player::Player;

	void RequestMove(Board& b) override { m_Turn = true; }
	void DoMove(Move m) { m_Turn = false; callback(m); }
	bool Turn() { return m_Turn; }

private:
	bool m_Turn = false;
};

// -------------------------------------------------------------------------- \\
// --------------------------- Computer Player ------------------------------ \\
// -------------------------------------------------------------------------- \\

class Later
{
public:
	Later()
	{}

	template <class callable>
	Later(int after, callable&& f)
	{
		std::thread([after, f]() 
			{
				std::this_thread::sleep_for(std::chrono::milliseconds(after));
				f();
			}
		).detach();
	}
};

template<Move(*Strategy)(Board&)>
class ComputerPlayer : public Player
{
public:
	using Player::Player;

	void RequestMove(Board& b) override 
	{
		static Later later_test;
		later_test = Later{ DELAY, [&]()
			{
				Move v = Strategy(b);
				callback(v);
			}
		};
	}

	static inline std::thread _t;
};

// -------------------------------------------------------------------------- \\
// ------------------------------ Strategies -------------------------------- \\
// -------------------------------------------------------------------------- \\

namespace Strategy
{
	static inline Move NaiveFirst(Board& board)
	{
		Move _m{ -1, 0 };
		bool _found = false;
		Move::ForEach([&](Move v)
			{
				if (_found)
					return;

				if (board.TryMove(v))
				{
					_found = true;
					_m = v;
				}
			}
		);

		return _m;
	}

	static inline Move Naive(Board& board) 
	{
		std::vector<Move> _moves;
		Move::ForEach([&](Move v) 
			{
				if (board.TryMove(v))
					_moves.push_back(v);
			}
		);
		if (_moves.size() == 0)
			return { -1, 0 };

		int _r = std::rand() % _moves.size();
		return _moves[_r];
	}

	static inline Move Greedy(Board& board)
	{
		Move _best{ -1, 0 };
		Score _high;
		Move::ForEach([&](Move v)
			{
				Score _s = board.TryMove(v);
				if (_s.TotalScore() > _high.TotalScore())
				{ _high = _s, _best = v; }
			}
		);

		return _best;
	}

	struct Tree
	{
		Tree(Matrix<Ball, COLORS>& b)
			: current(std::make_unique<Matrix<Ball, COLORS>>(b))
		{
			Move::ForEach([&](Move v)
				{
					auto& _c = current->Copy();
					Score _s;
					if (_s = Board::MoveOnMatrix(v, _c))
						nextStates.emplace_back(_c);
				}
			);
		}

		Score score;
		std::vector<Tree> nextStates;
		std::unique_ptr<Matrix<Ball, COLORS>> current;
	};

	static inline Move Smart(Board& board)
	{
		Move _best{ -1, 0 };
		Score _high;
		Move::ForEach([&](Move v)
			{
				Score _s = board.TryMove(v);
				if (_s.TotalScore() > _high.TotalScore())
				{
					_high = _s, _best = v;
				}
			}
		);

		return _best;
	}
};
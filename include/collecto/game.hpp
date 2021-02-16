#pragma once
#include "pch.hpp"
#include "collecto/board.hpp"
#include "collecto/player.hpp"

// -------------------------------------------------------------------------- \\
// ----------------------------- Collecto ----------------------------------- \\
// -------------------------------------------------------------------------- \\

class Collecto
{
public:
	int players = -1;

	Collecto()
	{}

	template<typename P, typename ...PT>
	void InitPlayers()
	{
		if (players == -1)
			players = sizeof...(PT) + 1;
		
		m_Players.emplace_back(std::make_unique<P>(Turn(players - sizeof...(PT) - 1)));
		if constexpr (sizeof...(PT)) InitPlayers<PT...>();
		else StartGame();
	}

	Board& Board()
	{
		return m_Board;
	}

	Player& GetTurn()
	{
		return *m_Players[m_Turn];
	}

	template<typename T>
	void ForPlayers(T t)
	{
		int i = 0;
		for (auto& _p : m_Players)
			t(*_p, i++);
	}

	void Restart() 
	{
		Board().Reset();
		ForPlayers([](Player& p, int nmr) { p.Reset(); });
		StartGame();
	}

protected:
	::Board m_Board;
	std::vector<std::unique_ptr<Player>> m_Players;
	int m_Turn = 0;

	void StartGame()
	{
		// Start the game by requesting player 1 to do a move
		m_Players[m_Turn]->RequestMove(Board());
	}

	auto Turn(int myturn)
	{
		return [&, myturn](Move v)
		{
			// Do nothing if not myturn
			if (m_Turn != myturn)
				return;

			// Gameover
			if (v.index == -1)
				return;

			// Check for valid move
			if (Board().TryMove(v))
			{
				// Do move if valid
				Score _s = Board().DoMove(v);
				m_Players[m_Turn]->AddScore(_s);
				if (Board().DoubleMove())
				{
					// If it was a double move, don't switch to next player
					m_Players[m_Turn]->RequestMove(Board());
					return;
				}

				// Request move from next player
				m_Turn = (m_Turn + 1) % players;
				m_Players[m_Turn]->RequestMove(Board());
			} 
			else
			{
				// If was invalid request another move
				m_Players[m_Turn]->RequestMove(Board());
			}
		};
	}
};

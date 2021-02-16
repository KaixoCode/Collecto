#pragma once
#include "pch.hpp"

#define COLORS 7
#define BALLS (((COLORS * COLORS) - 1) / (COLORS - 1))

struct Pos
{
	double x = 0, y = 0;
};

// -------------------------------------------------------------------------- \\
// -------------------------------- Ball ------------------------------------ \\
// -------------------------------------------------------------------------- \\

class Ball
{
public:
	enum Value
	{
		none = 0x0,
		red = 0xeb5959,
		orange = 0xfaa255,
		yellow = 0xf0c348,
		lime = 0x72d950,
		green = 0x50d989,
		magenta = 0xc250d9,
		dblue = 0x5075d9,
		blue = 0x66c6f2,
		purple = 0x9550d9,
		pink = 0xf5629a,
		c1 = 0x101010,
		c2 = 0x202020,
		c3 = 0x303030,
		c4 = 0x404040,
		c5 = 0x505050,
		c6 = 0x606060,
		c7 = 0x707070,
		c8 = 0x808080,
		c9 = 0x909090,
		c10 = 0xa0a0a0,
		c11 = 0xb0b0b0,
		c12 = 0xc0b0c0,
		c13 = 0xd0d0d0,
		c14 = 0xe0e0e0,
		c15 = 0xf0f0f0,
	};

	static Ball Random() { return Index(RandomIndex()); }
	static Ball Index(int i);
	static int  Index(Ball::Value v);

	Ball() : m_Value(Value::none)
	{}

	Ball(Value v) : m_Value(v)
	{}

	operator Value() { return GetValue(); }
	void operator =(Value v) { m_Value = v; }

	Value GetValue() { return m_Value; }
	int Index() { return Index(m_Value); }
	
	 // ---------------------------------------- \\ 
	// -- Stuff used by the GUI for animations -- \\ 
	void  PosMorph(double x, double y) { m_Target = { x, y }; }
	void  Pos(double x, double y) { m_Target = m_Pos = { x, y }; }
	
	::Pos Pos()
	{ 
		m_Pos = { 
			m_Speed * m_Target.x + (1 - m_Speed) * m_Pos.x,
			m_Speed * m_Target.y + (1 - m_Speed) * m_Pos.y
		};
		return m_Pos;
	}

	Value VisibleColor()
	{
		if (m_Timer >= 0) 
		{ 
			m_Timer--; 
			return m_Prev;
		}
		return m_Value;
	}

	void Explode() { m_Prev = m_Value; m_Value = Ball::none; m_Timer = 12; } // Determines delay till death
	bool Dead() { return m_Timer == 1; }
	// ------------------------------------------ \\ 

private:
	Value m_Value, m_Prev;

	 // ---------------------------------------- \\ 
    // -- Stuff used by the GUI for animations -- \\ 
	::Pos m_Pos, m_Target;
	double m_Speed = 0.3; // Speed of move animation
	int m_Timer = -1; // Timer to delay removing of ball
	// ------------------------------------------ \\ 

	static int RandomIndex();
};
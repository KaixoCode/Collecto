#include "collecto/ball.hpp"


// -------------------------------------------------------------------------- \\
// -------------------------------- Ball ------------------------------------ \\
// -------------------------------------------------------------------------- \\

Ball Ball::Index(int i)
{
	switch (i)
	{
	case 0: return Ball::none;
	case 1: return Ball::blue;
	case 2: return Ball::yellow;
	case 3: return Ball::red;
	case 4: return Ball::orange;
	case 5: return Ball::pink;
	case 6: return Ball::green;
	case 7: return Ball::purple;
	case 8: return Ball::dblue;
	case 9: return Ball::lime;
	case 10:return Ball::magenta;
	case 11:return Ball::c1;
	case 12:return Ball::c2;
	case 13:return Ball::c3;
	case 14:return Ball::c4;
	case 15:return Ball::c5;
	case 16:return Ball::c6;
	case 17:return Ball::c7;
	case 18:return Ball::c8;
	case 19:return Ball::c9;
	case 20:return Ball::c10;
	case 21:return Ball::c11;
	case 22:return Ball::c12;
	case 23:return Ball::c13;
	case 24:return Ball::c14;
	case 25:return Ball::c15;
	}
};

int Ball::Index(Ball::Value v)
{
	switch (v)
	{
	case Ball::none:   return 0;
	case Ball::blue:   return 1;
	case Ball::yellow: return 2;
	case Ball::red:    return 3;
	case Ball::orange: return 4;
	case Ball::pink:   return 5;
	case Ball::green:  return 6;
	case Ball::purple: return 7;
	case Ball::dblue:  return 8;
	case Ball::lime:   return 9;
	case Ball::magenta:return 10;
	case Ball::c1:     return 11;
	case Ball::c2:     return 12;
	case Ball::c3:     return 13;
	case Ball::c4:     return 14;
	case Ball::c5:     return 15;
	case Ball::c6:     return 16;
	case Ball::c7:     return 17;
	case Ball::c8:     return 18;
	case Ball::c9:     return 19;
	case Ball::c10:    return 20;
	case Ball::c11:    return 21;
	case Ball::c12:    return 22;
	case Ball::c13:    return 23;
	case Ball::c14:    return 24;
	case Ball::c15:    return 25;
	}
};

int Ball::RandomIndex()
{
	// Set random seed the first time
	static bool _i = true;
	if (_i) {
		_i = false;
		std::srand(std::time(nullptr));
	}

	return std::rand() % (COLORS-1) + 1;
}

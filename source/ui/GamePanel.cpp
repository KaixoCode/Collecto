#include "ui/GamePanel.hpp"

// -------------------------------------------------------------------------- \\
// ------------------------------ Particle ---------------------------------- \\
// -------------------------------------------------------------------------- \\

Particle::Particle(double x, double y, Ball::Value color)
	: m_Color(color), m_X(x), m_Y(y)
{
	m_DX = 9 * (1 - 0.002 * (std::rand() % 1000));
	m_DY = 9 * (1 - 0.002 * (std::rand() % 1000));
	m_DR = 20 * (1 - 0.002 * (std::rand() % 1000));
	m_R = TWO_PI * 0.001 * (std::rand() % 1000);
	m_W = 10 + 5 * (0.001 * (std::rand() % 1000));
	m_H = 10 + 5 * (0.001 * (std::rand() % 1000));
	m_Shape = std::rand() % 2;
}

void Particle::Render(CommandCollection& d)
{
	m_X += m_DX;
	m_Y += m_DY;
	m_R += m_DR;
	Color c = HexToRgb(m_Color);
	c.a = min(pow(m_Lifetime, 1.5) * 2, 255.0) * 0.5;
	d.Command<Graphics::Fill>(c);
	if (m_Shape == 0)
		d.Command<Graphics::Quad>(Vec4<int>{(int)m_X, (int)m_Y, (int)m_W, (int)m_H}, (float)m_R);
	else if (m_Shape == 1)
		d.Command<Graphics::Triangle>(Vec4<int>{(int)m_X, (int)m_Y, (int)m_W, (int)m_H}, (float)m_R);
	m_Lifetime--;
}

// -------------------------------------------------------------------------- \\
// ------------------------------ GamePanel --------------------------------- \\
// -------------------------------------------------------------------------- \\

GamePanel::GamePanel()
	: Panel()
{
	// ComputerPlayer<Strategy::Greedy>
	m_Listener += [this](Event::MouseMoved& m)
	{ m_MouseX = m.x, m_MouseY = m.y; };

	m_Listener += [this](Event::MouseDragged& m)
	{ m_DMouseX = m.x, m_DMouseY = m.y; };

	m_Listener += [this](Event::MousePressed& m)
	{
		m_DMouseX = m.x;
		m_DMouseY = m.y;
		m_MousePressed = true;
	};

	m_Listener += [this](Event::MouseReleased& m)
	{
		m_MousePressed = false;
		if (m_Dir)
		{
			Player& _p = Game().GetTurn();
			LocalPlayer* _l = nullptr;
			if (_l = dynamic_cast<LocalPlayer*>(&_p))
				if (_l->Turn())
					_l->DoMove({ m_Index, m_Dir });
			
			m_Dir = 0;
		}
	};
}

void GamePanel::Render(CommandCollection& d)
{
	Panel::Render(d);

	int _p = 15, // Padding
		_m = 80, // Margin
		_s = min((Height() - 2 * _m - (COLORS - 1) * _p), 720) / COLORS, // Size
		_x = _m + (Width() - 2 * _m) / 2 - (_s + _p) * (COLORS / 2.0) + _s / 2,
		_y = _m + (Height() - 2 * _m) / 2 - (_s + _p) * (COLORS / 2.0) + _s / 2;
	
	int _mCol = std::floor((m_MouseX - _x + _s / 2 + _p / 2) / (double)(_s + _p)),
	    _mRow = std::floor((m_MouseY - _y + _s / 2 + _p / 2) / (double)(_s + _p));
	
	int _dmCol = std::floor((m_DMouseX - _x + _s / 2 + _p / 2) / (double)(_s + _p)),
		_dmRow = std::floor((m_DMouseY - _y + _s / 2 + _p / 2) / (double)(_s + _p));

	double _dx = m_MouseX - m_DMouseX;
	double _dy = m_MouseY - m_DMouseY;
	
	if (m_MousePressed && !(_mCol < 0 || _mCol > (COLORS - 1) || _mRow < 0 || _mRow > (COLORS - 1)))
		if (_mCol != _dmCol && std::abs(_dx) > std::abs(_dy))
			m_Dir = 1 - 2 * (_dx < 0), m_Index = _mRow;
		else if (_mRow != _dmRow)
			m_Dir = 2 - 4 * (_dy < 0), m_Index = _mCol;
		else m_Dir = 0, m_Index = -1;
	else m_Dir = 0, m_Index = -1;

	if (!(_mCol < 0 || _mCol > (COLORS - 1) || _mRow < 0 || _mRow > (COLORS - 1)))
	{
		int _xmp = X() + _x + _mCol * (_s + _p) - _s / 2 - _p / 2,
			_ymp = Y() + _y + _mRow * (_s + _p) - _s / 2 - _p / 2,
			_xrp = X() + _x - _s / 2 - _m,
			_yrp = Y() + _y - _s / 2 - _m;
		
		d.Command<Graphics::Fill>(Color{ 35, 35, 35, 255 });
		if (std::abs(m_Dir) != 1) d.Command<Graphics::Quad>(Vec4<int>{_xmp, _yrp, _s + _p, _s * COLORS + _p * (COLORS - 1) + 2 * _m});
		if (std::abs(m_Dir) != 2) d.Command<Graphics::Quad>(Vec4<int>{_xrp, _ymp, _s * COLORS + _p * (COLORS - 1) + 2 * _m, _s + _p});
	}

	m_Game.Board().ForEach([&](Index i, ::Ball& b)
		{
			int _xp = X() + _x + i.x * (_s + _p) - _s / 2,
				_yp = Y() + _y + i.y * (_s + _p) - _s / 2,
				_ip = 3 * _s / 8;
			
			d.Command<Graphics::Fill>(Color{ 50, 50, 50, 255 });
			d.Command<Graphics::Quad>(Vec4<int>{_xp + _ip / 2, _yp + _ip / 2, _s - _ip, _s - _ip});
		}
	);

	m_Game.Board().ForEach([&](Index i, ::Ball& b)
		{
			Ball::Value _color = b.VisibleColor();
			if (_color == Ball::none)
				return;

			Pos _pos = b.Pos();
			int _xp = X() + _x + _pos.x * (_s + _p) - _s / 2,
				_yp = Y() + _y + _pos.y * (_s + _p) - _s / 2,
				_ip = 3 * _s / 8;

			if (b.Dead())
				for (int i = 0; i < 10; i++)
					m_Particles.push_back(Particle{ (double) _xp + _s / 2, (double)_yp + _s / 2, _color });

			DrawBall(d, b.VisibleColor(), _xp, _yp, _s, _ip);

			if (m_Dir && ((std::abs(m_Dir) == 2 && i.x == _mCol) || (std::abs(m_Dir) == 1 && i.y == _mRow)))
			{
				float _angle = std::abs(m_Dir) * 90 + (m_Dir < 0) * 180 + 90;
				int _xmp = X() + _x + _pos.x * (_s + _p),
					_ymp = Y() + _y + _pos.y * (_s + _p);

				d.Command<Graphics::Fill>(Color{ 255, 255, 255, 150 });
				d.Command<Graphics::Triangle>(Vec4<int>{_xmp, _ymp, _s / 2, _s / 2}, _angle);
			}
		}
	);

	static std::string str[2][2];
	str[0][0].reserve(10);
	str[0][1].reserve(10);
	str[1][0].reserve(10);
	str[1][1].reserve(10);
	
	m_Game.ForPlayers([&](Player& p, int nmr)
		{
			int _ytp = Height() - _m * 2;
			int _ts = _s / 2.5;
			int _tp = _p / 2.5;
			int _ip = 3 * _ts / 8;
			int _tm = 40;
			int _xtp = Width() / 2 - (_s + _p) * (COLORS / 2.0) - _ts * BALLS - _s - _tp * BALLS;
			if (nmr == 1)
				_xtp = Width() / 2 + (_s + _p) * (COLORS / 2.0) + _ts * (BALLS + 1) + _s + _tp * BALLS;
			str[nmr][0] = "Player " + std::to_string(nmr + 1);
			str[nmr][1] = "Score " + std::to_string(p.GetScore());
			if (nmr == 0)
				d.Command<Graphics::TextAlign>(Align::LEFT, Align::CENTER);
			else
				d.Command<Graphics::TextAlign>(Align::RIGHT, Align::CENTER);

			d.Command<Graphics::Fill>(Color{ 255, 255, 255, 255 });
			d.Command<Graphics::Font>(Graphics::Fonts::Gidole, _s * 0.6f);
			d.Command<Graphics::Text>(&str[nmr][0], _xtp, _ytp -= _s * 0.8f);
			d.Command<Graphics::Font>(Graphics::Fonts::Gidole, _s * 0.4f);
			d.Command<Graphics::Text>(&str[nmr][1], _xtp, _ytp -= _s * 0.6f);

			_ytp -= _s * 0.8;
			for (int i = 1; i < COLORS; i++)
			{
				d.Command<Graphics::Fill>(HexToRgb(Ball::Index(i)));
				d.Command<Graphics::Quad>(Vec4<int>{_xtp, _ytp, 2, _ts});


				for (int x = 0; x < p.score[Ball::Index(i)]; x++)
					DrawBall(d, Ball::Index(i), _xtp + (nmr == 1 ? -1 : 1) * (x * (_ts + _tp) + _tp) - (nmr == 1 ? _ts : 0), _ytp, _ts, _ip);

				_ytp -= _ts + _tp;
			}
			
		}
	);

	while (m_Particles.size() > 1000)
		m_Particles.erase(m_Particles.begin());

	for (int i = m_Particles.size() - 1; i >= 0; i--)
	{
		m_Particles[i].Size(_s * 0.5, _s * 0.5);
		m_Particles[i].Render(d);
		if (m_Particles[i].Dead())
			m_Particles.erase(m_Particles.begin() + i);
	}
}

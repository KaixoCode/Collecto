#pragma once
#include "pch.hpp"
#include "collecto/game.hpp"
#include "collecto/player.hpp"

static inline Color HexToRgb(int hexValue)
{
	Color rgbColor;
	rgbColor.r = ((hexValue >> 16) & 0xFF);  // Extract the RR byte
	rgbColor.g = ((hexValue >> 8) & 0xFF);   // Extract the GG byte
	rgbColor.b = ((hexValue) & 0xFF);        // Extract the BB byte
	rgbColor.a = 255;

	return rgbColor;
}

// -------------------------------------------------------------------------- \\
// ------------------------------ Particle ---------------------------------- \\
// -------------------------------------------------------------------------- \\

class Particle
{
public:
	Particle(double x, double y, Ball::Value color);

	void Render(CommandCollection& d);
	bool Dead() { return m_Lifetime < 0; }
	void Size(double w, double h) { m_W = w; m_H = h; }

private:
	double m_X, 
		m_Y, 
		m_R, 
		m_DX, 
		m_DY, 
		m_DR, 
		m_W, 
		m_H;

	int m_Shape = 0;

	int m_Lifetime = 50;

	Ball::Value m_Color;
};

// -------------------------------------------------------------------------- \\
// ------------------------------ GamePanel --------------------------------- \\
// -------------------------------------------------------------------------- \\

class GamePanel : public Panel
{
public:
	GamePanel();

	void Render(CommandCollection& d) override;

	Collecto& Game() { return m_Game; }

private:
	Collecto m_Game;
	std::vector<Particle> m_Particles;

	double m_MouseX = 0, 
		m_MouseY = 0,
		m_DMouseX = 0, 
		m_DMouseY = 0;

	int m_Dir = 0, 
		m_Index = -1;

	bool m_MousePressed = false;

	void DrawBall(CommandCollection& d, Ball::Value c, int x, int y, int s, int p)
	{
		d.Command<Graphics::Fill>(Color{ 0, 0, 0, 100 });
		d.Command<Graphics::Quad>(Vec4<int>{x, y, s, s});
		d.Command<Graphics::Fill>(HexToRgb(c));
		d.Command<Graphics::Quad>(Vec4<int>{x, y, s, s});
		d.Command<Graphics::Fill>(Color{ 0, 0, 0, 50 });
		d.Command<Graphics::Quad>(Vec4<int>{x + p / 2, y + p / 2, s - p, s - p});
	}
};
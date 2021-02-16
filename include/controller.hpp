#pragma once
#include "pch.hpp"
#include "ui/GamePanel.hpp"

// -------------------------------------------------------------------------- \\
// ---------------------------- Controller ---------------------------------- \\
// -------------------------------------------------------------------------- \\

class Controller
{
public:
    Controller();
    void Run();

private:
    Gui m_Gui;
    Frame& mainWindow;
};
#include "Controller.hpp"

// -------------------------------------------------------------------------- \\
// ---------------------------- Controller ---------------------------------- \\
// -------------------------------------------------------------------------- \\

Controller::Controller()
    : mainWindow(m_Gui.AddWindow<Frame>("Collecto", 1280, 720))
{}

void Controller::Run()
{
    namespace BG = ButtonGraphics; namespace BT = ButtonType; namespace MG = MenuGraphics; namespace MT = MenuType;
    using MenuButton = Button<BG::Menu, BT::Normal>;
    using TitleMenuButton = Button<BG::TitleMenu, BT::Menu<MG::Vertical, MT::Normal, BT::FocusToggle, Align::BOTTOM>>;
    using SubMenuButton = Button<BG::SubMenu, BT::Menu<MG::Vertical, MT::Normal, BT::Hover, Align::RIGHT>>;

    auto& _panel = mainWindow.Panel();
    auto& _menu = mainWindow.Menu();

    _panel.Layout<Layout::Border>(8, 8, false, false, false, false);
    _panel.Background(Color{ 23, 23, 23, 255 });

    auto& _file = _menu.Emplace<TitleMenuButton>("Game", Vec2<int>{ 40, 32 });
    int _height = 20, _width = 200;
    auto& _ng = _file.Emplace<SubMenuButton>("New Game", Vec2<int>{ _width, _height });
    {
        GamePanel* _game = &_panel.Emplace<GamePanel>(Layout::Hint::Center);
        _game->Game().InitPlayers<LocalPlayer, ComputerPlayer<Strategy::Greedy>>();

        _ng.Emplace<MenuButton>([&] 
            { 
                _panel.Clear();
                _game = &_panel.Emplace<GamePanel>(Layout::Hint::Center);
                _game->Game().InitPlayers<LocalPlayer, LocalPlayer>();

            }, "Human-Human", Vec2<int>{ _width, _height });
        
        _ng.Emplace<MenuButton>([&]
            {
                _panel.Clear();
                _game = &_panel.Emplace<GamePanel>(Layout::Hint::Center);
                _game->Game().InitPlayers<LocalPlayer, ComputerPlayer<Strategy::Naive>>();

            }, "Human-Naive", Vec2<int>{ _width, _height });

        _ng.Emplace<MenuButton>([&]
            {
                _panel.Clear();
                _game = &_panel.Emplace<GamePanel>(Layout::Hint::Center);
                _game->Game().InitPlayers<LocalPlayer, ComputerPlayer<Strategy::Greedy>>();

            }, "Human-Greedy", Vec2<int>{ _width, _height });

        _ng.Emplace<MenuButton>([&]
            {
                _panel.Clear();
                _game = &_panel.Emplace<GamePanel>(Layout::Hint::Center);
                _game->Game().InitPlayers<ComputerPlayer<Strategy::Naive>, ComputerPlayer<Strategy::Naive>>();

            }, "Naive-Naive", Vec2<int>{ _width, _height });
    
        _ng.Emplace<MenuButton>([&]
            {
                _panel.Clear();
                _game = &_panel.Emplace<GamePanel>(Layout::Hint::Center);
                _game->Game().InitPlayers<ComputerPlayer<Strategy::Greedy>, ComputerPlayer<Strategy::Naive>>();

            }, "Greedy-Naive", Vec2<int>{ _width, _height });
    
        _ng.Emplace<MenuButton>([&]
            {
                _panel.Clear();
                _game = &_panel.Emplace<GamePanel>(Layout::Hint::Center);
                _game->Game().InitPlayers<ComputerPlayer<Strategy::Greedy>, ComputerPlayer<Strategy::Greedy>>();

            }, "Greedy-Greedy", Vec2<int>{ _width, _height });

        _ng.Emplace<MenuButton>([&]
            {
                if (_game)
                    _game->Game().Restart();
            }, "Rematch", Vec2<int>{ _width, _height }, Key::CTRL_R);
    }

    auto& _dl = _file.Emplace<SubMenuButton>("Computer Delay", Vec2<int>{ _width, _height });
    {
        _dl.Emplace<MenuButton>([&] { Player::DELAY = 0; }, "0ms", Vec2<int>{ _width, _height });
        _dl.Emplace<MenuButton>([&] { Player::DELAY = 10; }, "10ms", Vec2<int>{ _width, _height });
        _dl.Emplace<MenuButton>([&] { Player::DELAY = 50; }, "50ms", Vec2<int>{ _width, _height });
        _dl.Emplace<MenuButton>([&] { Player::DELAY = 100; }, "100ms", Vec2<int>{ _width, _height });
        _dl.Emplace<MenuButton>([&] { Player::DELAY = 200; }, "200ms", Vec2<int>{ _width, _height });
        _dl.Emplace<MenuButton>([&] { Player::DELAY = 300; }, "300ms", Vec2<int>{ _width, _height });
        _dl.Emplace<MenuButton>([&] { Player::DELAY = 400; }, "400ms", Vec2<int>{ _width, _height });
        _dl.Emplace<MenuButton>([&] { Player::DELAY = 500; }, "500ms", Vec2<int>{ _width, _height });
        _dl.Emplace<MenuButton>([&] { Player::DELAY = 600; }, "600ms", Vec2<int>{ _width, _height });
        _dl.Emplace<MenuButton>([&] { Player::DELAY = 700; }, "700ms", Vec2<int>{ _width, _height });
        _dl.Emplace<MenuButton>([&] { Player::DELAY = 800; }, "800ms", Vec2<int>{ _width, _height });
        _dl.Emplace<MenuButton>([&] { Player::DELAY = 900; }, "900ms", Vec2<int>{ _width, _height });
        _dl.Emplace<MenuButton>([&] { Player::DELAY = 1000; }, "1000ms", Vec2<int>{ _width, _height });

    }

    _file.Emplace<MenuButton>([&] { mainWindow.Close(); }, "Quit", Vec2<int>{ _width, _height }, Key::CTRL_Q);

    while (m_Gui.Loop());
}
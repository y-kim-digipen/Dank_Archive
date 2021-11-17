#include <SFML/Audio.hpp>
#include <doodle/doodle.hpp>
#include <vector>
using namespace doodle;
using namespace std;

constexpr float         RADIUS{30.0f};
vector<sf::SoundBuffer> SoundBuffers{};
vector<sf::Sound>       Sounds{};

struct Ball
{
    float x{0}, y{0}, xSpeed{0}, ySpeed{0};
};


void DrawAndUpdateBall(Ball& ball) noexcept;
void LoadSound(const string& file_path);
void PlaySound() noexcept;

//int main(void)
//try
//{
//    sf::Music music;
//    if (!music.openFromFile("assets/orchestral.ogg"))
//    {
//        error("Failed to load the music file: assets/orchestral.ogg");
//    }
//
//    LoadSound("assets/Tambores_1.ogg");
//    LoadSound("assets/Tambores_2.ogg");
//    LoadSound("assets/Tambores_3.ogg");
//
//    music.setLoop(true);
//    music.play();
//
//    create_window();
//
//    vector<Ball> balls = {
//        {200.0f, 80.0f, 8.0f, 9.0f}, {70.0f, 200.0f, 10.0f / 8.0f, 10.0f}, {20.0f, 50.0f, 10.0f, 10.0f / 8.0f}};
//
//    no_fill();
//    set_outline_color(240, 230, 245);
//    set_outline_width(7.0f);
//    set_frame_of_reference(vectorHanded_OriginBottomLeft);
//    while (!is_window_closed())
//    {
//        update_window();
//        clear_background(0);
//        for (auto& ball : balls)
//        {
//            DrawAndUpdateBall(ball);
//        }
//    }
//    return 0;
//}
//catch (exception& e)
//{
//    cerr << e.what() << "\n";
//    return -1;
//}

//void on_key_released(KeyboardButtons button)
//{
//    if (button == KeyboardButtons::Escape)
//        close_window();
//}

void DrawAndUpdateBall(Ball& ball) noexcept
{
    draw_ellipse(ball.x, ball.y, RADIUS * 2.0f);
    ball.x += ball.xSpeed;
    ball.y += ball.ySpeed;
    if (ball.y + RADIUS > Height || ball.y < 0 + RADIUS)
    {
        ball.ySpeed *= -1;
        PlaySound();
    }
    else if (ball.x + RADIUS > Width || ball.x < 0 + RADIUS)
    {
        ball.xSpeed *= -1;
        PlaySound();
    }
}

void LoadSound(const string& file_path)
{
    SoundBuffers.emplace_back();
    sf::SoundBuffer& buffer = SoundBuffers.back();
    if (!buffer.loadFromFile(file_path))
    {

    }
}

void PlaySound() noexcept
{
    int       buffer_index = 0;
    const int random_value = random(0, 100);
    if (random_value > 90)
        buffer_index = 2;
    else if (random_value >= 45)
        buffer_index = 1;
    for (auto& sound : Sounds)
    {
        if (sound.getStatus() != sf::SoundSource::Playing)
        {
            sound.setBuffer(SoundBuffers[buffer_index]);
            sound.play();
            return;
        }
    }
    Sounds.emplace_back(SoundBuffers[buffer_index]);
    Sounds.back().play();
}

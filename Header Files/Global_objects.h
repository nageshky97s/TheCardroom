#pragma once
#include<irrKlang/irrKlang.h>
#include<string>
#include<random>
extern irrklang::ISoundEngine* SoundEngine = irrklang::createIrrKlangDevice();
//extern irrklang::ISound* title_theme = SoundEngine->play2D("D:/C_Proj/The Cardroom/res/Music/TitleTheme.wav", true, false, true, irrklang::ESM_AUTO_DETECT, true);
//extern irrklang::ISoundSource* title_theme = SoundEngine->addSoundSourceFromFile("D:/C_Proj/The Cardroom/res/Music/click.mp3");
extern irrklang::ISoundSource* click_sound = SoundEngine->addSoundSourceFromFile("D:/C_Proj/The Cardroom/res/Music/click.mp3");
extern irrklang::ISoundSource* flip_sound = SoundEngine->addSoundSourceFromFile("D:/C_Proj/The Cardroom/res/Music/flipcard.mp3");
extern irrklang::ISoundSource* win_sound = SoundEngine->addSoundSourceFromFile("D:/C_Proj/The Cardroom/res/Music/win_sound_effect.mp3");
extern irrklang::ISoundSource* loss_sound = SoundEngine->addSoundSourceFromFile("D:/C_Proj/The Cardroom/res/Music/loss_sound_effect.mp3");



extern float screen_height;
extern float screen_width;
extern float resize_factor[2];
enum suits { spades, hearts, diamonds, clubs };
enum cards { ace_one, deuce, three, four, five, six, seven, eight, nine, ten, jack, queen, king, ace };
enum animate {flop,turn,river,p_cards,c_cards};
enum actions { check, call, raise, fold };
extern float flip_speed = 4.5f;
extern std::string keyboard_stream="";
extern float cursor_xpos = 10000.0f;
extern float cursor_ypos = 10000.0f;
extern bool clicked =false;
extern int clicked_value;

auto rng = std::mt19937{ std::random_device{}() };
std::uniform_int_distribution<std::mt19937::result_type> dist6(0, 5);
std::uniform_int_distribution<std::mt19937::result_type> dist2(0, 1);
std::uniform_int_distribution<std::mt19937::result_type> dist3(0, 2);


enum events {init_e,pre_flop_e,pre_flop_animate_e,pre_flop_rob_e,flop_animate_e,flop_rob_e,turn_animate_e,turn_rob_e,river_animate_e,river_rob_e,reveal_e,settlement_e,game_over_e};
extern int state;
extern bool animate_state;
extern int gameOver_state;
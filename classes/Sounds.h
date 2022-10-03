#ifndef GAME_PROJECT_SOUNDS_H
#define GAME_PROJECT_SOUNDS_H

#include <irrKlang.h>

char const *backgroundMusic = "../Sounds/background.wav";
char const *menuMusic = "../Sounds/menu.wav";
char const *deathMusic = "../Sounds/death.wav";
char const *jumpSound = "../Sounds/Jump.wav";
char const *shootSound = "../Sounds/shoot.wav";
char const *playerDamage = "../Sounds/playerDamage.wav";
char const *victoryMusic = "../Sounds/finoSenhores.wav";

irrklang::ISoundEngine* engine = irrklang::createIrrKlangDevice();

map<string, char const *> sounds = {
        {"menu", menuMusic},
        {"death", deathMusic},
        {"background", backgroundMusic},
        {"jump", jumpSound},
        {"shoot", shootSound},
        {"playerDamage", playerDamage},
        {"victory", victoryMusic}
};

class Sounds {
    public: Sounds()= default;
    public: static void playSound(const char* soundName, bool loop);

    public: static void stopSounds();

    public: static void setVolume(float volume);
};

void Sounds::playSound(const char* soundName, bool loop = false) {
    engine->play2D(sounds[soundName], loop);
}

void Sounds::stopSounds() {
    engine->stopAllSounds();
}

void Sounds::setVolume(irrklang::ik_f32 volume) {
    engine->setSoundVolume(volume);
}

#endif //GAME_PROJECT_SOUNDS_H

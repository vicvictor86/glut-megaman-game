#ifndef GAME_PROJECT_SOUNDS_H
#define GAME_PROJECT_SOUNDS_H

char* backgroundMusic = "../Sounds/background.mp3";
char* jumpSound = "../Sounds/jump.wav";
char* shootSound = "../Sounds/shoot.wav";
char* playerDamage = "../Sounds/playerDamage.wav";
map<string, char*> sounds = {
        {"background", backgroundMusic},
        {"jump", jumpSound},
        {"shoot", shootSound},
        {"playerDamage", playerDamage}
};

class Sounds {
    public: Sounds()= default;
    public: static void playSound(const string& soundName);
};

void Sounds::playSound(const string& soundName) {
    sndPlaySound(sounds[soundName], SND_FILENAME | SND_ASYNC);
}

#endif //GAME_PROJECT_SOUNDS_H

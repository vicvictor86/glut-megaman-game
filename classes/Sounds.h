#ifndef GAME_PROJECT_SOUNDS_H
#define GAME_PROJECT_SOUNDS_H


class Sounds {
    public: Sounds()= default;
    public: static void playSound(const char* soundPath);
};

void Sounds::playSound(const char* soundPath) {
    char completePath[100];
    strcpy(completePath, "play ");
    strcat(completePath, soundPath);
    mciSendString(completePath, nullptr, SND_ASYNC, nullptr);
}


#endif //GAME_PROJECT_SOUNDS_H

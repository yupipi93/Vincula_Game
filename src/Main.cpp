#include <eng/GameEngine.hpp>


int stringToInt(std::string string) {
    int n {0};
    size_t length = string.size() - 1;

    for (char ch : string) {
        int exp = 1;
        for (size_t i = 0; i < length; ++i)
            exp *= 10;
        n += exp * (ch - 48);
        --length;
    }
    return n;
}

int main (int argc, char *argv[]) {
    std::string mode = "RELEASE VERSION";
    std::string resolution = "RESOLUTION: ";
    bool debugMode = false;
    bool windowsMode = true;
    uint16_t width = 1920;
    uint16_t height = 1080;

    if(argv[1]) {
        if(strcmp(argv[1], "debug") == 0) {
            mode = "DEBUG MODE ACTIVATE";
            debugMode = true;
        }
    }

    if(argv[2] && argv[3]) {
        std::string widthString = argv[2];
        std::string heightString = argv[3];
        width = stringToInt( widthString );
        height = stringToInt( heightString );
    }

    if(argv[4]) {
        if(strcmp(argv[4], "windows") == 0) {
            windowsMode = true;
        } else {
            windowsMode = debugMode;
        }
    } else {
        windowsMode = debugMode;
    }

    resolution += std::to_string( (int)width ) + "x" + std::to_string( (int)height );
    printf("%s\n", mode.data());
    printf("%s\n", resolution.data());
    GameEngine game_engine = GameEngine(debugMode, width, height, windowsMode);
    game_engine.run();

    return 0;
}


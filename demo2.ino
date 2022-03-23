#ifdef AGE_SNAKE_DEMO

#include "src/AGE.h"

AGE::Process snake_process({
    20, 4,
    120,
    9,
    { 12, 11, 5, 4, 3, 2 }
});

enum Scene {
    GAME_SCENE,
};

unsigned int score = 0;

class GameScene : public AGE::Component {
public:
    void draw(AGE::CharacterBuffer& charBuffer) {
        charBuffer.write("Hello World", snake_process.getWidth() / 2, snake_process.getHeight() / 2, AGE::CENTER);
    };
};

class Game : public AGE::Game {
protected:
    AGE::Component* buildScene(AGE::SceneID sceneId) {
        switch (sceneId)
        {
        case GAME_SCENE:
        return new GameScene();
            break;
        default:
            return nullptr;
        }
    };
};

Game game;

void setup() {
    snake_process.start(&game);
    snake_process.setScene(GAME_SCENE);
}

void loop() {
    snake_process.loop();
}

#endif
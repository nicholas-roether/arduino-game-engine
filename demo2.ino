#ifdef AGE_SNAKE_DEMO

#include "src/AGE.h"

AGE::Process snake_process({
    20, 4,
    120,
    9,
    { 12, 11, 5, 4, 3, 2 }
});

AGE::ClickTrigger leftTrigger  = { 7 };
AGE::ClickTrigger rightTrigger = { 8 };

AGE::TextureID SNAKE_BODY = snake_process.createTexture({
    B00000,
    B11111,
    B11111,
    B11111,
    B11111,
    B11111,
    B11111,
    B00000,
});

class SnakeBody : public AGE::SpawnableComponent {
    uint8_t xPos;
    uint8_t yPos;

public:
    SnakeBody(uint8_t xPos, uint8_t yPos) : xPos(xPos), yPos(yPos) {}

    void draw(AGE::CharacterBuffer& charBuffer) {
        charBuffer.put(SNAKE_BODY, xPos, yPos);
    }
};



enum Scene {
    GAME_SCENE,
};

unsigned int score = 0;

class GameScene : public AGE::Component {
private:
    SnakeBody snakeBody = { 1, 1 };
public:
    void build() {
        addChild(&snakeBody);
    }
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
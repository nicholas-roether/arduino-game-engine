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

enum class Direction {
    UP,
    DOWN,
    LEFT,
    RIGHT,
};

class Snake : public AGE::Component {
    uint8_t length = 2;
    Direction direction = Direction::RIGHT;
    float xPosHead = 1;
    float yPosHead = 1;
    AGE::Velocity velocity = 2;
    AGE::Spawner spawner = { 12 };

public:
    void build() {
        addChild(&spawner);
    }

    void update(uint8_t dt) {
        uint8_t prevPosX = round(xPosHead);
        uint8_t prevPosY = round(yPosHead);

        switch (direction) {
            case Direction::UP:
                if (leftTrigger.fired()) 
                    direction = Direction::LEFT;
                if (rightTrigger.fired()) 
                    direction = Direction::RIGHT;
                velocity.update(dt, yPosHead, true);
                break;
            case Direction::DOWN:
                if (leftTrigger.fired())
                    direction = Direction::RIGHT;
                if (rightTrigger.fired())
                    direction = Direction::LEFT;
                velocity.update(dt, yPosHead, false);
                break;
            case Direction::LEFT:
                if (leftTrigger.fired())
                    direction = Direction::DOWN;
                if (rightTrigger.fired())
                    direction = Direction::UP;
                velocity.update(dt, xPosHead, true);
                break;
            case Direction::RIGHT:
                if (leftTrigger.fired())
                    direction = Direction::UP;
                if (rightTrigger.fired())
                    direction = Direction::DOWN;
                velocity.update(dt, xPosHead, false);
                break;
        }
        if (round(xPosHead) != prevPosX || round(yPosHead) != prevPosY) {
            spawner.spawn(SnakeBody(xPosHead, yPosHead));
        }
    }
};





enum Scene {
    GAME_SCENE,
};

unsigned int score = 0;

class GameScene : public AGE::Component {
    Snake snake;

public:
    void build() {
        addChild(&snake);
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
    snake_process.registerTrigger(&leftTrigger);
    snake_process.registerTrigger(&rightTrigger);

    snake_process.start(&game);
    snake_process.setScene(GAME_SCENE);
}

void loop() {
    snake_process.loop();
}

#endif
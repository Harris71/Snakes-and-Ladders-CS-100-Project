#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Window.hpp>

#include <cstdlib>
#include <ctime>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

using namespace std;
using namespace sf;

class ResourceManager 
{                           //loads all the resources, returns false if any resource is not loaded
public:
    const string FONT_PATH = "assets/fonts/";
    const string IMAGE_PATH = "assets/images/";
    const string AUDIO_PATH = "assets/audio/";

    Font font, font2;
    Texture player1Texture, player2Texture, boardTexture, rightPanelTexture, menubackground;
    Music backgroundMusic;
    SoundBuffer winSound, moveSoundBuffer;

    bool loadResources() 
    {
        if (!font.loadFromFile(FONT_PATH + "arial.ttf") || !font2.loadFromFile(FONT_PATH + "SuperFunky.ttf"))   // Load font
        {
            cerr << "Error loading font!" << endl;
            return false;
        }
        // Load textures
        if (!player1Texture.loadFromFile(IMAGE_PATH + "Player1.png") || !player2Texture.loadFromFile(IMAGE_PATH + "Player2.png") || !boardTexture.loadFromFile(IMAGE_PATH + "GameBoard.png") || !rightPanelTexture.loadFromFile(IMAGE_PATH + "texture.png") || !menubackground.loadFromFile(IMAGE_PATH + "background.png")) 
        {   
            cerr << "Error loading textures!" << endl;
            return false;
        }

        if (!backgroundMusic.openFromFile(AUDIO_PATH + "background_music.ogg"))  // Load background music
        {
            cerr << "Error loading background music!" << endl;
            return false;
        }

        if (!winSound.loadFromFile(AUDIO_PATH + "WinSound.ogg") || !moveSoundBuffer.loadFromFile(AUDIO_PATH + "moveSound.ogg"))   // Load the move sound and the winning sound
        {
            cerr << "Error loading sound effects!";
            return false;
        }

        // Set background music to loop and play
        backgroundMusic.setLoop(true);
        backgroundMusic.play();

        return true;
    }
};

RectangleShape initializeShape(float x, float y, float width, float height, Color color, Color outlineColor, int outlineThickness) 
{
    RectangleShape shape(sf::Vector2f(width, height));  // Create rectangle with size
    shape.setPosition(x, y);                            // Set position
    shape.setFillColor(color);                          // Set color
    shape.setOutlineColor(outlineColor);                // Set Outline
    shape.setOutlineThickness(outlineThickness);        // Set Outline Thickness
    return shape;                                       // Return the shape
}

Text initializeText(const string& string, float x, float y, int fontSize, const Font& font, Color color) 
{
    Text text(string, font, fontSize);     // Create text with the given string, font, and font size
    text.setPosition(x, y);                // Set position
    text.setFillColor(color);              // Set text color
    return text;                           // Return the text
}

Sprite initializeSprite(const Texture& texture, float x, float y, float scaleX, float scaleY) 
{
    Sprite sprite(texture);             // Create sprite of the texture
    sprite.setPosition(x, y);           // Set position of the sprite
    sprite.setScale(scaleX, scaleY);    // Set scale of the sprite
    return sprite;                      // Return the Sprite
}

int RollDice()
{
    return rand() % 6 + 1;      //returns a random number between 1 and 6
    
}

bool isMouseOverButton(const Vector2i& mousePos, const RectangleShape& button)
{
    return button.getGlobalBounds().contains(static_cast<Vector2f>(mousePos)); // Check if the mouse position is within the button bounds
}

void showMenu(RenderWindow& window, const Texture background, Font& font, Font& font2, bool& ismultiplayer)
{
    window.clear(Color::Blue);

    Sprite backgroundSprite = initializeSprite(background, 0, 0, 1.0f, 1.0f);   // Create a sprite for the image
    window.draw(backgroundSprite);

    Text menuText = initializeText("Choose Game Mode", 200, 100, 50, font2, Color::White);
    menuText.setOutlineColor(Color::Black);         // Set Outline color for the Text
    menuText.setOutlineThickness(10);               // Set Outline thickness
    window.draw(menuText);

    RectangleShape singleplayerButton = initializeShape(375, 200, 250, 60, Color::Blue, Color::Black, 5);   // Singleplayer button
    window.draw(singleplayerButton);
    Text singleplayerText = initializeText("Singleplayer", 420, 215, 30, font, Color::White);
    window.draw(singleplayerText);

    RectangleShape multiplayerButton = initializeShape(375, 300, 250, 60, Color::Red, Color::Black, 5);     // Multiplayer button
    window.draw(multiplayerButton);
    Text multiplayerText = initializeText("Multiplayer", 420, 315, 30, font, Color::White);
    window.draw(multiplayerText);

    window.display();

    Event event;
    while (window.isOpen())
    {
        while (window.pollEvent(event))     // Event polling
        {
            if (event.type == Event::Closed || event.type==Event::KeyPressed && event.key.code==Keyboard::Escape)
            {    
                window.close();
            }
            if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left)
            {
                // Check if the click is on the Singleplayer button
                if (isMouseOverButton(Mouse::getPosition(window), singleplayerButton))
                {
                    ismultiplayer = false;
                    return;
                }
                // Check if the click is on the Multiplayer button
                else if (isMouseOverButton(Mouse::getPosition(window), multiplayerButton))
                {
                    ismultiplayer = true;
                    return;
                }
            }

        }
    }

    
}

void updatePlayerPosition(int& playerposition, int rollednumber, const vector<pair<int, int>>& snakes, const vector<pair<int, int>>& ladders, bool& isGameOver) 
{
    playerposition += rollednumber;
    if (playerposition >= 99)
    {
        playerposition = 99;  // Ensure player doesn't exceed the board
        isGameOver = true;
    }

    // Check for snakes
    for (auto& snake : snakes) {
        if (playerposition + 1 == snake.first) 
        {
            playerposition = snake.second - 1;  // Move player to the snake's tail
        }
    }

    // Check for ladders
    for (auto& ladder : ladders) 
    {
        if (playerposition + 1 == ladder.first) 
        {
            playerposition = ladder.second - 1;  // Move player to the top of the ladder
        }
    }
}

void updatePlayerSpritePosition(int playerposition, int x, const int GRIDSIZE, const int CELLSIZE, Sprite& playerSprite) 
{
    int row = GRIDSIZE - 1 - (playerposition / GRIDSIZE);   // flips the indexing of row so the player moves from bottom to top
    int col = playerposition % GRIDSIZE;                    // specifies in which column the player is in      
    if (row % 2 == 0) 
    {
        col = GRIDSIZE - 1 - col;                           // handles zig zag movement of player for alternating rows
    }
    playerSprite.setPosition(col * CELLSIZE + x, row * CELLSIZE + 10);
}

void twoPlayerMode(RenderWindow &window, Sprite &Gameboard, Sprite &rightPanel, const int GRIDSIZE, const int CELLSIZE, Text text, Text DiceText, Text player1pos, Text player2pos, Text exitText, Font &font, Sprite &player1Sprite, Sprite &player2Sprite,  vector<pair<int, int>> &snakes, vector<pair<int, int>> &ladders, RectangleShape gameOverBox, Text gameOverText, Sound winsound, Sound moveSound)
{
    bool isgameover = false;
    bool isplayeroneturn = true;

    // Player positions
    int player1position = 0;
    int player2position = 0;

    player1Sprite.setPosition(10,550);

    Text turnIndicator = initializeText("Player 1's Turn", 610, 300, 30, font, Color:: White);

    Event event;
    while (window.isOpen())
    {
        while (window.pollEvent(event)) // Event Polling
        {
            if (event.type == Event::Closed)
            {
                window.close();
            }
            if (!isgameover && event.type == Event::KeyPressed && event.key.code == Keyboard::Space)
            {
                moveSound.play();
                int rollednumber = RollDice();
                DiceText.setString("Rolled: " + to_string(rollednumber));

                if (isplayeroneturn)
                {
                    updatePlayerPosition(player1position, rollednumber, snakes, ladders, isgameover);
                    player1pos.setString("player 1 Position: " + to_string(player1position + 1));
                    updatePlayerSpritePosition(player1position, 10, GRIDSIZE, CELLSIZE, player1Sprite);

                    if (player1position == 99)      // displays winning message and plays a sound
                    {
                        gameOverText.setString("Player 1 wins!\nPress escape to go back\nPress enter to play again");
                        winsound.play();
                    }

                    if(!isgameover)
                    {
                        turnIndicator.setString("Player 2's Turn");
                        isplayeroneturn = false;
                    }
                }
                else
                {
                    updatePlayerPosition(player2position, rollednumber, snakes, ladders, isgameover);
                    player2pos.setString("player 2 Position: " + to_string(player2position + 1));
                    updatePlayerSpritePosition(player2position, 30, GRIDSIZE, CELLSIZE, player2Sprite);

                    if (player2position == 99)      // displays winning message for player 2 and plays a sound
                    {
                        gameOverText.setString("Player 2 wins!\nPress escape to go back\nPress enter to play again");
                        winsound.play();
                    }

                    if (!isgameover)
                    {
                        turnIndicator.setString("Player 1's Turn");
                        isplayeroneturn = true;
                    }
                }
            }
            if(event.type == Event::KeyPressed && event.key.code == Keyboard::Escape)   // resets everything and goes back to the main function
            {
                player1position=0;
                player2position=0;
                isplayeroneturn=true;
                turnIndicator.setString("Player 1's Turn");
                DiceText.setString("Roll Dice");
                player1Sprite.setPosition(20,550);
                player2Sprite.setPosition(30,550);
                return;

            }

            if (isgameover)
            {
                if(event.type == Event::KeyPressed && event.key.code == Keyboard::Enter)        // resets everything to initial position and restarts the game
                {
                    isgameover=false;
                    isplayeroneturn = true;
                    turnIndicator.setString("Player 1's Turn");
                    DiceText.setString("Roll the Dice:");
                    player1pos.setString("Player 1 Position: 1");
                    player2pos.setString("Player 2 Position: 1");
                    player1position = 1;
                    player2position = 1;
                    player1Sprite.setPosition(10,550);
                    player2Sprite.setPosition(30,550);
                }
            }
        }

        window.clear(Color::Green);     // displays everything
        window.draw(Gameboard);
        window.draw(rightPanel); 
        window.draw(text);
        window.draw(DiceText);
        window.draw(player1pos);
        window.draw(player2pos);
        window.draw(exitText);
        window.draw(player1Sprite);
        window.draw(player2Sprite);
        window.draw(turnIndicator);
        
        if (isgameover)         // displays the game over box when game ends
        {
            window.draw(gameOverBox);
            window.draw(gameOverText);
        }
        window.display();
    }
}

void singleplayer(RenderWindow &window, Sprite &Gameboard, Sprite &rightPanel, const int GRIDSIZE, const int CELLSIZE, Text text, Text &DiceText, Text &playerpos, Text exitText, Font &font, Sprite &playerSprite, vector<pair<int, int>> &snakes, vector<pair<int, int>> &ladders, RectangleShape gameOverBox, Text gameOverText, Sound winsound, Sound moveSound)
{
    bool isgameover = false;
    int playerposition = 0;
     
    gameOverText.setString("Game Over!\nPress escape to go back\nPress enter to play again");

    Event event;
    while (window.isOpen())
    {
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();

            if (!isgameover && event.type == Event::KeyPressed && event.key.code == Keyboard::Space)
            {
                moveSound.play();
                int rollednumber = RollDice();
                DiceText.setString("Rolled: " + to_string(rollednumber));
                
                updatePlayerPosition(playerposition, rollednumber, snakes, ladders, isgameover);

                playerpos.setString("player Position: " + to_string(playerposition+1));

                if (isgameover && playerposition == 99) 
                {
                    winsound.play(); // Play win sound when game is over
                }

                updatePlayerSpritePosition(playerposition, 15, GRIDSIZE, CELLSIZE, playerSprite);
            }

            if (event.type == Event::KeyPressed && event.key.code == Keyboard::Escape)      // resets everything and goes back to main function
            {
                playerposition=0;
                DiceText.setString("Roll The Dice:");
                playerpos.setString("Player Position: 1");
                playerSprite.setPosition(15.85, 550);
                return;
                
            }
            if (isgameover) 
            {
                if (event.type == Event::KeyPressed && event.key.code == Keyboard::Enter)   // resets everything and restarts the game
                {
                    playerposition = 0;
                    isgameover = false;
                    DiceText.setString("Roll the Dice:");
                    playerpos.setString("Player Position: 1");
                    playerSprite.setPosition(20, 550); // Reset position
                }
            }
        }

        window.draw(Gameboard);     // displays everything
        window.draw(rightPanel);
        window.draw(text);
        window.draw(DiceText);
        window.draw(playerpos);
        window.draw(exitText);
        window.draw(playerSprite);
        if (isgameover)         // displays the game over box when game ends
        {
            window.draw(gameOverBox);
            window.draw(gameOverText);
        }
        window.display();
    }
}

int main()
{
    srand(time(0));
    string gamestate = "menu"; 
    bool ismultiplayer = false;  // Variable to check the mode (Singleplayer or Multiplayer)
    const int CELLSIZE = 60;
    const int GRIDSIZE = 10;
    Event event;

    vector<pair<int, int>> snakes = {{17, 7}, {54, 34}, {62, 19}, {64, 60}, {87, 36}, {93, 73}, {95, 75}, {98, 79}};
    vector<pair<int, int>> ladders = {{4, 14}, {9, 31}, {21, 42}, {28, 84}, {51, 67}, {72, 91}, {80, 99}};

    ResourceManager resourceManager;
    
    if (!resourceManager.loadResources()) // Try to load all resources. If it fails, exit the program.
    {
        return -1; // Exit if resources couldn't be loaded
    }

    RenderWindow gameWindow(VideoMode(1000,600), "Snakes and Ladders");

    Text gameText = initializeText("Press Space to Roll Dice", 610, 60, 30, resourceManager.font, Color::White);    

    Text diceText = initializeText("Roll the dice", 610, 120, 30, resourceManager.font, Color::White);                             

    Text player1pos = initializeText("Player 1 position: 1", 610, 180, 25, resourceManager.font, Color::White);                         //Displays the current position of the player on the side bar

    Text player2pos = initializeText("Player 2 position: 1", 610, 240, 25, resourceManager.font, Color::White);

    Text exitText = initializeText("Press Escape to go back", 610, 540, 25, resourceManager.font, Color::White);

    RectangleShape gameOverBox = initializeShape((gameWindow.getSize().x - 400) / 2, (gameWindow.getSize().y - 300) / 2, 400, 300, Color::Black, Color::White, 5);

    Text gameOverText = initializeText("", gameOverBox.getPosition().x + 20, gameOverBox.getPosition().y + 40, 24, resourceManager.font, Color::White);

    Sprite GameBoard = initializeSprite(resourceManager.boardTexture, 0, 0, 1.0f, 1.0f);

    Sprite rightPanelSprite = initializeSprite(resourceManager.rightPanelTexture, 600, 0, 1.0f, 1.0f);

    Sprite player1Sprite = initializeSprite(resourceManager.player1Texture, 20, 550, 0.05f, 0.05f);   

    Sprite player2Sprite = initializeSprite(resourceManager.player2Texture, 30, 550, 0.05f, 0.05f);
    
    Sound winsound;
    winsound.setBuffer(resourceManager.winSound);

    Sound moveSound;
    moveSound.setBuffer(resourceManager.moveSoundBuffer);
    // Game loop
    while(gameWindow.isOpen())
    {
        if (gamestate == "menu")
        {
            showMenu(gameWindow, resourceManager.menubackground, resourceManager.font, resourceManager.font2, ismultiplayer);
            gamestate = ismultiplayer ? "multiplayer" : "singleplayer";
        }
        

        if (!ismultiplayer)
        {
            singleplayer(gameWindow, GameBoard, rightPanelSprite, GRIDSIZE, CELLSIZE, gameText, diceText, player1pos, exitText, resourceManager.font, player1Sprite, snakes, ladders, gameOverBox, gameOverText, winsound, moveSound);
            gamestate = "menu";
        }
            

        if(ismultiplayer)
        {
            twoPlayerMode(gameWindow, GameBoard, rightPanelSprite, GRIDSIZE, CELLSIZE, gameText, diceText, player1pos, player2pos, exitText, resourceManager.font, player1Sprite, player2Sprite, snakes, ladders, gameOverBox, gameOverText, winsound, moveSound);
            gamestate = "menu";
        }     
    }

    return 0;
}
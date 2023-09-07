#include <SFML/Graphics.hpp>  // Include the SFML graphics library
#include <time.h>             // Include the time library for random number generation

using namespace sf;

const int M = 20;             // Dimensions of the game field
const int N = 10;

int field[M][N] = {0};         // Matrix representing the game field

struct Point
{int x, y;} a[4], b[4];        // Define a structure for Tetris pieces

int figures[7][4] =
{
    1, 3, 5, 7, // I
    2, 4, 5, 7, // Z
    3, 5, 4, 6, // S
    3, 5, 4, 7, // T
    2, 3, 5, 7, // L
    3, 5, 7, 6, // J
    2, 3, 4, 5, // O
};  // Define the seven shapes of Tetris pieces

bool check(){
    for (int i = 0; i < 4; i++){
        if (a[i].x < 0 || a[i].x >= N || a[i].y >= M) return 0;
        else if (field[a[i].y] [a[i].x]) return 0;
    }
    return 1;
};  // Function to check if the piece position is valid on the game field

int main()
{
    srand(time(0));  // Initialize the random number generator with the current time

    RenderWindow window(VideoMode(320, 480), "Tetris!");  // Create the game window

    Texture t;
    Texture background_image;
    Texture frame_image;

    t.loadFromFile("images/tiles.png");  // Load texture for Tetris pieces
    background_image.loadFromFile("images/background.png");  // Load texture for the background
    frame_image.loadFromFile("images/frame.png");  // Load texture for the frame

    Sprite background(background_image);  // Create sprites for the background and frame
    Sprite frame(frame_image);
    Sprite s(t);
    s.setTextureRect(IntRect(0, 0, 18, 18));  // Set the size of the pieces
    
    int dx = 0;            // Variables for movement and rotation control
    bool rotate = 0;
    int colorNum = 1;      // Variable to control the color of the current piece

    float timer = 0, delay = 0.3;  // Variables for time control

    Clock clock;  // Create a clock to measure time

    while (window.isOpen()){  // Main game loop

        float time = clock.getElapsedTime().asSeconds();  // Get the elapsed time
        clock.restart();
        timer += time;  // Update the timer

        Event e;
        while (window.pollEvent(e)){
            if (e.type == Event::Closed){
                window.close();  // Close the game if the player closes the window
            }

            if (e.type == Event::KeyPressed){
                if (e.key.code == Keyboard::Up) rotate = true;  // Rotate the piece
                else if (e.key.code == Keyboard::Left) dx =-1;  // Move left
                else if (e.key.code == Keyboard::Right) dx =1;  // Move right
            }
        }

    if (Keyboard::isKeyPressed(Keyboard::Down)) delay = 0.05;  // Speed up piece descent

    //<MOVEMENT>//
    for (int i = 0; i < 4; i++) {b[i] = a[i]; a[i].x += dx;}  // Move the piece horizontally
    if(!check()) for (int i = 0; i < 4; i++) a[i] = b[i];  // Check if the position is valid

    //<ROTATION>//
    if (rotate){
        Point p = a[1];
        
        for (int i = 0; i < 4; i++){
            int x = a[i].y - p.y;
            int y = a[i].x - p.x;
            a[i].x = p.x - x;
            a[i].y = p.y - y;
        }

        if(!check()) for (int i = 0; i < 4; i++) a[i] = b[i];  // Check if the rotation is valid
    }

    //<TICK>//
    if(timer > delay){
        for (int i = 0; i < 4; i++) {b[i] = a[i]; a[i].y += 1;}  // Move the piece down
        if(!check()){
            for (int i = 0; i < 4; i++) field[b[i].y] [b[i].x] = colorNum;  // Lock the piece in the field
            
            colorNum = 1 + rand() % 7;  // Generate a new random color
            int n = rand() % 7;         // Select a new random piece shape
            for (int i = 0; i < 4; i++){
                a[i].x = figures[n][i] % 2;
                a[i].y = figures[n][i] / 2;
            }
        }

        timer = 0;  // Reset the timer
    }

    //<CHECK LINES>//
    int k = M -1;
    for (int i = M - 1; i > 0; i--){
        int count = 0;
        for (int j = 0; j < N; j++){
            if (field[i][j]) count++;  // Count filled cells in the row
            field[k][j] = field[i][j];  // Move cells down
        }

        if(count < N) k--;  // If the row is not full, move one row down
    }

    dx = 0;       // Reset movements and rotations
    rotate = 0;
    delay = 0.3;  // Reset the delay in piece descent

    //<DRAW AREA>//
    window.clear(Color::White);  // Clear the window with a white color
    window.draw(background);      // Draw the background

    for (int i = 0; i < M; i++)
        for (int j = 0; j < N; j++){
            if (field[i][j] == 0) continue;  // Skip empty cells
            s.setTextureRect(IntRect(field[i][j] * 18, 0, 18, 18));  // Set the piece texture
            s.setPosition(j * 18, i * 18);  // Set the piece position
            s.move(28, 31);  // Adjust the position for the game grid
            window.draw(s);  // Draw the piece
        }

    for (int i = 0; i < 4; i++){
        s.setTextureRect(IntRect(colorNum * 18, 0, 18, 18));  // Set the texture for the current piece
        s.setPosition(a[i].x * 18, a[i].y * 18);  // Set the position for the current piece
        s.move(28, 31);  // Adjust the position for the game grid
        window.draw(s);  // Draw the current piece
    }

    window.draw(frame);     // Draw the frame
    window.display();        // Display everything on the window
    }

    return 0;
}

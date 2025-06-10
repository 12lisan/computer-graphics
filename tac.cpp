#include <GL/glut.h>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <string>
#include <cmath>

using namespace std;

// Game constants
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;
const int BOARD_SIZE = 3;
const int CELL_SIZE = 150;
const int BOARD_OFFSET_X = (WINDOW_WIDTH - BOARD_SIZE * CELL_SIZE) / 2;
const int BOARD_OFFSET_Y = (WINDOW_HEIGHT - BOARD_SIZE * CELL_SIZE) / 2;

// Define PI if not already defined
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// Game state
enum GameState { MENU, SINGLE_PLAYER, TWO_PLAYER, GAME_OVER };
enum Player { NONE, X, O };

struct Game {
    Player board[BOARD_SIZE][BOARD_SIZE];
    Player currentPlayer;
    GameState state;
    Player winner;
    bool gameOver;
    int scoreX;    // Score for player X
    int scoreO;    // Score for player O
    int scoreDraw; // Score for draws
} game;

// Color structure
struct Color {
    float r, g, b;
    Color(float red, float green, float blue) : r(red), g(green), b(blue) {}
};

// Color palette
const Color COLOR_BG(0.1f, 0.1f, 0.15f);
const Color COLOR_MENU_BG(0.2f, 0.2f, 0.3f);
const Color COLOR_BOARD(0.95f, 0.95f, 0.95f);
const Color COLOR_LINES(0.2f, 0.2f, 0.2f);
const Color COLOR_X(0.96f, 0.27f, 0.27f);     // Bright Red
const Color COLOR_O(0.27f, 0.49f, 0.96f);     // Bright Blue
const Color COLOR_TEXT(1.0f, 1.0f, 1.0f);
const Color COLOR_HIGHLIGHT(0.6f, 0.9f, 0.6f);
const Color COLOR_BUTTON(0.4f, 0.4f, 0.6f);
const Color COLOR_BUTTON_HOVER(0.5f, 0.5f, 0.7f);
const Color COLOR_TITLE(0.9f, 0.8f, 0.3f);    // Gold
const Color COLOR_WIN_HIGHLIGHT(0.9f, 0.9f, 0.2f);
const Color COLOR_SCORE_BG(0.2f, 0.2f, 0.3f);
const Color COLOR_DRAW(0.8f, 0.8f, 0.8f);     // Gray for draws

// Initialize game
void initGame() {
    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            game.board[i][j] = NONE;
        }
    }
    game.currentPlayer = X;
    game.winner = NONE;
    game.gameOver = false;
}

// Check if there's a winner or draw
void checkGameState() {
    // Check rows
    for (int i = 0; i < BOARD_SIZE; ++i) {
        if (game.board[i][0] != NONE && 
            game.board[i][0] == game.board[i][1] && 
            game.board[i][1] == game.board[i][2]) {
            game.winner = game.board[i][0];
            game.gameOver = true;
            // Update score
            if (game.winner == X) game.scoreX++;
            else if (game.winner == O) game.scoreO++;
            return;
        }
    }

    // Check columns
    for (int j = 0; j < BOARD_SIZE; ++j) {
        if (game.board[0][j] != NONE && 
            game.board[0][j] == game.board[1][j] && 
            game.board[1][j] == game.board[2][j]) {
            game.winner = game.board[0][j];
            game.gameOver = true;
            // Update score
            if (game.winner == X) game.scoreX++;
            else if (game.winner == O) game.scoreO++;
            return;
        }
    }

    // Check diagonals
    if (game.board[0][0] != NONE && 
        game.board[0][0] == game.board[1][1] && 
        game.board[1][1] == game.board[2][2]) {
        game.winner = game.board[0][0];
        game.gameOver = true;
        // Update score
        if (game.winner == X) game.scoreX++;
        else if (game.winner == O) game.scoreO++;
        return;
    }

    if (game.board[0][2] != NONE && 
        game.board[0][2] == game.board[1][1] && 
        game.board[1][1] == game.board[2][0]) {
        game.winner = game.board[0][2];
        game.gameOver = true;
        // Update score
        if (game.winner == X) game.scoreX++;
        else if (game.winner == O) game.scoreO++;
        return;
    }

    // Check for draw
    bool isDraw = true;
    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            if (game.board[i][j] == NONE) {
                isDraw = false;
                break;
            }
        }
        if (!isDraw) break;
    }

    if (isDraw) {
        game.winner = NONE;
        game.gameOver = true;
        game.scoreDraw++; // Increment draw counter
    }
}

// Simple AI move (random)
void makeAIMove() {
    if (game.gameOver) return;

    vector<pair<int, int>> emptyCells;
    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            if (game.board[i][j] == NONE) {
                emptyCells.emplace_back(i, j);
            }
        }
    }

    if (!emptyCells.empty()) {
        int randomIndex = rand() % emptyCells.size();
        int row = emptyCells[randomIndex].first;
        int col = emptyCells[randomIndex].second;
        game.board[row][col] = O;
        game.currentPlayer = X;
        checkGameState();
    }
}

// Draw text with shadow effect
void drawText(float x, float y, const string& text, const Color& color, void* font = GLUT_BITMAP_HELVETICA_18) {
    // Shadow
    glColor3f(0.0f, 0.0f, 0.0f);
    glRasterPos2f(x+1, y+1);
    for (char c : text) {
        glutBitmapCharacter(font, c);
    }
    // Main text
    glColor3f(color.r, color.g, color.b);
    glRasterPos2f(x, y);
    for (char c : text) {
        glutBitmapCharacter(font, c);
    }
}

// Draw rectangle with rounded corners and gradient
void drawRoundedRect(float x, float y, float width, float height, const Color& color, float radius = 10.0f) {
    // Gradient background
    glBegin(GL_QUADS);
    glColor3f(color.r*0.8f, color.g*0.8f, color.b*0.8f);
    glVertex2f(x, y);
    glVertex2f(x + width, y);
    glColor3f(color.r, color.g, color.b);
    glVertex2f(x + width, y + height);
    glVertex2f(x, y + height);
    glEnd();
    
    // Rounded corners
    const int SEGMENTS = 20;
    glBegin(GL_TRIANGLE_FAN);
    for (int i = 0; i <= SEGMENTS; i++) {
        float angle = M_PI * 0.5f * i / SEGMENTS;
        glVertex2f(x + radius - cos(angle) * radius, y + radius - sin(angle) * radius);
    }
    glEnd();
    
    glBegin(GL_TRIANGLE_FAN);
    for (int i = 0; i <= SEGMENTS; i++) {
        float angle = M_PI * 0.5f * i / SEGMENTS;
        glVertex2f(x + width - radius + sin(angle) * radius, y + radius - cos(angle) * radius);
    }
    glEnd();
    
    glBegin(GL_TRIANGLE_FAN);
    for (int i = 0; i <= SEGMENTS; i++) {
        float angle = M_PI * 0.5f * i / SEGMENTS;
        glVertex2f(x + width - radius + cos(angle) * radius, y + height - radius + sin(angle) * radius);
    }
    glEnd();
    
    glBegin(GL_TRIANGLE_FAN);
    for (int i = 0; i <= SEGMENTS; i++) {
        float angle = M_PI * 0.5f * i / SEGMENTS;
        glVertex2f(x + radius - sin(angle) * radius, y + height - radius + cos(angle) * radius);
    }
    glEnd();
}

// Draw X symbol with 3D effect
void drawX(float x, float y, float size) {
    glLineWidth(6.0f);
    // Shadow
    glColor3f(0.3f, 0.3f, 0.3f);
    glBegin(GL_LINES);
    glVertex2f(x - size/2+2, y - size/2+2);
    glVertex2f(x + size/2+2, y + size/2+2);
    glVertex2f(x + size/2+2, y - size/2+2);
    glVertex2f(x - size/2+2, y + size/2+2);
    glEnd();
    
    // Main X
    glColor3f(COLOR_X.r, COLOR_X.g, COLOR_X.b);
    glBegin(GL_LINES);
    glVertex2f(x - size/2, y - size/2);
    glVertex2f(x + size/2, y + size/2);
    glVertex2f(x + size/2, y - size/2);
    glVertex2f(x - size/2, y + size/2);
    glEnd();
}

// Draw O symbol with 3D effect
void drawO(float x, float y, float size) {
    const int SEGMENTS = 50;
    
    // Shadow
    glColor3f(0.3f, 0.3f, 0.3f);
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < SEGMENTS; i++) {
        float angle = 2.0f * M_PI * i / SEGMENTS;
        glVertex2f(x + cos(angle) * size/2 + 2, y + sin(angle) * size/2 + 2);
    }
    glEnd();
    
    // Main O
    glColor3f(COLOR_O.r, COLOR_O.g, COLOR_O.b);
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < SEGMENTS; i++) {
        float angle = 2.0f * M_PI * i / SEGMENTS;
        glVertex2f(x + cos(angle) * size/2, y + sin(angle) * size/2);
    }
    glEnd();
}

// Draw the scoreboard
void drawScoreboard() {
    // Scoreboard background - made wider to accommodate draw counter
    drawRoundedRect(WINDOW_WIDTH/2 - 200, 10, 400, 60, COLOR_SCORE_BG, 15.0f);
    
    // Score text with animation
    float time = glutGet(GLUT_ELAPSED_TIME) / 1000.0f;
    float pulse = 0.9f + 0.1f * sin(time * 2.0f);
    
    // X score
    string xScoreText = "X: " + to_string(game.scoreX);
    Color xScoreColor = COLOR_X;
    xScoreColor.r *= pulse;
    xScoreColor.g *= pulse;
    xScoreColor.b *= pulse;
    drawText(WINDOW_WIDTH/2 - 150, 50, xScoreText, xScoreColor, GLUT_BITMAP_HELVETICA_18);
    
    // First separator
    drawText(WINDOW_WIDTH/2 - 90, 50, "|", COLOR_TEXT, GLUT_BITMAP_HELVETICA_18);
    
    // O score
    string oScoreText = "O: " + to_string(game.scoreO);
    Color oScoreColor = COLOR_O;
    oScoreColor.r *= pulse;
    oScoreColor.g *= pulse;
    oScoreColor.b *= pulse;
    drawText(WINDOW_WIDTH/2 - 50, 50, oScoreText, oScoreColor, GLUT_BITMAP_HELVETICA_18);
    
    // Second separator
    drawText(WINDOW_WIDTH/2 + 10, 50, "|", COLOR_TEXT, GLUT_BITMAP_HELVETICA_18);
    
    // Draw score
    string drawScoreText = "Draws: " + to_string(game.scoreDraw);
    Color drawScoreColor = COLOR_DRAW;
    drawScoreColor.r *= pulse;
    drawScoreColor.g *= pulse;
    drawScoreColor.b *= pulse;
    drawText(WINDOW_WIDTH/2 + 50, 50, drawScoreText, drawScoreColor, GLUT_BITMAP_HELVETICA_18);
}

// Draw the game board
void drawBoard() {
    // Draw scoreboard
    drawScoreboard();
    
    // Draw board background with shadow
    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glVertex2f(BOARD_OFFSET_X - 5, BOARD_OFFSET_Y + 5);
    glVertex2f(BOARD_OFFSET_X + BOARD_SIZE * CELL_SIZE + 15, BOARD_OFFSET_Y + 5);
    glVertex2f(BOARD_OFFSET_X + BOARD_SIZE * CELL_SIZE + 15, BOARD_OFFSET_Y + BOARD_SIZE * CELL_SIZE + 15);
    glVertex2f(BOARD_OFFSET_X - 5, BOARD_OFFSET_Y + BOARD_SIZE * CELL_SIZE + 15);
    glEnd();
    
    drawRoundedRect(BOARD_OFFSET_X - 10, BOARD_OFFSET_Y - 10, 
                   BOARD_SIZE * CELL_SIZE + 20, BOARD_SIZE * CELL_SIZE + 20, COLOR_BOARD, 15.0f);

    // Draw grid lines
    glColor3f(COLOR_LINES.r, COLOR_LINES.g, COLOR_LINES.b);
    glLineWidth(3.0f);
    
    // Vertical lines
    for (int i = 1; i < BOARD_SIZE; ++i) {
        glBegin(GL_LINES);
        glVertex2f(BOARD_OFFSET_X + i * CELL_SIZE, BOARD_OFFSET_Y);
        glVertex2f(BOARD_OFFSET_X + i * CELL_SIZE, BOARD_OFFSET_Y + BOARD_SIZE * CELL_SIZE);
        glEnd();
    }
    
    // Horizontal lines
    for (int j = 1; j < BOARD_SIZE; ++j) {
        glBegin(GL_LINES);
        glVertex2f(BOARD_OFFSET_X, BOARD_OFFSET_Y + j * CELL_SIZE);
        glVertex2f(BOARD_OFFSET_X + BOARD_SIZE * CELL_SIZE, BOARD_OFFSET_Y + j * CELL_SIZE);
        glEnd();
    }

    // Draw X's and O's
    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            float centerX = BOARD_OFFSET_X + i * CELL_SIZE + CELL_SIZE / 2;
            float centerY = BOARD_OFFSET_Y + j * CELL_SIZE + CELL_SIZE / 2;
            
            if (game.board[i][j] == X) {
                drawX(centerX, centerY, CELL_SIZE * 0.8f);
            } else if (game.board[i][j] == O) {
                drawO(centerX, centerY, CELL_SIZE * 0.8f);
            }
        }
    }

    // Draw current player indicator with animation
    string playerText = (game.currentPlayer == X) ? "Player X's Turn" : "Player O's Turn";
    Color playerColor = (game.currentPlayer == X) ? COLOR_X : COLOR_O;
    
    // Pulsing effect
    float pulse = 0.5f + 0.2f * sin(glutGet(GLUT_ELAPSED_TIME) / 200.0f);
    playerColor.r *= pulse;
    playerColor.g *= pulse;
    playerColor.b *= pulse;
    
    drawText(50, 50, playerText, playerColor, GLUT_BITMAP_HELVETICA_18);
}

// Draw animated menu title
void drawMenuTitle(float x, float y) {
    string title = "TIC-TAC-TOE";
    float titleWidth = 0;
    
    // Calculate total width
    for (char c : title) {
        titleWidth += glutBitmapWidth(GLUT_BITMAP_TIMES_ROMAN_24, c);
    }
    
    float startX = x - titleWidth/2;
    float time = glutGet(GLUT_ELAPSED_TIME) / 1000.0f;
    
    glPushMatrix();
    glTranslatef(startX, y, 0);
    
    for (size_t i = 0; i < title.size(); i++) {
        char c = title[i];
        float offset = sin(time * 2.0f + i * 0.5f) * 5.0f;
        
        // Rainbow color effect
        Color charColor(
            0.7f + 0.3f * sin(time + i * 0.5f),
            0.7f + 0.3f * sin(time + i * 0.5f + 2.0f),
            0.7f + 0.3f * sin(time + i * 0.5f + 4.0f)
        );
        
        glPushMatrix();
        glTranslatef(0, offset, 0);
        drawText(0, 0, string(1, c), charColor, GLUT_BITMAP_TIMES_ROMAN_24);
        glPopMatrix();
        
        glTranslatef(glutBitmapWidth(GLUT_BITMAP_TIMES_ROMAN_24, c), 0, 0);
    }
    
    glPopMatrix();
}

// Draw menu with animated buttons
void drawMenu() {
    // Menu background with shadow
    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glVertex2f(195, 145);
    glVertex2f(605, 145);
    glVertex2f(605, 455);
    glVertex2f(195, 455);
    glEnd();
    
    drawRoundedRect(200, 150, 400, 300, COLOR_MENU_BG, 20.0f);

    // Draw animated title
    drawMenuTitle(WINDOW_WIDTH/2, 200);

    // Check mouse position for button hover effects
    int mouseX = glutGet(GLUT_WINDOW_X);
    int mouseY = glutGet(GLUT_WINDOW_Y);
    
    // Button animation parameters
    float time = glutGet(GLUT_ELAPSED_TIME) / 1000.0f;
    float pulseSpeed = 2.0f;
    
    // Single player button
    bool hoverSingle = (mouseX >= 300 && mouseX <= 500 && mouseY >= 300 && mouseY <= 350);
    float singlePulse = 0.9f + 0.1f * sin(time * pulseSpeed);
    Color singleColor = hoverSingle ? COLOR_BUTTON_HOVER : COLOR_BUTTON;
    singleColor.r *= singlePulse;
    singleColor.g *= singlePulse;
    singleColor.b *= singlePulse;
    
    drawRoundedRect(300, 300, 200, 50, singleColor, 15.0f);
    drawText(350, 330, "Single Player", COLOR_TEXT, GLUT_BITMAP_HELVETICA_18);

    // Two players button
    bool hoverTwo = (mouseX >= 300 && mouseX <= 500 && mouseY >= 370 && mouseY <= 420);
    float twoPulse = 0.9f + 0.1f * sin(time * pulseSpeed + 1.0f);
    Color twoColor = hoverTwo ? COLOR_BUTTON_HOVER : COLOR_BUTTON;
    twoColor.r *= twoPulse;
    twoColor.g *= twoPulse;
    twoColor.b *= twoPulse;
    
    drawRoundedRect(300, 370, 200, 50, twoColor, 15.0f);
    drawText(360, 400, "Two Players", COLOR_TEXT, GLUT_BITMAP_HELVETICA_18);
}

// Draw game over screen with winner highlight
void drawGameOver() {
    // Semi-transparent overlay
    glColor4f(0.0f, 0.0f, 0.0f, 0.7f);
    glBegin(GL_QUADS);
    glVertex2f(0, 0);
    glVertex2f(WINDOW_WIDTH, 0);
    glVertex2f(WINDOW_WIDTH, WINDOW_HEIGHT);
    glVertex2f(0, WINDOW_HEIGHT);
    glEnd();
    
    // Result panel with animation
    float time = glutGet(GLUT_ELAPSED_TIME) / 1000.0f;
    float scale = 0.95f + 0.05f * sin(time * 2.0f);
    
    glPushMatrix();
    glTranslatef(WINDOW_WIDTH/2, WINDOW_HEIGHT/2, 0);
    glScalef(scale, scale, 1.0f);
    glTranslatef(-WINDOW_WIDTH/2, -WINDOW_HEIGHT/2, 0);
    
    drawRoundedRect(200, 200, 400, 250, COLOR_MENU_BG, 20.0f);
    
    // Result text
    string resultText;
    Color resultColor = COLOR_TEXT;
    
    if (game.winner == X) {
        resultText = "PLAYER X WINS!";
        resultColor = COLOR_X;
    } else if (game.winner == O) {
        resultText = "PLAYER O WINS!";
        resultColor = COLOR_O;
    } else {
        resultText = "IT'S A DRAW!";
        resultColor = COLOR_DRAW;
    }
    
    // Animate the result text
    float textWidth = 0;
    for (char c : resultText) {
        textWidth += glutBitmapWidth(GLUT_BITMAP_TIMES_ROMAN_24, c);
    }
    
    float textX = (WINDOW_WIDTH - textWidth) / 2;
    float textY = 270;
    
    // Pulsing text
    float textPulse = 1.0f + 0.1f * sin(time * 3.0f);
    glPushMatrix();
    glTranslatef(textX + textWidth/2, textY, 0);
    glScalef(textPulse, textPulse, 1.0f);
    glTranslatef(-textX - textWidth/2, -textY, 0);
    drawText(textX, textY, resultText, resultColor, GLUT_BITMAP_TIMES_ROMAN_24);
    glPopMatrix();
    
    glPopMatrix(); // End of panel animation

    // Check mouse position for button hover effects
    int mouseX = glutGet(GLUT_WINDOW_X);
    int mouseY = glutGet(GLUT_WINDOW_Y);
    
    // Menu button
    bool hoverMenu = (mouseX >= 300 && mouseX <= 500 && mouseY >= 370 && mouseY <= 420);
    drawRoundedRect(300, 370, 200, 50, hoverMenu ? COLOR_BUTTON_HOVER : COLOR_BUTTON, 15.0f);
    drawText(385, 400, "Main Menu", COLOR_TEXT, GLUT_BITMAP_HELVETICA_18);
}

// Display callback
void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(COLOR_BG.r, COLOR_BG.g, COLOR_BG.b, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    switch (game.state) {
        case MENU:
            drawMenu();
            break;
        case SINGLE_PLAYER:
        case TWO_PLAYER:
            drawBoard();
            if (game.gameOver) {
                game.state = GAME_OVER;
            }
            break;
        case GAME_OVER:
            drawBoard();
            drawGameOver();
            break;
    }

    glutSwapBuffers();
}

// Handle mouse click
void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        if (game.state == MENU) {
            // Check menu buttons
            if (x >= 300 && x <= 500) {
                if (y >= 300 && y <= 350) { // Single player
                    game.state = SINGLE_PLAYER;
                    initGame();
                } else if (y >= 370 && y <= 420) { // Two players
                    game.state = TWO_PLAYER;
                    initGame();
                }
            }
        } 
        else if (game.state == SINGLE_PLAYER || game.state == TWO_PLAYER) {
            // Check board clicks
            if (x >= BOARD_OFFSET_X && x <= BOARD_OFFSET_X + BOARD_SIZE * CELL_SIZE &&
                y >= BOARD_OFFSET_Y && y <= BOARD_OFFSET_Y + BOARD_SIZE * CELL_SIZE) {
                
                if (!game.gameOver) {
                    int row = (x - BOARD_OFFSET_X) / CELL_SIZE;
                    int col = (y - BOARD_OFFSET_Y) / CELL_SIZE;
                    
                    if (game.board[row][col] == NONE) {
                        game.board[row][col] = game.currentPlayer;
                        checkGameState();
                        
                        if (!game.gameOver) {
                            game.currentPlayer = (game.currentPlayer == X) ? O : X;
                            
                            // If single player and AI's turn
                            if (game.state == SINGLE_PLAYER && game.currentPlayer == O) {
                                glutPostRedisplay(); // Update display before AI move
                                glutTimerFunc(500, [](int) { makeAIMove(); glutPostRedisplay(); }, 0);
                            }
                        }
                    }
                }
            }
        } 
        else if (game.state == GAME_OVER) {
            // Check game over buttons
            if (x >= 300 && x <= 500 && y >= 370 && y <= 420) { // Menu
                game.state = MENU;
            }
        }
    }
    
    glutPostRedisplay();
}

// Handle mouse motion for hover effects
void mouseMotion(int x, int y) {
    glutPostRedisplay();
}

// Initialize OpenGL
void initGL() {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    // Enable blending for transparency
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    // Smooth lines
    glEnable(GL_LINE_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
}

// Timer function for animations
void timer(int value) {
    glutPostRedisplay();
    glutTimerFunc(16, timer, 0); // ~60 FPS
}

// Main function
int main(int argc, char** argv) {
    // Initialize random seed
    srand(time(nullptr));
    
    // Initialize game state
    game.state = MENU;
    game.scoreX = 0;
    game.scoreO = 0;
    game.scoreDraw = 0;
    
    // Initialize GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutCreateWindow("Tic-Tac-Toe Ultimate");
    
    // Register callbacks
    glutDisplayFunc(display);
    glutMouseFunc(mouse);
    glutPassiveMotionFunc(mouseMotion);
    glutTimerFunc(0, timer, 0);
    
    // Initialize OpenGL
    initGL();
    
    // Start main loop
    glutMainLoop();
    
    return 0;
}

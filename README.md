# computer graphics
Tic-tac-toe game
Tic-Tac-Toe is a classic two-player strategy game played on a 3x3 grid where opponents take turns marking X or O in empty cells, aiming to get three of their symbols in a row (horizontally, vertically, or diagonally); if all cells fill without a winner, the game ends in a draw. This implementation enhances the traditional game with sleek visuals, smooth animations, and both single-player (against AI) and two-player modes, complete with a dynamic menu system, score tracking, and interactive feedback. The polished design retains the simplicity of the original while delivering a more engaging and modern user experience.
       
# Key Features Implemented

# 1 Game Engine Foundation
   
   -Created a game state system with menu, single-player, two-player, and game-over states
   
   -Implemented a 3x3 game board with X and O players
   
   -Added win/draw detection logic for all possible combinations

# 2 Visual Enhancements
   
   -Designed a polished UI with rounded corners, shadows, and animations
   
   -Added a score tracking system (X wins, O wins, draws)
  
   -Created animated buttons with hover effects
  
   -Implemented pulsing text and smooth transitions

# 3 Gameplay Features
  
  -Single-player mode with AI opponent (currently random moves)
  
  -Two-player mode for local multiplayer
  
  -Turn indicator showing whose turn it is
  
  -Winner/draw detection with celebration screen

# Implementation Process

# 1 Initial Setup
  
  -Used OpenGL with GLUT for rendering
  
  -Defined constants for window size, board dimensions, and colors
  
  -Created game state structure to track all game variables

# 2 Core Game Logic
  
  -Implemented initGame() to reset the board
  
  -Created checkGameState() to detect wins/draws after each move
  
  -Added simple AI with makeAIMove() (currently makes random moves)

# 3 Rendering System
  
  -Developed drawing functions for all game elements
  
  -drawText() with shadow effects for better readability
  
  -drawRoundedRect() for attractive UI elements
  
  -Specialized drawX() and drawO() with 3D effects
  
  -Scoreboard display with animated elements
  
  -Created separate rendering paths for each game state

# 4 UI/UX Enhancements
  
  -Added animated menu with pulsing buttons
  
  -Implemented hover effects for interactive elements
  
  -Created a game-over screen with winner celebration
  
  -Added visual feedback for current player turn

# 5 Input Handling
  
  -Set up mouse input for menu navigation and board moves
  
  -Added proper state transitions between game modes

# 6 Polish and Effects
  
  -Implemented various animations:
  
  -Pulsing text for important messages
  
  -Smooth transitions between states
  
  -Rainbow color effect for the menu title
  
  -Added visual depth with shadows and gradients

# Technical Highlights

# 1 Object-Oriented Approach
  
  -Used structs to organize related data (Game, Color)
 
  -Created enum types for game states and players

# 2 Visual Effects
  
  -Implemented time-based animations using glutGet(GLUT_ELAPSED_TIME)
  
  -Added shadows and highlights for depth perception
  
  -Created smooth color transitions and pulsing effects

# 3 State Management
  
  -Clean separation between different game screens
  
  -Proper handling of game state transitions

# Tic-Tac-Toe video description

https://github.com/user-attachments/assets/ae19a30c-4286-40a8-baa1-04b24585e110


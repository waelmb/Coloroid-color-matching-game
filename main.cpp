//
// Program 4: Solution for Coloroid, color-matching game
// 
// Instructions:
//    To run this program within Codio, first select the "Build and Run" option at the top 
//    of the window. You can ignore the following error messages that will appear:
//         Failed to use the XRandR extension while trying to get the desktop video modes
//         Failed to use the XRandR extension while trying to get the desktop video modes
//         Failed to initialize inotify, joystick connections and disconnections won't be notified
//    To see the graphical output then select the "Viewer" option at the top of the window.
//    
// For more information about SFML graphics, see: https://www.sfml-dev.org/tutorials
// Be sure to close the old window each time you rebuild and rerun, to ensure you
// are seeing the latest output. 
// 
// Author: Wael Mobeirek
// Class:  UIC CS-141, Fall 2018
// System: C++ on cloud-based Codio.com 
//    
//     45 points Grading Rubric for Programming Style
//    
//        10 Meaningful identifier names
//        10 Comments: Has header.  Comments on each block of code
//        10 Functional Decomposition
//        10 Appropriate data and control structures
//       5 Code Layout: Appropriate indentation and blank lines
//       
#include <SFML/Graphics.hpp> // Needed to access all the SFML graphics libraries
#include <iostream>          // Since we are using multiple libraries, now use std::
                             // in front of every cin, cout, endl, and string 
#include <cstdio>            // For sprintf, "printing" to a string
#include <cstring>           // For c-string functions such as strlen()      

const int WindowXSize = 400;
const int WindowYSize = 500;
const int MaxBoardSize = 24;  // Max number of squares per side


//---------------------------------------------------------------------------------------
class Square {
	public:
		// Default Constructor 
		Square()
		{
			size = 0;
			xPosition = 0;
			yPosition = 0;
			color = sf::Color::Black;
			isVisible = false;
			isCaptured = false;
			text = "";		
		}
	
		// Fully-qualified constructor, used to set all fields
		Square( int theSize, int theXPosition, int theYPosition, 
					  const sf::Color &theColor, bool theVisibility, std::string theText)
		{
			// Set the class fields
			size = theSize;
			xPosition = theXPosition;
			yPosition = theYPosition;
			color = theColor;
			isVisible = theVisibility;
			isCaptured = false;   // By default squares have not been captured
			text = theText;
			// Use the values to set the display characteristics of theSquare
			theSquare.setSize( sf::Vector2f(theSize, theSize));
			theSquare.setPosition( theXPosition, theYPosition);   // Set the position of the square
			theSquare.setFillColor( theColor);
		}
			
		// Get (accessor) functions
		sf::RectangleShape getTheSquare() { return theSquare; }
		int getSize() { return size; }
		int getXPosition() { return xPosition; }
		int getYPosition() { return yPosition; }
		sf::Color& getColor() { return color; }
		bool getIsVisible() { return isVisible; }
		bool getIsCaptured() { return isCaptured; }
		std::string getText() { return text; }
	
		// Set (mutator) functions
		void setSize( int theSize) { 
			size = theSize; 
			theSquare.setSize( sf::Vector2f(theSize, theSize));
		}
		void setXPosition( int theXPosition) { 
				xPosition = theXPosition; 
				theSquare.setPosition( theXPosition, yPosition);   // Set the position of the square
		}
		void setYPosition( int theYPosition) { 
				yPosition = theYPosition; 
				theSquare.setPosition( xPosition, theYPosition);   // Set the position of the square
		}
		void setColor( sf::Color & theColor) { 
				color = theColor; 
				theSquare.setFillColor( theColor);    // Also update the color on the square itself
		}
		void setVisibility( bool theVisibility) { isVisible = theVisibility; }
		void setIsCaptured( bool isCaptured) { this->isCaptured = isCaptured; }
		void setText( std::string theText) { text = theText; }

		// Utility functions
		void displayText( sf::RenderWindow *pWindow, sf::Font theFont, sf::Color theColor, int textSize);
	
	private:
		int size;
		int xPosition;
		int yPosition;
		sf::Color color;
		bool isVisible;
		bool isCaptured;   // Indicates whether or not it is part of the captured area
		std::string text;
		sf::RectangleShape theSquare;

}; //end class Square


//---------------------------------------------------------------------------------------
// Square class utility function to create a sf::Text object to store and display text 
// associated with this Square.
// 
// Assuming we display output in sf::RenderWindow window(...), then call this function using: 
//    aSquare.displayTest( &window);
// or when using an array of Square pointers declared as:  Square *squaresArray[ 4];
// then call it using:  squaresArray[i]->displayText( &window);
void Square::displayText( 
			sf::RenderWindow *pWindow,   // The window into which we draw everything
			sf::Font theFont,            // Font to be used in displaying text   
			sf::Color theColor,          // Color of the font
			int textSize)                // Size of the text to be displayed
{	
		// Create a sf::Text object to draw the text, using a sf::Text constructor
		sf::Text theText( text,        // text is a class data member
								      theFont,     // font from a font file, passed as a parameter
								      textSize);   // this is the size of text to be displayed

		// Text color is the designated one, unless the background is Yellow, in which case the text
		// color gets changed to blue so we can see it, since we can't see white-on-yellow very well
		if( this->getColor() == sf::Color::Yellow) {
				theColor = sf::Color::Blue;
		}
		theText.setColor( theColor);

		// Place text in the corresponding square, centered in both x (horizontally) and y (vertically)
		// For horizontal center, find the center of the square and subtract half the width of the text 
		int theXPosition = xPosition + (size / 2) - (strlen(text.c_str()) * theText.getCharacterSize()) / 2;
		// For the vertical center, from the top of the square go down the amount: (square size - text size) / 2
		int theYPosition = yPosition + (size - theText.getCharacterSize()) / 2;
		// Use an additional offset to get it centered
		int offset = 5;
		theText.setPosition( theXPosition + offset, theYPosition - offset);

		// Finally draw the Text object in the RenderWindow
		pWindow->draw( theText);
}


//---------------------------------------------------------------------------------------
// Initialize the font
void initializeFont( sf::Font &theFont)
{
		// Create the global font object from the font file
		if (!theFont.loadFromFile("arial.ttf"))
		{
			 std::cout << "Unable to load font. " << std::endl;
			 exit( -1);
		}	
}


//---------------------------------------------------------------------------------------
// Get a random number, and use that to return the corresponding color
// The SFML Color documentation page indicates that the six valid colors 
// are: Black, Red, Green, Blue, Yellow, Cyan
sf::Color getRandomColor()
{
		sf::Color theColor;
		// Get a random number in the range 0..5
		switch (random()%6 ) {
				case 0: theColor = sf::Color::Black;  break;
				case 1: theColor = sf::Color::Red;    break;
				case 2: theColor = sf::Color::Green;  break;
				case 3: theColor = sf::Color::Blue;   break;
				case 4: theColor = sf::Color::Yellow; break;
				case 5: theColor = sf::Color::Cyan;   break;
				default: std::cout << "Invalid random number, exiting program..." << std::endl;
						exit( -1);
						break;
		}
	return theColor;
}//end getRandomColor()


//---------------------------------------------------------------------------------------
// Convert sf::Color to a string, for ease of printing when debugging.  When calling this
// you must previously have created an array of char that you send as the second parameter
// to store the resulting color name to use (for instance) in printing debugging information.
void getColorAsString( 
				sf::Color theColor,      // The sf::Color
				char theColorName[ 81])  // Resulting string will be stored here
{
		if( theColor == sf::Color::Black)      strcpy( theColorName, "Black");
		else if( theColor == sf::Color::Red)   strcpy( theColorName, "Red");
		else if( theColor == sf::Color::Green) strcpy( theColorName, "Green");
		else if( theColor == sf::Color::Blue)  strcpy( theColorName, "Blue");
		else if( theColor == sf::Color::Yellow)strcpy( theColorName, "Yellow");
		else if( theColor == sf::Color::Cyan)  strcpy( theColorName, "Cyan");
		else strcpy( theColorName, "Invalid color");
}

// create squares in squaresArray with random colors
void createSquares(Square* squaresArray,int arraySize, int level) {
  double squareSize = WindowXSize / level;
  int xPositionTracker = 0;
  int yPositionTracker = 0;
  
  for( int i=0; i<arraySize; i++) {
      //std::cout << "createSquares i: " << i << std::endl;
			// Allocate each square individually.
			// Store a string in each square which contains its number, in order of creation
			char name[ 81];
			sprintf( name, "%d", i);   // "print" the square creation order number into a string to be stored in the square
			//Track yPosition
			if(i%level == 0 && i != 0) {
        yPositionTracker++;
        xPositionTracker = 0;
        //std::cout << "y: " << yPositionTracker << std::endl;
      }
      // Set each array pointer have the address of a new Square, created with a Square constructor
      squaresArray[ i] = Square(squareSize, squareSize * xPositionTracker, squareSize * yPositionTracker, getRandomColor(), true, name);
      xPositionTracker++;
      //std::cout << "getYPosition: " << squaresArray[ i].getYPosition() << std::endl;
      //std::cout << "getXPosition: " << squaresArray[ i].getXPosition() << std::endl;
  }
}

// Determine initial captured area at the beginning of each level
void initialCapturedArea(Square* squaresArray,int arraySize, int level) {
  squaresArray[0].setIsCaptured(true);
  int squareSize = WindowXSize/level;
  
  for(int i = 0; i < arraySize; i++) {
    //if this square number i is captured 
    if(squaresArray[i].getIsCaptured()) {
      //if there is a square to your left, examine if it can be captured or not
       if(squaresArray[i].getXPosition() > 0 ){
         if(squaresArray[i].getColor() == squaresArray[i-1].getColor()) {
            squaresArray[i-1].setIsCaptured(true);
          }
       }
      //if there is a square to your right, examine if it can be captured or not
      if(squaresArray[i].getXPosition() < WindowXSize-squareSize){
         if(squaresArray[i].getColor() == squaresArray[i+1].getColor()) {
            squaresArray[i+1].setIsCaptured(true);
          }
       }
      //if there is a square above you, examine if it can be captured or not
      if(squaresArray[i].getYPosition() > 0){
         if(squaresArray[i].getColor() == squaresArray[i-level].getColor()) {
            squaresArray[i-level].setIsCaptured(true);
          }
       }
      //if there is a square under you, examine if it can be captured or not
      if(squaresArray[i].getYPosition() < WindowXSize-squareSize){
         if(squaresArray[i].getColor() == squaresArray[i+level].getColor()) {
            squaresArray[i+level].setIsCaptured(true);
          }
       }
     }
  }
}

// Update captured area
void updateCapturedArea(Square* squaresArray,int arraySize, int level, int boxNum) {
  int squareSize = WindowXSize/level;
  sf::Color clickedSquareColor = squaresArray[boxNum].getColor();
  
  //set the captured area to match the selected's square color
  for(int i = 0; i < arraySize; i++) { 
    if(squaresArray[i].getIsCaptured()) {
      squaresArray[i].setColor(clickedSquareColor);
    }
  }
  
  //look for adjacent squares with similar color
  for(int i = 0; i < arraySize; i++) {
    //if this square number i is captured 
    if(squaresArray[i].getIsCaptured()) {
      //if there is a square to your left, examine if it can be captured or not
       if(squaresArray[i].getXPosition() > 0 ){
         if(squaresArray[i].getColor() == squaresArray[i-1].getColor()) {
            squaresArray[i-1].setIsCaptured(true);
          }
       }
      //if there is a square to your right, examine if it can be captured or not
      if(squaresArray[i].getXPosition() < WindowXSize-squareSize){
         if(squaresArray[i].getColor() == squaresArray[i+1].getColor()) {
            squaresArray[i+1].setIsCaptured(true);
          }
       }
      //if there is a square above you, examine if it can be captured or not
      if(squaresArray[i].getYPosition() > 0){
         if(squaresArray[i].getColor() == squaresArray[i-level].getColor()) {
            squaresArray[i-level].setIsCaptured(true);
          }
       }
      //if there is a square under you, examine if it can be captured or not
      if(squaresArray[i].getYPosition() < WindowXSize-squareSize){
         if(squaresArray[i].getColor() == squaresArray[i+level].getColor()) {
            squaresArray[i+level].setIsCaptured(true);
          }
       }
     }
  }
}

//Determine the box clicked on
int determineBox(int level, float xPosition, float yPosition) {
  int boxNum = 0;
  float squarePerX = WindowXSize/level;
  float squarePerY = squarePerX;
  
  //determine box location on y-axis
  while(yPosition > squarePerY) {
    boxNum = boxNum + level;
    yPosition = yPosition - squarePerY;
  }
  //determine box location on x-axis
  while(xPosition > squarePerX) {
    boxNum = boxNum + 1;
    xPosition = xPosition - squarePerX;
  }
  return boxNum;
}

//Determine if the board is complete
bool isComplete(Square* squaresArray,int arraySize) {
  bool allSameColor = true;
  //Determine if all squares share the same color
  for(int i = 1; i < arraySize; i++) {
    if(squaresArray[0].getColor() != squaresArray[i].getColor()) {
      allSameColor = false;
    }
  }
  return allSameColor;
}

//Determine whether to go to the next level or not
//conditions: 
//1) all squares have the same color
//2) clicks must equal to par or less
//3) max level is 24
bool isWon(int level, int clicks, int par) {
  bool eligibleMoves = false;
  bool eligibleLevel = false;
  
  //Determine if the number of moves is eligibile
  if(clicks <= par) {
    eligibleMoves = true;
  }
  
  //Determine if the level is eligibile
  if(level < 24) {
    eligibleLevel = true;
  }
  
  //Return whether you can move to the next level or not
  if(eligibleMoves && eligibleLevel) {
    return true;
  }
  else {
    return false;
  } 
}

//---------------------------------------------------------------------------------------

int main()
{	
		char aString[ 81];    // c-string to hold concatenated output of character literals
	
		// Create and initialize the font, to be used in displaying text.
		sf::Font font;  
		initializeFont( font);

		// Create the destination window
		sf::RenderWindow window(sf::VideoMode(WindowXSize, WindowYSize), "Program 4: Color Match");

		// Create the messages label at the bottom of the screen, to be used in displaying debugging information.
		sf::Text messagesLabel( "", font, 20);
		// Make a text object from the font
		messagesLabel.setColor( sf::Color::White);
		// Place text at the bottom of the window. Position offsets are x,y from 0,0 in upper-left of window
		messagesLabel.setPosition( 0, WindowYSize - messagesLabel.getCharacterSize() - 5);  
		
		int level = 4;     // level is number of squares per side.  There are level * level number of squares.
		int arraySize = level * level;
		// Create array of Square objects to be the max size it will ever be.
		Square squaresArray[ MaxBoardSize * MaxBoardSize];
		bool mouseIsAlreadyPressed = false;
		bool settingUpBoard = true;
    int clicks = 0;
    int par = level*2;
    // Create squares with random colors
    createSquares(squaresArray, arraySize, level);
    // Determine initial captured area
    initialCapturedArea(squaresArray, arraySize, level);
    /*for( int i=0; i<arraySize; i++) {
      std::cout << "i: " << i << std::endl;
      std::cout << "getIsCaptured: " << squaresArray[ i].getIsCaptured() << std::endl;
    }*/
		// Run the program as long as the window is open.  This is known as the "Event loop".
		while (window.isOpen())
		{
				// check all the window's events that were triggered since the last iteration of the loop
				sf::Event event;
				while (window.pollEvent(event)) {
						// There was a "close requested" event, so we close the window
						if (event.type == sf::Event::Closed) { 
							window.close(); 
						}  	
				}
			
				// Check if squares are all the same color.  If they are, and the number of moves is within par,
				// then increment the level and indicate that we are again setting up the board.
				if(isComplete(squaresArray, arraySize)) {
          if(isWon(level, clicks, par)) {
            level++;
            arraySize = level * level;
            settingUpBoard = true;
            clicks = 0;
            par = level*2;
          }
          else {
            clicks = 0;
            settingUpBoard = true;
          }
        }
        			
				// Create a new board.  This is done at the beginning of the game, or after successful completion
				// of a level, when all squares are the same color. Immediately after creating a new board you should
				// also go through and mark adjacent squares of the same color as being part of the initial 
				// captured area.
				if( settingUpBoard == true) {
          // Create squares with random colors
          createSquares(squaresArray, arraySize, level);
          // Determine initial captured area
          initialCapturedArea(squaresArray, arraySize, level);
          /*for( int i=0; i<arraySize; i++) {
            std::cout << "i: " << i << std::endl;
            std::cout << "getIsCaptured: " << squaresArray[ i].getIsCaptured() << std::endl;
          } */
          settingUpBoard = false;    // Will be reset to true after level is complete
				}
			
				// Clear the window, erasing what is displayed
				window.clear();

				// Redraw all screen components to the background frame buffer
				for( int i=0; i<arraySize; i++) {
          //std::cout << "Draw i: " << i << std::endl;
          // Draw the square
          window.draw( squaresArray[ i].getTheSquare());
          // Draw the text associated with the Square, in the window with the indicated color and text size
          //squaresArray[i].displayText( &window, font, sf::Color::Blue, 30);
				}

				// Display the coordinates for the mouse when the mouse left button is pressed
				if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && !mouseIsAlreadyPressed)
				{
						// Set a flag to indicate mouse button has been pressed.  This is used to only increment the
						// numberOfMoves once, and not every time through the event loop.  This flag gets turned off
						// once the mouse is no longer pressed.
						mouseIsAlreadyPressed = true;
						// Increment the number of moves each time mouse is clicked
						clicks++;
					
						// Get the current mouse x,y position and store into a std::string to be displayed as text in debugging.
						sf::Vector2i localPosition = sf::Mouse::getPosition(window);   // Vector2i stores x,y value
						//sprintf( aString, "(%d, %d) Clicks: %d Par: %d", localPosition.x, localPosition.y, par, clicks);
            // Make a string with mouse x,y values		
						// Using the mouse x,y position, find which square was clicked upon.
						int boxNum = determineBox(level, localPosition.x, localPosition.y);
						//std::cout << "squareNumber: " << boxNum << std::endl;
						// Retrieve the color of the selected square
						/*sf::Color squareColor =  squaresArray[boxNum].getColor();
            char stringColor[81];
            for(int i = 0; i < 81; i++) {
              stringColor[i] = ' ';
            }
            getColorAsString(squareColor, stringColor);
            for(int i = 0; i < 10; i++) {
              std::cout << stringColor[i];
            }
            std::cout << std::endl; */
						// Go through the board and capture all adjacent squares of the selected color.
						updateCapturedArea(squaresArray, arraySize, level, boxNum);
            /* std::cout << "getXPosition: " << squaresArray[boxNum].getXPosition() << std::endl;
            std::cout << "getYPosition: " << squaresArray[boxNum].getYPosition() << std::endl;
            std::cout << "WindowXSize-squareSize: " << WindowXSize-(WindowXSize/level) << std::endl;
            std::cout << "getIsCaptured: " << squaresArray[boxNum].getIsCaptured() << std::endl;
           if(squaresArray[boxNum].getColor() == squaresArray[0].getColor()) {
              std::cout << "They have the same color" << std::endl;
            } */
				}
			
				// Set mouse press status to false.  This is used to only go through the mousedown code a single
				// time when the mouse is pressed, rather than running that code every time through the event loop.
				if (! sf::Mouse::isButtonPressed(sf::Mouse::Left) && mouseIsAlreadyPressed) {
						mouseIsAlreadyPressed = false;
				}
			
				// Display previously constructed string at the bottom of the screen 
				sprintf( aString, "(par %d) Move %d", par, clicks);
				messagesLabel.setString( aString);                    // Store the string into the messagesLabel
				window.draw( messagesLabel);                          // Display the messagesLabel
			
				// Display the background frame buffer, replacing the previous RenderWindow frame contents.
				// This is known as "double-buffering", where you first draw into a background frame, and then
				// replace the currently displayed frame with this background frame.
				window.display();

		}//end while( window.isOpen())

		return 0;
}//end main()
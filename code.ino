

// include nessecary libraries
#include <Adafruit_NeoPixel.h>
#include <LiquidCrystal_I2C.h>

// define pins
#define leftButton 5
#define middleButton 6
#define rightButton 7
#define selectionLEDS 10
#define slotLEDS 8
#define answerLEDS 9
#define answerButton 4
#define upButton 3
#define downButton 2

// create arrays
int slotLED[7][4];
int selectionLED[4];
int answer[4];
int guessHint[7][2];

// create and initialize variables
bool selectDifficulty = true;
bool winner = false;
bool loser = false;
int correctSpots = 0;
int difficulty;
int currentSlot = 0;
int currentGuess = 0;
int correct = 0;
int wrongSpot = 0;
int currentHint = 0;

// define the lcd and 2 pixel strips
LiquidCrystal_I2C lcd(0x20, 16, 2);
Adafruit_NeoPixel pixels(28, slotLEDS, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel picking(4, selectionLEDS, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel answerLED(4, answerLEDS, NEO_GRB + NEO_KHZ800);
/*
setup()

Use: sets pinMode of all pins and initiates needed components
*/
void setup()
{
  // define each state of each pin
  pinMode(leftButton, INPUT);
  pinMode(middleButton, INPUT);
  pinMode(rightButton, INPUT);
  pinMode(selectionLEDS, OUTPUT);
  pinMode(slotLEDS, OUTPUT);
  pinMode(answerButton, INPUT);
  pinMode(upButton, INPUT);
  pinMode(downButton, INPUT);
  
  // initialize the lcd, pixels, and create randomizer
  lcd.init();
  lcd.backlight();
  pixels.begin();
  answerLED.begin();
  randomSeed(analogRead(0));
}

/*
loop()

Use: Main loop in program, runs main function
*/
void loop()
{
  // only run to select difficulty
  while(selectDifficulty)
  {
    // clear the lcd and prompt the user for a selection
    lcd.setCursor(0,0);
    lcd.print("Difficulty:");
    lcd.setCursor(0,1);
    lcd.print("Easy  Med   Hard");
    // user selected easy mode
    if(digitalRead(leftButton) == 1)
    {
      // set difficulty and stop the loop
      difficulty = 1;
      selectDifficulty = false;
      lcd.clear();
      // create led sequence for 4 possible colours
      for(int i = 0; i < 4; i++)
      {
      	selectionLED[i] = 0;
        answer[i] = random(3);
        if(i == 1)
        {
          while(answer[i] == answer[i-1])
          {
            answer[i] = random(4);
          }
        }
        if(i == 2)
        {
          while(answer[i] == answer[i-2] || answer[i] == answer[i-1])
          {
            answer[i] = random(4);
          }
        }
        if(i == 3)
        {
          while(answer[i] == answer[i-3] || answer[i] == answer[i-2] || answer[i] == answer[i-1])
          {
            answer[i] = random(4);
          }
        }
      }
      // start the game using functions
      updateSelection();
      resetGuesses();
    }
    // set medium difficulty
    if(digitalRead(middleButton) == 1)
    {
      // set difficulty and exit loop
      difficulty = 2;
      selectDifficulty = false;
      lcd.clear();
      // generate 6 different possible colours in a 4 sequence
      for(int i = 0; i < 4; i++)
      {
      	selectionLED[i] = 0;
        answer[i] = random(6);
        if(i == 1)
        {
          while(answer[i] == answer[i-1])
          {
            answer[i] = random(6);
          }
        }
        if(i == 2)
        {
          while(answer[i] == answer[i-2] || answer[i] == answer[i-1])
          {
            answer[i] = random(6);
          }
        }
        if(i == 3)
        {
          while(answer[i] == answer[i-3] || answer[i] == answer[i-2] || answer[i] == answer[i-1])
          {
            answer[i] = random(6);
          }
        }
      }
      // start the game and reset values
      updateSelection();
      resetGuesses();
    }
    // user selected hard difficulty
    if(digitalRead(rightButton) == 1)
    {
      // set difficulty to hard and stop the loop
      difficulty = 3;
      selectDifficulty = false;
      lcd.clear();
      // generate a random 8 colour 4 led sequence
      for(int i = 0; i < 4; i++)
      {
      	selectionLED[i] = 0;
        answer[i] = random(8);
        if(i == 1)
        {
          while(answer[i] == answer[i-1])
          {
            answer[i] = random(8);
          }
        }
        if(i == 2)
        {
          while(answer[i] == answer[i-2] || answer[i] == answer[i-1])
          {
            answer[i] = random(8);
          }
        }
        if(i == 3)
        {
          while(answer[i] == answer[i-3] || answer[i] == answer[i-2] || answer[i] == answer[i-1])
          {
            answer[i] = random(8);
          }
        }
      }
      // start the game and reset variables
      updateSelection();
      resetGuesses();
    }
  }
  
  // check if left button is pressed
  if(digitalRead(leftButton) == 1)
  {
    // if the slot is not smaller than 0 then go one slot left
    if(currentSlot > 0)
    {
      currentSlot--;
      lcd.clear();
      delay(200);
    }
  }
  // check if right button is pressed
  if(digitalRead(rightButton) == 1)
  {
    // if the slot is not bigger than 4 go one slot right
    if(currentSlot < 4)
    {
      currentSlot++;
      lcd.clear();
      delay(200);
    }
  }
  //if the middle button is pressed cycle through different colors
  if(digitalRead(middleButton) == 1)
  {
    switch(difficulty)
    {
      case 1:
      	if(selectionLED[currentSlot] < 4)
        {
          selectionLED[currentSlot]++;
          delay(200);
        }
      	if(selectionLED[currentSlot] == 4)
        {
          selectionLED[currentSlot] = 0;
        }
      	break;
      case 2:
      	if(selectionLED[currentSlot] < 6)
        {
          selectionLED[currentSlot]++;
          delay(200);
        }
      	if(selectionLED[currentSlot] == 6)
        {
          selectionLED[currentSlot] = 0;
        }
      	break;
      case 3:
      	if(selectionLED[currentSlot] < 8)
        {
          selectionLED[currentSlot]++;
          delay(200);
        }
      	if(selectionLED[currentSlot] == 8)
        {
          selectionLED[currentSlot] = 0;
        }
      	break;
    }
    // update the selection led strip
    updateSelection();
  }
  
  // if the difficulty has been selected
  if(selectDifficulty != true)
  {
    // indicate where the slot being changed is
    switch(currentSlot)
  	{
   	  case 0:
    	lcd.setCursor(0,1);
    	lcd.print("()");
    	break;
      
      case 1:
    	lcd.setCursor(4,1);
    	lcd.print("()");
    	break;
      
      case 2:
    	lcd.setCursor(9,1);
    	lcd.print("()");
    	break;
      
      case 3:
    	lcd.setCursor(14,1);
    	lcd.print("()");
    	break;
  	}
  }
  // if the answer button is pressed drop the leds
  if(digitalRead(answerButton) == 1)
  {
    dropGuess();
  }
  // update the hint on the lcd
  updateHint();
  // if the user has won show the win screen
  if(winner)
  {
    winScreen();
  }
  // if the user has lost show the lose screen
  if(loser)
  {
    loseScreen();
  }
}
/*
updateSelection()

Use: update the colours in the selection strip
*/
void updateSelection()
{
  // for every led in the strip
  for(int i = 0; i < 4; i++)
  {
    // for each colour change the colour
    switch(selectionLED[i])
    {
      case 0:
      	picking.setPixelColor(i, pixels.Color(100, 100, 100));
      	break;
      case 1:
      	picking.setPixelColor(i, pixels.Color(255, 0, 0));
      	break;
      case 2:
      	picking.setPixelColor(i, pixels.Color(0, 255, 0));
      	break;
      case 3:
      	picking.setPixelColor(i, pixels.Color(0, 0, 255));
      	break;
      case 4:
      	picking.setPixelColor(i, pixels.Color(0, 155, 155));
      	break;
      case 5:
      	picking.setPixelColor(i, pixels.Color(155, 0, 155));
      	break;
      case 6:
      	picking.setPixelColor(i, pixels.Color(155, 155, 0));
      	break;
      case 7:
      	picking.setPixelColor(i, pixels.Color(155, 50, 80));
      	break;
    }
    
    for(int i = 0; i < 4;i++)
    {
      switch(answer[i])
    {
      case 0:
      	answerLED.setPixelColor(i, pixels.Color(100, 100, 100));
      	break;
      case 1:
      	answerLED.setPixelColor(i, pixels.Color(255, 0, 0));
      	break;
      case 2:
      	answerLED.setPixelColor(i, pixels.Color(0, 255, 0));
      	break;
      case 3:
      	answerLED.setPixelColor(i, pixels.Color(0, 0, 255));
      	break;
      case 4:
      	answerLED.setPixelColor(i, pixels.Color(0, 155, 155));
      	break;
      case 5:
      	answerLED.setPixelColor(i, pixels.Color(155, 0, 155));
      	break;
      case 6:
      	answerLED.setPixelColor(i, pixels.Color(155, 155, 0));
      	break;
      case 7:
      	answerLED.setPixelColor(i, pixels.Color(155, 50, 80));
      	break;
      
    }
    }
  }
  // update the leds
  picking.show();
  answerLED.show();
}
/*
dropGuess()

Use: transfer the leds from selection to the guesses
*/
void dropGuess()
{
  //for each slot
  for(int i = 0; i < 4; i++)
  {
    // copy over the leds and if it is correct add to the value
    slotLED[currentGuess][i] = selectionLED[i];
    if(selectionLED[i] == answer[i])
    {
      correct++;
    }
  }
  // store the number of correct in the hint
  guessHint[currentGuess][0] = correct;
  
  // check if leds are correct but in the wrong spot
  if(selectionLED[0] == answer[1] || selectionLED[0] == answer[2] || selectionLED[0] == answer[3])
  {
    wrongSpot++;
  }
  if(selectionLED[1] == answer[0] || selectionLED[1] == answer[2] || selectionLED[1] == answer[3])
  {
    wrongSpot++;
  }
  if(selectionLED[2] == answer[0] || selectionLED[2] == answer[1] || selectionLED[2] == answer[3])
  {
    wrongSpot++;
  }
  if(selectionLED[3] == answer[0] || selectionLED[3] == answer[1] || selectionLED[3] == answer[2])
  {
    wrongSpot++;
  }
  
  // store value in the hint and update for next turn
  guessHint[currentGuess][1] = wrongSpot;
  updateLED();
  checkAnswer();
  currentGuess++;
  wrongSpot = 0;
  correct = 0;
  if(currentGuess == 6)
  {
    loser = true;
  }
}

/*
updateLED()

Use: update each led colour in the main array of leds
*/
void updateLED()
{
  // for every slot change the colour in the slot
  for(int i = 0; i < 4; i++)
  {
    switch(selectionLED[i])
    {
      case 0:
      	pixels.setPixelColor(i+(currentGuess*4), pixels.Color(100, 100, 100));
      	break;
      case 1:
      	pixels.setPixelColor(i+(currentGuess*4), pixels.Color(255, 0, 0));
      	break;
      case 2:
      	pixels.setPixelColor(i+(currentGuess*4), pixels.Color(0, 255, 0));
      	break;
      case 3:
      	pixels.setPixelColor(i+(currentGuess*4), pixels.Color(0, 0, 255));
      	break;
      case 4:
      	pixels.setPixelColor(i+(currentGuess*4), pixels.Color(0, 155, 155));
      	break;
      case 5:
      	pixels.setPixelColor(i+(currentGuess*4), pixels.Color(155, 0, 155));
      	break;
      case 6:
      	pixels.setPixelColor(i+(currentGuess*4), pixels.Color(155, 155, 0));
      	break;
      case 7:
      	pixels.setPixelColor(i+(currentGuess*4), pixels.Color(155, 50, 80));
      	break;
    }
  }
  // update the leds
  pixels.show();
}
/*
resetGuesses

Use: reset each slot of the board at the start of the game
*/
void resetGuesses()
{
  // for each slot reset the value
  for(int i = 0; i < 7; i++)
  {
    for(int j = 0; j < 2; j++)
    {
      guessHint[i][j] == 0;
    }
  }
  correct = 0;
  wrongSpot = 0;
}

/*
checkAnswer()

Use: Checks the answer after each turn
*/
void checkAnswer()
{
  // for each slot in the selection
  for(int i = 0; i < 4; i++)
  {
    // check if correct
    if(selectionLED[i] == answer[i])
    {
      correctSpots++;
    }
  }
  // if all are correct make winner true
  if(correctSpots == 4)
  {
    winner = true;
  }
  correctSpots = 0;
}

/*
winScreen()

Use: show the win screen to the user and reset board for next game
*/
void winScreen()
{
  // print you win
  lcd.clear();
  lcd.print("YOU WIN!!!!");
  pixels.clear();
  // turn all pixels white and show winning combo on selection leds
  for(int i = 0; i < 4; i++)
  {
    for(int j = 0; j < 7; j++)
    {
      pixels.setPixelColor(i*j, pixels.Color(100, 100, 100));
    }
  }
  pixels.show();
  delay(3000);
  
  // reset each variable
  selectDifficulty = true;
  winner = false;
  loser = false;
  correctSpots = 0;
  difficulty;
  currentSlot = 0;
  currentGuess = 0;
  correct = 0;
  wrongSpot = 0;
  
  currentHint = 0;
  
  resetGuesses();
  lcd.clear();
}
/*
updateHint()

Use: updates the hint on the lcd with button presses
*/
void updateHint()
{
  // check if up or down button is pressed to cycle through hints
  if(digitalRead(upButton) == 1 && currentHint > 0)
  {
    currentHint--;
  }
  if(digitalRead(downButton) == 1 && currentHint < 6)
  {
    currentHint++;
  }
  // print out the current guesses hint
  lcd.setCursor(0,0);
  lcd.print("Guess:");
  lcd.print(currentHint);
  lcd.print(" C:");
  lcd.print(guessHint[currentHint][0]);
  lcd.print(" R:");
  lcd.print(guessHint[currentHint][1]);
}
/*
loseScreen()

Use: show the lose screen to the user and reset board for next game
*/
void loseScreen()
{
  // print you win
  lcd.clear();
  lcd.print("Sorry! You Lost!");
  lcd.setCursor(0,1);
  lcd.print("The Answer Is:");
  pixels.clear();
  // turn all pixels white and show winning combo on selection leds
  for(int i = 0; i < 4; i++)
  {
    for(int j = 0; j < 7; j++)
    {
      pixels.setPixelColor(i*j, pixels.Color(100, 100, 100));
    }
  }
  pixels.show();
  
  for(int i = 0; i < 4; i++)
  {
     switch(answer[i])
    {
      case 0:
      	picking.setPixelColor(i, pixels.Color(100, 100, 100));
      	break;
      case 1:
      	picking.setPixelColor(i, pixels.Color(255, 0, 0));
      	break;
      case 2:
      	picking.setPixelColor(i, pixels.Color(0, 255, 0));
      	break;
      case 3:
      	picking.setPixelColor(i, pixels.Color(0, 0, 255));
      	break;
      case 4:
      	picking.setPixelColor(i, pixels.Color(0, 155, 155));
      	break;
      case 5:
      	picking.setPixelColor(i, pixels.Color(155, 0, 155));
      	break;
      case 6:
      	picking.setPixelColor(i, pixels.Color(155, 155, 0));
      	break;
      case 7:
      	picking.setPixelColor(i, pixels.Color(155, 50, 80));
      	break;
    }
  }
  picking.show();
  delay(3000);
  
  // reset each variable
  selectDifficulty = true;
  winner = false;
  loser = false;
  correctSpots = 0;
  difficulty;
  currentSlot = 0;
  currentGuess = 0;
  correct = 0;
  wrongSpot = 0;
  
  currentHint = 0;
  
  resetGuesses();
  lcd.clear();
}








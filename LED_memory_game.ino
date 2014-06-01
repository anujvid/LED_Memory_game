/*

  "That memory game" - "Simon" clone using an Arduino kit

  LEDs light up with corresponding musical notes in a randomised sequence. Press the
  buttons in the same sequence to win a round. Starting difficulty level is configurable, as is the number
  of rounds you win before the difficulty level increases.

  tim.id.au/arduino/
  30/11/2013

*/

// Digital pin definitions
int leds[] = {10, 9, 8, 7}; // digital pins for green LEDs 1-4 and red LED
int led = 11;
int buttons[] = {6, 5, 4, 3}; // digital pins for buttons 1-4
int buzzer = 12; // digital pin for piezo module

// Define notes (freq values from Freetronics project #9)
int notes2[] = {261, 349, 493, 679};
/*              C    F    B    F
                0    3    6    10
*/

int notes[] = {261, 293, 329, 349, 392, 440, 493, 523, 587, 659, 679, 783, 880};
/*              C    D    E    F    G    A    B    C    D    E    F    G    H
                0    1    2    3    4    5    6    7    8    9    10   11   12
*/

// Game configuration
int Delay = 1000; // long pause between things
int Delay2 = 200; // medium pause
int Delay3 = 75; // short pause
int Difficulty = 1; // default difficulty level
int TurnsToLevelUp = 2; // difficulty level increments this many turns
bool sounds = true; // change to false to mute the game

int LevelUp = 0; // Not game config but needs to be global to persist past loops

void setup()
  {
  randomSeed(analogRead(0));
  pinMode(buzzer, OUTPUT); // Piezo pin is an output
  pinMode(led, OUTPUT);
  for (int i = 0; i < 5; i++)
    pinMode(leds[i], OUTPUT); // LED pins are outputs
  for (int i = 0; i < 4; i++)
    pinMode(buttons[i], INPUT); // Button pins are inputs
  }

void result(int result)
  {
  if (result == 0) // Wrong move
    {
    for (int i = 0; i < 3; i++)
      {
      note(12);
      on(leds[0]);
      on(leds[1]);
      on(leds[2]);
      on(leds[3]);
      delay(Delay3);
      off(leds[0]);
      off(leds[1]);
      off(leds[2]);
      off(leds[3]);
      delay(Delay3);
      }
    for (int i = 0; i < 6; i++)
      {
      on(leds[4]); note(0); delay(Delay/10);
      off(leds[4]); delay(Delay/10);
      }
    }
  if (result == 1) // Correct tune
    {
    for (int i = 0; i < 3; i++) // victory sounds and lights
      {
      on(leds[0]); note(0); delay(Delay3); off(leds[0]);
      on(leds[1]); note(3); delay(Delay3); off(leds[1]);
      on(leds[2]); note(6); delay(Delay3); off(leds[2]);
      on(leds[3]); note(10); delay(Delay3); off(leds[3]);
      }
    }  
  if (result == 2) // Level up!
    {
    for (int i = 0; i < 3; i++) // victory sounds and lights
      {
      on(leds[0]); note(0); delay(Delay3); off(leds[0]);
      on(leds[1]); note(3); delay(Delay3); off(leds[1]);
      on(leds[2]); note(6); delay(Delay3); off(leds[2]);
      on(leds[3]); note(10); delay(Delay3); off(leds[3]);
      on(leds[2]); note(6); delay(Delay3); off(leds[2]);
      on(leds[1]); note(3); delay(Delay3); off(leds[1]);
      }
    on(leds[0]); note(0); delay(Delay3); off(leds[0]);
    delay(Delay3);
    for (int i = 0; i < 6; i++)
      {
      on(leds[0]); on(leds[2]); note(10);
      delay(Delay3);
      off(leds[0]); off(leds[2]);
      on(leds[1]); on(leds[3]); note(0);
      delay(Delay3);
      off(leds[1]); off(leds[3]);
      }
    }
  }
  
void on(int Pin) // Turn an LED on
  {
  digitalWrite(Pin, HIGH);
  }

void off(int Pin) // Turn an LED off
  {
  digitalWrite(Pin, LOW);
  }

void note(int WhichNote) // sounds a tone with a certain freq and length - does not pause
  {
  if (sounds) // check if program is muted before actually doing anything
    tone(buzzer, notes[WhichNote], Delay2);
  }

void loop()
  {
 
  // Establish difficulty level for this round
  int const Level = Difficulty; // Difficulty changes if successive rounds are won
  
  // Generate tune for player to repeat
  int tune[Level]; // Initialise array for tune
  int i = 0;
  while (i < Level) // Generate random tune of length Level with no immediate repeated notes
    {
    int j = random(4);
    if (i == 0 || j != tune[i-1]) // You don't want to reference tune[-1] which it will otherwise if i=0
      tune[i++] = j;
    }
  
  for (int i = 0; i < Level; i++) // Play and display the tune
    {
    int j = tune[i];
    on(leds[j]);
    note(j);
    delay(Delay2);
    off(leds[j]);
    delay(Delay2);
    }
  
  int p = 0;
  while (p < Level)
    {
    
    int correct = tune[p];
    if (digitalRead(buttons[0]) == HIGH || digitalRead(buttons[1]) == HIGH ||
        digitalRead(buttons[2]) == HIGH || digitalRead(buttons[3]) == HIGH) // If any button is pushed...
      if (digitalRead(buttons[correct]) == HIGH) // ...and it happens to be the correct one...
        {
        on(leds[correct]); // Flash the correct LED
        note(tune[p]); // Play the note
        delay(Delay2); // Wait for a moment
        off(leds[correct]); // Turn the LED off
        p++; // Advance to the next note in the tune
        }
      else // ...or if it's not the right one...
        {
        break; // Get out of the while loop
        }
     
    }
  
  if (p == Level && LevelUp < TurnsToLevelUp) // Correct tune but not at the level cap yet
    {
    LevelUp++;
    result(1);
    }
  
  else if (p == Level) // Level up!
    {
    result(2);
    LevelUp = 0;
    Difficulty++;
    }
  
  else // You LOSE! Good DAY sir!
    result(0);
 
  delay(Delay);
   
  }

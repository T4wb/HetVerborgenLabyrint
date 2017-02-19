// PIT2
// Het verboden labyrint
// Tawwab Djalielie

#include <TimerOne.h>
const int latchPin = 8; //Pin connected to Pin 12 of 74HC595 (Latch)
const int clockPin = 12; //Pin connected to Pin 11 of 74HC595 (Clock)
const int dataPin = 11; //Pin connected to Pin 14 of 74HC595 (Data)
const int speakerPin = 7;

const int btn_right = 2;
const int btn_up = 3;
const int btn_down = 4;
const int btn_left = 5;
unsigned long changeTime; 

uint8_t mazes[5][8];
int random_maze;
int player_position[2];
uint8_t history[8];
long counter = 0;
bool game_finished = true;


void move_horizontal(const int player_direction)
{
  if (player_direction == -1 && player_position[0] > 0 || player_direction == 1 && player_position[0] < 7) // controleert of de beweging binnen het speelveld valt 
  {
    history[player_position[1]] = history[player_position[1]] | (1 << player_position[0]); // zet de LED van player_position op het veld aan
    if ((mazes[random_maze][player_position[1]] & (1 << (player_position[0] + player_direction))) != (char)0) // controleert of de zet is toegestaan
    {
      player_position[0] += player_direction; // update player_positon
      history[player_position[1]] = history[player_position[1]] | (1 << player_position[0]); // schrijft de nieuwe player_position
    }
    else {
      collision_wall();
    }
  }
  else {
    collision_edge();
  }
  changeTime = millis(); // werkt changeTime bij
}

void move_vertical(const int player_direction)
{
  if (player_direction == -1 && player_position[1] > 0 || player_direction == 1 && player_position[1] < 7) // controleert of de beweging binnen het speelveld valt 
  {
    history[player_position[1]] = history[player_position[1]] | (1 << player_position[0]); // zet de LED van player_position op het veld aan
    if ((mazes[random_maze][player_position[1] + player_direction] & (1 << player_position[0])) != char(0)) // controleert of de zet is toegestaan
    {
      player_position[1] += player_direction; // update player_positon
      history[player_position[1]] = history[player_position[1]] | (1 << player_position[0]); // schrijft de nieuwe player_position
    }
    else {
      collision_wall();
    }
  }
  else {
    collision_edge();
  }
  changeTime = millis(); // werkt changeTime bij
  
}

void check_completion()
{
  if (player_position[0] == 7 && player_position[1] == 7) // controleer of de speler het spel heeft uitgespeeld
  {
    for (int z = 1000; z < 3000; z++)
    {
      tone(speakerPin, z, 100); // speel wingeluidje af
    }
    digitalWrite(speakerPin, LOW);
    game_finished = true; // start het spel opnieuw
  }
}

void collision_wall() {
  for (int z = 1500; z > 1000; z--)
  {
    tone(speakerPin, z, 100); // speel verliesgeluidje af
  }
  game_finished = true; // start het spel opnieuw
}

void collision_edge() {
  for (int z = 1500; z > 1000; z -= 100)
  {
    tone(speakerPin, z, 50); // speel botsgeluidje af
  }
}

void screenUpdate() {
  uint8_t row = B00000001;
  for (byte k = 0; k < 9; k++) {
    // Open up the latch ready to receive data
    digitalWrite(latchPin, LOW);
    shiftIt(~row );
    shiftIt(history[k]); // LED array
    // Close the latch, sending the data in the registers out to the
    digitalWrite(latchPin, HIGH);
    row = row << 1;
  }
}

void shiftIt(byte dataOut) {
  // Shift out 8 bits LSB first,
  // on rising edge of clock

  boolean pinState;

  //clear shift register read for sending data
  digitalWrite(dataPin, LOW);

  // for each bit in dataOut send out a bit
  for (int i = 0; i < 8; i++) {
    //set clockPin to LOW prior to sending bit
    digitalWrite(clockPin, LOW);

    // if the value of DataOut and (logical AND) a bitmask
    // are true, set pinState to 1 (HIGH)
    if ( dataOut & (1 << i) ) {
      pinState = HIGH;
    }
    else {
      pinState = LOW;
    }
    //sets dataPin to HIGH or LOW depending on pinState
    digitalWrite(dataPin, pinState);
    //send bit out on rising edge of clock
    digitalWrite(clockPin, HIGH);
    digitalWrite(dataPin, LOW);
  }
  //stop shifting
  digitalWrite(clockPin, LOW);
}

void setup() {
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);

  pinMode(btn_left, INPUT);
  pinMode(btn_up, INPUT);
  pinMode(btn_down, INPUT);
  pinMode(btn_right, INPUT);

  pinMode(speakerPin, OUTPUT);

  Timer1.initialize(10000);
  Timer1.attachInterrupt(screenUpdate);

  randomSeed(analogRead(0));
  random_maze = random(0,5);
}

void new_game() {
  mazes[0][7] = B11100000;
  mazes[0][6] = B00111111;
  mazes[0][5] = B00001001;
  mazes[0][4] = B00011001;
  mazes[0][3] = B00010000;
  mazes[0][2] = B00011111;
  mazes[0][1] = B00000001;
  mazes[0][0] = B00000001;

  mazes[1][7] = B10000000;
  mazes[1][6] = B10000000;
  mazes[1][5] = B11111000;
  mazes[1][4] = B00001000;
  mazes[1][3] = B10011000;
  mazes[1][2] = B10010000;
  mazes[1][1] = B11111100;
  mazes[1][0] = B00000111;

  mazes[2][7] = B11100000;
  mazes[2][6] = B00111111;
  mazes[2][5] = B00001001;
  mazes[2][4] = B00011001;
  mazes[2][3] = B00010000;
  mazes[2][2] = B00111111;
  mazes[2][1] = B00010001;
  mazes[2][0] = B00010001;

  mazes[3][7] = B11111111;
  mazes[3][6] = B00111111;
  mazes[3][5] = B00001001;
  mazes[3][4] = B00011001;
  mazes[3][3] = B00010000;
  mazes[3][2] = B00111111;
  mazes[3][1] = B00010001;
  mazes[3][0] = B00010001;

  mazes[4][7] = B11111111;
  mazes[4][6] = B00111111;
  mazes[4][5] = B00001001;
  mazes[4][4] = B00011001;
  mazes[4][3] = B00010001;
  mazes[4][2] = B00111111;
  mazes[4][1] = B00010001;
  mazes[4][0] = B00010001;

  for (int p = 0; p < 8; p++)
  {
    history[p] = B00000000;
  }

  player_position[0] = 0; // x-positie speler
  player_position[1] = 0; // y-positie speler
}

void loop() {
  if (game_finished) {
    game_finished = false;
    new_game();
  }

  if ((millis() - changeTime) > 150) // wordt gebruikt om te voorkomen dat de gebruiker per ongeluk dubbel op een knop drukt.
  {
    if (digitalRead(btn_left) == HIGH) {
      move_horizontal(-1);
    }
    if (digitalRead(btn_right) == HIGH) {
      move_horizontal(1);
    }
    if (digitalRead(btn_up) == HIGH) {
      move_vertical(-1);
    }
    if (digitalRead(btn_down) == HIGH) {
      move_vertical(1);
    }
    check_completion();
  }

  counter++;
  if (counter >= 10000) {
    counter = 0;
    history[player_position[1]] = history[player_position[1]] ^ (1 << player_position[0]);
  }
}

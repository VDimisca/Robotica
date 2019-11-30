#include <LiquidCrystal.h>
#include <string.h>
#include <EEPROM.h>

const int MAX_NAME_LEN = 10;

const int RSPin = 12;
const int EPin = 11;
const int D4Pin = 5;
const int D5Pin = 4;
const int D6Pin = 3;
const int D7Pin = 2;

LiquidCrystal lcd(RSPin, EPin, D4Pin, D5Pin, D6Pin, D7Pin);

unsigned long int lastDisplayMillis = 0;
int displayDelay = 200;
int startingLevelValue = 0;
int currentLives = 3;
unsigned long int startingTime = 0;
int menuSelected = 1;
int menuWasClicked = 0;

int functionJustStarted = 1;

int gameHighScore = 0;
char bestPlayer[MAX_NAME_LEN + 1] = "Unknown";
char currentPlayer[MAX_NAME_LEN + 1] = "Player";
int charNo = 0;
int justPass = 0;

const int pinX = A5;
const int pinY = A4;
const int buttonPin = 8;

const int lowAxis = 100;
const int highAxis = 800;
int xAxisReset = 1;
int yAxisReset = 1;
int lastButtonValue = 1;

int button_pressed() {
  int buttonValue = digitalRead(buttonPin);
  if (buttonValue == 0 && lastButtonValue == 1) {
    lastButtonValue = 0;
    return 1;
  }
  if (buttonValue == 1 && lastButtonValue == 0) {
      lastButtonValue = 1;
  }
  return 0;
}

void save_highscore() {
  int v[MAX_NAME_LEN + 1];
  for (int i = 0; i <= MAX_NAME_LEN; i++) {
    v[i] = (int)bestPlayer[i];
  }
  EEPROM[0] = gameHighScore;
  for (int i = 0; i <= MAX_NAME_LEN; i++) {
    EEPROM[i + 1] = v[i];
  }
}

void load_highscore() {
  if (EEPROM[0] == 0) {
    return;
  }
  int v[MAX_NAME_LEN + 1];
  for (int i = 0; i <= MAX_NAME_LEN; i++) {
    v[i] = EEPROM[i + 1];
  }
  Serial.print("\n");
  gameHighScore = EEPROM[0];
  for (int i = 0; i <= MAX_NAME_LEN; i++) {
    bestPlayer[i] = (char)v[i];
    Serial.print(v[i]);
    Serial.print(" ");
    Serial.println(bestPlayer[i]);
  }
}

void clear_disk() {
  for (int i = 0 ; i < EEPROM.length() ; i++) {
    EEPROM.write(i, 0);
  }
}

void display_game_status(int currentLives, int levelValue, int currentScore) {
  if (millis() - lastDisplayMillis > displayDelay) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Lives:");
    lcd.print(currentLives);
    lcd.print(" Level:");
    lcd.print(levelValue);
    lcd.setCursor(0, 1);
    lcd.print("Score: ");
    lcd.print(currentScore);
    lastDisplayMillis = millis();
  }
}

int game_ended() {
  if (millis() - startingTime > 9900) {
    return 1;
  }
  return 0;
}

void play_game() {
  if (functionJustStarted) { // Game Setup
    currentLives = 3;
    startingTime = millis();
    functionJustStarted = 0;
  }
  int levelValue = startingLevelValue + (millis() - startingTime)/5000;
  int currentScore = levelValue * 3;
  if (currentScore > gameHighScore) {
    gameHighScore = currentScore;
    strncpy(bestPlayer, currentPlayer, MAX_NAME_LEN + 1);
    save_highscore();
  }
  display_game_status(currentLives, levelValue, currentScore);
  if (game_ended()) {
    lcd.clear();
    lcd.print("Congratulations!");
    lcd.setCursor(0, 1);
    lcd.print("Press the button");
    while (1) {
      if (button_pressed()) {
        menuWasClicked = 0;
        functionJustStarted = 1;
        break;
      }
    }
  }
}

void add_name() {
  while (Serial.available() > 0) {
    int incomingByte = Serial.read();
    if (justPass) {
      continue;
    }
    char ch = (char)incomingByte;
    if (incomingByte != 10) {
      currentPlayer[charNo] = ch;
      charNo++;
    }
    else {
      currentPlayer[charNo] = NULL;
      charNo = 0;
      justPass = 1;
    }
    if (charNo == MAX_NAME_LEN) {
      currentPlayer[charNo] = NULL;
      charNo = 0;
      justPass = 1;
    }
  }
  justPass = 0;
}

void change_settings() {
  if (millis() - lastDisplayMillis > displayDelay) {
    lcd.clear();
    lcd.print("Level: ");
    lcd.print(startingLevelValue);
    lcd.setCursor(0, 1);
    lcd.print(currentPlayer);
    lastDisplayMillis = millis();
  }
  int xAxis = analogRead(pinX);
  if (xAxis < lowAxis && xAxisReset) {
    startingLevelValue--;
    if (startingLevelValue < 0) {
      startingLevelValue = 9;
    }
    xAxisReset = 0;
  }
  else if (xAxis > highAxis && xAxisReset) {
    startingLevelValue++;
    if (startingLevelValue > 9) {
      startingLevelValue = 0;
    }
    xAxisReset = 0;
  }
  else if (xAxis >= lowAxis && xAxis <= highAxis){
    xAxisReset = 1;
  }
  add_name();
  if (button_pressed()) {
    menuWasClicked = 0;
  }
}

void display_high_score() {
  lcd.clear();
  lcd.print(bestPlayer);
  lcd.print("  ");
  lcd.print(gameHighScore);
  while (1) {
    if (button_pressed()) {
      menuWasClicked = 0;
      break;
    }
  }
}

void display_menu() {
  if(millis() - lastDisplayMillis > displayDelay) {
    lcd.clear();
    lcd.setCursor(0, 0);
    if (menuSelected == 1) {
      lcd.print(">");
    }
    else {
      lcd.print(" ");
    }
    lcd.print("Start ");
    if (menuSelected == 2) {
      lcd.print(">");
    }
    else {
      lcd.print(" ");
    }
    lcd.print("Setup");
    
    lcd.setCursor(0, 1);
    if (menuSelected == 3) {
      lcd.print(">");
    }
    else {
      lcd.print(" ");
    }
    lcd.print("HighScore ");
    
    lastDisplayMillis = millis();
  }
}

void setup() {
  lcd.begin(16, 2);
  pinMode(pinX, INPUT);
  pinMode(pinY, INPUT);
  pinMode(buttonPin, INPUT_PULLUP);
  Serial.begin(9600);
  clear_disk();
  load_highscore();
}

void loop() {
  
  if (!menuWasClicked) {
    if (button_pressed()){
      menuWasClicked = 1;
    }
    else {
      int xAxis = analogRead(pinX);
      if (xAxis < lowAxis && xAxisReset) {
        menuSelected--;
        if (menuSelected < 1) {
          menuSelected = 3;
        }
        xAxisReset = 0;
      }
      else if (xAxis > highAxis && xAxisReset) {
        menuSelected++;
        if (menuSelected > 3) {
          menuSelected = 1;
        }
        xAxisReset = 0;
      }
      else if (xAxis >= lowAxis && xAxis <= highAxis){
        xAxisReset = 1;
      }
      display_menu();
    }
  }
  else {
    if (menuSelected == 1) {
      play_game();    
    }
    else if (menuSelected == 2) {
      change_settings();
    }
    else if (menuSelected == 3) {
      display_high_score();
    }
  }
}

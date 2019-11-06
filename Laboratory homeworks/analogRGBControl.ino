const int redPin = 9;
const int greenPin = 10;
const int bluePin = 11;

const int redInput = A0;
const int greenInput = A1;
const int blueInput = A2;

int redValue;
int greenValue;
int blueValue;

void setup() {
  pinMode(redInput, INPUT);
  pinMode(greenInput, INPUT);
  pinMode(blueInput, INPUT);
  
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);

  Serial.begin(9600);
}

void setColor(int redValue, int greenValue, int blueValue){
  analogWrite(redPin, redValue);
  analogWrite(greenPin, greenValue);
  analogWrite(bluePin, blueValue);
}

void loop() {
  int potRed, potGreen, potBlue;

  potRed = analogRead(redInput);
  potGreen = analogRead(greenInput);
  potBlue = analogRead(blueInput);

  redValue = map(potRed, 0, 1023, 0, 255);
  blueValue = map(potGreen, 0, 1023, 0, 255);
  greenValue = map(potBlue, 0, 1023, 0, 255);
  
  setColor(redValue, greenValue, blueValue);

  Serial.println(String(redValue)+","+String(greenValue)+","+String(blueValue));
}

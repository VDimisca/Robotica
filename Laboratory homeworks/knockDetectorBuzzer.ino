const int speakerPin = A0;
int speakerValue = 0;

const int threshold = 1;

unsigned long previousMillis = 0;
unsigned long currentMillis = 0;
const long interval = 5000;

const int buzzerPin = 11;
int buzzerTone = 1000;
int buzzerState = 0;

const int pushButton = 2;
int buttonState = 0;

void setup() {
    pinMode(speakerPin, INPUT);
    pinMode(buzzerPin, OUTPUT);
    pinMode(pushButton, INPUT_PULLUP);
    Serial.begin(9600);
}

void loop() {
  
speakerValue = analogRead(speakerPin);
if (speakerValue != 0)
Serial.println(speakerValue);

if (speakerValue > threshold) {
    Serial.println("Knock");
    buzzerState = 1;
}

 currentMillis = millis();
 
 if (buzzerState == 1){
    previousMillis = currentMillis;
    buzzerState = 0;
 }
 
 if (currentMillis - previousMillis >= interval) {
   tone(buzzerPin, buzzerTone);
   previousMillis = currentMillis;
   } 
    
buttonState = digitalRead(pushButton);
   
if (buttonState == LOW){
  noTone(buzzerPin);
  }
        
}

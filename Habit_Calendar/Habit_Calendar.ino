#include <FastLED.h>

#define DATA_PIN    13
#define LED_TYPE    WS2811
#define COLOR_ORDER GRB
#define NUM_LEDS    28
CRGB leds[NUM_LEDS];
const int buttonPin = 2;

#define BRIGHTNESS          100
#define FRAMES_PER_SECOND  10

bool gReverseDirection = false;
int buttonState = 0; 
int day = 1;
int hue = 100;
unsigned long newTime;
unsigned long oldTime;
unsigned long startTime;
unsigned long timeLimit = 86400000;
unsigned long threshold = 600000;
unsigned long c;
bool buttonReady = true;
bool celebrate = false;
bool started = false;
bool down = true;
bool missed = false;

void setup() {
  // put your setup code here, to run once:
  delay(3000); // 3 second delay for recovery
  // initialize the pushbutton pin as an input:
  pinMode(buttonPin, INPUT);
  FastLED.addLeds<LED_TYPE,DATA_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  // set master brightness control
  FastLED.setBrightness(BRIGHTNESS);
  // reset LEDs to home state
  reset();
  oldTime = millis();
  Serial.begin(9600);
  Serial.print("Program started at: ");
  Serial.println(oldTime); //prints time since program started
}

void loop() {
  // read the state of the pushbutton value:
  buttonState = digitalRead(buttonPin);
  newTime = millis();
  if(day == NUM_LEDS){
    if(missed == false){
      Serial.println("FIREWORKS! -------");
      Fire2012(); // run simulation frame
      FastLED.show(); // display this frame
      FastLED.delay(1000 / FRAMES_PER_SECOND);
    }else{
      Serial.println("Missed out fireworks");
      fade(day-1, 160);
    }
    buttonReady = true;
    celebrate = true;
  }
  
  // check if the pushbutton is pressed.
  // if it is, the buttonState is HIGH:
  if (buttonState == HIGH && buttonReady == true) {
    Serial.print("Button pressed at: ");
    Serial.println(newTime);
    buttonReady = false;
    iteration();
  }
  if(started == false){
    fade(day, 100);
  }
  
  if(celebrate == false && started == true){
    if(newTime >= startTime + timeLimit){
      Serial.println("Day over");
      if(buttonReady == true){
        if(day < NUM_LEDS){
          Serial.print("Missed habit today at: ");
          Serial.println(newTime);
          startTime = millis();
          leds[day] = CHSV(0,255,BRIGHTNESS);
          day++;
          for(int k = 0; k < BRIGHTNESS; k++){
            leds[day] = CHSV(100,255,k);
            FastLED.show();
            FastLED.delay(100/FRAMES_PER_SECOND);
          }
          hue = 100;
          buttonReady = true;
          missed = true;
        }
      }
      else{
        if(day < NUM_LEDS){
          Serial.print("Completed habit today at: ");
          Serial.println(newTime);
          startTime = millis();
          day++;
          for(int k = 0; k < BRIGHTNESS; k++){
            leds[day] = CHSV(100,255,k);
            FastLED.show();
            FastLED.delay(100/FRAMES_PER_SECOND);
          }
          hue = 100;
          buttonReady = true;
        }
      }
      Serial.print("It is now Day ");
      Serial.println(day);
    }
    else{
      if(buttonReady == true && (newTime-startTime) < (timeLimit-threshold)){
        fade(day, 100);
      }
      if(buttonReady == true && (newTime-startTime) > (timeLimit-threshold)){
        if(hue > 0){
          hue = hue-10;
        }
        Serial.print("Hue: ");
        Serial.println(hue);
        if(down == true){
          for(int i = BRIGHTNESS; i > 40; i--){
            leds[day] = CHSV(hue,255,i);
            FastLED.show();
            FastLED.delay(100/FRAMES_PER_SECOND);
          }
          down = false;
        }
        else{
          for(int i = 40; i < BRIGHTNESS; i++){
            leds[day] = CHSV(hue,255,i);
            FastLED.show();
            FastLED.delay(100/FRAMES_PER_SECOND);            
          }
          down = true;
        }
        //leds[day] = CHSV(hue,255,BRIGHTNESS);
        FastLED.show();
        FastLED.delay(1000/FRAMES_PER_SECOND);
        
      }
    }
  }
}

void iteration(){
  if(day == 0){
    startTime = millis();
    started = true;
  }
  for(int i = hue; i < 160; i++){
    leds[day] = CHSV(i,255,BRIGHTNESS);
    FastLED.show();
    FastLED.delay(100/FRAMES_PER_SECOND);
  }
  Serial.print("Day ");
  Serial.print(day);
  Serial.println(" checked");
  hue = 100;
  if(day+1 < NUM_LEDS){
      flutter(day+1);
  }
  if(day == NUM_LEDS && celebrate == true){
    reset();
  }
  if(day == NUM_LEDS-1){
    day++;
    delay(1000);
  }
  //oldTime = newTime;
}

void flutter(int x){
  for(int j = x; j < NUM_LEDS; j++){
    leds[j] = CHSV(160,255,BRIGHTNESS);
    FastLED.show();
    FastLED.delay(1000/FRAMES_PER_SECOND);
  }
  for(int j = x; j < NUM_LEDS; j++){
    leds[j] = CHSV(64,255,BRIGHTNESS);
    FastLED.show();
    FastLED.delay(1000/FRAMES_PER_SECOND);
  }
  for(int j = x; j < NUM_LEDS; j++){
    leds[j] = CHSV(64,255,0);
    FastLED.show();
    FastLED.delay(1000/FRAMES_PER_SECOND);
  }
}

void reset(){
  Serial.println("------ Reset -------");
  for(int i = 0; i < NUM_LEDS; i++){
    leds[i] = CHSV(160,255,0);
    FastLED.show();
    FastLED.delay(1000/FRAMES_PER_SECOND);
  }
  for(int i = 0; i < NUM_LEDS; i++){
    leds[i] = CHSV(160,255,BRIGHTNESS);
    FastLED.show();
    FastLED.delay(1000/FRAMES_PER_SECOND);
  }
  for( int j = BRIGHTNESS; j > BRIGHTNESS-70; j--){  
    j = j-4;
    for(int i = 0; i < NUM_LEDS; i++){
      leds[i] = CHSV(160,255,j);
      FastLED.show();
    }
    FastLED.delay(100/FRAMES_PER_SECOND);
  }
  for(int i = 0; i < NUM_LEDS; i++){
    leds[i] = CHSV(160,255,0);
    FastLED.show();
  }
  day = 0;
  buttonReady = true;
  celebrate = false;
  started = false;
  missed = false;
  leds[day] = CHSV(96,255,BRIGHTNESS);
  FastLED.show();
}

void fade(int d, int c){
  if(down == false){
    for(int i = BRIGHTNESS; i > BRIGHTNESS-50; i--){
      leds[d] = CHSV(c,255,i);
      
      FastLED.show();
      FastLED.delay(100/FRAMES_PER_SECOND);
    }
    down = true;
  }
  else{
    for(int i = BRIGHTNESS-50; i < BRIGHTNESS; i++){
      leds[d] = CHSV(c,255,i);
      
      FastLED.show();
      FastLED.delay(100/FRAMES_PER_SECOND);
    }
    down = false;
  }
  Serial.print("States: buttonReady: "); 
  Serial.print(buttonReady);
  Serial.print(" celebrate: "); 
  Serial.print(celebrate);
  Serial.print(" started: ");
  Serial.print(started);
  Serial.print(" down: ");
  Serial.println(down);
}



void cycle(){
    for(int i = 0; i < NUM_LEDS; i++){
    if(i>0){
      leds[i-1] = CHSV(160,255,BRIGHTNESS);
    }
    leds[i] = CHSV(96,255,255);
    FastLED.show();
    if(i+1 < NUM_LEDS){
      flutter(i+1);
    }
    FastLED.delay(10000/FRAMES_PER_SECOND);
  }
  leds[6] = CHSV(160,255,BRIGHTNESS);
  FastLED.show();
  FastLED.delay(10000/FRAMES_PER_SECOND);
}

// Fire2012 by Mark Kriegsman, July 2012
// as part of "Five Elements" shown here: http://youtu.be/knWiGsmgycY
//// 
// This basic one-dimensional 'fire' simulation works roughly as follows:
// There's a underlying array of 'heat' cells, that model the temperature
// at each point along the line.  Every cycle through the simulation, 
// four steps are performed:
//  1) All cells cool down a little bit, losing heat to the air
//  2) The heat from each cell drifts 'up' and diffuses a little
//  3) Sometimes randomly new 'sparks' of heat are added at the bottom
//  4) The heat from each cell is rendered as a color into the leds array
//     The heat-to-color mapping uses a black-body radiation approximation.
//
// Temperature is in arbitrary units from 0 (cold black) to 255 (white hot).
//
// This simulation scales it self a bit depending on NUM_LEDS; it should look
// "OK" on anywhere from 20 to 100 LEDs without too much tweaking. 
//
// I recommend running this simulation at anywhere from 30-100 frames per second,
// meaning an interframe delay of about 10-35 milliseconds.
//
// Looks best on a high-density LED setup (60+ pixels/meter).
//
//
// There are two main parameters you can play with to control the look and
// feel of your fire: COOLING (used in step 1 above), and SPARKING (used
// in step 3 above).
//
// COOLING: How much does the air cool as it rises?
// Less cooling = taller flames.  More cooling = shorter flames.
// Default 50, suggested range 20-100 
#define COOLING  55

// SPARKING: What chance (out of 255) is there that a new spark will be lit?
// Higher chance = more roaring fire.  Lower chance = more flickery fire.
// Default 120, suggested range 50-200.
#define SPARKING 120


void Fire2012()
{
// Array of temperature readings at each simulation cell
  static byte heat[NUM_LEDS];

  // Step 1.  Cool down every cell a little
    for( int i = 0; i < NUM_LEDS; i++) {
      heat[i] = qsub8( heat[i],  random8(0, ((COOLING * 10) / NUM_LEDS) + 2));
    }
  
    // Step 2.  Heat from each cell drifts 'up' and diffuses a little
    for( int k= NUM_LEDS - 1; k >= 2; k--) {
      heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2] ) / 3;
    }
    
    // Step 3.  Randomly ignite new 'sparks' of heat near the bottom
    if( random8() < SPARKING ) {
      int y = random8(7);
      heat[y] = qadd8( heat[y], random8(160,255) );
    }

    // Step 4.  Map from heat cells to LED colors
    for( int j = 0; j < NUM_LEDS; j++) {
      CRGB color = HeatColor( heat[j]);
      int pixelnumber;
      if( gReverseDirection ) {
        pixelnumber = (NUM_LEDS-1) - j;
      } else {
        pixelnumber = j;
      }
      leds[pixelnumber] = color;
    }
}

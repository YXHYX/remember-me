// SCHEMATIC



const char leds[4] = {3,4,5,6};    // Led pins array 
const char buttons[4] = {7,8,9,10}; // signal pin of sensor to digital pin 5.
const char buzzer = 11;

const int buzzer_color[4] = {1500, 2000, 2500, 3000};

//button states
bool btnState1 = false;
bool btnState2 = false;
bool btnState3 = false;
bool btnState4 = false;

//max pattern length is 20
const unsigned short MAX_PATTERN = 5;
unsigned short pattern[MAX_PATTERN] = {0};
unsigned short recordedPattern[MAX_PATTERN] = {0};
unsigned short patternSize = 0;

bool gameOver = false;
//time before showing each bit in the pattern, decreases with time to make game more difficult 
long timeToWait = 500; // in millisecond

//for push buttons
const long debounceDelay = 50;

void setup()
{
    Serial.begin(9600);
    pinMode(buzzer, OUTPUT);
    for(int i = 0; i < 4; i++)
    {
        pinMode(leds[i], OUTPUT);   // set leds as outputs
        pinMode(buttons[i], INPUT_PULLUP); // buttons as inputs - note that if you're not using an external pullup resistor, do INPUT_PULLUP to use the internal one! 
    }
    Serial.print("Pattern: ");
    for(int i = 0; i < patternSize; i++)
    {
        pattern[i] = i;
        Serial.print(pattern[i]);
        Serial.print(" ");
    }
    Serial.println();

    //randomize seed
    randomSeed(analogRead(0));

}


void showPattern()
{
    //should not happen
    if(patternSize == 0)
        return;


    digitalWrite(leds[0], 0);
    digitalWrite(leds[1], 0);
    digitalWrite(leds[2], 0);
    digitalWrite(leds[3], 0);

    for(int i = 0; i < patternSize; i++)
    {    
        digitalWrite(leds[pattern[i]], 1);
        tone(buzzer, buzzer_color[pattern[i]], timeToWait);
        delay(timeToWait);
        
        digitalWrite(leds[pattern[i]], 0);
        noTone(buzzer);
        delay(timeToWait);
    }

    //decrease the timeToWait to increase Difficulty
    //timeToWait = double(timeToWait)/1.2;
}

void retrieveAnswer()
{
    
    digitalWrite(leds[0], 0);
    digitalWrite(leds[1], 0);
    digitalWrite(leds[2], 0);
    digitalWrite(leds[3], 0);

    bool prevButtonState = false;
    int i = 0;
    while(i < patternSize && !gameOver)
    {
        //reset button states
        btnState1 = false;
        btnState2 = false;
        btnState3 = false;
        btnState4 = false;
        //confirm button press
        long lastDebounceTime = millis();
        
        while(1)
        {
            //retrieve each answer
            bool reading1 = !digitalRead(buttons[0]);
            bool reading2 = !digitalRead(buttons[1]);
            bool reading3 = !digitalRead(buttons[2]);
            bool reading4 = !digitalRead(buttons[3]);
             
            Serial.print("reading1 :");
            Serial.println(reading1);
            Serial.print("reading2 :");
            Serial.println(reading2);
            Serial.print("reading3 :");
            Serial.println(reading3);
            Serial.print("reading4 :");
            Serial.println(reading4);
            bool reading = reading1||reading2||reading3||reading4;
            //if any button pressed
            if(prevButtonState!=reading)
                lastDebounceTime = millis();

            if(millis() - lastDebounceTime > debounceDelay )
            {
                // the button is still pressed after a certain time
                if(reading != false){
                    //update the readings and break
                    btnState1 = reading1;
                    btnState2 = reading2;
                    btnState3 = reading3;
                    btnState4 = reading4;
                    break;
                }
            }
            prevButtonState = reading;
        }
        
        //record the pattern
        if(btnState1)
            recordedPattern[i] = 0;
        if(btnState2)
            recordedPattern[i] = 1;
        if(btnState3)
            recordedPattern[i] = 2;
        if(btnState4)
            recordedPattern[i] = 3;
        
        //show corresponding color for each button and wait for a short time
        

        digitalWrite(leds[recordedPattern[i]], 1);
        tone(buzzer, buzzer_color[recordedPattern[i]], 300);
        delay(300);
        digitalWrite(leds[recordedPattern[i]], 0);

        if(recordedPattern[i] != pattern[i])
        {
            gameOver = true;
            break;
        }
        i++;

    }
}

void evaluate()
{
    delay(500);
    if(gameOver)
        //game is over, display loss and buzz at lower freq
        showGameOver();
    else
        // correct move, show corret move
        showCorrectGuess();
    
    delay(1000);
}

void showGameOver(){

    //rotate leds
    
    digitalWrite(leds[0], 0);
    digitalWrite(leds[1], 0);
    digitalWrite(leds[2], 0);
    digitalWrite(leds[3], 0);


    digitalWrite(leds[0], 1);
    tone(buzzer, 440, 300);
    delay(300);
    digitalWrite(leds[0], 0);
    
    digitalWrite(leds[1], 1);
    noTone(buzzer);
    delay(300);
    digitalWrite(leds[1], 0);

    digitalWrite(leds[3], 1);
    tone(buzzer, 400, 300);
    delay(300);
    digitalWrite(leds[3], 0);
}
void showCorrectGuess(){
    
    digitalWrite(leds[0], 0);
    digitalWrite(leds[1], 1);
    digitalWrite(leds[2], 0);
    digitalWrite(leds[3], 1);
    tone(buzzer, 1850, 300);
    delay(300);
    tone(buzzer, 1960, 300);
    delay(300);
    
    digitalWrite(leds[0], 1);
    digitalWrite(leds[1], 0);
    digitalWrite(leds[2], 1);
    digitalWrite(leds[3], 0);
    noTone(buzzer);
    delay(300);
    
    digitalWrite(leds[0], 1);
    digitalWrite(leds[1], 1);
    digitalWrite(leds[2], 1);
    digitalWrite(leds[3], 1);
    tone(buzzer, 2200, 300);
    delay(300);
    
    digitalWrite(leds[0], 0);
    digitalWrite(leds[1], 0);
    digitalWrite(leds[2], 0);
    digitalWrite(leds[3], 0);
}

void showGameWon()
{
    digitalWrite(leds[0], 1);
    digitalWrite(leds[1], 1);
    digitalWrite(leds[2], 1);
    digitalWrite(leds[3], 1);
    tone(buzzer, 2489, 300);
    delay(300);
    
    digitalWrite(leds[0], 0);
    digitalWrite(leds[1], 0);
    digitalWrite(leds[2], 0);
    digitalWrite(leds[3], 0);
    noTone(buzzer);
    delay(200);
    
    digitalWrite(leds[0], 1);
    digitalWrite(leds[1], 1);
    digitalWrite(leds[2], 1);
    digitalWrite(leds[3], 1);
    tone(buzzer, 2200, 1000);
    delay(1000);
    
    digitalWrite(leds[0], 0);
    digitalWrite(leds[1], 0);
    digitalWrite(leds[2], 0);
    digitalWrite(leds[3], 0);
}


void increasePattern(){
    if(gameOver){
        gameOver = false;
        //restart
        patternSize = 0;
    }
    
    patternSize++;
    if(patternSize<MAX_PATTERN)
    {
        //add new bit
        pattern[patternSize-1] = random(4);
    }
    else{
        //game won
        showGameWon();
        delay(1000);
        //reset the game
        patternSize = 0;
    }
}

void loop()
{ 
    // show pattern
    showPattern();
    // get answer
    retrieveAnswer();
    // show if it is correct
    evaluate();
    // add a new bit to the pattern
    increasePattern();

}

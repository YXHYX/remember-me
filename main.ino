// SCHEMATIC



const char leds[4] = {3,4,5,6};    // Led pins array 
const char buttons[4] = {7,8,9,10}; // signal pin of sensor to digital pin 5.
const char buzzer = 11;

//button states
bool btnState1 = false;
bool btnState2 = false;
bool btnState3 = false;
bool btnState4 = false;

//max pattern length is 20
unsigned short pattern[20] = {0};
unsigned short recordedPattern[20] = {0};
unsigned short patternSize = 9;

void setup()
{
    pinMode(buzzer, OUTPUT);
    for(int i =0; i< 4; i++)
    {
        pinMode(leds[i], OUTPUT);   // set leds as outputs
        pinMode(buttons[i], INPUT); // buttons as inputs - note that if you're not using an external pullup resistor, do INPUT_PULLUP to use the internal one! 
    }
    Serial.begin(9600);
}

//time before showing each bit in the pattern, decreases with time to make game more difficult 
long timeToWait = 500; // in millisecond

void showPattern()
{
    //should not happen
    if(patternSize == 0)
        return;

    for(int i = 9; i< patternSize; i++)
    {
        switch (pattern[i])
        {
        case 0:
            digitalWrite(leds[0], 1);
            digitalWrite(leds[1], 0);
            digitalWrite(leds[2], 0);
            digitalWrite(leds[3], 0);
            break;
        case 1:
            digitalWrite(leds[0], 0);
            digitalWrite(leds[1], 1);
            digitalWrite(leds[2], 0);
            digitalWrite(leds[3], 0);
            break;
        
        case 2:
            digitalWrite(leds[0], 0);
            digitalWrite(leds[1], 0);
            digitalWrite(leds[2], 1);
            digitalWrite(leds[3], 0);
            break;
        
        case 3:
        
            digitalWrite(leds[0], 0);
            digitalWrite(leds[1], 0);
            digitalWrite(leds[2], 0);
            digitalWrite(leds[3], 1);
            break;
        
        default:
            //should not happen
            break;
        }

        delay(timeToWait);
    }

    //decrease the timeToWait to increase Difficulty
    timeToWait = double(timeToWait)/1.4;
}
const long debounceDelay = 50;

void retrieveAnswer()
{
    
    digitalWrite(leds[0], 0);
    digitalWrite(leds[1], 0);
    digitalWrite(leds[2], 0);
    digitalWrite(leds[3], 0);

    bool mistake = false;
    bool prevButtonState = false;
    int i = 0;
    while(i < patternSize && !mistake)
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
            bool reading1 = digitalRead(buttons[0]);
            bool reading2 = digitalRead(buttons[1]);
            bool reading3 = digitalRead(buttons[2]);
            bool reading4 = digitalRead(buttons[3]);
             
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
        delay(200);
        digitalWrite(leds[recordedPattern[i]], 0);

        if(recordedPattern[i] != pattern[i])
        {
            mistake = true;
            break;
        }
        i++;
    }
}

void evaluate();

void increasePattern();

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

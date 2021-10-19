// Game discription: BOPIT
// The game is simply BopIt the player is given a limited amount of time
// to complete the given instruction on the LCD, if he fails
// he loses the game, if he succeeds he gets points and continues with the game
// as the game progresses the player has less and less time to complete the insturction
// and the rate at which events occur becomes much more rapid
//


#include <string.h>; // makes working with strings much easier
#include <LiquidCrystal.h>;

const int RS = 11, EN = 12, D4 = 2, D5 = 3, D6 = 4, D7 = 5;
LiquidCrystal lcd(RS, EN, D4, D5, D6, D7);

const int VRyPin = A1;
const int VRxPin = A0;
const int SWPin = 7;

int VRx = 0; // value read from the horizontal pot
int VRy = 0; // value read from the vertical pot
int SW = 0;  // value read from the switch

String events[5] = {
    "up",
    "down",
    "left",
    "right",
    "click"};

void setup()
{
    Serial.begin(9600);
    lcd.begin(16, 2); // set up number of columns and rows
    pinMode(SWPin, INPUT_PULLUP);
    delay(5000);
}

String getJoyStickPosition(int x, int y, int z)
{

    x = map(x, 0, 1000, 1, 3);
    y = map(y, 0, 1000, 1, 3);

    // Serial.print(x);
    // Serial.print("\t");
    // Serial.print(y);
    // Serial.print("\t");
    // Serial.print(x - y);

    if (z == 0)
    {
        return "click";
    }

    switch (x - y)
    {
    case -1:
    {
        if (x == 1)
        {
            return "up";
        }
        else
        {
            return "left";
        }
    }
    case 1:
    {
        if (x == 3)
        {
            return "down";
        }
        else
        {
            return "right";
        }
    }
    case 0:
    {
        return "center";
        break;
    }
    default:
    {
        return "error";
        break;
    }
    }
}
void handlePoints(int amount, bool Reset)
{
    lcd.setCursor(8, 0);
    lcd.print(amount);

    if (Reset)
    {
        lcd.setCursor(8, 0);
        lcd.print("------");
    }
}

// Game pre-sets

int score = 0;
int timePerRound = 3;      // seconds
int timeBetweenRounds = 5; // seconds

bool hasWon;

void loop()
{

    handlePoints(score, false);

    lcd.setCursor(0, 0);
    lcd.print("Bop It");

    int randomNumber = rand() % 5;
    String chosenEvent = events[randomNumber];

    Serial.println("Starting round");

    lcd.setCursor(0, 2);
    lcd.print(chosenEvent);

    for (int i = 0; timePerRound / 0.05 > i; i++)
    {

        delay(200);

        VRx = analogRead(VRxPin);
        VRy = analogRead(VRyPin);
        SW = digitalRead(SWPin);

        Serial.print(" SW: ");
        Serial.println(SW);

        // print the results to the Serial Monitor:
        String joyStickPos = getJoyStickPosition(VRx, VRy, SW);

        Serial.print(joyStickPos);

        if (joyStickPos == chosenEvent) {
            score = score + 1;
            handlePoints(score, false);
            hasWon = true;
            break;
        } else if (joyStickPos != "center") {
            handlePoints(score, true);
            hasWon = false;
            break;
        }
    }

    if (hasWon)
    {
        lcd.setCursor(0, 2);
        lcd.print("You passed!");
    }
    else
    {
        lcd.setCursor(0, 2);
        lcd.print("You Lost :(");
        score = 0;
    }

    delay(timeBetweenRounds * 800);
    lcd.clear();
}

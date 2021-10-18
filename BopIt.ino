#include <string.h>; // makes working with strings much easier
#include <LiquidCrystal.h>;

const int RS = 11, EN = 12, D4 = 2, D5 = 3, D6 = 4, D7 = 5;
LiquidCrystal lcd(RS, EN, D4, D5, D6, D7);

#define joyX A0
#define joyY A1

String events[5] = {
    "up",
    "down",
    "left",
    "right"};

void setup()
{
    Serial.begin(9600);
    lcd.begin(16, 2); // set up number of columns and rows
}

String getJoyStickPosition(int x, int y)
{

    x = map(x, 0, 1000, 1, 3);
    y = map(y, 0, 1000, 1, 3);

    // Serial.print(x);
    // Serial.print(y);
    // Serial.print("\t");
    // Serial.print(x - y);

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
    default:
    {
        return "center";
    }
    }
};

void handleRound(bool playerPassed)
{
    if (playerPassed)
    {
        lcd.clear();
        lcd.print("Good Job!");
    }
    else
    {
        lcd.clear();
        lcd.print("You lost :(");
    }
}

int score = 0;

void loop()
{
    int chosenNumber = rand() % 4;
    String chosenEvent = events[chosenNumber];

    bool playerPassedRound;

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(chosenEvent);

    for (int i; i < 120; i++)
    {
        int xValue = analogRead(joyX);
        int yValue = analogRead(joyY);

        String joyStickPos = getJoyStickPosition(xValue, yValue);

        if (joyStickPos == chosenEvent)
        {
            playerPassedRound = true;
            handleRound(true);
            break;
        }
        else if (joyStickPos == "center")
        {
            continue;
        }
        else
        {
            handleRound(false);
            playerPassedRound = true;
        }
    }

    
    lcd.clear();
    lcd.print("Next");
}

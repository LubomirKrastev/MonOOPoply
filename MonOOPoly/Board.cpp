#include "Board.h"
#include "Property.h"
#include "CardField.h"
#include "GoToJailField.h"
#include "ColorGroup.h"

Board::Board() {
    initializeBoard();
}

Board::~Board() {
    for (size_t i = 0; i < fields.size(); i++) {
        delete fields[i];
    }
    for (size_t i = 0; i < colorGroups.size(); i++) {
        delete colorGroups[i];
    }
}

void Board::setCardDeck(CardDeck* chanceDeck, CardDeck* communityDeck) {
    for (int i = 0; i < fields.size(); i++) {
        if (fields[i] == nullptr) {
            continue;
        }

        CardField* cardField = dynamic_cast<CardField*>(fields[i]);
        if (cardField != nullptr) {
            MyString fieldName = fields[i]->getName();
            if (fieldName == "Chance") {
                delete fields[i];
                fields[i] = new CardField("Chance", chanceDeck);
            }
            else if (fieldName == "Community Chest") {
                delete fields[i];
                fields[i] = new CardField("Community Chest", communityDeck);
            }
        }
    }
}

Field* Board::getField(int position) const {
    if (position >= 0 && position < fields.size()) {
        return fields[position];
    }
    return nullptr;
}

int Board::getSize() const {
    return fields.size();
}

void Board::initializeBoard() {
    ColorGroup* brown = new ColorGroup(Color::BROWN, "Brown", 50, 50);
    ColorGroup* lightBlue = new ColorGroup(Color::LIGHT_BLUE, "Light Blue", 50, 50);
    ColorGroup* pink = new ColorGroup(Color::PINK, "Pink", 100, 100);
    ColorGroup* orange = new ColorGroup(Color::ORANGE, "Orange", 100, 100);
    ColorGroup* red = new ColorGroup(Color::RED, "Red", 150, 150);
    ColorGroup* yellow = new ColorGroup(Color::YELLOW, "Yellow", 150, 150);
    ColorGroup* green = new ColorGroup(Color::GREEN, "Green", 200, 200);
    ColorGroup* darkBlue = new ColorGroup(Color::DARK_BLUE, "Dark Blue", 200, 200);

    colorGroups.pushBack(brown);
    colorGroups.pushBack(lightBlue);
    colorGroups.pushBack(pink);
    colorGroups.pushBack(orange);
    colorGroups.pushBack(red);
    colorGroups.pushBack(yellow);
    colorGroups.pushBack(green);
    colorGroups.pushBack(darkBlue);

    // GO - Start position
    fields.pushBack(new Property("GO", 0, 0));                        //0

    // Brown properties
    Property* mediterranean = new Property("Mediterranean Avenue", 60, 2, 10, 30, 90, 160, 250);
    mediterranean->setColorGroup(brown);
    fields.pushBack(mediterranean);                                   //1

    fields.pushBack(new CardField("Community Chest", nullptr));       //2

    Property* baltic = new Property("Baltic Avenue", 60, 4, 20, 60, 180, 320, 450);
    baltic->setColorGroup(brown);
    fields.pushBack(baltic);                                          //3

    // Income Tax
    fields.pushBack(new Property("Income Tax", 0, -200));             //4

    // Railroad
    fields.pushBack(new Property("Reading Railroad", 200, 25));       //5

    // Light Blue properties
    Property* oriental = new Property("Oriental Avenue", 100, 6, 30, 90, 270, 400, 550);
    oriental->setColorGroup(lightBlue);
    fields.pushBack(oriental);                                        //6

    fields.pushBack(new CardField("Chance", nullptr));                //7

    Property* vermont = new Property("Vermont Avenue", 100, 6, 30, 90, 270, 400, 550);
    vermont->setColorGroup(lightBlue);
    fields.pushBack(vermont);                                         //8

    Property* connecticut = new Property("Connecticut Avenue", 120, 8, 40, 100, 300, 450, 600);
    connecticut->setColorGroup(lightBlue);
    fields.pushBack(connecticut);                                     //9

    // Jail
    fields.pushBack(new Property("Jail/Just Visiting", 0, 0));        //10

    // Pink properties
    Property* stCharles = new Property("St. Charles Place", 140, 10, 50, 150, 450, 625, 750);
    stCharles->setColorGroup(pink);
    fields.pushBack(stCharles);                                       //11

    // Utility
    fields.pushBack(new Property("Electric Company", 150, 0));        //12

    Property* states = new Property("States Avenue", 140, 10, 50, 150, 450, 625, 750);
    states->setColorGroup(pink);
    fields.pushBack(states);                                          //13

    Property* virginia = new Property("Virginia Avenue", 160, 12, 60, 180, 500, 700, 900);
    virginia->setColorGroup(pink);
    fields.pushBack(virginia);                                        //14

    // Railroad
    fields.pushBack(new Property("Pennsylvania Railroad", 200, 25));  //15

    // Orange properties
    Property* stJames = new Property("St. James Place", 180, 14, 70, 200, 550, 750, 950);
    stJames->setColorGroup(orange);
    fields.pushBack(stJames);                                         //16

    fields.pushBack(new CardField("Community Chest", nullptr));       //17

    Property* tennessee = new Property("Tennessee Avenue", 180, 14, 70, 200, 550, 750, 950);
    tennessee->setColorGroup(orange);
    fields.pushBack(tennessee);                                       //18

    Property* newYork = new Property("New York Avenue", 200, 16, 80, 220, 600, 800, 1000);
    newYork->setColorGroup(orange);
    fields.pushBack(newYork);                                         //19

    // Free Parking 
    fields.pushBack(new Property("Free Parking", 0, 0));              //20   

    // Red properties
    Property* kentucky = new Property("Kentucky Avenue", 220, 18, 90, 250, 700, 875, 1050);
    kentucky->setColorGroup(red);
    fields.pushBack(kentucky);                                        //21

    fields.pushBack(new CardField("Chance", nullptr));                //22

    Property* indiana = new Property("Indiana Avenue", 220, 18, 90, 250, 700, 875, 1050);
    indiana->setColorGroup(red);
    fields.pushBack(indiana);                                         //23

    Property* illinois = new Property("Illinois Avenue", 240, 20, 100, 300, 750, 925, 1100);
    illinois->setColorGroup(red);
    fields.pushBack(illinois);                                        //24

    // Railroad
    fields.pushBack(new Property("B&O Railroad", 200, 25));           //25

    // Yellow properties
    Property* atlantic = new Property("Atlantic Avenue", 260, 22, 110, 330, 800, 975, 1150);
    atlantic->setColorGroup(yellow);
    fields.pushBack(atlantic);                                        //26

    Property* ventnor = new Property("Ventnor Avenue", 260, 22, 110, 330, 800, 975, 1150);
    ventnor->setColorGroup(yellow);
    fields.pushBack(ventnor);                                         //27

    // Utility
    fields.pushBack(new Property("Water Works", 150, 0));             //28

    Property* marvin = new Property("Marvin Gardens", 280, 24, 120, 360, 850, 1025, 1200);
    marvin->setColorGroup(yellow);
    fields.pushBack(marvin);                                          //29

    // Go To Jail
    fields.pushBack(new GoToJailField());                             //30

    // Green properties
    Property* pacific = new Property("Pacific Avenue", 300, 26, 130, 390, 900, 1100, 1275);
    pacific->setColorGroup(green);
    fields.pushBack(pacific);                                         //31

    Property* northCarolina = new Property("North Carolina Avenue", 300, 26, 130, 390, 900, 1100, 1275);
    northCarolina->setColorGroup(green);
    fields.pushBack(northCarolina);                                  //32

    fields.pushBack(new CardField("Community Chest", nullptr));      //33

    Property* pennsylvania = new Property("Pennsylvania Avenue", 320, 28, 150, 450, 1000, 1200, 1400);
    pennsylvania->setColorGroup(green);
    fields.pushBack(pennsylvania);                                   //34

    // Railroad
    fields.pushBack(new Property("Short Line", 200, 25));            //35

    // Chance
    fields.pushBack(new CardField("Chance", nullptr));               //36

    // Dark Blue properties
    Property* parkPlace = new Property("Park Place", 350, 35, 175, 500, 1100, 1300, 1500);
    parkPlace->setColorGroup(darkBlue);
    fields.pushBack(parkPlace);                                      //37

    // Luxury Tax
    fields.pushBack(new Property("Luxury Tax", 0, -100));            //38

    Property* boardwalk = new Property("Boardwalk", 400, 50, 200, 600, 1400, 1700, 2000);
    boardwalk->setColorGroup(darkBlue);
    fields.pushBack(boardwalk);                                      //39
}
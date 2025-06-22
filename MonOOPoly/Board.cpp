#include "Board.h"
#include "Property.h"
#include "CardField.h"
#include "GoToJailField.h"

Board::Board() {
    initializeBoard();
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

Board::~Board() {
    for (size_t i = 0; i < fields.size(); i++) {
        delete fields[i];
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
    // GO - Start position
    fields.pushBack(new Property("GO", 0, 0));                        //0

    // Brown properties
    fields.pushBack(new Property("Mediterranean Avenue", 60, 2));     //1 
    fields.pushBack(new CardField("Community Chest", nullptr));       //2
    fields.pushBack(new Property("Baltic Avenue", 60, 4));            //3

    // Income Tax
    fields.pushBack(new Property("Income Tax", 0, -200));             //4

    // Railroad
    fields.pushBack(new Property("Reading Railroad", 200, 25));       //5

    // Light Blue properties
    fields.pushBack(new Property("Oriental Avenue", 100, 6));         //6
    fields.pushBack(new CardField("Chance", nullptr));                //7
    fields.pushBack(new Property("Vermont Avenue", 100, 6));          //8
    fields.pushBack(new Property("Connecticut Avenue", 120, 8));      //9

    // Jail
    fields.pushBack(new Property("Jail/Just Visiting", 0, 0));        //10

    // Pink properties - 1
    fields.pushBack(new Property("St. Charles Place", 140, 10));      //11

    // Utility
    fields.pushBack(new Property("Electric Company", 150, 0));        //12

    // Pink properties - 2
    fields.pushBack(new Property("States Avenue", 140, 10));          //13
    fields.pushBack(new Property("Virginia Avenue", 160, 12));        //14

    // Railroad
    fields.pushBack(new Property("Pennsylvania Railroad", 200, 25));  //15

    // Orange properties
    fields.pushBack(new Property("St. James Place", 180, 14));        //16
    fields.pushBack(new CardField("Community Chest", nullptr));       //17
    fields.pushBack(new Property("Tennessee Avenue", 180, 14));       //18  
    fields.pushBack(new Property("New York Avenue", 200, 16));        //19

    // Free Parking 
    fields.pushBack(new Property("Free Parking", 0, 0));              //20   

    // Red properties
    fields.pushBack(new Property("Kentucky Avenue", 220, 18));        //21
    fields.pushBack(new CardField("Chance", nullptr));                //22
    fields.pushBack(new Property("Indiana Avenue", 220, 18));         //23
    fields.pushBack(new Property("Illinois Avenue", 240, 20));        //24

    // Railroad
    fields.pushBack(new Property("B&O Railroad", 200, 25));           //25

    // Yellow properties - 1
    fields.pushBack(new Property("Atlantic Avenue", 260, 22));        //26
    fields.pushBack(new Property("Ventnor Avenue", 260, 22));         //27

    // Utility
    fields.pushBack(new Property("Water Works", 150, 0));             //28

    // Yellow properties - 2
    fields.pushBack(new Property("Marvin Gardens", 280, 24));         //29
     
    // Go To Jail
    fields.pushBack(new GoToJailField());                             //30

    // Green properties
    fields.pushBack(new Property("Pacific Avenue", 300, 26));         //31
    fields.pushBack(new Property("North Carolina Avenue", 300, 26));  //32
    fields.pushBack(new CardField("Community Chest", nullptr));       //33
    fields.pushBack(new Property("Pennsylvania Avenue", 320, 28));    //34

    // Railroad
    fields.pushBack(new Property("Short Line", 200, 25));             //35

    // Chance
    fields.pushBack(new CardField("Chance", nullptr));                //36

    // Dark Blue properties
    fields.pushBack(new Property("Park Place", 350, 35));             //37
    fields.pushBack(new Property("Luxury Tax", 0, -100));             //38
    fields.pushBack(new Property("Boardwalk", 400, 50));              //39
}
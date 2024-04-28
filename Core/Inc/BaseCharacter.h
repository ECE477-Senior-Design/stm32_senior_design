#ifndef BASECHRACTER_H
#define BASECHARACTER_H

#include "Character.h"

//Class definition for a player hexagon
class Character {
    protected:
        std::string _name; //Name of character
        int _column; //Column position of character
        int _row; //Row position of character

        int _strength; //Strength ability score of character
        int _dexterity; //Dexterity ability score of character
        int _constitution; //Constitution ability score of character
        int _intelligence; //Intelligence ability score of character
        int _wisdom; //Wisdom ability score of character
        int _charisma; //Charisma ability score of character
        int _max_health_points; //Max health points of character
        int _current_health_points; //Current health points of character
        int _armor_class; //Armor class of character
        int _initiative; //Initiative of character
        int _speed; //Speed of character
        int _visibility;
        int _gold;
        CharacterType _character_type;
        Class _class; //Class of character
        bool _active; //if character is active

    public:
        Character(std::string name, int column, int row,
        int strength, int dexterity, int constitution, int intelligence, int wisdom, int charisma,
        int max_health_points, int current_health_points, int armor_class, int initiative, int speed, int gold, int visibility); //Constructor for character class that keeps track of hex position, ability score, and some combat values
        std::string GetName(void); //Returns name of character

        int GetColumn(void); //Returns column position of character
        int GetRow(void); //Returns row position of character
        std::pair<int, int> GetPosition(void); //Returns position of character

        int GetStrength(void); //Returns strength of character
        int GetDexterity(void); //Returns dexterity of character
        int GetConstitution(void); //Returns constitution of character
        int GetIntelligence(void); //Returns intelligence of character
        int GetWisdom(void); //Returns wisdom of character
        int GetCharisma(void); //Returns charisma of character
        int GetMaxHealthPoints(void); //Returns max health points of character
        int GetCurrentHealthPoints(void); //Returns current health points of character
        int GetArmorClass(void); //Returns armor class of character
        int GetInitiative(void)const; //Returns initiative of character
        int GetSpeed(void); //Returns speed of character
        int GetGold(void);
        int GetVisibility(void);
        void DisplayCharacterInfo(void); //Displays character info
        CharacterType GetCharacterType(void); //Returns type of character
        Class GetClass(void); //Returns class of character
        bool GetActive(void); //returns if character is active on map
        void SetColumn(int column); //Sets column position of character
        void SetRow(int row); //Sets row position of character
        void SetStrength(int strength); //Sets strength of character
        void SetDexterity(int dexterity); //Sets dexterity of character
        void SetConstitution(int constitution); //Sets constitution of character
        void SetIntelligence(int intelligence); //Sets intelligence of character
        void SetWisdom(int wisdom); //Sets wisdom of character
        void SetCharisma(int charisma); //Sets charisma of character
        void SetMaxHealthPoints(int max_health_points); //Sets max health points of character
        void SetCurrentHealthPoints(int current_health_points); //Sets current hit points of character
        void SetArmorClass(int armor_class); //Sets armor class of character
        void SetInitiative(int initiative); //Sets initiative of character
        void SetSpeed(int speed); //Sets speed of character
        void SetGold(int gold);
        void SetVisibility(int visibility);
        void SetCharacterType(CharacterType character_type);
        void SetClass(Class class_); //Sets class of character
        void SetActive(bool active); //sets if character is active or not

        int CalculateModifier(int ability_score); //Returns modifier based on the ability score (may want to move to GamePlay file)

        ~Character(); //Destructor for character class
};

#endif

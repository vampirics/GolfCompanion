#pragma once

#include <Arduboy2.h>
#include "Utils.h"

#define EEPROM_START                  EEPROM_STORAGE_SPACE_START + 100
#define EEPROM_START_C1               EEPROM_START
#define EEPROM_START_C2               EEPROM_START + 1 
#define EEPROM_GAME_STATE             EEPROM_START_C2 + 1
#define EEPROM_NO_OF_PLAYERS          EEPROM_GAME_STATE + 1
#define EEPROM_PLAYER_BEING_EDITED    EEPROM_NO_OF_PLAYERS + 1
#define EEPROM_NO_OF_HOLES            EEPROM_PLAYER_BEING_EDITED + 1
#define EEPROM_CURRENT_HOLE           EEPROM_NO_OF_HOLES + 1
#define EEPROM_CURSOR_X               EEPROM_CURRENT_HOLE + 1
#define EEPROM_CURSOR_Y               EEPROM_CURSOR_X + 1
#define EEPROM_CURSOR_MODE            EEPROM_CURSOR_Y + 1
#define EEPROM_HOLES                  EEPROM_CURSOR_MODE + 1
#define EEPROM_TOTAL                  EEPROM_HOLES + 90
#define EEPROM_NAMES                  EEPROM_TOTAL + 5
    
struct Game {

  const uint8_t letter1 = 'G';
  const uint8_t letter2 = 'C';

  public: 
    
    Game() {
        for (uint8_t x = 0; x < NAME_LENGTH; x++) {
          name1[x] = ' ';
          name2[x] = ' ';
          name3[x] = ' ';
          name4[x] = ' ';
        }
    };

    uint8_t numberOfPlayers = 1;
    uint8_t playerBeingEdited = 1; 

    uint8_t numberOfHoles = 18;
    uint8_t currentHoleNumber = 3;

    Cursor cursor;

    Hole holes[18];
    Hole total;

    char name1[NAME_LENGTH + 1];
    char name2[NAME_LENGTH + 1];
    char name3[NAME_LENGTH + 1];
    char name4[NAME_LENGTH + 1];

  private:

    GameState _state = GameState::VSBoot;


  //--------------------------------------------------------------------------------------------------------------------------
  // Methods ..

  public: 

    GameState getState() {
      return _state;
    }

    void setState(GameState val) {
      _state = val;
      saveEEPROM();
    }

   /* ----------------------------------------------------------------------------
    *   Is the EEPROM initialised?
    *
    *   Looks for the characters 'G' and 'C' in the first two bytes of the EEPROM
    *   memory range starting from byte EEPROM_STORAGE_SPACE_START.  If not found,
    *   it resets the settings ..
    */


    void initEEPROM(bool forceClear) {

      byte c1 = EEPROM.read(EEPROM_START_C1);
      byte c2 = EEPROM.read(EEPROM_START_C2);

      if (forceClear || c1 != letter1 || c2 != letter2) { 

        EEPROM.update(EEPROM_START_C1, letter1);
        EEPROM.update(EEPROM_START_C2, letter2);

        saveEEPROM();

      }

    }


    void saveEEPROM() {

      EEPROM.update(EEPROM_GAME_STATE, static_cast<uint8_t>(_state));
      EEPROM.update(EEPROM_NO_OF_PLAYERS, numberOfPlayers);
      EEPROM.update(EEPROM_PLAYER_BEING_EDITED, playerBeingEdited);
      EEPROM.update(EEPROM_NO_OF_HOLES, numberOfHoles);
      EEPROM.update(EEPROM_CURRENT_HOLE, currentHoleNumber);
      EEPROM.update(EEPROM_CURSOR_X, cursor.x);
      EEPROM.update(EEPROM_CURSOR_Y, cursor.y);
      EEPROM.update(EEPROM_CURSOR_MODE, cursor.mode);

      for (uint8_t x = 0; x < 18; x++) {

        EEPROM.update(EEPROM_HOLES + (5 * x), holes[x].par);
        EEPROM.update(EEPROM_HOLES + (5 * x) + 1, holes[x].player1Score);
        EEPROM.update(EEPROM_HOLES + (5 * x) + 2, holes[x].player2Score);
        EEPROM.update(EEPROM_HOLES + (5 * x) + 3, holes[x].player3Score);
        EEPROM.update(EEPROM_HOLES + (5 * x) + 4, holes[x].player4Score);
        
      }

      EEPROM.update(EEPROM_TOTAL, total.par);
      EEPROM.update(EEPROM_TOTAL + 1, total.player1Score);
      EEPROM.update(EEPROM_TOTAL + 2, total.player2Score);
      EEPROM.update(EEPROM_TOTAL + 3, total.player3Score);
      EEPROM.update(EEPROM_TOTAL + 4, total.player4Score);

      uint8_t i = EEPROM_NAMES;

      for (uint8_t x = 0; x < NAME_LENGTH + 1; x++) {

        EEPROM.update(i, name1[x]);
        i++;
        
      }

      for (uint8_t x = 0; x < NAME_LENGTH + 1; x++) {

        EEPROM.update(i, name2[x]);
        i++;
        
      }

      for (uint8_t x = 0; x < NAME_LENGTH + 1; x++) {

        EEPROM.update(i, name3[x]);
        i++;
        
      }

      for (uint8_t x = 0; x < NAME_LENGTH + 1; x++) {

        EEPROM.update(i, name4[x]);
        i++;
        
      }

    }

    void loadEEPROM() {

      _state = static_cast<GameState>(EEPROM.read(EEPROM_GAME_STATE));
      numberOfPlayers = EEPROM.read(EEPROM_NO_OF_PLAYERS);
      playerBeingEdited = EEPROM.read(EEPROM_PLAYER_BEING_EDITED);
      numberOfHoles = EEPROM.read(EEPROM_NO_OF_HOLES);
      currentHoleNumber = EEPROM.read(EEPROM_CURRENT_HOLE);
      cursor.x = EEPROM.read(EEPROM_CURSOR_X);
      cursor.y = EEPROM.read(EEPROM_CURSOR_Y);
      cursor.mode = EEPROM.read(EEPROM_CURSOR_MODE);

      for (uint8_t x = 0; x < 18; x++) {

        holes[x].par = EEPROM.read(EEPROM_HOLES + (5 * x));
        holes[x].player1Score = EEPROM.read(EEPROM_HOLES + (5 * x) + 1);
        holes[x].player2Score = EEPROM.read(EEPROM_HOLES + (5 * x) + 2);
        holes[x].player3Score = EEPROM.read(EEPROM_HOLES + (5 * x) + 3);
        holes[x].player4Score = EEPROM.read(EEPROM_HOLES + (5 * x) + 4);
        
      }

      total.par = EEPROM.read(EEPROM_TOTAL);
      total.player1Score = EEPROM.read(EEPROM_TOTAL + 1);
      total.player2Score = EEPROM.read(EEPROM_TOTAL + 2);
      total.player3Score = EEPROM.read(EEPROM_TOTAL + 3);
      total.player4Score = EEPROM.read(EEPROM_TOTAL + 4);

      uint8_t i = EEPROM_NAMES;

      for (uint8_t x = 0; x < NAME_LENGTH + 1; x++) {

        name1[x] = EEPROM.read(i);
        i++;
        
      }

      for (uint8_t x = 0; x < NAME_LENGTH + 1; x++) {

        name2[x] = EEPROM.read(i);
        i++;
        
      }

      for (uint8_t x = 0; x < NAME_LENGTH + 1; x++) {

        name3[x] = EEPROM.read(i);
        i++;
        
      }

      for (uint8_t x = 0; x < NAME_LENGTH + 1; x++) {

        name4[x] = EEPROM.read(i);
        i++;
        
      }

    }

    void clear() {

      numberOfPlayers = 1;
      playerBeingEdited = 1; 
      numberOfHoles = 18;
      currentHoleNumber = 3;
      cursor.x = 0;
      cursor.y = 0;
      cursor.mode = 0;

      for (uint8_t x = 0; x < 18; x++) {
        holes[x].par = 4;
        holes[x].player1Score = 0;
        holes[x].player2Score = 0;
        holes[x].player3Score = 0;
        holes[x].player4Score = 0;
      }

      total.par = 4;
      total.player1Score = 0;
      total.player2Score = 0;
      total.player3Score = 0;
      total.player4Score = 0;

      for (uint8_t x = 0; x < NAME_LENGTH; x++) {
        name1[x] = ' ';
        name2[x] = ' ';
        name3[x] = ' ';
        name4[x] = ' ';
      }

    }
    
};

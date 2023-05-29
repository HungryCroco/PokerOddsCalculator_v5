#pragma once
#include "Hand.h"

// -> Struct containing the times each Hand has won trough for a Calculation:
//      - ( hand1_Won , hand2_Won , hand3_Won);
// -> If a Calculation is initialized with only 2 Hands, hand3_Won = 0;
struct Odds
{
    
    double hand_1{}; // Count of the times hand_1 did win though the Computation;
    double hand_2{}; // Count of the times hand_2 did win though the Computation;
    double hand_3{}; // Count of the times hand_3 did win though the Computation;
};

// -> Compares myHand1.HandPower with myHand2.HandPower using the luTables LuTablesSet;
//      - Returning (1,0,0) if Hand_1 won;
//      - Returning (0,1,0) if Hand_2 won;
//      - Returning (0.5,0.5,0) if both hands did split;
Odds compareHandStrength(Hand& myHand1, Hand& myHand2, LuTables& luTables);

// -> Calculating the Odds of 2 Hands with unknown Flop, Turn and River;
// -> Simulating Flop FC1, FC2, FC3, updating the combosLeft and calling SimulateTurn();
Odds simulateFlop(Hand& myHand1, Hand& myHand2, uint_fast64_t& combosLeft, LuTables& luTables);

// -> Calculating the Odds of 2 Hands with known Flop and unknown Turn and River;
// -> Simulating Turn, updating the combosLeft and calling SimulateRiver();
Odds simulateTurn(Hand& myHand1, Hand& myHand2, uint_fast64_t& combosLeft, const uint_fast16_t& maxCard, LuTables& luTables);

// -> Calculating the Odds of 2 Hands with known Flop and Turn and unknown River;
// -> Simulating River and calling compareHandStrength();
Odds simulateRiver(Hand& myHand1, Hand& myHand2, uint_fast64_t& combosLeft, const uint_fast16_t& maxCard, LuTables& luTables);

// -> Calculates the Odds of 2 Hands with unknown Flop+, Turn+ or River
Odds computeOdds(Hand& myHand1, Hand& myHand2, LuTables& luTables);




// -> Compares myHand1.HandPower, myHand2.HandPower and myHand3.HandPower using the luTables LuTablesSet;
//      - Returning (1,0,0) if Hand_1 won;
//      - Returning (0,1,0) if Hand_2 won;
//      - Returning (0,0,1) if Hand_3 won;
//      - Returning (0.5,0.5,0) if hands 1 & 2 did split;
//      ...
//      - Returning (0.33,0.33,0.33) if all hands did split;
Odds compareHandStrength(Hand& myHand1, Hand& myHand2, Hand& myHand3, LuTables& luTables);

// -> Calculating the Odds of 3 Hands with unknown Flop, Turn and River;
// -> Simulating Flop FC1, FC2, FC3, updating the combosLeft and calling SimulateTurn();
Odds simulateFlop(Hand& myHand1, Hand& myHand2, Hand& myHand3, uint_fast64_t& combosLeft, LuTables& luTables);

// -> Calculating the Odds of 3 Hands with known Flop and unknown Turn and River;
// -> Simulating Turn, updating the combosLeft and calling SimulateRiver();
Odds simulateTurn(Hand& myHand1, Hand& myHand2, Hand& myHand3, uint_fast64_t& combosLeft, const uint_fast16_t& maxCard, LuTables& luTables);

// -> Calculating the Odds of 3 Hands with known Flop and Turn and unknown River;
// -> Simulating River and calling compareHandStrength();
Odds simulateRiver(Hand& myHand1, Hand& myHand2, Hand& myHand3, uint_fast64_t& combosLeft, const uint_fast16_t& maxCard, LuTables& luTables);

// -> Calculates the Odds of 3 Hands with unknown Flop+, Turn+ or River
Odds computeOdds(Hand& myHand1, Hand& myHand2, Hand& myHand3, LuTables& luTables);
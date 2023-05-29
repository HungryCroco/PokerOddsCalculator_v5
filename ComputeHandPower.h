#pragma once
#include <iostream>
#include "Hand.h"

// -> Calculates the Flush, StraightFlush, and RoyalFlush of the Hand myHand (given as parameter) using the precalculated LookUpTablesSet luTables;
// -> Updates the value of myHand.HandPower;
// -> Possible HandPower values:
//	- 00000000000: No Flush, StraightFlush, or RoyalFlush;
//	- 6xxxxxxxxxx: Flush (but not STR8FL+);
//	- 9xxxxxxxxxx: StraightFlush or RoyalFlush; 
void calculateFlushAndStraightFlushPlus(Hand& myHand, LuTables& luTables);

// -> Calculates the Straight of the Hand myHand(given as parameter) using the precalculated LookUpTablesSet luTables;
// -> Updates the value of myHand.HandPower accordingly;
// -> Possible HandPower values:
//	- 00000000000 - No Straight;
//	- 5xxxxxxxxxx - Straight;   
void calculateStraight(Hand& myHand, LuTables& luTables);

// -> Calculates all PAIR-Type HandPowers of the Hand myHand(given as parameter) using the precalculated LookUpTablesSet luTables;
// -> Modifications: Changes the value of myHand.HandPower to the biggest PAIR-Type HandPower that is available;
// -> Possible HandPower values:
//	- 1xxxxxxxxxx - HighCard;
//	- 2xxxxxxxxxx - OnePair;
//	- 3xxxxxxxxxx - TwoPairs;
//	- 4xxxxxxxxxx - TThreeOfAKind;
//	- 7xxxxxxxxxx - FullHouse;
//	- 8xxxxxxxxxx - FourOfAKind;
void calculatePairs(Hand& myHand, LuTables& luTables);

// -> Calculates the Kickers of the Hand myHand(given as parameter) using logical operations;
// -> Modifies myHand.HandPower by adding the kickers value to the already calculated handPower: myHand.HandPower += xxxxxxxxxx ;
void calculateKickersPairedHand(Hand& myHand);

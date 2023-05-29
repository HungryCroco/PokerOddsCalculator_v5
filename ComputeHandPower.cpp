#pragma once
#include "ComputeHandPower.h"
//#include "ComputeLookupTables.h"




void calculateFlushAndStraightFlushPlus(Hand& myHand, LuTables& luTables)
{
	// Set myHand.handPower to the corresponding value in the precalculated luTableFlushAndStraightFlushPlus Array using as index the 13bits base10 cardFaceMask by Suit;
	// Need to check 4 times with each CardSuit or until the value != 0(2 Flushes from 2 different Suits are not possible in Holdem NoLimit);

	if (luTables.getFlushAndStraightFlushHandPower(myHand.cardFaceMask_Spades_13bits_base10) != 0)
	{
		myHand.handPower = luTables.getFlushAndStraightFlushHandPower(myHand.cardFaceMask_Spades_13bits_base10);
	}
	else if (luTables.getFlushAndStraightFlushHandPower(myHand.cardFaceMask_Hearts_13bits_base10) != 0)
	{
		myHand.handPower = luTables.getFlushAndStraightFlushHandPower(myHand.cardFaceMask_Hearts_13bits_base10);
	}
	else if (luTables.getFlushAndStraightFlushHandPower(myHand.cardFaceMask_Diamonds_13bits_base10) != 0)
	{
		myHand.handPower = luTables.getFlushAndStraightFlushHandPower(myHand.cardFaceMask_Diamonds_13bits_base10);
	}
	else if (luTables.getFlushAndStraightFlushHandPower(myHand.cardFaceMask_Clubs_13bits_base10) != 0)
	{
		myHand.handPower = luTables.getFlushAndStraightFlushHandPower(myHand.cardFaceMask_Clubs_13bits_base10);
	}
}


void calculateStraight(Hand& myHand , LuTables& luTables)
{
	// Set myHand.handPower to the corresponding value in the precalculated luTableStraight Array using as index cardFaceMask_13bits_base10 if the vlue !=0;

	uint_fast64_t handPower = luTables.getStraightHandPower(myHand.cardFaceMask_13bits_base10);
	if (handPower != 0)
	{
		myHand.handPower = handPower;
	}	
}


void calculatePairs(Hand& myHand, LuTables& luTables)
{
	// Set myHand.handPower to the corresponding value in the precalculated luTablePairs Array using as index cardFaceMask_3bits_base5;
	// Calculate the kickers calling the Function calculateKickersPairedHand(); As futher optimization it's possible to move the kicker's calculation after hand's comparison and do it only when (myHand1.handPower == myHand2.handPower && myHand1.handPower == 1,2,3,4,7,8);
	// Pairs calculation CAN'T be equal to 0, as any Hand will have atleast HighCard Value;

	myHand.handPower = luTables.getPairsHandPower(myHand.cardFaceMask_3bits_base5);

	calculateKickersPairedHand(myHand);
}


void calculateKickersPairedHand(Hand& myHand)
{
	// Check what is the pair's handPower;
	// Calculate the necessary kickers as follows:
	// handPower is represented like 010203040506 in BASE_14, where CardFace of DEUCE = 01 and of Ace =13;
	// As 1 Pair in terms of kickers is meaning 2 times the same CardFace, TRIPS is meaning 3 times the same CardFace etc, we dont copy the CardFace, just write it 1 time and leave for every unwritten repeating CardFace an unused "bit" at the end {00}; As we compare kickers only when the handPower of 2+ Hands is equal, this approach can't lead to logical mistakes;
	// 1 - HighCard: 01 - handPower {01}; 02 - highest Kicker; 03 - 2.Kicker; 04 - 3.Kicker; 05 - 4.Kicker; 06 - 5.Kicker;
	// 2 - OnePair: 01 - handPower {02}; 02 - Pair's CardFace; 03 - highest Kicker; 04 - 2.Kicker; 05 - 3.Kicker; 06 - 00; 
	// 3 - TwoPairs: 01 - handPower {03}; 02 - higher Pair's CardFace; 03 - lower Pair's CardFace; 04 - highest Kicker; 05 - 00; 06 - 00; 
	// 4 - ThreeOfAKind: 01 - handPower {04}; 02 - TRIPS CardFace; 03 - highest Kicker; 04 - 2.Kicker; 05 - 00; 06 - 00;
	// 7 - FullHouse: 01 - handPower {07}; 02 - TRIPS CardFace; 03 - Pair's CardFace; 04 - 00; 05 - 00; 06 - 00; 
	// 8 - FourOfAKind: 01 - handPower {08}; 02 - QUADS CardFace; 03 - highest Kicker; 04 - 00; 05 - 00; 06 - 00;
	// This Function can be improved; 

	uint_fast64_t cardFaceMask{ myHand.cardFaceMask_13bits_base5 };

	uint_fast64_t currDigit{ 0 }; // The current CardFace that we are looping trough;
	int_fast64_t kickerIndex; // index of the 1.Kicker (Highest Card not beeing a part of any Pair+);
	uint_fast64_t lastKickerIndex; // Index of the last kicker !=0;
	uint_fast16_t cntRepeatingCards{ 0 }; // Used if TwoPairs is available to make sure that if we have a Hand, that for example contains Three Pairs (77-66-55-2) we can calculate the highest Pair even if it's a part of the 3.Pair(55);
	uint_fast64_t cntDigits_cardFaceMask{ 1 }; // Position of the highest CardFace in the CardFaceMAsk_2 == 1; (DEUCE = 1, THREE = 10, ACE = 1000000000000);
	uint_fast16_t currCard{ 1 }; // Value of the highest CardFace in the CardFaceMAsk_2 == 1;

	// CallCulate cntDigits_cardFaceMask and currCard
	while (cardFaceMask / cntDigits_cardFaceMask >= 10) 
	{
		cntDigits_cardFaceMask *= 10;
		++currCard;
	}


	if (myHand.handPower == HANDPOWER_HIGHCARD)
	{
		kickerIndex = OFFSET_KICKER_1;
		lastKickerIndex = OFFSET_KICKER_5;

		for (uint_fast64_t i = cntDigits_cardFaceMask; i > 0; i /= 10)
		{
			currDigit = (cardFaceMask / i) % 10;
			
			if (currDigit != 0)
			{
				if (currDigit == 1)
				{
					myHand.handPower += currCard * kickerIndex;
					kickerIndex /= 100;
				}
				if (kickerIndex < lastKickerIndex)
				{
					i = 0;
				}
			}
			--currCard;
		}
	}

	else if (myHand.handPower == HANDPOWER_ONEPAIR)
	{

		kickerIndex = OFFSET_KICKER_2;
		lastKickerIndex = OFFSET_KICKER_4;

		for (uint_fast64_t i = cntDigits_cardFaceMask; i > 0; i /= 10)
		{
			currDigit = (cardFaceMask / i) % 10;
			

			if (currDigit != 0)
			{
				if (currDigit == 1 && kickerIndex >= lastKickerIndex)
				{
					myHand.handPower += currCard * kickerIndex;
					kickerIndex /= 100;
				}
				else if (currDigit == 2)
				{
					myHand.handPower += currCard * OFFSET_KICKER_1;
					++cntRepeatingCards;
				}
				if (kickerIndex < lastKickerIndex && cntRepeatingCards == 1)
				{
					i = 0;
				}
			}
			--currCard;
		}
	}
	else if (myHand.handPower == HANDPOWER_TWOPAIRS)
	{
		kickerIndex = OFFSET_KICKER_3;
		lastKickerIndex = OFFSET_KICKER_3;

		for (uint_fast64_t i = cntDigits_cardFaceMask; i > 0; i /= 10)
		{
			currDigit = (cardFaceMask / i) % 10;
			

			if (currDigit != 0)
			{
				if ((currDigit == 1 && kickerIndex >= lastKickerIndex) || (currDigit == 2 && cntRepeatingCards == 2))
				{
					myHand.handPower += currCard * kickerIndex;
					kickerIndex /= 100;
				}
				else if (currDigit == 2)
				{
					if (cntRepeatingCards == 0)
					{
						myHand.handPower += currCard * OFFSET_KICKER_1;
						++cntRepeatingCards;
					}
					else
					{
						myHand.handPower += currCard * OFFSET_KICKER_2;
						++cntRepeatingCards;
					}
				}
				if (kickerIndex < lastKickerIndex && cntRepeatingCards == 2)
				{
					i = 0;
				}
			}
			--currCard;
		}
	}
	else if (myHand.handPower == HANDPOWER_THREEOFAKIND)
	{
		kickerIndex = OFFSET_KICKER_2;
		lastKickerIndex = OFFSET_KICKER_3;

		for (uint_fast64_t i = cntDigits_cardFaceMask; i > 0; i /= 10)
		{
			currDigit = (cardFaceMask / i)% 10;
			

			if (currDigit != 0)
			{
				if (currDigit == 1 && kickerIndex >= lastKickerIndex)
				{
					myHand.handPower += currCard * kickerIndex;
					kickerIndex /= 100;
				}
				else if (currDigit == 3)
				{
					myHand.handPower += currCard * OFFSET_KICKER_1;
					++cntRepeatingCards;
				}
				if (kickerIndex < lastKickerIndex && cntRepeatingCards == 1)
				{
					i = 0;
				}
			}
			--currCard;
		}
	}
	else if (myHand.handPower == HANDPOWER_FULLHOUSE)
	{
		kickerIndex = OFFSET_KICKER_2;
		lastKickerIndex = OFFSET_KICKER_2;

		for (uint_fast64_t i = cntDigits_cardFaceMask; i > 0; i /= 10)
		{
			currDigit = (cardFaceMask / i) % 10;
			

			if (currDigit != 0)
			{
				if (currDigit == 3)
				{
					if (cntRepeatingCards == 0)
					{
						myHand.handPower += currCard * OFFSET_KICKER_1;
						++cntRepeatingCards;
					}
					else
					{
						myHand.handPower += currCard * OFFSET_KICKER_2;
						//++cntRepeatingCards;
						kickerIndex /= 100;
					}


				}
				else if (currDigit == 2 && cntRepeatingCards < 2 && kickerIndex >= lastKickerIndex)
				{
					myHand.handPower += currCard * OFFSET_KICKER_2;
					kickerIndex /= 100;
				}

				if (cntRepeatingCards > 0 && kickerIndex < lastKickerIndex)
				{
					i = 0;
				}
			}
			--currCard;
		}
	}
	else if (myHand.handPower == HANDPOWER_FOUROFAKIND)
	{

		kickerIndex = OFFSET_KICKER_2;
		lastKickerIndex = OFFSET_KICKER_2;

		for (uint_fast64_t i = cntDigits_cardFaceMask; i > 0; i /= 10)
		{
			currDigit = (cardFaceMask / i) % 10;
			

			if (currDigit != 0)
			{
				if (currDigit == 4)
				{
					myHand.handPower += currCard * OFFSET_KICKER_1;
					++cntRepeatingCards;

				}
				else if (kickerIndex == OFFSET_KICKER_2)
				{
					myHand.handPower += currCard * OFFSET_KICKER_2;
					kickerIndex /= 100;
				}

				if (kickerIndex < lastKickerIndex && cntRepeatingCards == 1)
				{
					i = 0;
				}
			}
			--currCard;
		}
	}	
}
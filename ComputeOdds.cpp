#include "ComputeOdds.h"
//#include "Hand.h"
#include <ppl.h>

 



Odds  compareHandStrength(Hand& myHand1, Hand& myHand2, LuTables& luTables)
{
	// 1. Calculate the handPower of all Hands;
	// 2. Compare all handPowers and increase the Odds of the winning Hand; If split increase the Odds of all splitting Hands by 1/count(splittingHands);
	myHand1.computeHand(luTables);
	myHand2.computeHand(luTables);

	double oddsHand1{ 0 }; 
	double oddsHand2{ 0 };

	if (myHand1.handPower > myHand2.handPower)
	{
		++oddsHand1;
	}
	else if (myHand1.handPower < myHand2.handPower)
	{
		++oddsHand2;
	}
	/*else if((myHand1.handPower < HANDPOWER_STRAIGHT ) || (myHand1.handPower != HANDPOWER_FULLHOUSE) || (myHand1.handPower != HANDPOWER_FOUROFAKIND))
	{
		CalculateKickersPairedHand(myHand1);
		CalculateKickersPairedHand(myHand2);

		if (myHand1.handPower > myHand2.handPower)
		{
			++oddsHand1;
		}
		else if (myHand1.handPower < myHand2.handPower)
		{
			++oddsHand2;
		}
		else
		{
			oddsHand1 += 0.5;
			oddsHand2 += 0.5;
		}
	}*/
	else
	{
		oddsHand1 += 0.5;
		oddsHand2 += 0.5;
	}

	return{ oddsHand1 , oddsHand2 , 0.0 };
}

Odds  compareHandStrength(Hand& myHand1, Hand& myHand2, Hand& myHand3, LuTables& luTables)
{
	// 1. Calculate the handPower of all Hands;
	// 2. Compare all handPowers and increase the Odds of the winning Hand; If split increase the Odds of all splitting Hands by 1/count(splittingHands);
	myHand1.computeHand(luTables);
	myHand2.computeHand(luTables);
	myHand3.computeHand(luTables);

	double oddsHand1{ 0 };
	double oddsHand2{ 0 };
	double oddsHand3{ 0 };

	if (myHand1.handPower > myHand2.handPower)
	{
		if (myHand1.handPower > myHand3.handPower)
		{
			++oddsHand1;
		}
		else if (myHand1.handPower < myHand3.handPower)
		{
			++oddsHand3;
		}
		else
		{
			oddsHand1 += 0.5;
			oddsHand3 += 0.5;
		}
	}
	else if (myHand1.handPower < myHand2.handPower)
	{
		if (myHand2.handPower > myHand3.handPower)
		{
			++oddsHand2;
		}
		else if (myHand2.handPower < myHand3.handPower)
		{
			++oddsHand3;
		}
		else
		{
			oddsHand2 += 0.5;
			oddsHand3 += 0.5;
		}
	}

	else
	{
		if (myHand1.handPower > myHand3.handPower)
		{
			oddsHand1 += 0.5;
			oddsHand2 += 0.5;
		}
		else if (myHand1.handPower < myHand3.handPower)
		{
			++oddsHand3;
		}
		else
		{
			oddsHand1 += 1 / 3.0;
			oddsHand2 += 1 / 3.0;
			oddsHand3 += 1 / 3.0;
		}
	}

	return{ oddsHand1 , oddsHand2 , oddsHand3 };
}

Odds computeOdds(Hand& myHand1, Hand& myHand2, LuTables& luTables)
{
	// 1. Find all unused Cards as 52bit's Mask;
	// 2. Call the appropriate Street's Simulation;
	// 3. Calculate the real Odds of each as winning % ;
	// This entire Looping could be improved with 1 recursive function;
	// An implementation of parallel_for should improve the performance;
	Odds oddsTotal{ 0,0,0 };
	unsigned long long combosLeft{ 0b1111111111111111111111111111111111111111111111111111 }; //52 bit
	combosLeft ^= (myHand1.handMask_52b | myHand2.handMask_52b);

	if (myHand1.street == 2)
	{
		oddsTotal = simulateFlop(myHand1, myHand2, combosLeft, luTables);
	}
	else if (myHand1.street == 5)
	{
		oddsTotal = simulateTurn(myHand1, myHand2, combosLeft, Hand::FULLHANDMASK_BITS, luTables);
	}
	else if (myHand1.street == 6)
	{
		oddsTotal = simulateRiver(myHand1, myHand2, combosLeft, Hand::FULLHANDMASK_BITS, luTables);
	}

	return {oddsTotal.hand_1 * 100/(oddsTotal.hand_1+oddsTotal.hand_2) , oddsTotal.hand_2 * 100 / (oddsTotal.hand_1 + oddsTotal.hand_2)  , 0};
}

Odds computeOdds(Hand& myHand1, Hand& myHand2, Hand& myHand3, LuTables& luTables)
{
	// 1. Find all unused Cards as 52bit's Mask;
	// 2. Call the appropriate Street's Simulation;
	// 3. Calculate the real Odds of each as winning % ;
	// This entire Looping could be improved with 1 recursive function;
	// An implementation of parallel_for should improve the performance;
	Odds oddsTotal{ 0,0,0 };
	unsigned long long combosLeft{ 0b1111111111111111111111111111111111111111111111111111 }; //52 bit
	combosLeft ^= (myHand1.handMask_52b | myHand2.handMask_52b | myHand3.handMask_52b);

	if (myHand1.street == 2)
	{
		oddsTotal = simulateFlop(myHand1, myHand2, myHand3, combosLeft, luTables);
	}
	else if (myHand1.street == 5)
	{
		oddsTotal = simulateTurn(myHand1, myHand2, myHand3, combosLeft, Hand::FULLHANDMASK_BITS, luTables);
	}
	else if (myHand1.street == 6)
	{
		oddsTotal = simulateRiver(myHand1, myHand2, myHand3, combosLeft, Hand::FULLHANDMASK_BITS, luTables);
	}

	return { oddsTotal.hand_1 * 100 / (oddsTotal.hand_1 + oddsTotal.hand_2 + oddsTotal.hand_3) , oddsTotal.hand_2 * 100 / (oddsTotal.hand_1 + oddsTotal.hand_2 + oddsTotal.hand_3)  , oddsTotal.hand_3 * 100 / (oddsTotal.hand_1 + oddsTotal.hand_2 + oddsTotal.hand_3) };
}

Odds simulateFlop(Hand& myHand1, Hand& myHand2, uint_fast64_t& combosLeft, LuTables& luTables)
{
	// For each of the three FlopCards:
	//	1. Loop through all unused Cards 
	//  2. Update the 52bits HandMasks of all Hands and combosLeft;
	//  3. Loop again untill all 3 FlopCards are known;
	//	4. Call SimulateTurn in the FlopCard3 loop;
	//  When finishing any of the loop update all 52bits HandMasks deleting the Card we have added at the beginning of the loop(i); 
	
	unsigned long long combosLeft_FC2{ combosLeft }; //52 bit
	
	
	Odds oddsTotal{ 0,0,0 };
	double oddsH1{ 0 };
	double oddsH2{ 0 };

   //simulate FC1
	for (int8_t i = Hand::FULLHANDMASK_BITS - 1; i >= 0; i--)
	{
		if (((combosLeft & (1ull << i)) == 1ull << i))
		{
			myHand1.handMask_52b |= 1ULL << i;
			myHand2.handMask_52b |= 1ULL << i;

			combosLeft_FC2 ^= 1ULL << i;
			uint_fast64_t combosLeft_FC3{ combosLeft_FC2 };

			//simulate FC2
			for (int8_t j = i - 1; j >= 0; j--)
				{
					if (((combosLeft_FC2 & (1ull << j)) == 1ull << j))
					{
						myHand1.handMask_52b |= 1ULL << j;
						myHand2.handMask_52b |= 1ULL << j;

						combosLeft_FC3 ^= 1ULL << j;
						uint_fast64_t combosLeft_Turn{ combosLeft_FC3 };

						//simulate FC3
						for (int8_t k = j - 1; k >= 0; k--)
							{
								if (((combosLeft_FC3 & (1ull << k)) == 1ull << k))
								{
									myHand1.handMask_52b |= 1ULL << k;
									myHand2.handMask_52b |= 1ULL << k;


									combosLeft_Turn ^= 1ULL << k;

									oddsTotal = simulateTurn(myHand1, myHand2, combosLeft_Turn, k, luTables);
									oddsH1 += oddsTotal.hand_1;
									oddsH2 += oddsTotal.hand_2;

									myHand1.handMask_52b &= ~(1ULL << k); // Remove the card from handMask using bitwise AND with complement
									myHand2.handMask_52b &= ~(1ULL << k); // Remove the card from handMask using bitwise AND with complement
								}
							}

						myHand1.handMask_52b &= ~(1ULL << j); // Remove the card from handMask using bitwise AND with complement
						myHand2.handMask_52b &= ~(1ULL << j); // Remove the card from handMask using bitwise AND with complement
					}
				}

			myHand1.handMask_52b &= ~(1ULL << i); // Remove the card from handMask using bitwise AND with complement
			myHand2.handMask_52b &= ~(1ULL << i); // Remove the card from handMask using bitwise AND with complement
		}
	}

	return { oddsH1 , oddsH2 , 0 };
}

Odds simulateFlop(Hand& myHand1, Hand& myHand2, Hand& myHand3, uint_fast64_t& combosLeft, LuTables& luTables)
{
	// For each of the three FlopCards:
	//	1. Loop through all unused Cards 
	//  2. Update the 52bits HandMasks of all Hands and combosLeft;
	//  3. Loop again untill all 3 FlopCards are known;
	//	4. Call SimulateTurn in the FlopCard3 loop;
	//  When finishing any of the loop update all 52bits HandMasks deleting the Card we have added at the beginning of the loop(i); 

	unsigned long long combosLeft_FC2{ combosLeft }; //52 bit


	Odds oddsTotal{ 0,0,0 };
	double oddsH1{ 0 };
	double oddsH2{ 0 };
	double oddsH3{ 0 };

	//simulate FC1
	for (int8_t i = Hand::FULLHANDMASK_BITS - 1; i >= 0; i--)
	{
		if (((combosLeft & (1ull << i)) == 1ull << i))
		{
			myHand1.handMask_52b |= 1ULL << i;
			myHand2.handMask_52b |= 1ULL << i;
			myHand3.handMask_52b |= 1ULL << i;

			combosLeft_FC2 ^= 1ULL << i;
			uint_fast64_t combosLeft_FC3{ combosLeft_FC2 };

			//simulate FC2
			for (int8_t j = i - 1; j >= 0; j--)
			{
				if (((combosLeft_FC2 & (1ull << j)) == 1ull << j))
				{
					myHand1.handMask_52b |= 1ULL << j;
					myHand2.handMask_52b |= 1ULL << j;
					myHand3.handMask_52b |= 1ULL << j;

					combosLeft_FC3 ^= 1ULL << j;
					uint_fast64_t combosLeft_Turn{ combosLeft_FC3 };

					//simulate FC3
					for (int8_t k = j - 1; k >= 0; k--)
					{
						if (((combosLeft_FC3 & (1ull << k)) == 1ull << k))
						{
							myHand1.handMask_52b |= 1ULL << k;
							myHand2.handMask_52b |= 1ULL << k;
							myHand3.handMask_52b |= 1ULL << k;


							combosLeft_Turn ^= 1ULL << k;

							oddsTotal = simulateTurn(myHand1, myHand2, myHand3, combosLeft_Turn, k, luTables);
							oddsH1 += oddsTotal.hand_1;
							oddsH2 += oddsTotal.hand_2;
							oddsH3 += oddsTotal.hand_3;

							myHand1.handMask_52b &= ~(1ULL << k); // Remove the card from handMask using bitwise AND with complement
							myHand2.handMask_52b &= ~(1ULL << k); // Remove the card from handMask using bitwise AND with complement
							myHand3.handMask_52b &= ~(1ULL << k); // Remove the card from handMask using bitwise AND with complement
						}
					}

					myHand1.handMask_52b &= ~(1ULL << j); // Remove the card from handMask using bitwise AND with complement
					myHand2.handMask_52b &= ~(1ULL << j); // Remove the card from handMask using bitwise AND with complement
					myHand3.handMask_52b &= ~(1ULL << j); // Remove the card from handMask using bitwise AND with complement
				}
			}

			myHand1.handMask_52b &= ~(1ULL << i); // Remove the card from handMask using bitwise AND with complement
			myHand2.handMask_52b &= ~(1ULL << i); // Remove the card from handMask using bitwise AND with complement
			myHand3.handMask_52b &= ~(1ULL << i); // Remove the card from handMask using bitwise AND with complement
		}
	}

	return { oddsH1 , oddsH2 , oddsH3 };
}

Odds simulateTurn(Hand& myHand1, Hand& myHand2, uint_fast64_t& combosLeft, const uint_fast16_t& maxCard, LuTables& luTables)
{
	// For each of the three FlopCards:
	//	1. Loop through all unused Cards 
	//  2. Update the 52bits HandMasks of all Hands and combosLeft;
	//  3. Call SimulateRiver;
	//  4. Update all 52bits HandMasks deleting the Card we have added at the beginning of the loop(i); 
	uint_fast64_t combosLeft_River{ combosLeft }; //52 bit
	
	Odds oddsTotal{ 0,0,0 };
	double oddsH1{ 0 };
	double oddsH2{ 0 };

	for (int i = maxCard - 1; i >= 0; i--)
	{
		if (((combosLeft & (1ull << i)) == 1ull << i))
		{

			combosLeft_River ^= 1LL << i;

			myHand1.handMask_52b |= 1ULL << i;
			myHand2.handMask_52b |= 1ULL << i;

			oddsTotal = simulateRiver(myHand1, myHand2, combosLeft_River, i, luTables);

			oddsH1 += oddsTotal.hand_1;
			oddsH2 += oddsTotal.hand_2;

			myHand1.handMask_52b ^= 1ULL << i; // Remove the card from handMask using bitwise AND with complement
			myHand2.handMask_52b ^= 1ULL << i; // Remove the card from handMask using bitwise AND with complement
		}
	}

	return { oddsH1 , oddsH2 , 0 };
}

Odds simulateTurn(Hand& myHand1, Hand& myHand2, Hand& myHand3, uint_fast64_t& combosLeft, const uint_fast16_t& maxCard, LuTables& luTables)
{
	// For each of the three FlopCards:
	//	1. Loop through all unused Cards 
	//  2. Update the 52bits HandMasks of all Hands and combosLeft;
	//  3. Call SimulateRiver;
	//  4. Update all 52bits HandMasks deleting the Card we have added at the beginning of the loop(i); 
	uint_fast64_t combosLeft_River{ combosLeft }; //52 bit

	Odds oddsTotal{ 0,0,0 };
	double oddsH1{ 0 };
	double oddsH2{ 0 };
	double oddsH3{ 0 };

	for (int i = maxCard - 1; i >= 0; i--)
	{
		if (((combosLeft & (1ull << i)) == 1ull << i))
		{

			combosLeft_River ^= 1LL << i;

			myHand1.handMask_52b |= 1ULL << i;
			myHand2.handMask_52b |= 1ULL << i;
			myHand3.handMask_52b |= 1ULL << i;

			oddsTotal = simulateRiver(myHand1, myHand2, myHand3, combosLeft_River, i, luTables);

			oddsH1 += oddsTotal.hand_1;
			oddsH2 += oddsTotal.hand_2;
			oddsH3 += oddsTotal.hand_3;

			myHand1.handMask_52b ^= 1ULL << i; // Remove the card from handMask using bitwise AND with complement
			myHand2.handMask_52b ^= 1ULL << i; // Remove the card from handMask using bitwise AND with complement
			myHand3.handMask_52b ^= 1ULL << i; // Remove the card from handMask using bitwise AND with complement
		}
	}

	return { oddsH1 , oddsH2 , oddsH3 };
}

Odds simulateRiver(Hand& myHand1, Hand& myHand2, uint_fast64_t& combosLeft, const uint_fast16_t& maxCard, LuTables& luTables)
{
	// For each of the three FlopCards:
	//	1. Loop through all unused Cards 
	//  2. Update the 52bits HandMasks of all Hands and combosLeft;
	//  3. Call myHnd.reset() for each Hand to null all HandMasks and the handPowers;
	//	4. Call myHand.calculateHandMasks() for each Hand to calculate the handPower;
	//  5. Call compareHandStrength() to find the winner;
	//  6. Increasing the current winner's Odds;
	//  7. Update all 52bits HandMasks deleting the Card we have added at the beginning of the loop(i); 

	Odds oddsTotal{ 0,0,0 };
	double oddsH1{0};
	double oddsH2{0};

	
	for (int i = maxCard - 1; i >= 0; i--)
		{
			if (i == 3)
			{
				bool check = true;
			}
			if (((combosLeft & (1ull << i)) == 1ull << i))
			{

				myHand1.handMask_52b |= 1ULL << i;
				myHand2.handMask_52b |= 1ULL << i;


				myHand1.reset();
				myHand2.reset();
				myHand1.calculateHandMasks(luTables);
				myHand2.calculateHandMasks(luTables);

				oddsTotal = compareHandStrength(myHand1, myHand2, luTables);
				oddsH1 += oddsTotal.hand_1;
				oddsH2 += oddsTotal.hand_2;

				myHand1.handMask_52b ^= 1ULL << i;
				myHand2.handMask_52b ^= 1ULL << i;
			}
		}

	return { oddsH1 , oddsH2 , 0};
}

Odds simulateRiver(Hand& myHand1, Hand& myHand2, Hand& myHand3, uint_fast64_t& combosLeft, const uint_fast16_t& maxCard, LuTables& luTables)
{
	// For each of the three FlopCards:
	//	1. Loop through all unused Cards 
	//  2. Update the 52bits HandMasks of all Hands and combosLeft;
	//  3. Call myHnd.reset() for each Hand to null all HandMasks and the handPowers;
	//	4. Call myHand.calculateHandMasks() for each Hand to calculate the handPower;
	//  5. Call compareHandStrength() to find the winner;
	//  6. Increasing the current winner's Odds;
	//  7. Update all 52bits HandMasks deleting the Card we have added at the beginning of the loop(i); 

	Odds oddsTotal{ 0,0,0 };
	double oddsH1{ 0 };
	double oddsH2{ 0 };
	double oddsH3{ 0 };


	for (int i = maxCard - 1; i >= 0; i--)
	{
		if (i == 45)
		{
			bool check = true; //debug
		}

		if (((combosLeft & (1ull << i)) == 1ull << i))
		{

			myHand1.handMask_52b |= 1ULL << i;
			myHand2.handMask_52b |= 1ULL << i;
			myHand3.handMask_52b |= 1ULL << i;


			myHand1.reset();
			myHand2.reset();
			myHand3.reset();
			myHand1.calculateHandMasks(luTables);
			myHand2.calculateHandMasks(luTables);
			myHand3.calculateHandMasks(luTables);

			oddsTotal = compareHandStrength(myHand1, myHand2, myHand3, luTables);
			oddsH1 += oddsTotal.hand_1;
			oddsH2 += oddsTotal.hand_2;
			oddsH3 += oddsTotal.hand_3;

			myHand1.handMask_52b ^= 1ULL << i;
			myHand2.handMask_52b ^= 1ULL << i;
			myHand3.handMask_52b ^= 1ULL << i;
		}
	}

	return { oddsH1 , oddsH2 , oddsH3 };
}
#pragma once
#include "Hand.h"
#include "ComputeHandPower.h"



Hand::Hand(const std::string& all_cards_param)
{
	// Loop through the HAnd's string;
	//  - Convert the Card to bitMssk;
	//	- update the Hand's bitMask;
	// - Update street;

	for (size_t i = 0; i < all_cards_param.length(); i = i + 2)
	{
		uint_fast64_t bitMasks = convertCardFaceToCardMask(all_cards_param.substr(i, 2));
		handMask_52b |= bitMasks;

		++street;
	}
}

void Hand::calculateHandMasks(LuTables& luTables) 
{
	// Here we calculate all necessary CardFaceMasks looping through the handMask_52b 5 times(Splitting the 52bit HandMask into pieces of 12 bits, as we did precalculate LookUpTables for 12 bit masks;
	//  - Update the indexes for each CardFaceMask;
	//  - "Cut" the 52bits HandMask to the "active" 12 bits;
	//  - Copy the value of this 12 bits for each Mask and multiply by the corresponding index;

	uint_fast64_t bitIndex_13b_base2{ 1 };
	uint_fast64_t bitIndex_13b_base10{ 1 };
	uint_fast8_t bitIndex_3b{ 1 };
	uint_fast16_t temp_cardFaceMask_3bits_base4{ 0 };

	for (int i = 4; i >= 0; i--)
	{
		//bitIndex_13b = std::pow(10, i*3); //This way is much faster;
		switch (i)
		{
		case 0:
			bitIndex_13b_base2 = 1;
			bitIndex_13b_base10 = 1;
			break;
		case 1:
			bitIndex_13b_base2 = 1000;
			bitIndex_13b_base10 = 8;
			break;
		case 2:
			bitIndex_13b_base2 = 1000000;
			bitIndex_13b_base10 = 64;
			break;
		case 3:
			bitIndex_13b_base2 = 1000000000;
			bitIndex_13b_base10 = 512;
			break;
		case 4:
			bitIndex_13b_base2 = 1000000000000;
			bitIndex_13b_base10 = 4096;
			break;
		}

		this->cardFaceMask_13bits_base2 += luTables.getIndex_Base2_12bits((handMask_52b >> (i * 12)) & 0xFFF) * bitIndex_13b_base2;
		this->cardFaceMask_13bits_base10 += luTables.getIndex_Base10_12bits((handMask_52b >> (i * 12)) & 0xFFF) * bitIndex_13b_base10;
		this->cardFaceMask_13bits_base5 += luTables.getIndex_Base5_12bits_full((handMask_52b >> (i * 12)) & 0xFFF) * bitIndex_13b_base2;

		this->cardFaceMask_Spades_13bits_base10 += luTables.getIndex_Base10_12bits_Spades((handMask_52b >> (i * 12)) & 0b100010001000) * bitIndex_13b_base10;
		this->cardFaceMask_Hearts_13bits_base10 += luTables.getIndex_Base10_12bits_Hearts((handMask_52b >> (i * 12)) & 0b010001000100) * bitIndex_13b_base10;
		this->cardFaceMask_Diamonds_13bits_base10 += luTables.getIndex_Base10_12bits_Diamonds((handMask_52b >> (i * 12)) & 0b001000100010) * bitIndex_13b_base10;
		this->cardFaceMask_Clubs_13bits_base10 += luTables.getIndex_Base10_12bits_Clubs((handMask_52b >> (i * 12)) & 0b000100010001) * bitIndex_13b_base10;


		temp_cardFaceMask_3bits_base4 = luTables.getIndex_Base5_12bits_short((handMask_52b >> (i * 12)) & 0xFFF);
		if (temp_cardFaceMask_3bits_base4 > 1)
		{
			if (temp_cardFaceMask_3bits_base4 < 10)
			{
				this->cardFaceMask_3bits_base5 += temp_cardFaceMask_3bits_base4 * bitIndex_3b;
				bitIndex_3b *= 10;
			}
			else
			{
				this->cardFaceMask_3bits_base5 += temp_cardFaceMask_3bits_base4 * bitIndex_3b;
				bitIndex_3b *= 100;
			}
		}
		
	}
};

inline const uint_fast64_t convertCardFaceToCardMask(const std::string& CARD)
{
	// Return the Enum value of the Hand;
	if (CARD[0] == '2')
	{
		if (CARD[1] == 's')
		{
			return static_cast<uint_fast64_t>(CardMask::_2s);
		}
		else if (CARD[1] == 'd')
		{
			return static_cast<uint_fast64_t>(CardMask::_2d);
		}
		else if (CARD[1] == 'h')
		{
			return static_cast<uint_fast64_t>(CardMask::_2h);
		}
		else if (CARD[1] == 'c')
		{
			return static_cast<uint_fast64_t>(CardMask::_2c);
		}
	}
	else if (CARD[0] == '3')
	{
		if (CARD[1] == 's')
		{
			return static_cast<uint_fast64_t>(CardMask::_3s);
		}
		else if (CARD[1] == 'd')
		{
			return static_cast<uint_fast64_t>(CardMask::_3d);
		}
		else if (CARD[1] == 'h')
		{
			return static_cast<uint_fast64_t>(CardMask::_3h);
		}
		else if (CARD[1] == 'c')
		{
			return static_cast<uint_fast64_t>(CardMask::_3c);
		}
	}
	else if (CARD[0] == '4')
	{
		if (CARD[1] == 's')
		{
			return static_cast<uint_fast64_t>(CardMask::_4s);
		}
		else if (CARD[1] == 'd')
		{
			return static_cast<uint_fast64_t>(CardMask::_4d);
		}
		else if (CARD[1] == 'h')
		{
			return static_cast<uint_fast64_t>(CardMask::_4h);
		}
		else if (CARD[1] == 'c')
		{
			return static_cast<uint_fast64_t>(CardMask::_4c);
		}
	}
	else if (CARD[0] == '5')
	{
		if (CARD[1] == 's')
		{
			return static_cast<uint_fast64_t>(CardMask::_5s);
		}
		else if (CARD[1] == 'd')
		{
			return static_cast<uint_fast64_t>(CardMask::_5d);
		}
		else if (CARD[1] == 'h')
		{
			return static_cast<uint_fast64_t>(CardMask::_5h);
		}
		else if (CARD[1] == 'c')
		{
			return static_cast<uint_fast64_t>(CardMask::_5c);
		}
	}
	else if (CARD[0] == '6')
	{
		if (CARD[1] == 's')
		{
			return static_cast<uint_fast64_t>(CardMask::_6s);
		}
		else if (CARD[1] == 'd')
		{
			return static_cast<uint_fast64_t>(CardMask::_6d);
		}
		else if (CARD[1] == 'h')
		{
			return static_cast<uint_fast64_t>(CardMask::_6h);
		}
		else if (CARD[1] == 'c')
		{
			return static_cast<uint_fast64_t>(CardMask::_6c);
		}
	}
	else if (CARD[0] == '7')
	{
		if (CARD[1] == 's')
		{
			return static_cast<uint_fast64_t>(CardMask::_7s);
		}
		else if (CARD[1] == 'd')
		{
			return static_cast<uint_fast64_t>(CardMask::_7d);
		}
		else if (CARD[1] == 'h')
		{
			return static_cast<uint_fast64_t>(CardMask::_7h);
		}
		else if (CARD[1] == 'c')
		{
			return static_cast<uint_fast64_t>(CardMask::_7c);
		}
	}
	else if (CARD[0] == '8')
	{
		if (CARD[1] == 's')
		{
			return static_cast<uint_fast64_t>(CardMask::_8s);
		}
		else if (CARD[1] == 'd')
		{
			return static_cast<uint_fast64_t>(CardMask::_8d);
		}
		else if (CARD[1] == 'h')
		{
			return static_cast<uint_fast64_t>(CardMask::_8h);
		}
		else if (CARD[1] == 'c')
		{
			return static_cast<uint_fast64_t>(CardMask::_8c);
		}
	}
	else if (CARD[0] == '9')
	{
		if (CARD[1] == 's')
		{
			return static_cast<uint_fast64_t>(CardMask::_9s);
		}
		else if (CARD[1] == 'd')
		{
			return static_cast<uint_fast64_t>(CardMask::_9d);
		}
		else if (CARD[1] == 'h')
		{
			return static_cast<uint_fast64_t>(CardMask::_9h);
		}
		else if (CARD[1] == 'c')
		{
			return static_cast<uint_fast64_t>(CardMask::_9c);
		}
	}
	else if (CARD[0] == 'T')
	{
		if (CARD[1] == 's')
		{
			return static_cast<uint_fast64_t>(CardMask::_Ts);
		}
		else if (CARD[1] == 'd')
		{
			return static_cast<uint_fast64_t>(CardMask::_Td);
		}
		else if (CARD[1] == 'h')
		{
			return static_cast<uint_fast64_t>(CardMask::_Th);
		}
		else if (CARD[1] == 'c')
		{
			return static_cast<uint_fast64_t>(CardMask::_Tc);
		}
	}
	else if (CARD[0] == 'J')
	{
		if (CARD[1] == 's')
		{
			return static_cast<uint_fast64_t>(CardMask::_Js);
		}
		else if (CARD[1] == 'd')
		{
			return static_cast<uint_fast64_t>(CardMask::_Jd);
		}
		else if (CARD[1] == 'h')
		{
			return static_cast<uint_fast64_t>(CardMask::_Jh);
		}
		else if (CARD[1] == 'c')
		{
			return static_cast<uint_fast64_t>(CardMask::_Jc);
		}
	}
	else if (CARD[0] == 'Q')
	{
		if (CARD[1] == 's')
		{
			return static_cast<uint_fast64_t>(CardMask::_Qs);
		}
		else if (CARD[1] == 'd')
		{
			return static_cast<uint_fast64_t>(CardMask::_Qd);
		}
		else if (CARD[1] == 'h')
		{
			return static_cast<uint_fast64_t>(CardMask::_Qh);
		}
		else if (CARD[1] == 'c')
		{
			return static_cast<uint_fast64_t>(CardMask::_Qc);
		}
	}
	else if (CARD[0] == 'K')
	{
		if (CARD[1] == 's')
		{
			return static_cast<uint_fast64_t>(CardMask::_Ks);
		}
		else if (CARD[1] == 'd')
		{
			return static_cast<uint_fast64_t>(CardMask::_Kd);
		}
		else if (CARD[1] == 'h')
		{
			return static_cast<uint_fast64_t>(CardMask::_Kh);
		}
		else if (CARD[1] == 'c')
		{
			return static_cast<uint_fast64_t>(CardMask::_Kc);
		}
	}
	else if (CARD[0] == 'A')
	{
		if (CARD[1] == 's')
		{
			return static_cast<uint_fast64_t>(CardMask::_As);
		}
		else if (CARD[1] == 'd')
		{
			return static_cast<uint_fast64_t>(CardMask::_Ad);
		}
		else if (CARD[1] == 'h')
		{
			return static_cast<uint_fast64_t>(CardMask::_Ah);
		}
		else if (CARD[1] == 'c')
		{
			return static_cast<uint_fast64_t>(CardMask::_Ac);
		}
	}
	else
	{
		throw std::runtime_error("Wrong Card input ConvertCardFaceToCardMask(const std::string& CARD)");
	}
}

void Hand::computeHand(LuTables& luTables)
{
	// 1. Calculate the Pair's value;
	// 2. Calculate the Flush/STR8FL;
	// 3. Calculate the STR8 values;
	// As handValues of different categories cross eachother we use if-else to make sure that we dont "downgrade" the handPower;

	calculatePairs(*this, luTables);

	if (this->handPower < HANDPOWER_FLUSH)
	{
		calculateFlushAndStraightFlushPlus(*this, luTables);
	}

	if (this->handPower < HANDPOWER_STRAIGHT)
	{
		calculateStraight(*this, luTables);
	}
}
void Hand::reset()
{
	// Set handpower and all necessary CardFaceMAsk that need to be recalculated to 0;
	this->handPower = 0;
	this->cardFaceMask_13bits_base2 = 0;
	this->cardFaceMask_13bits_base5 = 0;
	this->cardFaceMask_13bits_base10 = 0;
	this->cardFaceMask_3bits_base5 = 0;

	this->cardFaceMask_Spades_13bits_base10 = 0;
	this->cardFaceMask_Hearts_13bits_base10 = 0;
	this->cardFaceMask_Diamonds_13bits_base10 = 0;
	this->cardFaceMask_Clubs_13bits_base10 = 0;
}

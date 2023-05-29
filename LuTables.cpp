#pragma once
#include "LuTables.h"
#include "Hand.h"


void LuTables::computeStraightFlushTable()
{
    // Loop Through all indexes(CardFaceMasks);
    // Loop Though all CardFace's in the 13-bit CardFaceMask;
    //  - Update the straightFlushMask and check if the current CardFaceMask includes the StraightFlushMask; If yes set the handPower to 9XX00000000(for STR8 we look only for 1 Kicker) and exit the loop;
    //  - If we reach the last CardFace in the inner Loop check for Alow-StraightFlush(A2345) and if available update handPower;

    //  As every Hand contains exactly 7 cards, we have to check only the range { lowest 7bits to highest 7bits == 1 };    
    for (size_t i = LuTables::SIZE_FLUSH - 1; i >= 0b11111; i--)
    {
        uint_fast16_t straightFlushMask{ 0b11111 };
        uint_fast16_t straightFlushMaskA5{ 0b1000000001111 };
        uint_fast64_t handPower{ 0 };

        for (int j = 9; j >= 0; j--)
        {

            if ((i & (straightFlushMask << j)) == (straightFlushMask << j)) //  As we are working with 13bits mask we can ignore the 4 Bytes overflow warnings;
            {
                handPower += (j + 5) * OFFSET_KICKER_1 + HANDPOWER_STRAIGHTFLUSH;
                luTableFlushAndStraightFlushPlus[i] = handPower;
                j = -1;
            }
            if (j == 0)
            {
                if ((i & straightFlushMaskA5) == straightFlushMaskA5)
                {
                    luTableFlushAndStraightFlushPlus[i] = 90400000000;
                }
            }
        }
    }
}

void LuTables::computeFlushTable()
{
    // !!! We use as index to access the Array that we calculate here the CardMAsk by Suit, maening we have to check it 4 times for each suit;
    // 
    // Loop Through all indexes(CardFaceMasks);
    // Loop Though all CardFace's in the 13-bit CardFaceMask;
    //  - Check if the current bit is set and if yes increase cntSetBits, save the kicker and update the kickerIndex;
    //  - If cntSetBits == 5 update the handPower to Flush and exit the loop;

    //As every Hand contains exactly 7 cards, we have to check only the range { lowest 7bits to highest 7bits == 1 };
    //As we are working with 13bits mask we can ignore the 4 Bytes overflow warnings;
    for (size_t i = LuTables::SIZE_FLUSH-1; i >= 0b11111; i--)
    {
        uint_fast8_t cntSetBits{0}; // 
        uint_fast64_t kickerOffset{ OFFSET_KICKER_1 };
        uint_fast64_t handPower{ 0 };

        for (int j = Hand::CARDFACEMASK_BITS - 1; j >= 0; j--)
        {
            if ((i & (1 << j)) == (1 << j))
            {
                handPower += (j + 1) * kickerOffset;
                kickerOffset /= 100;
                ++cntSetBits;
            }
            if (cntSetBits == 5)
            {
                handPower += HANDPOWER_FLUSH;
                luTableFlushAndStraightFlushPlus[i] = handPower;
                j = -1;
            }
        }
    }
}

void LuTables::computeStraightTable()
{
    // Loop Through all indexes(CardFaceMasks);
    // Loop Though all CardFace's in the 13-bit CardFaceMask;
    //  - Update the straightMask and check if the current CardFaceMask includes the StraightMask; If yes set the handPower to 5XX00000000(for STR8 we look only for 1 Kicker) and exit the loop;
    //  - If we reach the last CardFace in the inner Loop check for Alow-Straight(A2345) and if available update handPower;
    // It's very similar to computeStraightFlushTable() , however here we set the handPower to 5xx00000000;

    //As every Hand contains exactly 7 cards, we have to check only the range { lowest 7bits to highest 7bits == 1 };
    for (size_t i = LuTables::SIZE_STRAIGHT - 1; i >= 0b11111; i--)
    {
        //if (i ==4032)
        //{
        //    bool xx = true;
        //}
        uint_fast16_t straightMask{ 0b11111 };
        uint_fast16_t straightMaskA5{ 0b1000000001111 };
        for (int j = 8; j >=0; j--)
        {

            if ((i & straightMask << j) == straightMask << j) // As we are working with 13bits mask we can ignore the 4 Bytes overflow warnings;
            {
                LuTables::luTableStraight[i] = HANDPOWER_STRAIGHT + (j+5)*OFFSET_KICKER_1;
                j = -1;
            }
            if (j==0)
            {
                if ((i & straightMaskA5) == straightMaskA5)
                {
                    LuTables::luTableStraight[i] = 50400000000;
                }
            }
            
        }  
    }
}

void LuTables::compute2PairsPlusTable()
{
    // Loop Through all indexes(CardFaceMasks);
    // Loop through any Digit of the index:
    //  - Update cntPair/cntTrips; If we reach currDigit == 4, Quads is available, set handPower to 80000000000 and exit the loop;
    //  - If we reach the last Digit, check what is the highest pair's combination and set handPower accordingly;

    LuTables::luTablePairs[0] = HANDPOWER_HIGHCARD; //Set [0] = HighCard

    //As we search only for 2Ps+ => min gonna be with 0 set Base-5 "bits" and max gonna be with 3 set Base-4"bits", we have to check only the range { 2-322 };
    for (size_t i = LuTables::SIZE_PAIRS - 1; i > 1; i--)
    {

        std::string currNum = std::to_string(i);
        uint_fast16_t cntCurrNumDigits = currNum.size();
        uint_fast16_t cntTrips = 0;
        uint_fast16_t cntPairs = 0;
        for (size_t j = 0; j < cntCurrNumDigits; j++)
        {
            uint_fast16_t currDigit = currNum[j] - '0';

            if (currDigit == 2)
            {
                ++cntPairs;
            }
            else if (currDigit == 3)
            {
                ++cntTrips;
            }
            else if (currDigit == 4)
            {
                LuTables::luTablePairs[i] = HANDPOWER_FOUROFAKIND; // Quads
                j = cntCurrNumDigits;
            }
            else // invalid mask
            {
                j = cntCurrNumDigits;
            }

            if (j== cntCurrNumDigits - 1)
            {
                if (cntTrips == 0)
                {
                    
                    if (cntPairs == 1)
                    {
                        LuTables::luTablePairs[i] = HANDPOWER_ONEPAIR; // OnePair
                    }
                    else
                    {
                        LuTables::luTablePairs[i] = HANDPOWER_TWOPAIRS; // TwoPairs
                    }
                }
                else if (cntTrips == 1)
                {
                    if (cntPairs == 0)
                    {
                        LuTables::luTablePairs[i] = HANDPOWER_THREEOFAKIND; // ThreeOfAKind
                    }
                    else
                    {
                        LuTables::luTablePairs[i] = HANDPOWER_FULLHOUSE; // FH
                    }
                }
                else // => cntTrips == 2
                {
                    LuTables::luTablePairs[i] = HANDPOWER_FULLHOUSE; // FH
                }
            }
        }
    }
}

void LuTables::compute12BitsTables()
{
    // We look at all 12bit numbers dividing them into 3 pieces of 4 bits;
    // Loop Through all indexes(CardFaceMasks);
    // Loop Though all CardFace's in the 13-bit CardFaceMask;
    //  - Update the bitIndexes;
    //  - Find the 4bits number and update the Arrays by adding the Value multiplied by Index;
    // Convert to base_10 and update the base10 Arrays;

    //As we are working with 12bits mask we can ignore the 4 Bytes overflow warnings;
    for (size_t i = 0; i < LuTables::SIZE_12BITS; i++)
    {
        uint_fast64_t temp = 0;
        uint_fast16_t temp_total = 0;
        uint_fast16_t bitIndex_13b{0};
        uint_fast16_t bitIndex_3b{1};

        for (int j = 8; j >= 0; j -= 4)
        {
            temp = (i & (0b1111 << j)) >> j;
            
            //bitIndex_13b = std::pow(10, j / 4); This way is much faster;
            switch (j)
            {
            case 0:
                bitIndex_13b = 1;
                break;
            case 4:
                bitIndex_13b = 10;
                break;
            case 8:
                bitIndex_13b = 100;
                break;
            }

            switch (temp)
            {
            //case 0b0000:
            //    LuTables::sBase2_12bits[i] += 0;
            //    LuTables::sBase4_12bits_full[i] += 0;
            //    break;
            case 0b0001:
                LuTables::base2_12bits[i]           += 1 * bitIndex_13b;
                LuTables::base5_12bits_full[i]      += 1 * bitIndex_13b;
                LuTables::base2_12bits_Clubs[i]     += 1 * bitIndex_13b;
                break;
            case 0b0010:
                LuTables::base2_12bits[i]           += 1 * bitIndex_13b;
                LuTables::base5_12bits_full[i]      += 1 * bitIndex_13b;
                LuTables::base2_12bits_Diamonds[i]  += 1 * bitIndex_13b;
                break;
            case 0b0011:
                LuTables::base2_12bits[i]           += 1 * bitIndex_13b;
                LuTables::base5_12bits_full[i]      += 2 * bitIndex_13b;
                LuTables::base5_12bits_short[i]     += 2 * bitIndex_3b;
                bitIndex_3b *= 10;
                LuTables::base2_12bits_Clubs[i]     += 1 * bitIndex_13b;
                LuTables::base2_12bits_Diamonds[i]  += 1 * bitIndex_13b;
                break;
            case 0b0100:
                LuTables::base2_12bits[i]           += 1 * bitIndex_13b;
                LuTables::base5_12bits_full[i]      += 1 * bitIndex_13b;
                LuTables::base2_12bits_Hearts[i]    += 1 * bitIndex_13b;
                break;
            case 0b0101:
                LuTables::base2_12bits[i]           += 1 * bitIndex_13b;
                LuTables::base5_12bits_full[i]      += 2 * bitIndex_13b;
                LuTables::base5_12bits_short[i]     += 2 * bitIndex_3b;
                bitIndex_3b *= 10;
                LuTables::base2_12bits_Clubs[i]     += 1 * bitIndex_13b;
                LuTables::base2_12bits_Hearts[i]    += 1 * bitIndex_13b;
                break;
            case 0b0110:
                LuTables::base2_12bits[i]           += 1 * bitIndex_13b;
                LuTables::base5_12bits_full[i]      += 2 * bitIndex_13b;
                LuTables::base5_12bits_short[i]     += 2 * bitIndex_3b;
                bitIndex_3b *= 10;
                LuTables::base2_12bits_Diamonds[i]  += 1 * bitIndex_13b;
                LuTables::base2_12bits_Hearts[i]    += 1 * bitIndex_13b;
                break;
            case 0b0111:
                LuTables::base2_12bits[i]           += 1 * bitIndex_13b;
                LuTables::base5_12bits_full[i]      += 3 * bitIndex_13b;
                LuTables::base5_12bits_short[i]     += 3 * bitIndex_3b;
                bitIndex_3b *= 10;
                LuTables::base2_12bits_Clubs[i]     += 1 * bitIndex_13b;
                LuTables::base2_12bits_Diamonds[i]  += 1 * bitIndex_13b;
                LuTables::base2_12bits_Hearts[i]    += 1 * bitIndex_13b;
                break;
            case 0b1000:
                LuTables::base2_12bits[i]           += 1 * bitIndex_13b;
                LuTables::base5_12bits_full[i]      += 1 * bitIndex_13b;
                LuTables::base2_12bits_Spades[i]    += 1 * bitIndex_13b;
                break;
            case 0b1001:
                LuTables::base2_12bits[i]           += 1 * bitIndex_13b;
                LuTables::base5_12bits_full[i]      += 2 * bitIndex_13b;
                LuTables::base5_12bits_short[i]     += 2 * bitIndex_3b;
                bitIndex_3b *= 10;
                LuTables::base2_12bits_Clubs[i]     += 1 * bitIndex_13b;
                LuTables::base2_12bits_Spades[i]    += 1 * bitIndex_13b;
                break;
            case 0b1010:
                LuTables::base2_12bits[i]           += 1 * bitIndex_13b;
                LuTables::base5_12bits_full[i]      += 2 * bitIndex_13b;
                LuTables::base5_12bits_short[i]     += 2 * bitIndex_3b;
                bitIndex_3b *= 10;
                LuTables::base2_12bits_Diamonds[i]  += 1 * bitIndex_13b;
                LuTables::base2_12bits_Spades[i]    += 1 * bitIndex_13b;
                break;
            case 0b1011:
                LuTables::base2_12bits[i]           += 1 * bitIndex_13b;
                LuTables::base5_12bits_full[i]      += 3 * bitIndex_13b;
                LuTables::base5_12bits_short[i]     += 3 * bitIndex_3b;
                bitIndex_3b *= 10;
                LuTables::base2_12bits_Clubs[i]     += 1 * bitIndex_13b;
                LuTables::base2_12bits_Diamonds[i]  += 1 * bitIndex_13b;
                LuTables::base2_12bits_Spades[i]    += 1 * bitIndex_13b;
                break;
            case 0b1100:
                LuTables::base2_12bits[i]           += 1 * bitIndex_13b;
                LuTables::base5_12bits_full[i]      += 2 * bitIndex_13b;
                LuTables::base5_12bits_short[i]     += 2 * bitIndex_3b;
                bitIndex_3b *= 10;
                LuTables::base2_12bits_Hearts[i]    += 1 * bitIndex_13b;
                LuTables::base2_12bits_Spades[i]    += 1 * bitIndex_13b;
                break;
            case 0b1101:
                LuTables::base2_12bits[i]           += 1 * bitIndex_13b;
                LuTables::base5_12bits_full[i]      += 3 * bitIndex_13b;
                LuTables::base5_12bits_short[i]     += 3 * bitIndex_3b;
                bitIndex_3b *= 10;
                LuTables::base2_12bits_Clubs[i]     += 1 * bitIndex_13b;
                LuTables::base2_12bits_Hearts[i]    += 1 * bitIndex_13b;
                LuTables::base2_12bits_Spades[i]    += 1 * bitIndex_13b;
                break;
            case 0b1110:
                LuTables::base2_12bits[i]           += 1 * bitIndex_13b;
                LuTables::base5_12bits_full[i]      += 3 * bitIndex_13b;
                LuTables::base5_12bits_short[i]     += 3 * bitIndex_3b;
                bitIndex_3b *= 10;
                LuTables::base2_12bits_Diamonds[i]  += 1 * bitIndex_13b;
                LuTables::base2_12bits_Hearts[i]    += 1 * bitIndex_13b;
                LuTables::base2_12bits_Spades[i]    += 1 * bitIndex_13b;
                break;
            case 0b1111:
                LuTables::base2_12bits[i]           += 1 * bitIndex_13b;
                LuTables::base5_12bits_full[i]      += 4 * bitIndex_13b;
                LuTables::base5_12bits_short[i]     += 4 * bitIndex_3b;
                bitIndex_3b *= 10;
                LuTables::base2_12bits_Clubs[i]     += 1 * bitIndex_13b;
                LuTables::base2_12bits_Diamonds[i]  += 1 * bitIndex_13b;
                LuTables::base2_12bits_Hearts[i]    += 1 * bitIndex_13b;
                LuTables::base2_12bits_Spades[i]    += 1 * bitIndex_13b;
                break;
            }  
        }

        base10_12bits[i]            = std::stoi(std::to_string(base2_12bits[i]), 0, 2);
        base10_12bits_Clubs[i]      = std::stoi(std::to_string(base2_12bits_Clubs[i]), 0, 2);
        base10_12bits_Diamonds[i]   = std::stoi(std::to_string(base2_12bits_Diamonds[i]), 0, 2);
        base10_12bits_Hearts[i]     = std::stoi(std::to_string(base2_12bits_Hearts[i]), 0, 2);
        base10_12bits_Spades[i]     = std::stoi(std::to_string(base2_12bits_Spades[i]), 0, 2);
    }
}

uint_fast64_t LuTables::getStraightHandPower(const uint_fast16_t& index) const
{
    return this->luTableStraight[index];
}

uint_fast64_t LuTables::getFlushAndStraightFlushHandPower(const uint_fast16_t& index) const
{
    return this->luTableFlushAndStraightFlushPlus[index];
}

uint_fast64_t LuTables::getPairsHandPower(const uint_fast16_t& index) const
{
    return luTablePairs[index];
}

uint_fast16_t LuTables::getIndex_Base5_12bits_short(const uint_fast16_t& index) const
{
    return this->base5_12bits_short[index];
}

uint_fast16_t LuTables::getIndex_Base5_12bits_full(const uint_fast16_t& index) const
{
    return this->base5_12bits_full[index];
}

uint_fast16_t LuTables::getIndex_Base2_12bits(const uint_fast16_t& index) const
{
    return this->base2_12bits[index];
}

uint_fast16_t LuTables::getIndex_Base10_12bits(const uint_fast16_t& index) const
{
    return this->base10_12bits[index];
}

uint_fast16_t LuTables::getIndex_Base2_12bits_Spades(const uint_fast16_t& index) const
{
    return this->base2_12bits_Spades[index];
}

uint_fast16_t LuTables::getIndex_Base10_12bits_Spades(const uint_fast16_t& index) const
{
    return this->base10_12bits_Spades[index];
}

uint_fast16_t LuTables::getIndex_Base2_12bits_Hearts(const uint_fast16_t& index) const
{
    return this->base2_12bits_Hearts[index];
}

uint_fast16_t LuTables::getIndex_Base10_12bits_Hearts(const uint_fast16_t& index) const
{
    return this->base10_12bits_Hearts[index];
}

uint_fast16_t LuTables::getIndex_Base2_12bits_Diamonds(const uint_fast16_t& index) const
{
    return this->base2_12bits_Diamonds[index];
}

uint_fast16_t LuTables::getIndex_Base10_12bits_Diamonds(const uint_fast16_t& index) const
{
    return this->base10_12bits_Diamonds[index];
}

uint_fast16_t LuTables::getIndex_Base2_12bits_Clubs(const uint_fast16_t& index) const
{
    return this->base2_12bits_Clubs[index];
}

uint_fast16_t LuTables::getIndex_Base10_12bits_Clubs(const uint_fast16_t& index) const
{
    return this->base10_12bits_Clubs[index];
}
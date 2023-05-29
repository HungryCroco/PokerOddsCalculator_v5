#pragma once
#include "LuTables.h"

    //          -- HandPower Values --
    //    Empty = 0,
    //    HighCard = 1 * OFFSET_HANDSTRENGHT,
    //    OnePair = 2 * OFFSET_HANDSTRENGHT,
    //    TwoPairs = 3 * OFFSET_HANDSTRENGHT,
    //    ThreeOfAKind = 4 * OFFSET_HANDSTRENGHT,
    //    Straight = 5 * OFFSET_HANDSTRENGHT,
    //    Flush = 6 * OFFSET_HANDSTRENGHT,
    //    FullHouse = 7 * OFFSET_HANDSTRENGHT,
    //    FourOfAKind = 8 * OFFSET_HANDSTRENGHT,
    //    StraightFlush = 9 * OFFSET_HANDSTRENGHT //As RoyalFL is actually a STR8FL with the highest kicker and we dont print the HandPower, we dont really set the handPower to 10, we leave it as 9 + kicker A;


class Hand
{

public:
    Hand() {};
    Hand(const std::string& all_cards_param);

    static const uint_fast8_t FULLHANDMASK_BITS = 52;       // A 2-A deck is containing 52 Cards;

    static const uint_fast8_t CARDFACEMASK_BITS = 13;       // A 2-A deck is containing 13 CardFaces (52 Cards/ 4 Suits);

    uint_fast64_t handMask_52b{};                           // HandMask of 52 bits , every Card represents 1 bit (2c = 0, As = 51); If a bit is set, the representing card is contained in the Hand;
    uint_fast64_t cardFaceMask_13bits_base2{};              // HandMask of 13 bits, representing CardFaces; Set if any of the 4 corresponding CardFace-Bits in the handMask_52b is set; for example 0b11000 represents in handMask_52b 3c = 1and 2s = 1 -> 0000000000011;
    uint_fast16_t cardFaceMask_13bits_base10{};             // Base10 representation of cardFaceMask_13bits_base2;
    uint_fast64_t cardFaceMask_13bits_base5{};              // HandMask of 13 bits in Base5, representing CardFaces; Counts all 4 corresponding CardFace-Bits in the handMask_52b; for example handMask_52b{0b1000} = -> 0000000000001; handMask_52b{0b1110} = -> 0000000000003;
    uint_fast16_t cardFaceMask_3bits_base5{};               // HandMask of 3 bits in Base5, representing CardFaces; Counts all 4 corresponding CardFace-Bits in the handMask_52b; for example handMask_52b{0b1000} = -> 0; handMask_52b{0b1110} = -> 3; However, set only if the CardFace is 2,3 or 4, meaning that a pair+ is available;

    uint_fast16_t cardFaceMask_Spades_13bits_base10{};      // HandMask of 13 bits, representing only CardFaces of SPADES; Set if any of the 4 corresponding CardFace-Bits in the handMask_52b is set; for example 0b00011000 represents in handMask_52b 3c = 1and 2s = 1 -> 0000000000001;
    uint_fast16_t cardFaceMask_Hearts_13bits_base10{};      // HandMask of 13 bits, representing only CardFaces of HEARTS; Set if any of the 4 corresponding CardFace-Bits in the handMask_52b is set; for example 0b01001000 represents in handMask_52b 3h = 1and 2s = 1 -> 0000000000010;
    uint_fast16_t cardFaceMask_Diamonds_13bits_base10{};    // HandMask of 13 bits, representing only CardFaces of DIAMONDS; Set if any of the 4 corresponding CardFace-Bits in the handMask_52b is set; for example 0b00101000 represents in handMask_52b 3d = 1and 2s = 1 -> 0000000000010;
    uint_fast16_t cardFaceMask_Clubs_13bits_base10{};       // HandMask of 13 bits, representing only CardFaces of CLUBS; Set if any of the 4 corresponding CardFace-Bits in the handMask_52b is set; for example 0b11000 represents in handMask_52b 3c = 1and 2s = 1 -> 0000000000010;

    
    uint_fast64_t handPower{ 0 };                           // 60102030405 6 - handStrength {0-9}, 05-01 - kicker5-1{1-13}

    unsigned short street{ 0 };                             // 0 = no HCs, 2 = PREFLOP, 5 = FLOP, 6 = TURN, 7 = RIVER

    // -> Calculates all cardFaceMasks;
    void calculateHandMasks(LuTables& luTables);

    // -> Resets all cardFaceMAsks to 0;
    void reset();

    // -> Calculates and modifies the handPower;
    void computeHand(LuTables& luTables);
    

private:
    

};

const uint_fast64_t OFFSET_HANDSTRENGHT{ 10000000000 };     // Set the position of the handStrength in the HandPower;
const uint_fast32_t OFFSET_KICKER_1{100000000};             // Set the 1.Kicker of the handStrength in the HandPower;
const uint_fast32_t OFFSET_KICKER_2{1000000};               // Set the 2.Kicker of the handStrength in the HandPower;
const uint_fast16_t OFFSET_KICKER_3{10000};                 // Set the 3.Kicker of the handStrength in the HandPower;
const uint_fast8_t OFFSET_KICKER_4{100};                    // Set the 4.Kicker of the handStrength in the HandPower;
const uint_fast8_t OFFSET_KICKER_5{1};                      // Set the 5.Kicker of the handStrength in the HandPower;


const uint_fast64_t HANDPOWER_HIGHCARD{ 1 * OFFSET_HANDSTRENGHT };      // 1
const uint_fast64_t HANDPOWER_ONEPAIR{ 2 * OFFSET_HANDSTRENGHT };       // 2
const uint_fast64_t HANDPOWER_TWOPAIRS{ 3 * OFFSET_HANDSTRENGHT };      // 3
const uint_fast64_t HANDPOWER_THREEOFAKIND{ 4 * OFFSET_HANDSTRENGHT };  // 4
const uint_fast64_t HANDPOWER_STRAIGHT{ 5 * OFFSET_HANDSTRENGHT };      // 5
const uint_fast64_t HANDPOWER_FLUSH{ 6 * OFFSET_HANDSTRENGHT };         // 6
const uint_fast64_t HANDPOWER_FULLHOUSE{ 7 * OFFSET_HANDSTRENGHT };     // 7
const uint_fast64_t HANDPOWER_FOUROFAKIND{ 8 * OFFSET_HANDSTRENGHT };   // 8
const uint_fast64_t HANDPOWER_STRAIGHTFLUSH{ 9 * OFFSET_HANDSTRENGHT }; // 9; As RoyalFL is actually a STR8FL with the highest kicker and we dont print the HandPower, we dont really set the handPower to 10, we leave it as 9 + kicker A;

// -> Converts a string containing Card to handMask_52b; Example As2c = 0b1000000000000000000000000000000000000000000000000001 ;
inline const uint_fast64_t convertCardFaceToCardMask(const std::string& CARD);

// -> Set each Card to the corresponding handMask_52b; Example 2c = 0b1; 3c = 0b10000;
// -> CardFaces are in order 2(bitPositions 0-3) to A (bitpositions 48-51);
// -> CardSuits are in order Clubs (0,4, 8 ... 48) -> Diamonds (1,5, 9 ... 49)-> Hearts (2,6, 10 ... 50)-> Spades (3,7, 11 ... 51);
enum class CardMask : uint_fast64_t
{
    _2c = 1LL,
    _2d = 1LL << 1LL,
    _2h = 1LL << 2LL,
    _2s = 1LL << 3LL,
    _3c = 1LL << 4LL,
    _3d = 1LL << 5LL,
    _3h = 1LL << 6LL,
    _3s = 1LL << 7LL,
    _4c = 1LL << 8LL,
    _4d = 1LL << 9LL,
    _4h = 1LL << 10LL,
    _4s = 1LL << 11LL,
    _5c = 1LL << 12LL,
    _5d = 1LL << 13LL,
    _5h = 1LL << 14LL,
    _5s = 1LL << 15LL,
    _6c = 1LL << 16LL,
    _6d = 1LL << 17LL,
    _6h = 1LL << 18LL,
    _6s = 1LL << 19LL,
    _7c = 1LL << 20LL,
    _7d = 1LL << 21LL,
    _7h = 1LL << 22LL,
    _7s = 1LL << 23LL,
    _8c = 1LL << 24LL,
    _8d = 1LL << 25LL,
    _8h = 1LL << 26LL,
    _8s = 1LL << 27LL,
    _9c = 1LL << 28LL,
    _9d = 1LL << 29LL,
    _9h = 1LL << 30LL,
    _9s = 1LL << 31LL,
    _Tc = 1LL << 32LL,
    _Td = 1LL << 33LL,
    _Th = 1LL << 34LL,
    _Ts = 1LL << 35LL,
    _Jc = 1LL << 36LL,
    _Jd = 1LL << 37LL,
    _Jh = 1LL << 38LL,
    _Js = 1LL << 39LL,
    _Qc = 1LL << 40LL,
    _Qd = 1LL << 41LL,
    _Qh = 1LL << 42LL,
    _Qs = 1LL << 43LL,
    _Kc = 1LL << 44LL,
    _Kd = 1LL << 45LL,
    _Kh = 1LL << 46LL,
    _Ks = 1LL << 47LL,
    _Ac = 1LL << 48LL,
    _Ad = 1LL << 49LL,
    _Ah = 1LL << 50LL,
    _As = 1LL << 51LL
};



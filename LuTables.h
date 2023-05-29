#ifndef ComputeLookupTables
#define ComputeLookupTables
#include <string>



// -> Class initializing a LuTablesSet with precalculates Arrays for all possible handPowers accessables via CardFace-indexes;
class LuTables
{
public:
    static const uint_fast16_t SIZE_STRAIGHT = 0b1111111000001;     // Size of the STRAIGHT Array ( 0b1111111000001 );
    static const uint_fast16_t SIZE_FLUSH = 0b1111111000001;        // Size of the FLUSH Array ( 0b1111111000001 );
    static const uint_fast16_t SIZE_PAIRS = 323;                    // Size of the PAIRS Array ( 322+1 )_10;
    static const uint_fast16_t SIZE_12BITS = 0b111111111111 +1;     // Size of the 12BITS Array ( 0b111111111111 +1 );

private:
    uint_fast64_t luTableStraight[SIZE_STRAIGHT]{};                 // Array containing the STAIGHT's handPower for all possible hand combinations accessible by index = f(CardFaceMask); 
    uint_fast64_t luTableFlushAndStraightFlushPlus[SIZE_FLUSH]{};   // Array containing the FLUSH's, STAIGHTFLUSH's and ROYALFLUSH's handPower for all possible hand combinations accessible by index = f(CardFaceMask by CardSuits); 

    uint_fast64_t luTablePairs[SIZE_PAIRS]{};                       // Array containing all PAIRS handPower for all possible hand combinations accessible by index = f(CardFaceMask_5); 

    uint_fast16_t base5_12bits_short[SIZE_12BITS]{};                // Array containing the count of SET-BITS for each 4bits; Zeros are deleted; Example: 1111 0000 0011 -> 42; 
    uint_fast16_t base5_12bits_full[SIZE_12BITS]{};                 // Array containing the count of SET-BITS for each 4bits; With the zeros; Example: 1111 0000 0011 -> 402; 

    uint_fast16_t base2_12bits[SIZE_12BITS]{};                      // Array containing the count of SET-NIBBLES; With the zeros; Example: 1111 0000 0011 -> 101; 
    uint_fast16_t base10_12bits[SIZE_12BITS]{};                     // Array containing the Base_10 represantation of the numbers in base2_12bits; Example 101 -> 5;

    uint_fast16_t base2_12bits_Spades[SIZE_12BITS]{};               // Array containing the count of SET-NIBBLES only for Spades position xxxY; With the zeros; Example: 0001 0000 0001 -> 101; 
    uint_fast16_t base10_12bits_Spades[SIZE_12BITS]{};              // Array containing the Base_10 represantation of the numbers in base10_12bits_Spades; Example 101 -> 5;
    uint_fast16_t base2_12bits_Hearts[SIZE_12BITS]{};               // Array containing the count of SET-NIBBLES only for Hearts position xxYx; With the zeros; Example: 0010 0000 0010 -> 101;
    uint_fast16_t base10_12bits_Hearts[SIZE_12BITS]{};              // Array containing the Base_10_Hearts represantation of the numbers in base10_12bits_Spades; Example 101 -> 5;
    uint_fast16_t base2_12bits_Diamonds[SIZE_12BITS]{};             // Array containing the count of SET-NIBBLES only for Diamonds position xYxx; With the zeros; Example: 0100 1000 0100 -> 101;
    uint_fast16_t base10_12bits_Diamonds[SIZE_12BITS]{};            // Array containing the Base_10_Diamonds represantation of the numbers in base10_12bits_Spades; Example 101 -> 5;
    uint_fast16_t base2_12bits_Clubs[SIZE_12BITS]{};                // Array containing the count of SET-NIBBLES only for Clubs position Yxxx; With the zeros; Example: 1001 0001 1000 -> 101;
    uint_fast16_t base10_12bits_Clubs[SIZE_12BITS]{};               // Array containing the Base_10_Clubs represantation of the numbers in base10_12bits_Spades; Example 101 -> 5;

private:
    void computeStraightTable();                                    // Function that Calculates the STRAIGHTs for all possible CardFaceMasks and writes the HandPower to luTableStraight; If no Straight -> 0;
    void computeFlushTable();                                       // Function that Calculates the FLUSHes for all possible CardFaceMasks(byCardSuit) and writes the HandPower to luTableFlushAndStraightFlushPlus (!!! Same as computeStraightFlushTable() !!!); If no Flush -> 0;
    void computeStraightFlushTable();                               // Function that Calculates the STRAIGHTFLUSHes for all possible CardFaceMasks(byCardSuit) and writes the HandPower to luTableFlushAndStraightFlushPlus (!!! Same as computeFlushTable() !!!); If no STRAIGHTFlush -> doesnt write;
    void compute2PairsPlusTable();                                  // Function that Calculates the highest Pair-Combo for all possible CardFaceMasks(base5) and writes the HandPower to luTablePairs;
    void compute12BitsTables();                                     // Function that Calculates all 12BitsTables;

public:
    // -> Computes a LuTablesSet;
    LuTables()
    {
        computeFlushTable(); // first have to compute Flush and after StraightFlush as both use the same  array and str8fl > fl;
        computeStraightFlushTable();
        
        computeStraightTable();
        compute2PairsPlusTable();
        compute12BitsTables();
    };

public:
    uint_fast64_t getStraightHandPower(const uint_fast16_t& index) const;               //Get-Function to access the STRAIGHT handPower via CardFace-index;
    uint_fast64_t getFlushAndStraightFlushHandPower(const uint_fast16_t& index) const;  //Get-Function to access the FLUSH/STR8FL handPower via CardFaceByCardSuit-index;
    uint_fast64_t getPairsHandPower(const uint_fast16_t& index) const;                  //Get-Function to access the PAIRs handPower via CardFace_base5_short-index;
    uint_fast16_t getIndex_Base5_12bits_short(const uint_fast16_t& index) const;        //Get-Function to access the Base5_12bits_short index via Position-index of the 12bitsMask in the 52bitsMask ( F FFF FFF FFF FFF );
    uint_fast16_t getIndex_Base5_12bits_full(const uint_fast16_t& index) const;         //Get-Function to access the Base5_12bits_full index via Position-index of the 12bitsMask in the 52bitsMask ( F FFF FFF FFF FFF );
    uint_fast16_t getIndex_Base2_12bits(const uint_fast16_t& index) const;              //Get-Function to access the Base2_12bits index via Position-index of the 12bitsMask in the 52bitsMask ( F FFF FFF FFF FFF );
    uint_fast16_t getIndex_Base10_12bits(const uint_fast16_t& index) const;             //Get-Function to access the Base10_12bits index via Position-index of the 12bitsMask in the 52bitsMask ( F FFF FFF FFF FFF );
    uint_fast16_t getIndex_Base2_12bits_Spades(const uint_fast16_t& index) const;       //Get-Function to access the Base2_12bits_Spades index via Position-index of the 12bitsMask in the 52bitsMask ( F FFF FFF FFF FFF );
    uint_fast16_t getIndex_Base10_12bits_Spades(const uint_fast16_t& index) const;      //Get-Function to access the Base10_12bits_Spades index via Position-index of the 12bitsMask in the 52bitsMask ( F FFF FFF FFF FFF );
    uint_fast16_t getIndex_Base2_12bits_Hearts(const uint_fast16_t& index) const;       //Get-Function to access the Base2_12bits_Hearts index via Position-index of the 12bitsMask in the 52bitsMask ( F FFF FFF FFF FFF );
    uint_fast16_t getIndex_Base10_12bits_Hearts(const uint_fast16_t& index) const;      //Get-Function to access the Base10_12bits_Hearts index via Position-index of the 12bitsMask in the 52bitsMask ( F FFF FFF FFF FFF );
    uint_fast16_t getIndex_Base2_12bits_Diamonds(const uint_fast16_t& index) const;     //Get-Function to access the Base2_12bits_Diamonds index via Position-index of the 12bitsMask in the 52bitsMask ( F FFF FFF FFF FFF );
    uint_fast16_t getIndex_Base10_12bits_Diamonds(const uint_fast16_t& index) const;    //Get-Function to access the Base10_12bits_Diamonds index via Position-index of the 12bitsMask in the 52bitsMask ( F FFF FFF FFF FFF );
    uint_fast16_t getIndex_Base2_12bits_Clubs(const uint_fast16_t& index) const;        //Get-Function to access the Base2_12bits_Clubs index via Position-index of the 12bitsMask in the 52bitsMask ( F FFF FFF FFF FFF );
    uint_fast16_t getIndex_Base10_12bits_Clubs(const uint_fast16_t& index) const;       //Get-Function to access the Base10_12bits_Clubs index via Position-index of the 12bitsMask in the 52bitsMask ( F FFF FFF FFF FFF );


};



#endif // ComputeLookupTables

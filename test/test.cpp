#include "chess.hpp"
#include "gtest/gtest.h"
#include <sstream>
#include <vector>
#include <string>

TEST(testChess, fileToColumn)
{
    EXPECT_EQ(chess::fileToColumn('a'), 0);
    EXPECT_EQ(chess::fileToColumn('b'), 1);
    EXPECT_EQ(chess::fileToColumn('c'), 2);
    EXPECT_EQ(chess::fileToColumn('d'), 3);
    EXPECT_EQ(chess::fileToColumn('e'), 4);
    EXPECT_EQ(chess::fileToColumn('f'), 5);
    EXPECT_EQ(chess::fileToColumn('g'), 6);
    EXPECT_EQ(chess::fileToColumn('h'), 7);
    EXPECT_THROW(chess::fileToColumn('`'),
    	std::invalid_argument);
    EXPECT_THROW(chess::fileToColumn('i'),
    	std::invalid_argument);
}

TEST(testChess, rankToRow)
{
    EXPECT_EQ(chess::rankToRow('1'), 0);
    EXPECT_EQ(chess::rankToRow('2'), 1);
    EXPECT_EQ(chess::rankToRow('3'), 2);
    EXPECT_EQ(chess::rankToRow('4'), 3);
    EXPECT_EQ(chess::rankToRow('5'), 4);
    EXPECT_EQ(chess::rankToRow('6'), 5);
    EXPECT_EQ(chess::rankToRow('7'), 6);
    EXPECT_EQ(chess::rankToRow('8'), 7);
    EXPECT_THROW(chess::rankToRow('/'),
    	std::invalid_argument);
    EXPECT_THROW(chess::rankToRow('9'),
    	std::invalid_argument);
}

TEST(testChess, columnToFile)
{
    EXPECT_EQ(chess::columnToFile(0), 'a');
    EXPECT_EQ(chess::columnToFile(1), 'b');
    EXPECT_EQ(chess::columnToFile(2), 'c');
    EXPECT_EQ(chess::columnToFile(3), 'd');
    EXPECT_EQ(chess::columnToFile(4), 'e');
    EXPECT_EQ(chess::columnToFile(5), 'f');
    EXPECT_EQ(chess::columnToFile(6), 'g');
    EXPECT_EQ(chess::columnToFile(7), 'h');
    EXPECT_THROW(chess::columnToFile(-1),
    	std::invalid_argument);
    EXPECT_THROW(chess::columnToFile(8),
    	std::invalid_argument);
}

TEST(testChess, rowToRank)
{
    EXPECT_EQ(chess::rowToRank(0), '1');
    EXPECT_EQ(chess::rowToRank(1), '2');
    EXPECT_EQ(chess::rowToRank(2), '3');
    EXPECT_EQ(chess::rowToRank(3), '4');
    EXPECT_EQ(chess::rowToRank(4), '5');
    EXPECT_EQ(chess::rowToRank(5), '6');
    EXPECT_EQ(chess::rowToRank(6), '7');
    EXPECT_EQ(chess::rowToRank(7), '8');
    EXPECT_THROW(chess::rowToRank(-1),
    	std::invalid_argument);
    EXPECT_THROW(chess::rowToRank(8),
    	std::invalid_argument);
}

TEST(testChess, decryptPosition)
{
    EXPECT_EQ(chess::decryptPosition("a2"), 8);
    EXPECT_EQ(chess::decryptPosition("e6"), 44);
    EXPECT_THROW(chess::decryptPosition("`6"),
    	std::invalid_argument);
    EXPECT_THROW(chess::decryptPosition("c9"),
    	std::invalid_argument);
    EXPECT_THROW(chess::decryptPosition("c10"),
    	std::invalid_argument);
    std::array<bool, 64> accessedIndex;
    accessedIndex.fill(false);

    for(char i1='1' ; i1<='8' ; i1++)
    {
    	for(char i0='a' ; i0<='h' ; i0++)
    	{
    		std::stringstream encryptedPosition;
    		encryptedPosition<<i0<<i1;
    		accessedIndex[
    			chess::decryptPosition(encryptedPosition.str())] = true;
    	}	
    }
    for(auto& b :accessedIndex)
    	EXPECT_EQ(b, true);
}

TEST(testChess, encryptPosition)
{
	/**
    for(char i1='1' ; i1<='8' ; i1++)
    {
    	for(char i0='a' ; i0<='h' ; i0++)
    	{
    		std::stringstream encryptedPosition;
    		encryptedPosition<<i0<<i1;
    		std::cout<<"\""<<encryptedPosition.str()<<"\", ";
    	}
    	std::cout<<std::endl;
    }
    **/
    EXPECT_EQ(chess::encryptPosition(8), "a2");
    EXPECT_EQ(chess::encryptPosition(44), "e6");
    EXPECT_THROW(chess::encryptPosition(-1),
    	std::invalid_argument);
    EXPECT_THROW(chess::encryptPosition(64),
    	std::invalid_argument);
	std::array<const char*, 64> encryptedPositions{
		"a1", "b1", "c1", "d1", "e1", "f1", "g1", "h1",
		"a2", "b2", "c2", "d2", "e2", "f2", "g2", "h2",
		"a3", "b3", "c3", "d3", "e3", "f3", "g3", "h3",
		"a4", "b4", "c4", "d4", "e4", "f4", "g4", "h4",
		"a5", "b5", "c5", "d5", "e5", "f5", "g5", "h5",
		"a6", "b6", "c6", "d6", "e6", "f6", "g6", "h6",
		"a7", "b7", "c7", "d7", "e7", "f7", "g7", "h7",
		"a8", "b8", "c8", "d8", "e8", "f8", "g8", "h8",
	};
	for(int i=0; i<64 ; i++)
	{
		EXPECT_EQ(chess::encryptPosition(i),
			encryptedPositions[i]);
	}
}

TEST(testChess, isNotationValid)
{
	chess::isNotationValid("test a long_word");
	chess::isNotationValid("Ka1");

	chess::isNotationValid("e1 c6");

	std::vector<std::string> kasparov_vs_the_world= {
		"e4 c5", "Nf3 d6", "Bb5+ Bd7", "Bxd7+ Qxd7", "c4 Nc6", "Nc3 Nf6",
		"O-O g6", "d4 cxd4", "Nxd4 Bg7", "Nde2 Qe6", "Nd5 Qxe4", "Nc7+ Kd7",
		"Nxa8 Qxc4", "Nb6+ axb6", "Nc3 Ra8", "a4 Ne4", "Nxe4 Qxe4", "Qb3 f5",
		"Bg5 Qb4", "Qf7 Be5", "h3 Rxa4", "Rxa4 Qxa4", "Qxh7 Bxb2", "Qxg6 Qe4",
		"Qf7 Bd4", "Qb3 f4", "Qf7 Be5", "h4 b5", "h5 Qc4", "Qf5+ Qe6",
		"Qxe6+ Kxe6", "g3 fxg3", "fxg3 b4", "Bf4 Bd4+", "Kh1 b3", "g4 Kd5",
		"g5 e6", "h6 Ne7", "Rd1 e5", "Be3 Kc4", "Bxd4 exd4", "Kg2 b2",
		"Kf3 Kc3", "h7 Ng6", "Ke4 Kc2", "Rh1 d3", "Kf5 b1=Q", "Rxb1 Kxb1",
		"Kxg6 d2", "h8=Q d1=Q", "Qh7 b5", "Kf6+ Kb2", "Qh2+ Ka1", "Qf4 b4",
		"Qxb4 Qf3", "Kg7 d5", "Qd4+ Kb1", "g6 Qe4", "Qg1+ Kb2", "Qf2+ Kc1",
		"Kf6 d4", "g7 1-0"};
	for (auto const & move :kasparov_vs_the_world)
	{
		EXPECT_TRUE(chess::isNotationValid(move).size() > 0) 
		<<"No chess notation match for: \""<<move<<"\"";
	}
}

int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
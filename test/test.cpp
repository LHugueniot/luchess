#include "luchess/core/chess.h"
#include "gtest/gtest.h"
#include <sstream>
#include <vector>
#include <string>


TEST(testChess, BoardPosition)
{
    chess::BoardPosition pos(5, 6);
    EXPECT_EQ(pos.column, 5);
    EXPECT_EQ(pos.row, 6);
}

TEST(testChess, BoardPosition_operators)
{
    using bp = chess::BoardPosition;
    bp originalPos;
    bp modifiedPos; 
    bp expectedPos;

    originalPos = {5, 6};
    modifiedPos = originalPos + bp{1, 1};
    expectedPos = {6, 7};
    EXPECT_EQ(modifiedPos, expectedPos);

    modifiedPos = originalPos - bp{1, 1};
    expectedPos = {4, 5};
    EXPECT_EQ(modifiedPos, expectedPos);
}


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
	EXPECT_FALSE(chess::isNotationValid("test a long_word").size() > 0);
	EXPECT_TRUE(chess::isNotationValid("Ka1").size() > 0);

	EXPECT_TRUE(chess::isNotationValid("e1 c6").size() > 0);

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

TEST(testChess, ChessBoard)
{

    chess::ChessBoard chessBoard;
    std::array<chess::Piece*, 64> pieceAddr;
    for (int i=0 ; i<chessBoard.layout.size() ; i++)
    {
        pieceAddr[i] = &chessBoard.layout[i];
    }

    for (int row=chess::kMinRow ; row<chess::kMaxRow ; row++)
    {
        for (int col=chess::kMinColumn ; col<chess::kMaxColumn ; col++)
        {
            chess::Piece& piece = chessBoard.getAt(
                chess::BoardPosition(col, row));
            EXPECT_EQ(piece.type, chess::Empty);
            EXPECT_EQ(piece.color, chess::Black);
        }
    }
    
    for(auto& p1: chessBoard.layout)
    {
        bool found = false;
        for(auto p2: pieceAddr)
        {
            if (&p1 == p2)
            {
                found = true;
            }
        }
        EXPECT_TRUE(found);
    }
    EXPECT_EQ(chessBoard.nextGo, chess::White);
}

TEST(testChess, ChessBoard_pawnDoubleSteped_indexChecker)
{
    chess::ChessBoard chessBoard;
    auto& isValidPosition = chessBoard.pawnDoubleSteped.isValidPosition;
    auto& getIndex = chessBoard.pawnDoubleSteped.getIndex;
    
    std::array<bool, 16> duplicateCache;
    duplicateCache.fill(false);

    // Check for valid positions
    for (auto& row : std::vector<int>{1, 6})
    {
        for (int col=chess::kMinColumn ; col<chess::kMaxColumn ; col++)
        {
            EXPECT_TRUE(isValidPosition({col, row}));
            auto idx = getIndex({col, row});
            EXPECT_TRUE(idx >= 0);
            EXPECT_TRUE(15 >= idx);
            EXPECT_FALSE(duplicateCache[idx]);
            duplicateCache[idx] = true;
        }
    }
    // Check for invalid positions
    for (auto& row : std::vector<int>{0, 2, 3, 4, 5, 7})
    {
        for (int col=chess::kMinColumn ; col<chess::kMaxColumn ; col++)
        {
            EXPECT_FALSE(isValidPosition({col, row}));
        }
    }
}

TEST(testChess, ChessBoard_rookCastleable_indexChecker)
{
    chess::ChessBoard chessBoard;
    auto& isValidPosition = chessBoard.rookCastleable.isValidPosition;
    auto& getIndex = chessBoard.rookCastleable.getIndex;

    EXPECT_TRUE(isValidPosition({0, 0}));
    EXPECT_EQ(getIndex({0, 0}), 0);
    EXPECT_TRUE(isValidPosition({0, 7}));
    EXPECT_EQ(getIndex({0, 7}), 1);
    EXPECT_TRUE(isValidPosition({7, 0}));
    EXPECT_EQ(getIndex({7, 0}), 2);
    EXPECT_TRUE(isValidPosition({7, 7}));
    EXPECT_EQ(getIndex({7, 7}), 3);

    // Check for all invalid positions
    for (auto& row : std::vector<int>{1, 2, 3, 4, 5, 6})
    {
        for (auto& col : std::vector<int>{0, 1, 2, 3, 4, 5, 6, 7})
        {
            EXPECT_FALSE(isValidPosition({col, row}));
        }
    }

    for (auto& row : std::vector<int>{0, 1, 2, 3, 4, 5, 6, 7})
    {
        for (auto& col : std::vector<int>{1, 2, 3, 4, 5, 6})
        {
            EXPECT_FALSE(isValidPosition({col, row}));
        }
    }
}

TEST(testChess, populateDefaultLayout)
{
    chess::ChessBoard chessBoard;
    chess::populateDefaultLayout(chessBoard);
    /*
    Do comparisons
    */

    EXPECT_TRUE(true);
}


TEST(testChess, doesMoveCollide)
{
    chess::ChessBoard chessBoard;
    chess::Piece& piece = chessBoard.getAt({4, 4});
    piece.type = chess::Pawn;
    chess::Move move;

    move.originPos = {2, 2};
    move.targetPos = {5, 5};
    bool collides = chess::doesMoveCollide(
        chessBoard, move);
    EXPECT_TRUE(collides);

    move.originPos = {5, 5};
    move.targetPos = {2, 2};
    collides = chess::doesMoveCollide(
        chessBoard, move);
    EXPECT_TRUE(collides);

    move.originPos = {6, 2};
    move.targetPos = {2, 6};
    collides = chess::doesMoveCollide(
        chessBoard, move);
    EXPECT_TRUE(collides);

    move.originPos = {2, 4};
    move.targetPos = {6, 4};
    collides = chess::doesMoveCollide(
        chessBoard, move);
    EXPECT_TRUE(collides);

    move.originPos = {4, 1};
    move.targetPos = {4, 7};
    collides = chess::doesMoveCollide(
        chessBoard, move);
    EXPECT_TRUE(collides);

    move.originPos = {1, 4};
    move.targetPos = {7, 4};
    collides = chess::doesMoveCollide(
        chessBoard, move);
    EXPECT_TRUE(collides);
}

chess::ChessBoard executeMoveSetup()
{
    chess::ChessBoard chessBoard;
    chess::populateDefaultLayout(chessBoard);
    return chessBoard;
}



namespace luchess
{

TEST(testChess, populateDefaultLayout)
{
    auto chessBoard = executeMoveSetup();
    EXPECT_EQ(chessBoard.nextGo, White);

    // Check white's back row
    EXPECT_EQ(chessBoard.getAt({0,0}), Piece(Rook, White));
    EXPECT_EQ(chessBoard.getAt({1,0}), Piece(Knight, White));
    EXPECT_EQ(chessBoard.getAt({2,0}), Piece(Bishop, White));
    EXPECT_EQ(chessBoard.getAt({3,0}), Piece(Queen, White));
    EXPECT_EQ(chessBoard.getAt({4,0}), Piece(King, White));
    EXPECT_EQ(chessBoard.getAt({5,0}), Piece(Bishop, White));
    EXPECT_EQ(chessBoard.getAt({6,0}), Piece(Knight, White));
    EXPECT_EQ(chessBoard.getAt({7,0}), Piece(Rook, White));
    //std::cout<<
    for(int col=0 ; col<8 ; col++)
    {
        // Check white's front row
        EXPECT_EQ(chessBoard.getAt({col, 1}), Piece(Pawn, White));
        // Check middle rows are empty
        for(int row=2 ; row<5 ; row++)
            EXPECT_EQ(chessBoard.getAt({col, row}), Piece(Empty, Black));
        // Check black's front row
        EXPECT_EQ(chessBoard.getAt({col, 6}), Piece(Pawn, Black));
    }
    // Check black's back row
    EXPECT_EQ(chessBoard.getAt({0,7}), Piece(Rook, Black));
    EXPECT_EQ(chessBoard.getAt({1,7}), Piece(Knight, Black));
    EXPECT_EQ(chessBoard.getAt({2,7}), Piece(Bishop, Black));
    EXPECT_EQ(chessBoard.getAt({3,7}), Piece(Queen, Black));
    EXPECT_EQ(chessBoard.getAt({4,7}), Piece(King, Black));
    EXPECT_EQ(chessBoard.getAt({5,7}), Piece(Bishop, Black));
    EXPECT_EQ(chessBoard.getAt({6,7}), Piece(Knight, Black));
    EXPECT_EQ(chessBoard.getAt({7,7}), Piece(Rook, Black));

    for (int col=kMinColumn ; col<kMaxColumn ; col++)
    {
        for (int row : {1, 6})
        {
            //auto valid = 
            //    chessBoard.pawnDoubleSteped.isValidPosition({col, row});
            //EXPECT_TRUE(valid);
            //auto index = chessBoard.pawnDoubleSteped.getIndex({col, row});
        }
    }
}

}

TEST(testChess, executePawnMoves)
{
    chess::ChessBoard chessBoard = executeMoveSetup();

    auto executePawnDoubleStep = [&](
        chess::BoardPosition const& origin,
        chess::BoardPosition const& dest,
        chess::PieceColor expectedCurrentPiece)
    {
        EXPECT_EQ(chessBoard.nextGo, expectedCurrentPiece);
        EXPECT_FALSE(chessBoard.pawnDoubleSteped.getAt(origin));
        bool success = executeMove(
            chessBoard, {origin, dest}
        );
        EXPECT_TRUE(success);
        EXPECT_EQ(chessBoard.nextGo, !expectedCurrentPiece);
        EXPECT_FALSE(chessBoard.pawnDoubleSteped.getAt(origin));
    };
    executePawnDoubleStep({6, 1}, {6, 2}, chess::White);
    executePawnDoubleStep({6, 6}, {6, 5}, chess::Black);

    executePawnDoubleStep({3, 1}, {3, 3}, chess::White);
    executePawnDoubleStep({5, 6}, {5, 4}, chess::Black);
}


int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
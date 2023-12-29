#ifndef LUCHESS_CORE_BOARD_H_
#define LUCHESS_CORE_BOARD_H_

#include <bitset>
#include <array>
#include <cstddef>
#include <optional>
#include <vector>
#include <stdexcept>


#include "luchess/core/pieces.h"
#include "luchess/core/types.h"
#include "luchess/core/static.h"

/**

Terms:
	File = column
	Rank = row

Chess board layout:

			blacks
	Rank(row)
	| ________________
	8|  ##  ##  ##  ##|
	7|##  ##  ##  ##  |
	6|  ##  ##  ##  ##|
	5|##  ##  ##  ##  |
	4|  ##  ##  ##  ##|
	3|##  ##  ##  ##  |
	2|  ##  ##  ##  ##|
	1|##__##__##__##__|
	  a b c d e f g h -- File(column)
			whites

**/

namespace luchess{

using BoardSquare = std::optional<Piece>;
#define EMPTY_SQUARE std::nullopt


struct BoardPosition
{
	BoardPosition operator+(BoardPosition const& other) const;

	BoardPosition operator-(BoardPosition const& other) const;

	BoardPosition& operator+=(BoardPosition const& other);

	BoardPosition& operator-=(BoardPosition const& other);

	auto operator<=>(const BoardPosition&) const = default;

	int column;
	int row;
};


struct BoardMove
{
	BoardPosition originPos;
	BoardPosition targetPos;
};

// or another floating-point type
typedef bool (*PositionValidator)(BoardPosition const&);
typedef uint (*Indexer)(BoardPosition const&);

template<std::size_t _nStateElems,
		 PositionValidator _isValidPosition,
		 Indexer _getIndex>
struct SpecialMoveState
{
	
	static constexpr std::size_t nStateElems = _nStateElems;
	static constexpr PositionValidator isValidPosition = _isValidPosition;
	static constexpr Indexer getIndex = _getIndex;

	bool getAt(BoardPosition const& pos)
	{
		if(!this->isValidPosition(pos))
			throw std::invalid_argument(
				"SpecialMoveState::getAt invalid argument: 'pos' must "
				"satisfy SpecialMoveState::isValidPosition.");
		return stateData[getIndex(pos)];
	}

	void setAt(BoardPosition const& pos, bool val)
	{
		if(!this->isValidPosition(pos))
			throw std::invalid_argument(
				"SpecialMoveState::setAt invalid argument: 'pos' must "
				"satisfy SpecialMoveState::isValidPosition.");
		stateData[getIndex(pos)] = val;
	}

	std::bitset<nStateElems> stateData = EMPTY_STATE;
};

using RookCastleState = SpecialMoveState<
	4, //_nStateElems
	[](BoardPosition const& pos){
		return (pos.column == 0 || pos.column == 7) &&
			(pos.row == 0 || pos.row == 7);
	}, // isValidPosition
	[](BoardPosition const& pos){
		// Use bitwise operations to get unique
		// index for each position combination
		uint index = 0;
		if (pos.row == 0)
			index |= 0;
		else if(pos.row == 7)
			index |= 1;
		if (pos.column == 0)
			index |= 0;
		else if(pos.column == 7)
			index |= 2;
		return index;
	} // getIndex
>;

using PawnDoubleStepedState = SpecialMoveState<16, //_nStateElems
	[](BoardPosition const& pos){
		return pos.row == 1 || pos.row == 6;
	}, // isValidPosition
	[](BoardPosition const& pos){
		return static_cast<uint>(
			(pos.row == 1 ? 0 : 7) + pos.column);
	}// getIndex
>;

struct ChessBoard
{
	static const std::size_t boardSize = 64;

	ChessBoard(BoardSquare _default=EMPTY_SQUARE);
	ChessBoard(std::array<BoardSquare, boardSize> _default);
	ChessBoard(ChessBoard&&) = default;

	bool isValidPosition(BoardPosition const& pos);

	uint getIndex(BoardPosition const& pos);

	BoardSquare& getAt(BoardPosition const& pos);

	BoardSquare const& getAt(BoardPosition const& pos) const;

	struct MoveResult
	{
		bool validMove;
		bool nextPlayerColour;
		bool finished;
		std::optional<bool> winner;
	};
	MoveResult executeMove(BoardMove const& move);

	bool doesLineCollide(BoardPosition const& originPos, BoardPosition const& targetPos, BoardPosition& collisionPos);

	bool _isSquareExposed(BoardPosition const& pos, PieceColor opponent);
	std::vector<BoardPosition> _positionsInRangeOfRook(BoardPosition const& pos);

	// State
	PieceColor nextGo = PieceColor::White;

	PawnDoubleStepedState pawnDoubleSteped;

	RookCastleState rookCastleable;

	bool whiteKingInCheck;

	bool blackKingInCheck;

	std::array<BoardSquare, boardSize> layout;
};

}

#endif // LUCHESS_CORE_BOARD_H_

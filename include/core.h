#include <array>
#include <assert.h>
#include <cmath>
#include <iostream>
#include <regex>
#include <stdexcept>
#include <string>
#include <string_view>
#include <bitset>
#include <algorithm>
	
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

Todo:
	1. Write chess notation validator
	2. Write chess notation encrypter and decrypter
	3. Move legality function

**/

namespace chess{


using uint = unsigned int;

static const uint kMinRow = 0;
static const uint kMaxRow = 7;
static const uint kMinColumn = 0;
static const uint kMaxColumn = 7;

enum PieceType : signed char
{
	Empty=0x0,
	Pawn=0x1,
	Bishop=0x2,
	Knight=0x3,
	Rook=0x4,
	Queen=0x5,
	King=0x7
};

std::ostream &operator<<(std::ostream &os, PieceType const& p);

enum PieceColor : bool
{
	Black=false,
	White=true
};

std::ostream &operator<<(std::ostream &os, PieceColor const& p)

struct Piece
{
	Piece() = default;
	Piece(PieceType _type,
		PieceColor _color) : 
	type(_type), color(_color)
	{}

	auto operator<=>(const Piece&) const = default;

	PieceType type;
	PieceColor color;
};

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

std::ostream &operator<<(std::ostream &os, BoardPosition const& bp);

struct Move
{
	BoardPosition originPos;
	BoardPosition targetPos;
};

// or another floating-point type
typedef bool (*PositionValidator)(BoardPosition const&);
typedef uint (*Indexer)(BoardPosition const&);

template<std::size_t N,
		 PositionValidator _isValidPosition,
		 Indexer _getIndex>
struct SpecialMoveStatus
{
	static constexpr PositionValidator isValidPosition = _isValidPosition;
	static constexpr Indexer getIndex = _getIndex;

	bool getAt(BoardPosition const& pos);

	void setAt(BoardPosition const& pos, bool val);

	std::bitset<N> data = 0;
};

struct ChessBoard
{
	ChessBoard(Piece _default={Empty, Black});
	ChessBoard(ChessBoard&&) = default;

	bool isValidPosition(BoardPosition const& pos);

	uint getIndex(BoardPosition const& pos);

	Piece& getAt(BoardPosition const& pos);

	Piece const& getAt(BoardPosition const& pos) const;

	PieceColor nextGo = White;

	SpecialMoveStatus<16,
		// isValidPosition
		[](BoardPosition const& pos){
			return pos.row == 1 || pos.row == 6;
		},
		// getIndex
		[](BoardPosition const& pos){
			return static_cast<uint>(
				(pos.row == 1 ? 0 : 7) + pos.column);
		}
	> pawnDoubleSteped;

	SpecialMoveStatus<4,
		// isValidPosition
		[](BoardPosition const& pos){
			return (pos.column == 0 || pos.column == 7) &&
				(pos.row == 0 || pos.row == 7);
		},
		// getIndex
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
		}
	> rookCastleable;

	bool whiteKingInCheck;

	bool blackKingInCheck;

	std::array<Piece, 64> layout;
};



bool doesMoveCollide(ChessBoard& board, Move const& move)
{
	auto posDiff = move.targetPos - move.originPos;
	if (abs(posDiff.column) == abs(posDiff.row) ||  // Diagonal move
		posDiff.column != 0 && posDiff.row == 0 ||  // Vertical move
		posDiff.column == 0 && posDiff.row != 0)	// Horizontal move
	{
		int columnUnit = sgn(posDiff.column);
		int rowUnit = sgn(posDiff.row);
		BoardPosition unitMove(columnUnit, 
							   rowUnit);
		DEBUG("	unitMove:");
		DEBUG(unitMove);
		for (BoardPosition colliderPos = move.originPos + unitMove;
			 colliderPos != move.targetPos;
			 colliderPos += unitMove)
		{
			if (board.getAt(colliderPos).type != Empty)
			{
				DEBUG("	Does move collide:");
				return true;
			}
		}
	}
	return false;
}


struct MoveState
{
	bool targetIsTeamPiece;
	BoardPosition posDiff;

	int direction;
}

bool executeMove(ChessBoard& board, Move const& move)
{
	// Check we're on the board
	if (!board.isValidPosition(move.originPos))
	{
		DEBUG("Origin postion not valid");
		return false;
	}
	if (!board.isValidPosition(move.targetPos))
	{
		DEBUG("Target postion not valid");
		return false;
	}

	// Piece checks
	auto& originPiece = board.getAt(move.originPos);
	DEBUG("originPiece: ");
	DEBUG(originPiece.color);
	DEBUG(originPiece.type);
	auto& targetPiece = board.getAt(move.targetPos);
	DEBUG("targetPiece: ");
	DEBUG(targetPiece.color);
	DEBUG(targetPiece.type);
	// Check it's our go
	if (originPiece.color != board.nextGo)
	{
		DEBUG("Wrong color's go.");
		return false;
	}
	// Check we're not trying to eat one of our own
	bool targetIsTeamPiece = (
			targetPiece.type != Empty &&
			originPiece.color != targetPiece.color
		);
	// Check we're not trying to eat the king
	//if (targetPiece.type == King)
	//{
	//	
	//	DEBUG("Wrong color's go.");
	//	return false;
	//}
	// TODO: Check who's turn it is
	// TODO: Check if move exposes king

	auto posDiff = move.targetPos - move.originPos;
	DEBUG(posDiff);
	uint backRow = originPiece.color == White ? kMinRow : kMaxRow;

	bool validMove = false;
	PieceType originReplacementPiece = Empty;
	PieceColor originReplacementColor = Black;

	switch(originPiece.type)
	{
		case Empty:
		{
			validMove = false;
			break;
		}
		case Pawn:
		{
			DEBUG("Checking for pawn move.");
			if(targetIsTeamPiece)
			{
				DEBUG("Target is team piece.");
				validMove = false;
				break;
			}
			int direction = originPiece.color == White ? 1 : -1;

			// Move 1 step in pawn direction
			if (posDiff.column == 0 &&
			    (posDiff.row == 1 * direction))
			{
				if (targetPiece.type == Empty)
				{
					board.pawnDoubleSteped.setAt(move.originPos, false);
					validMove = true;
					break;
				}
				validMove = false;
				break;
			}
			// Try to directly take or take en passant
			else if ((posDiff.column == -1 || 
					  posDiff.column == 1) &&
					 (posDiff.row == 1 * direction))
			{
				// Pawn takes en passant
				if (targetPiece.type == Empty)
				{
					auto potentialEnPassantPawn = board.getAt(
						move.targetPos + BoardPosition(0, direction));

					 // Check en passant pawn is different colour
					bool differentColors =
						potentialEnPassantPawn.color != originPiece.color;
					// Check we're on en passant row
					bool onEnPassantRow =
						move.targetPos.row == (backRow + 5 * direction);
					// Check en passantable pawn has double stepped
					bool pawnDoubleSteped = board.pawnDoubleSteped.getAt(
						move.targetPos - BoardPosition(0, direction));
					if (differentColors && onEnPassantRow && pawnDoubleSteped)
					{
						validMove = true;
						break;
					}
				}
				// Opponent piece is different colour
				else if (targetPiece.color != 
						 originPiece.color)	// Check target is different colour
				{
					validMove = true;
					break;
				}
				validMove = false;
				break;
			}
			// Move 2 steps in pawn direction
			else if (posDiff.column == 0 &&
					 (posDiff.row == 2 * direction) &&
					 board.pawnDoubleSteped.isValidPosition(move.originPos))
			{
				DEBUG("	Pawn moving two tiles.");
				// Check it's first move
				bool onBackRow = (move.originPos.row ==
								  (backRow + direction));

				DEBUG("	On back row:");
				DEBUG(onBackRow);
				// Check the target space is free
				bool targetIsEmpty = targetPiece.type == Empty;
				DEBUG("	Target is empty:");
				DEBUG(targetIsEmpty);
				// Check move doesn't collide with other pieces
				bool moveCollides = doesMoveCollide(board, move);
				DEBUG("	Does move collide:");
				DEBUG(moveCollides);
				if (onBackRow && targetIsEmpty && ! moveCollides)
				{
					DEBUG("move.originPos: ");
					DEBUG(move.originPos);
					board.pawnDoubleSteped.setAt(move.originPos, true);
					validMove = true;
					break;
				}
				
				validMove = false;
				break;
			}
			break;
		}
		case Bishop:
		{
			if(targetIsTeamPiece)
			{
				validMove = false;
				break;
			}

			// Bishop moves diagonally
			if (abs(posDiff.row) == abs(posDiff.column) &&
				!doesMoveCollide(board, move))
			{
				validMove = true;
				break;
			}
			break;
		}
		case Knight:
		{
			if(targetIsTeamPiece)
			{
				validMove = false;
				break;
			}

			// Is Knight moving by 1 in one dimension and 
			// by 2 in the other dimension
			if (abs(posDiff.row) == 1 && abs(posDiff.column) == 2 ||
				abs(posDiff.row) == 2 && abs(posDiff.column) == 1)
			{
				validMove = true;
				break;
			}
			break;
		}
		case Rook:
		{
			if (bool(posDiff.row) != bool(posDiff.column) &&
				!doesMoveCollide(board, move))
			{
				if (targetPiece.type == King)
				{
					originReplacementPiece = targetPiece.type;
					originReplacementColor = targetPiece.color;
				}
				validMove = true;
				break;
			}
			break;
		}
		case Queen:
		{
			if(targetIsTeamPiece)
			{
				validMove = false;
				break;
			}

			break;
		}
		case King:
		{
			if(targetIsTeamPiece)
			{
				validMove = false;
				break;
			}

			break;
		}
		default:
		{
			validMove = false;
			break;
		}
	}

	if (validMove)
	{
		targetPiece = originPiece;
		originPiece.type = originReplacementPiece;
		board.nextGo = static_cast<PieceColor>(
			!board.nextGo);
	}
	else
	{
		DEBUG("Invalid move.");
	}
	return validMove;
}

} // Namespace chess
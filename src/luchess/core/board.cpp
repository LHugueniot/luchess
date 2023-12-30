#include "luchess/core/board.h"
#include <stdexcept>
#include "util.h"
#include "board.h"

namespace luchess{

BoardPosition BoardPosition::operator+(BoardPosition const& other) const
{
	return BoardPosition(
		this->column + other.column,
		this->row + other.row);
}

BoardPosition BoardPosition::operator-(BoardPosition const& other) const
{
	return BoardPosition(
		this->column - other.column,
		this->row - other.row);
}

BoardPosition& BoardPosition::operator+=(BoardPosition const& other)
{
	this->column += other.column;
	this->row += other.row;
	return *this;
}

BoardPosition& BoardPosition::operator-=(BoardPosition const& other)
{
	this->column -= other.column;
	this->row -= other.row;
	return *this;
}


// ========================ChessBoard=================================


ChessBoard::ChessBoard(std::optional<Piece> _default)
{
	this->layout.fill(_default);
}

bool ChessBoard::isValidPosition(BoardPosition const& pos)
{
	return (pos.column >= 0 || pos.column <= 7) &&
		(pos.row >= 0 || pos.row <= 7);
}

uint ChessBoard::getIndex(BoardPosition const& pos)
{
	return pos.column + 8 * pos.row;
}

BoardSquare& ChessBoard::getAt(BoardPosition const& pos)
{
	if(!this->isValidPosition(pos))
		throw std::invalid_argument(
			"ChessBoard::getAt invalid argument: 'pos' must "
			"satisfy ChessBoard::isValidPosition.");
	return this->layout[getIndex(pos)];
}

BoardSquare const& ChessBoard::getAt(BoardPosition const& pos) const
{
	return this->getAt(pos);
}


static const uint kMinRow = 0;
static const uint kMaxRow = 7;
static const uint kMinColumn = 0;
static const uint kMaxColumn = 7;

bool ChessBoard::doesLineCollide(
	BoardPosition const& originPos,
	BoardPosition const& targetPos
)
{
	ChessBoard& board = *this;
	auto posDiff = targetPos - originPos;
	if (abs(posDiff.column) == abs(posDiff.row) ||  // Diagonal move
		posDiff.column != 0 && posDiff.row == 0 ||  // Vertical move
		posDiff.column == 0 && posDiff.row != 0)	// Horizontal move
	{
		int columnUnit = sgn(posDiff.column);
		int rowUnit = sgn(posDiff.row);
		BoardPosition unitMove(columnUnit, 
							   rowUnit);
		DEBUG("unitMove:");
		DEBUG(unitMove);
		for (BoardPosition colliderPos = originPos + unitMove;
			 colliderPos != targetPos;
			 colliderPos += unitMove)
		{
			if (board.getAt(colliderPos) != EMPTY_SQUARE)
			{
				return true;
			}
		}
	}
	return false;
}

bool ChessBoard::doesLineCollide(
	BoardPosition const& originPos,
	BoardPosition const& targetPos,
	BoardPosition& collisionPos
)
{
	ChessBoard& board = *this;
	auto posDiff = targetPos - originPos;
	if (abs(posDiff.column) == abs(posDiff.row) ||  // Diagonal move
		posDiff.column != 0 && posDiff.row == 0 ||  // Vertical move
		posDiff.column == 0 && posDiff.row != 0)	// Horizontal move
	{
		int columnUnit = sgn(posDiff.column);
		int rowUnit = sgn(posDiff.row);
		BoardPosition unitMove(columnUnit, 
							   rowUnit);
		DEBUG("unitMove:");
		DEBUG(unitMove);
		for (BoardPosition colliderPos = originPos + unitMove;
			 colliderPos != targetPos;
			 colliderPos += unitMove)
		{
			if (board.getAt(colliderPos) != EMPTY_SQUARE)
			{
				collisionPos = colliderPos;
				return true;
			}
		}
	}
	return false;
}

std::vector<BoardPosition> ChessBoard::_positionsInRangeOfRook(BoardPosition const& originPos)
{
	ChessBoard& board = *this;

	std::vector<BoardPosition> result ={};

	for (auto targetPos: std::array<BoardPosition, 4>{
		originPos + BoardPosition{8, 0},
		originPos + BoardPosition{-8, 0},
		originPos + BoardPosition{0, 8},
		originPos + BoardPosition{0, -8},
		}
	)
	{
		BoardPosition collisionPos;
		if (board.doesLineCollide(originPos, targetPos, collisionPos))
		{
			result.push_back(collisionPos);
		}
	}
}

bool positionsInRangeOfPos(BoardPosition const& pos, std::vector<BoardPosition>& positionsInRange)
{
}

bool ChessBoard::_isSquareExposed(BoardPosition const& pos, PieceColor opponent)
{
	ChessBoard& board = *this;



	for (uint col=-1; col<2; col++)
	{
		for (uint row=-1; row<2; row++)
		{
			BoardPosition collisionPos;
			BoardPosition originPos(pos);
			BoardPosition targetPos(originPos.column + col * 8, originPos.row + row * 8);
			auto lineCollides = board.doesLineCollide(pos, targetPos, collisionPos);
			if (lineCollides)
			{
				auto collisionSquare = board.getAt(pos);
				if (
					(collisionSquare->color == opponent) && collisionSquare->type 
				)
				{

				}
			}
		}
	}
	// board.doesLineCollide(pos, BoardPosition(8, pos.row), collisionPos);

	for (auto& boardSquare: layout)
	{
	}
}


ChessBoard::MoveResult ChessBoard::executeMove(BoardMove const& move)
{
	ChessBoard& board = *this;

	#define INVALID_MOVE MoveResult(false, board.nextGo, false, std::nullopt)

	// Check we're on the board
	if (!board.isValidPosition(move.originPos))
	{
		DEBUG("Origin postion not valid");
		return INVALID_MOVE;
	}
	if (!board.isValidPosition(move.targetPos))
	{
		DEBUG("Target postion not valid");
		return INVALID_MOVE;
	}

	// Piece checks
	BoardSquare& originSquare = board.getAt(move.originPos);
	if (originSquare == EMPTY_SQUARE){
		DEBUG("originSquare empty");
		return INVALID_MOVE;
	}
	Piece& originPiece = *originSquare;
	DEBUG("originPiece: ");
	DEBUG(originPiece.color);
	DEBUG(originPiece.type);
	// Check it's our go
	if (originPiece.color != board.nextGo)
	{
		DEBUG("Wrong color's go.");
		return INVALID_MOVE;
	}

	BoardSquare& targetSquare = board.getAt(move.targetPos);
	DEBUG("targetPiece: ");
	DEBUG(targetPiece.color);
	DEBUG(targetPiece.type);
	// Check we're not trying to eat one of our own
	bool targetIsTeamPiece;
	if (targetSquare == EMPTY_SQUARE)
	{
		targetIsTeamPiece = false;
	}
	else
	{
		Piece& targetPiece = *targetSquare;
		targetIsTeamPiece = (originPiece.color == targetPiece.color);

		if (targetIsTeamPiece)
		{
			return INVALID_MOVE;
		}

		// Check we're not trying to eat the king
		if (targetPiece.type == King)
		{
			throw std::runtime_error("invalid state; execute move should not be called if enemy king in check");
		}
	}
	// TODO: Check if move exposes king

	auto posDiff = move.targetPos - move.originPos;
	DEBUG(posDiff);
	uint backRow = originPiece.color == White ? kMinRow : kMaxRow;

	bool validMove = false;

	bool isCasteling = false;

	switch(originPiece.type)
	{
		case Pawn:
		{
            validMove = _isValidPawnMove(move, originPiece, posDiff, targetSquare, backRow);
            break;
		}
		case Bishop:
		{
            validMove = _isValidBishopMove(targetIsTeamPiece, validMove, posDiff, move);
            break;
        }      
		case Knight:
		{
            validMove = _isValidKnightMove(targetIsTeamPiece, validMove, posDiff, move);
            break;
		}
		case Rook:
		{
			if (bool(posDiff.row) != bool(posDiff.column) &&
				!doesLineCollide(move.originPos, move.targetPos))
			{
				if (targetIsTeamPiece)
				{
					Piece& targetPiece = *targetSquare;
					if(targetPiece.type == King)
					{
						isCasteling = true;
						validMove = true;
					}
				}
				else
				{
					validMove = true;
				}
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
		originPiece.type = originReplacementSquare.value_or();
		board.nextGo = static_cast<PieceColor>(
			!board.nextGo);
	}
	else
	{
		DEBUG("Invalid move.");
	}
	return validMove;
}

bool ChessBoard::_isValidBishopMove(bool targetIsTeamPiece, bool &validMove, luchess::BoardPosition &posDiff, const luchess::BoardMove &move)
{
	if (targetSquare == EMPTY_SQUARE)
	{
		return true;
	}
    if (!targetIsTeamPiece)
    {
    	// Bishop moves diagonally
    	if (abs(posDiff.row) == abs(posDiff.column) &&
    	    !doesLineCollide(move.originPos, move.targetPos))
    	{
    	    return true;
    	}
    }
}

bool ChessBoard::_isValidKnightMove(bool targetIsTeamPiece, bool &validMove, luchess::BoardPosition &posDiff, const luchess::BoardMove &move)
{
	if(!targetIsTeamPiece)
	{
		// Is Knight moving by 1 in one dimension and 
		// by 2 in the other dimension
		if (abs(posDiff.row) == 1 && abs(posDiff.column) == 2 ||
			abs(posDiff.row) == 2 && abs(posDiff.column) == 1)
		{
			return true;
		}
	}
}

bool ChessBoard::_isValidPawnMove(const luchess::BoardMove &move, luchess::Piece &originPiece, luchess::BoardPosition &posDiff, luchess::BoardSquare &targetSquare, uint backRow)
{
	luchess::ChessBoard &board = *this;

    int direction = originPiece.color == White ? 1 : -1;

    // Move 1 step in pawn direction
    if (posDiff.column == 0 &&
        (posDiff.row == 1 * direction))
    {
        if (targetSquare == EMPTY_SQUARE)
        {
            board.pawnDoubleSteped.setAt(move.originPos, false);
            return true;
        }
    }
    // Try to directly take or take en passant
    else if ((posDiff.column == -1 ||
              posDiff.column == 1) &&
             (posDiff.row == 1 * direction))
    {
        // Pawn takes en passant
        if (targetSquare == EMPTY_SQUARE)
        {
            BoardSquare enPassantTargetSquare = board.getAt(
                move.targetPos - BoardPosition(0, direction));

            if (enPassantTargetSquare != std::nullopt)
            {
                const Piece &enPassantTargetPiece = *enPassantTargetSquare;
	
                bool differentColors = enPassantTargetPiece.color != originPiece.color;
                bool onEnPassantRow = (move.targetPos.row == 4 || move.targetPos.row == 5); // Rows 4 and 5
                bool pawnDoubleSteped = board.pawnDoubleSteped.getAt(move.targetPos - BoardPosition(0, direction));
		
                if (differentColors && onEnPassantRow && pawnDoubleSteped)
                {
            		return true;
                }
            }
        }
        else
        {
            // Check target is different colour
            const Piece &targetPiece = *targetSquare;
            if (targetPiece.color != originPiece.color)
            {
            	return true;
            }
        }
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
        if (targetSquare == EMPTY_SQUARE)
        {
            DEBUG("	Target is empty:");
            // Check move doesn't collide with other pieces
            BoardPosition _;
            bool moveCollides = doesLineCollide(move.originPos, move.targetPos, _);
            DEBUG("	Does move collide:");
            DEBUG(moveCollides);
            if (onBackRow && !moveCollides)
            {
                DEBUG("move.originPos: ");
                DEBUG(move.originPos);
                board.pawnDoubleSteped.setAt(move.originPos, true);
            	return true;
            }
        }
    }
	return false;
}

} // end namespace luchess

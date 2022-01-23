#include <array>
#include <assert.h>
#include <cmath>
#include <iostream>
#include <regex>
#include <stdexcept>
#include <string>
#include <string_view>
	
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

static const int asciiLowerCaseOffset = 97;
static const int asciiDecimalOffset = 49;
const char* chessNotationRegexStr = 
	"([KQRNB]{0,1})([a-h]|[1-8]){0,1}(x{0,1})([a-h][1-8]){0,1}"
	"(\\+){0,1}( ){0,1}"
	"([KQRNB]{0,1})([a-h]|[1-8]){0,1}(x{0,1})([a-h][1-8]){0,1}"
	"(\\+){0,1}";

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

enum PieceColor : bool
{
	Black=false,
	White=true
};

struct Piece
{
	PieceType type;
	PieceColor color;
};

struct BoardPosition
{
	BoardPosition(int _column, int _row) : 
		column(_column), 
		row(_row){}

	BoardPosition operator+(BoardPosition const& other)
	{
		return BoardPosition(
			other.column + this->column,
			other.row + this->row);
	}

	BoardPosition operator-(BoardPosition const& other)
	{
		return BoardPosition(
			other.column - this->column,
			other.row - this->row);
	}

	BoardPosition& operator+=(BoardPosition const& other)
	{
		this->column = other.column + this->column;
		this->row = other.row + this->row;
		return *this;
	}

	BoardPosition& operator-=(BoardPosition const& other)
	{
		this->column = other.column - this->column;
		this->row = other.row - this->row;
		return *this;
	}

	auto operator<=>(const BoardPosition&) const = default;

	int column;
	int row;
};

struct Move
{
	BoardPosition currentPos;
	BoardPosition targetPos;
};

struct ChessBoard
{
	Piece& getAt(BoardPosition const& pos)
	{
		return this->layout[pos.column + 8 * pos.row];
	}

	Piece const& getAt(BoardPosition const& pos) const
	{
		return this->getAt(pos);
	}
	PieceColor nextGo;
	bool whiteCastled;
	bool whiteKingInCheck;
	bool blackCastled;
	bool blackKingInCheck;
	std::array<Piece, 64> layout;
};

template<typename T>
T abs(T val)
{
	return val >= 0 ? val : -val;
}

int fileToColumn(const char& file)
{
	if(file < 'a' || 'h' < file)
		throw std::invalid_argument(
			"fileToColumn invalid argument: "
			"'file' arg must be in range 'a' to 'h'.");
	return static_cast<int>(file) - asciiLowerCaseOffset;
}

int rankToRow(const char& rank)
{
	if(rank < '1' || '8' < rank)
		throw std::invalid_argument(
			"rankToRow invalid argument: "
			"'rank' arg must be in range '1' to '8'.");
	return static_cast<int>(rank) - asciiDecimalOffset;
}

char columnToFile(const int& column)
{
	if(column < 0 || 7 < column)
		throw std::invalid_argument(
			"columnToFile invalid argument: "
			"'column' arg must be in range 1 to 8.");
	return static_cast<char>(column + asciiLowerCaseOffset);
}

char rowToRank(const int& row)
{
	if(row < 0 || 7 < row)
		throw std::invalid_argument(
			"rowToRank invalid argument: "
			"'row' arg must be in range 1 to 8.");
	return static_cast<char>(row + asciiDecimalOffset);
}


uint decryptPosition(std::string_view position)
{
	if(position.length() == 3) // 3 including null termination
		throw std::invalid_argument(
			"decryptPosition invalid argument: 'position' must be two chars long.");
	int column = fileToColumn(position[0]);
	int row = rankToRow(position[1]);
	return column + 8 * row;
}

std::string encryptPosition(uint index)
{
	if(63 < index)
		throw std::invalid_argument(
			"encryptPosition invalid argument: 'index' must be 64 or less.");
	int column = index % 8; 
	int row = std::floor<int>((float)index / 8.f);
	return {columnToFile(column), rowToRank(row)};
}

std::smatch isNotationValid(std::string const& pos)
{
	std::regex chessNotationRegex(chessNotationRegexStr);
    std::smatch match;

    if (std::regex_match(pos, match, chessNotationRegex)) {
        std::cout << pos << '\n';
        for (size_t i = 0; i < match.size(); ++i) {
            std::string subMatch = match[i].str();
            std::cout << "  submatch " << i << ": " << subMatch << '\n';
        }
    }
    return match;
}

bool isPositionValid(BoardPosition const& pos)
{
	return 0 <= pos.column && pos.column <= 8 &&
		0 <= pos.row && pos.row <= 8;
}

bool doesMoveCollide(ChessBoard& board, Move& move)
{
	auto posDiff = move.targetPos - move.currentPos;
	if (abs(posDiff.column) == abs(posDiff.row) ||  // Diagonal move
		posDiff.column != 0 && posDiff.row == 0 ||  // Vertical move
		posDiff.column == 0 && posDiff.row != 0)	// Horizontal move
	{
		uint columnUnit = 
			posDiff.column/(posDiff.column ? posDiff.column : 1);
		uint rowUnit = 
			posDiff.row/(posDiff.row ? posDiff.row : 1);
		BoardPosition unitMove(columnUnit, 
							   rowUnit);
		for (BoardPosition colliderPos = move.currentPos + unitMove;
			 colliderPos < move.targetPos;
			 colliderPos += unitMove)
		{
			if (board.getAt(colliderPos).type != Empty)
				return true;
		}
	}
	return false;
}

bool _executeMove(ChessBoard& board, Move& move)
{
	// Check we're on the board
	if (!isPositionValid(move.currentPos))
		return false;
	if (!isPositionValid(move.targetPos))
		return false;

	// Piece checks
	auto& currentPiece = board.getAt(move.currentPos);
	auto& targetPiece = board.getAt(move.targetPos);
	// Check it's our go
	if (currentPiece.color != board.nextGo)
		return false;
	// Check we're not trying to eat one of our own
	if (targetPiece.type != Empty &&
		currentPiece.color != targetPiece.color)
		return false;
	// Check we're not trying to eat the king
	if (targetPiece.type == King)
		return false;
	// TODO: Check who's turn it is
	// TODO: Check if move exposes king

	auto posDiff = move.targetPos - move.currentPos;
	uint backRow = currentPiece.color == White ? kMinRow : kMaxRow;

	switch(currentPiece.type)
	{
		case Empty:
		{
			return false;
		}
		case Pawn:
		{
			int direction = currentPiece.color == White ? 1 : -1;
			if (posDiff.column == 0 &&
			    (posDiff.row == 1 * direction))
			{
				// Pawn moves 1
				if (targetPiece.type != Empty)
					return false;
				return true;
			}
			else if ((posDiff.column == -1 || 
					  posDiff.column == 1) &&
					 (posDiff.row == 1 * direction))
			{
				// Pawn takes
				if (targetPiece.type == Empty ||	// Check if target is empty
					targetPiece.color ==
						currentPiece.color)			// Check target is
													// different colour
					return false;
				return true;
			}
			else if (posDiff.column == 0 &&
					 (posDiff.row == 2 * direction))
			{
				// Pawn moves 2 (opening move)
				if (move.currentPos.row !=
						(backRow + direction) ||	// Check it's first move
					targetPiece.type != Empty || 	// Check if target is empty
					doesMoveCollide(board, move)) 	// Check it doesn't colide
					return false;
				return true;
			}
			break;
		}
		case Bishop:
		{
			if (abs(posDiff.row) == abs(posDiff.column))
			{
				// Bishop moves diagonally
				if (doesMoveCollide(board, move))
					return false;
				return true;
			}
			break;
		}
		case Knight:
		{
			if (abs(posDiff.row) == 1 && abs(posDiff.column) == 2 ||
				abs(posDiff.row) == 2 && abs(posDiff.column) == 1 )
			{

			}
			break;
		}
		case Rook:
		{
			break;
		}
		case Queen:
		{
			break;
		}
		case King:
		{
			break;
		}
		default:
		{
			return false;
		}
	}
	return false;
}

bool executeMove(ChessBoard& board, Move& move)
{
	bool validMove = executeMove(board, move);
	if(validMove)
	{
		auto& currentPiece = board.getAt(move.currentPos);
		auto& targetPiece = board.getAt(move.targetPos);
		targetPiece = currentPiece;
		currentPiece.type = Empty;
		board.nextGo = static_cast<PieceColor>(
			!board.nextGo);
	}
	return validMove;
}

} // Namespace chess
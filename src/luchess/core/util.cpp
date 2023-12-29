#include <ostream>
#include <sstream>

#include "luchess/core/util.h"

namespace luchess{

std::ostream &operator<<(std::ostream &os, PieceType const& p) {
	std::string_view result;
	switch(p)
	{
		case Pawn:
			result = {STR(Pawn)};
			break;
		case Bishop:
			result = {STR(Bishop)};
			break;
		case Knight:
			result = {STR(Knight)};
			break;
		case Rook:
			result = {STR(Rook)};
			break;
		case Queen:
			result = {STR(Queen)};
			break;
		case King:
			result = {STR(King)};
			break;
	}
  	return std::operator<<(os, result);
}


// ========================PieceColor===========================

std::ostream &operator<<(std::ostream &os, PieceColor const& p) {
	std::string_view result;
	switch(p)
	{
		case Black:
			result = {STR(Black)};
			break;
		case White:
			result = {STR(White)};
			break;
	}
  	return std::operator<<(os, result);
}

std::ostream &operator<<(std::ostream &os, BoardPosition const& bp)
{
	std::stringstream result;
	result<<"{ c:"<<bp.column<<", r:"<<bp.row<<" }";
  	return std::operator<<(os, result.str());
}

}

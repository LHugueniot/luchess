#ifndef LUCHESS_CORE_PIECES_H_
#define LUCHESS_CORE_PIECES_H_

#include "luchess/core/types.h"


namespace luchess {

enum PieceType : uint
{
	Pawn=0x0,
	Bishop=0x1,
	Knight=0x2,
	Rook=0x3,
	Queen=0x4,
	King=0x5,
};

enum PieceColor : bool
{
	Black=false,
	White=true
};

struct Piece
{
	constexpr Piece(PieceType _type, PieceColor _color) :
	type(_type), color(_color)
	{}

	auto operator<=>(const Piece&) const = default;

	PieceType type;
	PieceColor color;

};

}

#endif // LUCHESS_CORE_PIECES_H_
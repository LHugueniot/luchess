namespace luchess {

enum PieceType : uint8_t
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
	Piece() = default;

	constexpr const Piece(const PieceType _type, const PieceColor _color) :
	type(_type), color(_color)
	{}

	Piece(PieceType _type, PieceColor _color) :
	type(_type), color(_color)
	{}

	auto operator<=>(const Piece&) const = default;

	PieceType type;
	PieceColor color;
};

}

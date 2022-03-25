

namespace chess{

static const auto L = PieceColor::Black;
static const auto W = PieceColor::White;
static const auto E = PieceType::Empty;
static const auto P = PieceType::Pawn;
static const auto B = PieceType::Bishop;
static const auto N = PieceType::Knight;
static const auto R = PieceType::Rook;
static const auto Q = PieceType::Queen;
static const auto K = PieceType::King;

static const Piece defaultBoard[64] = {
	{R, W}, {N, W}, {B, W}, {Q, W}, {K, W}, {B, W}, {N, W}, {R, W},
	{P, W}, {P, W}, {P, W}, {P, W}, {P, W}, {P, W}, {P, W}, {P, W},

	{E, L}, {E, L}, {E, L}, {E, L}, {E, L}, {E, L}, {E, L}, {E, L},
	{E, L}, {E, L}, {E, L}, {E, L}, {E, L}, {E, L}, {E, L}, {E, L},
	{E, L}, {E, L}, {E, L}, {E, L}, {E, L}, {E, L}, {E, L}, {E, L},
	{E, L}, {E, L}, {E, L}, {E, L}, {E, L}, {E, L}, {E, L}, {E, L},

	{P, L}, {P, L}, {P, L}, {P, L}, {P, L}, {P, L}, {P, L}, {P, L},
	{R, L}, {N, L}, {B, L}, {Q, L}, {K, L}, {B, L}, {N, L}, {R, L}
};

void populateDefaultLayout(ChessBoard& board)
{
	memcpy(&board.layout[0], &defaultBoard[0], 64 * sizeof(Piece));
}

}

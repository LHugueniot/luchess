#include <string_view>

#include "luchess/core/chess.h"

namespace luchess{

static constexpr auto L = PieceColor::Black;
static constexpr auto W = PieceColor::White;

static constexpr auto P = PieceType::Pawn;
static constexpr auto B = PieceType::Bishop;
static constexpr auto N = PieceType::Knight;
static constexpr auto R = PieceType::Rook;
static constexpr auto Q = PieceType::Queen;
static constexpr auto K = PieceType::King;

constexpr const BoardSquare const sp(PieceType t, PieceColor c) {
	return BoardSquare(Piece(t, c));
}

static constexpr const std::array<BoardSquare, 64> defaultBoard = {
	sp(R, W), sp(N, W), sp(B, W), sp(Q, W), sp(K, W), sp(B, W), sp(N, W), sp(R, W),
	sp(P, W), sp(P, W), sp(P, W), sp(P, W), sp(P, W), sp(P, W), sp(P, W), sp(P, W),

	{}, {}, {}, {}, {}, {}, {}, {},
	{}, {}, {}, {}, {}, {}, {}, {},
	{}, {}, {}, {}, {}, {}, {}, {},
	{}, {}, {}, {}, {}, {}, {}, {},

	sp(P, L), sp(P, L), sp(P, L), sp(P, L), sp(P, L), sp(P, L), sp(P, L), sp(P, L),
	sp(R, L), sp(N, L), sp(B, L), sp(Q, L), sp(K, L), sp(B, L), sp(N, L), sp(R, L),
};

void populateDefaultLayout(ChessBoard& board)
{
	memcpy(&board.layout[0], &defaultBoard[0], 64 * sizeof(BoardSquare));
}

bool doesMoveCollide(ChessBoard& board, BoardMove const& move)
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
			if (board.getAt(colliderPos) != std::nullopt)
			{
				DEBUG("	Does move collide: true");
				return true;
			}
		}
	}
	DEBUG("	Does move collide: false");
	return false;
}

}

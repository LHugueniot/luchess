#ifndef LUCHESS_CORE_CHESS_H_
#define LUCHESS_CORE_CHESS_H_

#include <luchess/core/board.h>
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


#include "luchess/core/types.h"
#include "util.h"


/**

Todo:
	1. Write chess notation validator
	2. Write chess notation encrypter and decrypter
	3. Move legality function

**/

namespace luchess{

void populateDefaultLayout(ChessBoard& board);


struct MoveState
{
	bool targetIsTeamPiece;
	BoardPosition posDiff;

	int direction;
};


} // namespace luchess

#endif // LUCHESS_CORE_CHESS_H_
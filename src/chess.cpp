
namespace chess{

const char* chessNotationRegexStr = 
	"([KQRNB]{0,1})([a-h]|[1-8]){0,1}(x{0,1})([a-h][1-8]){0,1}"
	"(\\+){0,1}"
	"( ){0,1}"
	"([KQRNB]{0,1})([a-h]|[1-8]){0,1}(x{0,1})([a-h][1-8]){0,1}"
	"(\\+){0,1}";


// ========================PieceType===========================

std::ostream &operator<<(std::ostream &os, PieceType const& p) {
	std::string_view result;
	switch(p)
	{
		case Empty:
			result = {STR(Empty)};
			break;
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

// ========================BoardPosition===========================

BoardPosition BoardPosition::operator+(BoardPosition const& other) const
{
	return BoardPosition(
		this->column+ other.column,
		this->row+ other.row);
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

std::ostream &operator<<(std::ostream &os, BoardPosition const& bp)
{
	std::stringstream result;
	result<<"{ c:"<<bp.column<<", r:"<<bp.row<<" }";
  	return std::operator<<(os, result.str());
}

// ========================SpecialMoveStatus========================

bool SpecialMoveStatus::getAt(BoardPosition const& pos)
{
	if(!this->isValidPosition(pos))
		throw std::invalid_argument(
			"SpecialMoveStatus::getAt invalid argument: 'pos' must "
			"satisfy SpecialMoveStatus::isValidPosition.");
	return data[getIndex(pos)];
}

void SpecialMoveStatus::setAt(BoardPosition const& pos, bool val)
{
	if(!this->isValidPosition(pos))
		throw std::invalid_argument(
			"SpecialMoveStatus::setAt invalid argument: 'pos' must "
			"satisfy SpecialMoveStatus::isValidPosition.");
	data[getIndex(pos)] = val;
}

// ========================ChessBoard=================================


ChessBoard::ChessBoard(Piece _default={Empty, Black})
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

Piece& ChessBoard::getAt(BoardPosition const& pos)
{
	if(!this->isValidPosition(pos))
		throw std::invalid_argument(
			"ChessBoard::getAt invalid argument: 'pos' must "
			"satisfy ChessBoard::isValidPosition.");
	return this->layout[getIndex(pos)];
}

Piece const& ChessBoard::getAt(BoardPosition const& pos) const
{
	return this->getAt(pos);
}
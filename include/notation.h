
namespace chess{

static const int asciiLowerCaseOffset = 97;
static const int asciiDecimalOffset = 49;


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

std::smatch isNotationValid(std::string const& move)
{
	std::regex chessNotationRegex(chessNotationRegexStr);
    std::smatch match;

    if (std::regex_match(move, match, chessNotationRegex)) {
        //std::cout << pos << '\n';
        for (size_t i = 0; i < match.size(); ++i) {
            std::string subMatch = match[i].str();
            //std::cout << "  submatch " << i << ": " << subMatch << '\n';
        }
    }
    return match;
}

}
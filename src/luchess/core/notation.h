#ifndef LUCHESS_CORE_NOTATION_H_
#define LUCHESS_CORE_NOTATION_H_

#include <stdexcept>
#include <regex>

#include "luchess/core/types.h"

namespace luchess{

static const int asciiLowerCaseOffset = 97;
static const int asciiDecimalOffset = 49;

const char* chessNotationRegexStr = 
	"([KQRNB]{0,1})([a-h]|[1-8]){0,1}(x{0,1})([a-h][1-8]){0,1}"
	"(\\+){0,1}"
	"( ){0,1}"
	"([KQRNB]{0,1})([a-h]|[1-8]){0,1}(x{0,1})([a-h][1-8]){0,1}"
	"(\\+){0,1}";


int fileToColumn(const char& file);

int rankToRow(const char& rank);

char columnToFile(const int& column);

char rowToRank(const int& row);


uint decryptPosition(std::string_view position);

std::string encryptPosition(uint index);

std::smatch isNotationValid(std::string const& move);

}

#endif // LUCHESS_CORE_NOTATION_H_
#include<iostream>
#include<string>
#include<map>
#include<vector>
std::string code = R"VOGON(
MODEL FLAG
{
CUBE ->  0, 1, 1, 10, 60, 1, WHITE  
CUBE -> 20, 1, 1, 10, 60, 1, BLUE 
  MODEL RED_LINE
  {
  CUBE -> 40, 1, 1, 10, 60, 1, RED
  }
}
MODEL RED_ROAD
{
CUBE -> RED_LINE.X, RED_LINE.Y, RED_LINE.Z, 20, 120, RED
}
RETURN 
( RED_LINE,
  FLAG,
  SPHERE, 30, 4, 8, 1, GREEN
))VOGON";

std::string tokens[] = 
{
	"MODEL",

	"CUBE",
	"SPHERE",
	"PLANE",

	"BLACK",
	"RED",
	"GREEN",
	"BLUE",
	"YELLOW",
	"MAGENTA",
	"CYAN",
	"WHITE",

	"RETURN"
};
std::map<std::string, float[3]> colors[8];

std::map<std::string, std::vector<float>> models;

bool hasSpecial(char character)
{
	const char* specialCharacters = ",.{}()-> ";
	for (int i = 0; i < 8; i++)
	{
		if (character == specialCharacters[i])
		{
			return true;
		}
	}
	return false;
}

std::vector<std::string> getTokens(std::string string)
{
	std::vector<std::string> codeTokens;
	const int FIND_END = 0;
	const char* space = " ";
	const int FIND_WORD_START = 1;
	int find = FIND_END;;
	std::string currentToken;
	std::string::const_iterator itt;
	for ( itt = string.begin (); itt != string.end (); ++ itt )
	{
		/* MODEL -> 
		M -> M!=SPACE -> find = FIND_END -> currentToken+=M
		O -> O!=SPACE -> find = FIND_END
		*/
		if ( hasSpecial(*itt) )
		{
			codeTokens.push_back(currentToken);
			currentToken = "";
			std::string specialToken;
			specialToken+=*itt;
			codeTokens.push_back(specialToken);
			find = FIND_WORD_START;
		}
	 	else if ( *itt!=space[0])
		{
			std::cout<<*itt;
			find = FIND_END;
			currentToken+= *itt;
		}
		else
		{
			codeTokens.push_back(currentToken);
			currentToken = "";
			find = FIND_WORD_START;
		}
	}
	return codeTokens;
}

int main()
{
	std::vector<std::string> codeTokens = getTokens(code);
	for (std::vector<std::string>::iterator i = codeTokens.begin(); i != codeTokens.end(); i++)
	{
		std::cout << *i << " ";
	}
	std::string l;
	std::cin >> l;
};
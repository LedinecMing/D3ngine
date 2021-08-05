/*
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
)
*/
string tokens[] = 
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
}
std::map<string, float[3]> colors[8];

std::map<string, std::vector<float>> models;

std::vector<float>

int main()
{
	return 0;
};
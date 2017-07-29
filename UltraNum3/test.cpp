
#include <utility>

#include "Num.hpp"
#include "List.hpp"

const uint16_t& testfunc(const uint16_t& t)
{
	return t;
}
uint16_t& testfunc(uint16_t& t)
{
	return t;
}

struct testclass {
	uint16_t& Get()
	{
		return n;
	}
	const uint16_t& Get() const
	{
		return n;
	}
	uint16_t n = 5555;
};

template<bool UseStack>
class Points
{
public:
	/* ... Member functions ...*/
private:
	cg::List<int, UseStack ? 100 : 0> m_points;
};

int main(int argc, char ** argv)
{
	cg::List<int, 30> stackList; //Allocated on the stack, will not resize.
	int i = 0;
	while (i < 25)
		stackList.PushBack(i++); //add 25 items to the list.
	for (std::size_t i = 0; i < stackList.Size(); ++i)
	{
		std::cout << stackList[i] << ", ";
		//std::cout << stackList.Get(i++) << ", ";   //also works the same.
	}

	int stop = 0;
	return stop;
}
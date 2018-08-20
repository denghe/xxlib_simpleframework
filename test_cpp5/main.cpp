// todo: noexcept 狂加一波

#include "../docs/fixed_function.hpp"

template <typename TK, typename TV>
class Xxxxxx
{
public:
	struct Data
	{
		TK              key;
		TV              value;
	};
	void Xxx(kapala::fixed_function<void(Data&)> f = nullptr)
	{
	}
};

int main()
{
	Xxxxxx<int, int> xxx;
	xxx.Xxx();

	return 0;
}

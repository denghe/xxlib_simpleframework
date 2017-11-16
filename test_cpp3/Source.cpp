#include <functional>
#include <iostream>
#include <array>

template<typename T>
struct TypeId
{
	static const int value = -1;
};
template<typename T>
constexpr int TypeId_v = TypeId<T>::value;

struct Base
{
	virtual int GetTypeId() = 0;
};
struct A : Base
{
	int GetTypeId() override;
};
struct B : Base
{
	int GetTypeId() override;
};

template<> struct TypeId<A> { static const int value = 0; };
template<> struct TypeId<B> { static const int value = 1; };

inline int A::GetTypeId() { return TypeId_v<A>; }
inline int B::GetTypeId() { return TypeId_v<B>; }


template<typename T, typename... Args>
struct MaxTypeId
{
	static const int value = TypeId<T>::value > MaxTypeId<Args...>::value
		? TypeId<T>::value
		: MaxTypeId<Args...>::value;
};
template<typename T>
struct MaxTypeId<T>
{
	static const int value = TypeId<T>::value;
};

std::array<std::function<void(void*)>, MaxTypeId<A, B>::value + 1> fs;

template<typename T>
void RegFunc(std::function<void(T*)> f)
{
	fs[TypeId_v<T>] = [f = std::move(f)](void* in)
	{
		f((T*)in);
	};
}

void CallFunc(Base* o)
{
	fs[o->GetTypeId()](o);
}

int main()
{
	A a;
	B b;
	std::cout << a.GetTypeId() << b.GetTypeId() << std::endl;

	RegFunc<A>([](A* o)
	{
		std::cout << "A\n";
	});
	RegFunc<B>([](B* o)
	{
		std::cout << "B\n";
	});

	CallFunc(&a);
	CallFunc(&b);

	return 0;
}

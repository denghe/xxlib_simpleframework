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

//template<typename T>
//struct F2T;
//
//template<typename R, typename T>
//struct F2T<std::function<R(T*)>>
//{
//	using type = T;
//};
//
//template<typename F>
//void RegFunc2(F f)
//{
//	using T = typename F2T<F>::type;
//	fs[TypeId_v<T>] = [f = std::move(f)](void* in)
//	{
//		f((T*)in);
//	};
//}

void CallFunc(Base* o)
{
	fs[o->GetTypeId()](o);
}

//#include <windows.h>
//#undef min
//#undef max
//#include "xx_logger.h"

int main(int argc, char* argv[])
{
	//std::string fn = argv[0];
	//fn += ".log.db";
	//xx::Logger logger(fn.c_str());

	//xx::Stopwatch sw;
	//logger.SetDefaultValue("machine1", "service1", "instance1");
	//for (int i = 0; i < 10000000; ++i)
	//{
	//	logger.Write(xx::LogLevel::Warning, "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa", 123
	//		, "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb");
	//	//logger.WriteAll(xx::LogLevel::Warning, i, "machine1", "service1", "instance1", "title1", 123, "desc");
	//}
	//std::cout << sw() << std::endl;
	//Sleep(5000);
	//sw.Reset();
	//for (int i = 0; i < 10000000; ++i)
	//{
	//	logger.Write(xx::LogLevel::Warning, "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa", 123
	//		, "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb");
	//}
	//std::cout << sw() << std::endl;
	//return 0;






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

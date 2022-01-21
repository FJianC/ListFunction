#ifndef __LIST_FUNCTION_HPP__
#define __LIST_FUNCTION_HPP__

#include <functional>

/*
 * auto list = ListFunction<int(int)>(
 *		[](int i) { return i + 1; }
 * ).then(
 *		[](int i) { return i + 2; }
 * );
 * 
 * int a = 1;
 * auto ret = list.run(std::move(a));	// list.run(1);
 *
 */

template<typename T>
class ListFunction;

template<typename Ret, typename... Args>
class ListFunction<Ret(Args...)>
{
public:
	ListFunction() = delete;
	ListFunction(std::function<Ret(Args...)>&& func) :
		m_func(std::move(func)) {}
	ListFunction(const ListFunction<Ret(Args...)>&) = delete;
	~ListFunction() = default;

	ListFunction<Ret<Args...>>& operator=(const ListFunction<Ret(Args...)>&) = delete;

	Ret run(Args&&... args) {
		return m_func(std::forward<Args>(args)...);
	}

	template<typename Func>
	auto then(Func&& f) -> ListFunction<typename std::result_of<Func(Ret)>::type(Args...)> {
		auto func = std::move(m_func);
		return ListFunction<typename std::result_of<Func(Ret)>::type(Args...)>(
			[func](Func&& f, Args&&... args) {
				return f(func(std::forward<Args>(args)...));
			});
	}

private:

};

#endif // !__LIST_FUNCTION_HPP__

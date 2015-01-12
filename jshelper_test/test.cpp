#include "test.h"

#include <assert.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include "..\helper\TinyJS_Helper.h"


class cboy
{
public:
	int add_age(int n)
	{
		m_age += n;
		return m_age;
	}

	void set_name(std::string name)
	{
	}

	std::string do_some_thing(int n, double d, std::string str)
	{
		return "ok";
	}
private:
	int m_age;
};

static double mySqr(double d)
{
	return d*d;
}


static std::string myStringConcat(std::string strL, std::string strR)
{
	return strL + strR;
}

static double myDoSomeThing(int n, double d, std::string str1, std::string str2)
{
	return n + d;
}

void test()
{
	CTinyJS_Wrapper s;

	cboy boy;
	s.addExFun(MakeFunObj<int, MakeTypelist<int>>("add_boy_age", &boy, &cboy::add_age));
	s.addExFun(MakeFunObj<void, MakeTypelist<std::string>>("set_boy_name", &boy, &cboy::set_name));
	s.addExFun(MakeFunObj<std::string, MakeTypelist<int, double, std::string>>("boy_do_some_thing", &boy, &cboy::do_some_thing));

	s.addExFun(MakeFunObj<double, MakeTypelist<double>>("sqr", &mySqr));
	s.addExFun(MakeFunObj<std::string, MakeTypelist<std::string, std::string>>("strcon", &myStringConcat));
	s.addExFun(MakeFunObj<double, MakeTypelist<int, double, std::string, std::string>>("do_some_thing", &myDoSomeThing));

	s.execute("var lets_quit = 0; function quit() { lets_quit = 1;  } add_boy_age(1); do_some_thing(1, 0.5, 'aa', 'bb') ");
}

#pragma comment(linker, "/subsystem:console") 

int main(int argc, char **argv)
{
	test();
	return 0;
}

#pragma once



#include "..\TinyJS.h"
#include "TypeList.h"
#include "Functor.h"
#include <string>
#include <list>


using namespace Loki;
using namespace Loki::TL;

//////////////////////////////////////////////////////////////////////////
template <typename T>
inline T ConvertToType(const CScriptVarPtr &var);

template <>
inline int ConvertToType<int>(const CScriptVarPtr &var)
{
	return var->toNumber().toInt32();
}

template <>
inline long ConvertToType<long>(const CScriptVarPtr &var)
{
	return var->toNumber().toInt32();
}

template <>
inline double ConvertToType<double>(const CScriptVarPtr &var)
{
	return var->toNumber().toDouble();
}

template <>
inline std::string ConvertToType<std::string>(const CScriptVarPtr &var)
{
	return var->toString();
}

//////////////////////////////////////////////////////////////////////////

#define DEF_STRING(x)					(#x)
#define DEF_TSTRING(x)					_T(#x)
#define I_CONCATENATE_DIRECT(s1, s2)	s1##s2
#define I_CONCATENATE(s1, s2)			I_CONCATENATE_DIRECT(s1, s2)

#define SET_RETURN_VAL(v)	{ c->setReturnVar(c->newScriptVar(v)); }
#define GET_ARGUMENT(n)		ConvertToType<P##n>(c->getArgument( "p"###n))

//////////////////////////////////////////////////////////////////////////
template <typename R, class TList>
class FunTreat {};


template<typename R>
struct FunTreat<R, typename MakeTypelist<>>
{
	template<typename T>
	void execute(T &fun, const CFunctionsScopePtr &c) const
	{
		R val = fun();
		SET_RETURN_VAL(val);
	}
	std::string param_str() const { return "()"; }
};

template<>
struct FunTreat<void, typename MakeTypelist<>>
{
	template<typename T>
	void execute(T &fun, const CFunctionsScopePtr &c) const
	{
		fun();
	}
	std::string param_str() const { return "()"; }
};


template<typename R, typename P1>
struct FunTreat<R, typename MakeTypelist<P1>>
{
	template<typename T>
	void execute(T &fun, const CFunctionsScopePtr &c) const
	{
		R val = fun(GET_ARGUMENT(1));
		SET_RETURN_VAL(val);
	}
	std::string param_str() const { return "(p1)"; }
};

template<typename P1>
struct FunTreat<void, typename MakeTypelist<P1>>
{
	template<typename T>
	void execute(T &fun, const CFunctionsScopePtr &c) const
	{
		fun(GET_ARGUMENT(1));
	}
	std::string param_str() const { return "(p1)"; }
};


template<typename R, typename P1, typename P2>
struct FunTreat<R, typename MakeTypelist<P1, P2>>
{
	template<typename T>
	void execute(T &fun, const CFunctionsScopePtr &c) const
	{
		R val = fun(GET_ARGUMENT(1), GET_ARGUMENT(2));
		SET_RETURN_VAL(val);
	}
	std::string param_str() const { return "(p1, p2)"; }
};

template<typename P1, typename P2>
struct FunTreat<void, typename MakeTypelist<P1, P2>>
{
	template<typename T>
	void execute(T &fun, const CFunctionsScopePtr &c) const
	{
		fun(GET_ARGUMENT(1), GET_ARGUMENT(2));
	}
	std::string param_str() const { return "(p1, p2)"; }
};


template<typename R, typename P1, typename P2, typename P3>
struct FunTreat<R, typename MakeTypelist<P1, P2, P3>>
{
	template<typename T>
	void execute(T &fun, const CFunctionsScopePtr &c) const
	{
		R val = fun(GET_ARGUMENT(1), GET_ARGUMENT(2), GET_ARGUMENT(3));
		SET_RETURN_VAL(val);
	}
	std::string param_str() const { return "(p1, p2, p3)"; }
};

template<typename P1, typename P2, typename P3>
struct FunTreat<void, typename MakeTypelist<P1, P2, P3>>
{
	template<typename T>
	void execute(T &fun, const CFunctionsScopePtr &c) const
	{
		fun(GET_ARGUMENT(1), GET_ARGUMENT(2), GET_ARGUMENT(3));
	}
	std::string param_str() const { return "(p1, p2, p3)"; }
};


template<typename R, typename P1, typename P2, typename P3, typename P4>
struct FunTreat<R, typename MakeTypelist<P1, P2, P3, P4>>
{
	template<typename T>
	void execute(T &fun, const CFunctionsScopePtr &c) const
	{
		R val = fun(GET_ARGUMENT(1), GET_ARGUMENT(2), GET_ARGUMENT(3), GET_ARGUMENT(4));
		SET_RETURN_VAL(val);
	}
	std::string param_str() const { return "(p1, p2, p3, p4)"; }
};

template<typename P1, typename P2, typename P3, typename P4>
struct FunTreat<void, typename MakeTypelist<P1, P2, P3, P4>>
{
	template<typename T>
	void execute(T &fun, const CFunctionsScopePtr &c) const
	{
		fun(GET_ARGUMENT(1), GET_ARGUMENT(2), GET_ARGUMENT(3), GET_ARGUMENT(4));
	}
	std::string param_str() const { return "(p1, p2, p3, p4)"; }
};


template<typename R, typename P1, typename P2, typename P3, typename P4, typename P5>
struct FunTreat<R, typename MakeTypelist<P1, P2, P3, P4, P5>>
{
	template<typename T>
	void execute(T &fun, const CFunctionsScopePtr &c) const
	{
		R val = fun(GET_ARGUMENT(1), GET_ARGUMENT(2), GET_ARGUMENT(3), GET_ARGUMENT(4), GET_ARGUMENT(5));
		SET_RETURN_VAL(val);
	}
	std::string param_str() const { return "(p1, p2, p3, p4, p5)"; }
};

template<typename P1, typename P2, typename P3, typename P4, typename P5>
struct FunTreat<void, typename MakeTypelist<P1, P2, P3, P4, P5>>
{
	template<typename T>
	void execute(T &fun, const CFunctionsScopePtr &c) const
	{
		fun(GET_ARGUMENT(1), GET_ARGUMENT(2), GET_ARGUMENT(3), GET_ARGUMENT(4), GET_ARGUMENT(5));
	}
	std::string param_str() const { return "(p1, p2, p3, p4, p5)"; }
};


//////////////////////////////////////////////////////////////////////////

struct CFunObj
{
	virtual std::string func_desc() const = 0;
	virtual void execute(const CFunctionsScopePtr &c) = 0;
	CScriptVarFunctionNativePtr reg_fun(CTinyJS &js);
};

template<typename R, typename SeqT>
class CFunObjImpl : public CFunObj
{
public:
	typedef typename SeqT::Result	TList;

	template<typename F>
	CFunObjImpl(const std::string &fName, F fun) : m_name(fName), m_fun(fun) {}

	template<typename T, typename F>
	CFunObjImpl(const std::string &fName, T obj, F fun) : m_name(fName), m_fun(obj, fun) {}

	std::string func_desc() const
	{
		return "function " + m_name + m_fun_treat.param_str();
	}

	void execute(const CFunctionsScopePtr &c)
	{
		m_fun_treat.execute(m_fun, c);
	}

protected:
	Functor<R, TList>	m_fun;
	FunTreat<R, SeqT>	m_fun_treat;
	std::string				m_name;
};

//////////////////////////////////////////////////////////////////////////

template<typename R, typename SeqP, typename F>
CFunObj* MakeFunObj(const std::string &fName, F fun)
{
	return new CFunObjImpl<R, SeqP>(fName, fun);
}

template<typename R, typename SeqP, typename T, typename F>
CFunObj* MakeFunObj(const std::string &fName, T obj, F fun)
{
	return new CFunObjImpl<R, SeqP>(fName, obj, fun);
}



//////////////////////////////////////////////////////////////////////////
typedef std::list<CFunObj*>	listFunObj;
class CTinyJS_Wrapper : public CTinyJS
{
public:
	~CTinyJS_Wrapper();
	CScriptVarFunctionNativePtr addExFun(CFunObj *pFun);

protected:
	listFunObj m_funList;
};

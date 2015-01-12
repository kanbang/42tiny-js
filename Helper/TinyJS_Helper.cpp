#include "TinyJS_Helper.h"



static void fun_call_back(const CFunctionsScopePtr &c, void *userdata)
{
	CFunObj *pFun = (CFunObj*)userdata;
	pFun->execute(c);
}

CScriptVarFunctionNativePtr CFunObj::reg_fun(CTinyJS &js)
{
	return js.addNative(func_desc(), fun_call_back, this);
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
CTinyJS_Wrapper::~CTinyJS_Wrapper()
{
	for (listFunObj::iterator it = m_funList.begin(); it != m_funList.end(); ++it)
	{
		delete (*it);
	}
}

CScriptVarFunctionNativePtr CTinyJS_Wrapper::addExFun(CFunObj *pFun)
{
	return pFun->reg_fun(*this);
}



/*
 * 42TinyJS
 *
 * A fork of TinyJS with the goal to makes a more JavaScript/ECMA compliant engine
 *
 * Authored By Armin Diedering <armin@diedering.de>
 *
 * Copyright (C) 2010 ardisoft
 *
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is furnished to do
 * so, subject to the following conditions:

 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.

 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <algorithm>
#include "TinyJS.h"

#ifndef NO_REGEXP 
#	if defined HAVE_TR1_REGEX
#		include <tr1/regex>
		using namespace std::tr1;
#	elif defined HAVE_BOOST_REGEX
#		include <boost/tr1/tr1/regex>
		using namespace boost;
#	else
#		include <regex>
#	endif
#endif
using namespace std;
// ----------------------------------------------- Actual Functions

static void scStringCharAt(const CFunctionsScopePtr &c, void *) {
	string str = c->getArgument("this")->getString();
	int p = c->getArgument("pos")->getInt();
	if (p>=0 && p<(int)str.length())
		c->setReturnVar(c->newScriptVar(str.substr(p, 1)));
	else
		c->setReturnVar(c->newScriptVar(""));
}

static void scStringCharCodeAt(const CFunctionsScopePtr &c, void *) {
	string str = c->getArgument("this")->getString();
	int p = c->getArgument("pos")->getInt();
	if (p>=0 && p<(int)str.length())
		c->setReturnVar(c->newScriptVar(str.at(p)));
	else
		c->setReturnVar(c->newScriptVar(0));
}

static void scStringConcat(const CFunctionsScopePtr &c, void *userdata) {
	int length = c->getArgumentsLength();
	string str = c->getArgument("this")->getString();
	for(int i=(int)userdata; i<length; i++)
		str.append(c->getArgument(i)->getString());
	c->setReturnVar(c->newScriptVar(str));
}

static void scStringIndexOf(const CFunctionsScopePtr &c, void *userdata) {
	string str = c->getArgument("this")->getString();
	string search = c->getArgument("search")->getString();
	size_t p = (userdata==0) ? str.find(search) : str.rfind(search);
	int val = (p==string::npos) ? -1 : p;
	c->setReturnVar(c->newScriptVar(val));
}

static void scStringSlice(const CFunctionsScopePtr &c, void *userdata) {
	string str = c->getArgument("this")->getString();
	int length = c->getArgumentsLength()-((int)userdata & 1);
	bool slice = ((int)userdata & 2) == 0;
	int start = c->getArgument("start")->getInt();
	int end = (int)str.size();
	if(slice && start<0) start = str.size()+start;
	if(length>1) {
		end = c->getArgument("end")->getInt();
		if(slice && end<0) end = str.size()+end;
	}
	if(!slice && end < start) { end^=start; start^=end; end^=start; }
	if(start<0) start = 0;
	if(start>=(int)str.size()) 
		c->setReturnVar(c->newScriptVar(""));
	else if(end <= start)
		c->setReturnVar(c->newScriptVar(""));
	else
		c->setReturnVar(c->newScriptVar(str.substr(start, end-start)));
}

static void scStringSplit(const CFunctionsScopePtr &c, void *) {
	string str = c->getArgument("this")->getString();
	CScriptVarPtr sep_var = c->getArgument("separator");
	CScriptVarPtr limit_var = c->getArgument("limit");
	int limit = limit_var->isUndefined() ? 0x7fffffff : limit_var->getInt();

	CScriptVarPtr result(newScriptVar(c->getContext(), Array));
	c->setReturnVar(result);
	if(limit == 0 || !str.size())
		return;
	else if(sep_var->isUndefined()) {
		result->setArrayIndex(0, c->newScriptVar(str));
		return;
	}
	string sep = sep_var->getString();
	if(sep.size() == 0) {
		for(int i=0; i<min((int)sep.size(), limit); ++i)
			result->setArrayIndex(i, c->newScriptVar(str.substr(i,1)));
		return;
	}
	int length = 0;
	string::size_type pos = 0, last_pos=0;
	do {
		pos = str.find(sep, last_pos);
		result->setArrayIndex(length++, c->newScriptVar(str.substr(last_pos ,pos-last_pos)));
		if(length == limit || pos == string::npos) break;
		last_pos = pos+sep.size();
	} while (last_pos < str.size());
}

static void scStringSubstr(const CFunctionsScopePtr &c, void *userdata) {
	string str = c->getArgument("this")->getString();
	int length = c->getArgumentsLength()-(int)userdata;
	int start = c->getArgument("start")->getInt();
	if(start<0 || start>=(int)str.size()) 
		c->setReturnVar(c->newScriptVar(""));
	else if(length>1) {
		int length = c->getArgument("length")->getInt();
		c->setReturnVar(c->newScriptVar(str.substr(start, length)));
	} else
		c->setReturnVar(c->newScriptVar(str.substr(start)));
}

static void scStringToLowerCase(const CFunctionsScopePtr &c, void *) {
	string str = c->getArgument("this")->getString();
	transform(str.begin(), str.end(), str.begin(), ::tolower);
	c->setReturnVar(c->newScriptVar(str));
}

static void scStringToUpperCase(const CFunctionsScopePtr &c, void *) {
	string str = c->getArgument("this")->getString();
	transform(str.begin(), str.end(), str.begin(), ::toupper);
	c->setReturnVar(c->newScriptVar(str));
}

static void scStringTrim(const CFunctionsScopePtr &c, void *userdata) {
	string str = c->getArgument("this")->getString();
	string::size_type start = 0;
	string::size_type end = string::npos;
	if((((int)userdata) & 2) == 0) {
		start = str.find_first_not_of(" \t\r\n");
		if(start == string::npos) start = 0;
	}
	if((((int)userdata) & 1) == 0) {
		end = str.find_last_not_of(" \t\r\n");
		if(end != string::npos) end = 1+end-start;
	}
	c->setReturnVar(c->newScriptVar(str.substr(start, end)));
}



static void scCharToInt(const CFunctionsScopePtr &c, void *) {
	string str = c->getArgument("ch")->getString();;
	int val = 0;
	if (str.length()>0)
		val = (int)str.c_str()[0];
	c->setReturnVar(c->newScriptVar(val));
}


static void scStringFromCharCode(const CFunctionsScopePtr &c, void *) {
	char str[2];
	str[0] = c->getArgument("char")->getInt();
	str[1] = 0;
	c->setReturnVar(c->newScriptVar(str));
}

//////////////////////////////////////////////////////////////////////////
// RegExp-Stuff
//////////////////////////////////////////////////////////////////////////

#ifndef NO_REGEXP

static void scRegExpExec(const CFunctionsScopePtr &c, void *) {
	CScriptVarRegExpPtr This = c->getArgument("this");
	if(This)
		c->setReturnVar(This->exec(c->getArgument("str")->getString()));
	else
		c->throwError(TypeError, "Object is not an RegExp in exec(str)");
}
//print(/a.*l(l)/.exec("Hallo"))
#endif /* NO_REGEXP */

// ----------------------------------------------- Register Functions
//typedef basic_regex<char> regex;
void registerStringFunctions(CTinyJS *tinyJS) {
	CScriptVarPtr fnc;
	// charAt
	tinyJS->addNative("function String.prototype.charAt(pos)", scStringCharAt, 0);
	tinyJS->addNative("function String.charAt(this,pos)", scStringCharAt, 0);
	// charCodeAt
	tinyJS->addNative("function String.prototype.charCodeAt(pos)", scStringCharCodeAt, 0);
	tinyJS->addNative("function String.charCodeAt(this,pos)", scStringCharCodeAt, 0);
	// concat
	tinyJS->addNative("function String.prototype.concat()", scStringConcat, 0);
	tinyJS->addNative("function String.concat(this)", scStringConcat, (void*)1);
	// indexOf
	tinyJS->addNative("function String.prototype.indexOf(search)", scStringIndexOf, 0); // find the position of a string in a string, -1 if not
	tinyJS->addNative("function String.indexOf(this,search)", scStringIndexOf, 0); // find the position of a string in a string, -1 if not
	// lastIndexOf
	tinyJS->addNative("function String.prototype.lastIndexOf(search)", scStringIndexOf, (void*)-1); // find the last position of a string in a string, -1 if not
	tinyJS->addNative("function String.lastIndexOf(this,search)", scStringIndexOf, (void*)-1); // find the last position of a string in a string, -1 if not
	// slice
	tinyJS->addNative("function String.prototype.slice(start,end)", scStringSlice, 0); // find the last position of a string in a string, -1 if not
	tinyJS->addNative("function String.slice(this,start,end)", scStringSlice, (void*)1); // find the last position of a string in a string, -1 if not
	// split
	tinyJS->addNative("function String.prototype.split(separator,limit)", scStringSplit, 0);
	tinyJS->addNative("function String.split(this,separator,limit)", scStringSplit, 0);
	// substr
	tinyJS->addNative("function String.prototype.substr(start,length)", scStringSubstr, 0);
	tinyJS->addNative("function String.substr(this,start,length)", scStringSubstr, (void*)1);
	// substring
	tinyJS->addNative("function String.prototype.substring(start,end)", scStringSlice, (void*)2);
	tinyJS->addNative("function String.substring(this,start,end)", scStringSlice, (void*)3);
	// toLowerCase
	tinyJS->addNative("function String.prototype.toLowerCase()", scStringToLowerCase, 0);
	tinyJS->addNative("function String.toLowerCase(this)", scStringToLowerCase, 0);
	// toUpperCase
	tinyJS->addNative("function String.prototype.toUpperCase()", scStringToUpperCase, 0);
	tinyJS->addNative("function String.toUpperCase(this)", scStringToUpperCase, 0);
	// trim
	tinyJS->addNative("function String.prototype.trim()", scStringTrim, 0);
	tinyJS->addNative("function String.trim(this)", scStringTrim, 0);
	// trimLeft
	tinyJS->addNative("function String.prototype.trimLeft()", scStringTrim, (void*)1);
	tinyJS->addNative("function String.trimLeft(this)", scStringTrim, (void*)1);
	// trimRight
	tinyJS->addNative("function String.prototype.trimRight()", scStringTrim, (void*)2);
	tinyJS->addNative("function String.trimRight(this)", scStringTrim, (void*)2);

	tinyJS->addNative("function charToInt(ch)", scCharToInt, 0); //  convert a character to an int - get its value
	
	tinyJS->addNative("function String.prototype.fromCharCode(char)", scStringFromCharCode, 0);
#ifndef NO_REGEXP
	tinyJS->addNative("function RegExp.prototype.exec(str)", scRegExpExec, 0);
#endif /* NO_REGEXP */

}


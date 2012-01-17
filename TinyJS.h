/*
 * TinyJS
 *
 * A single-file Javascript-alike engine
 *
 * Authored By Gordon Williams <gw@pur3.co.uk>
 *
 * Copyright (C) 2009 Pur3 Ltd
 *

 * 42TinyJS
 *
 * A fork of TinyJS with the goal to makes a more JavaScript/ECMA compliant engine
 *
 * Authored / Changed By Armin Diedering <armin@diedering.de>
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

#ifndef TINYJS_H
#define TINYJS_H

#include <string>
#include <vector>
#include <map>
#include <set>
#include "pool_allocator.h"
#include <stdint.h>
#include <assert.h>

#ifndef ASSERT
#	define ASSERT(X) assert(X)
#endif

#ifndef __GNUC__
#	define __attribute__(x)
#endif

#ifdef _MSC_VER
#	define DEPRECATED(_Text) __declspec(deprecated(_Text))
#elif defined(__GNUC__)
#	define DEPRECATED(_Text) __attribute__ ((deprecated))
#else
#	define DEPRECATED(_Text)
#endif

#undef TRACE
#ifndef TRACE
#define TRACE printf
#endif // TRACE

enum LEX_TYPES {
	LEX_EOF = 0,
#define LEX_RELATIONS_1_BEGIN LEX_EQUAL
	LEX_EQUAL = 256,
	LEX_TYPEEQUAL,
	LEX_NEQUAL,
	LEX_NTYPEEQUAL,
#define LEX_RELATIONS_1_END LEX_NTYPEEQUAL
	LEX_LEQUAL,
	LEX_GEQUAL,
#define LEX_SHIFTS_BEGIN LEX_LSHIFT
	LEX_LSHIFT,
	LEX_RSHIFT,
	LEX_RSHIFTU, // unsigned
#define LEX_SHIFTS_END LEX_RSHIFTU
	LEX_PLUSPLUS,
	LEX_MINUSMINUS,
	LEX_ANDAND,
	LEX_OROR,
	LEX_INT,

#define LEX_ASSIGNMENTS_BEGIN LEX_PLUSEQUAL
	LEX_PLUSEQUAL,
	LEX_MINUSEQUAL,
	LEX_ASTERISKEQUAL,
	LEX_SLASHEQUAL,
	LEX_PERCENTEQUAL,
	LEX_LSHIFTEQUAL,
	LEX_RSHIFTEQUAL,
	LEX_RSHIFTUEQUAL, // unsigned
	LEX_ANDEQUAL,
	LEX_OREQUAL,
	LEX_XOREQUAL,
#define LEX_ASSIGNMENTS_END LEX_XOREQUAL

#define LEX_TOKEN_STRING_BEGIN LEX_ID
	LEX_ID,
	LEX_STR,
	LEX_REGEXP,
	LEX_T_LABEL,
#define LEX_TOKEN_STRING_END LEX_T_LABEL

	LEX_FLOAT,

	// reserved words
	LEX_R_IF,
	LEX_R_ELSE,
#define LEX_TOKEN_LOOP_BEGIN LEX_R_DO
	LEX_R_DO,
	LEX_R_WHILE,
	LEX_R_FOR,
#define LEX_TOKEN_LOOP_END LEX_R_FOR
	LEX_R_IN,
	LEX_R_BREAK,
	LEX_R_CONTINUE,
	LEX_R_FUNCTION,
	LEX_R_RETURN,
	LEX_R_VAR,
	LEX_R_LET,
	LEX_R_WITH,
	LEX_R_TRUE,
	LEX_R_FALSE,
	LEX_R_NULL,
	LEX_R_NEW,
	LEX_R_TRY,
	LEX_R_CATCH,
	LEX_R_FINALLY,
	LEX_R_THROW,
	LEX_R_TYPEOF,
	LEX_R_VOID,
	LEX_R_DELETE,
	LEX_R_INSTANCEOF,
	LEX_R_SWITCH,
	LEX_R_CASE,
	LEX_R_DEFAULT,

	// special token
//	LEX_T_FILE,
	LEX_T_FUNCTION_OPERATOR,
	LEX_T_GET,
	LEX_T_SET,

	LEX_T_FOR_IN,
	LEX_T_FOR_EACH_IN,

	LEX_T_SKIP,

};
#define LEX_TOKEN_DATA_STRING(tk) ((LEX_TOKEN_STRING_BEGIN<= tk && tk <= LEX_TOKEN_STRING_END))
#define LEX_TOKEN_DATA_FLOAT(tk) (tk==LEX_FLOAT)
#define LEX_TOKEN_DATA_FUNCTION(tk) (tk==LEX_R_FUNCTION || tk==LEX_T_FUNCTION_OPERATOR || tk==LEX_T_SET || tk==LEX_T_GET)
#define LEX_TOKEN_DATA_SIMPLE(tk) (!LEX_TOKEN_DATA_STRING(tk) && !LEX_TOKEN_DATA_FLOAT(tk) && !LEX_TOKEN_DATA_FUNCTION(tk))
/*
enum SCRIPTVAR_FLAGS {
	SCRIPTVAR_UNDEFINED				= 0,
	SCRIPTVAR_FUNCTION				= 1<<0,
	SCRIPTVAR_OBJECT					= 1<<1,
	SCRIPTVAR_ARRAY					= 1<<2,
	SCRIPTVAR_DOUBLE					= 1<<3,  // floating point double
	SCRIPTVAR_INTEGER					= 1<<4, // integer number
	SCRIPTVAR_BOOLEAN					= 1<<5, // boolean
	SCRIPTVAR_STRING					= 1<<6, // string
	SCRIPTVAR_NULL						= 1<<7, // it seems null is its own data type
	SCRIPTVAR_INFINITY				= 1<<8, // it seems infinity is its own data type
	SCRIPTVAR_NAN						= 1<<9, // it seems NaN is its own data type
	SCRIPTVAR_ACCESSOR				= 1<<10, // it seems an Object with get() and set()

	SCRIPTVAR_END_OF_TYPES			= 1<<15, // it seems NaN is its own data type

	SCRIPTVAR_NATIVE_FNC				= 1<<16, // to specify this is a native function
	SCRIPTVAR_NATIVE_MFNC			= 1<<17, // to specify this is a native function from class->memberFunc
	
	SCRIPTVAR_NUMBERMASK				= SCRIPTVAR_DOUBLE |
											  SCRIPTVAR_INTEGER,
	SCRIPTVAR_NUMERICMASK			= SCRIPTVAR_NUMBERMASK |
											  SCRIPTVAR_NULL |
											  SCRIPTVAR_BOOLEAN,
	
	SCRIPTVAR_VARTYPEMASK			= SCRIPTVAR_END_OF_TYPES - 1,
									  
	SCRIPTVAR_NATIVE					= SCRIPTVAR_NATIVE_FNC |
											  SCRIPTVAR_NATIVE_MFNC,
};
*/
enum SCRIPTVARLINK_FLAGS {
	SCRIPTVARLINK_OWNED			= 1<<0,
	SCRIPTVARLINK_WRITABLE		= 1<<1,
	SCRIPTVARLINK_DELETABLE		= 1<<2,
	SCRIPTVARLINK_ENUMERABLE	= 1<<3,
	SCRIPTVARLINK_HIDDEN			= 1<<4,
	SCRIPTVARLINK_DEFAULT		= SCRIPTVARLINK_WRITABLE | SCRIPTVARLINK_DELETABLE | SCRIPTVARLINK_ENUMERABLE,
};
enum RUNTIME_FLAGS {
	RUNTIME_CANRETURN			= 1<<0,

	RUNTIME_CANBREAK			= 1<<1,
	RUNTIME_BREAK				= 1<<2,
	RUNTIME_BREAK_MASK		= RUNTIME_CANBREAK | RUNTIME_BREAK,

	RUNTIME_CANCONTINUE		= 1<<3,
	RUNTIME_CONTINUE			= 1<<4,
	RUNTIME_LOOP_MASK			= RUNTIME_BREAK_MASK | RUNTIME_CANCONTINUE | RUNTIME_CONTINUE,

	RUNTIME_NEW					= 1<<5,

	RUNTIME_CANTHROW			= 1<<6,
	RUNTIME_THROW				= 1<<7,
	RUNTIME_THROW_MASK		= RUNTIME_CANTHROW | RUNTIME_THROW,

};

#define SAVE_RUNTIME_RETURN	int old_return_runtimeFlags = runtimeFlags & RUNTIME_CANRETURN
#define RESTORE_RUNTIME_RETURN	runtimeFlags = (runtimeFlags & ~RUNTIME_CANRETURN) | old_pass_runtimeFlags
#define SET_RUNTIME_CANRETURN runtimeFlags |= RUNTIME_CANRETURN
#define IS_RUNTIME_CANRETURN ((runtimeFlags & RUNTIME_CANRETURN) == RUNTIME_CANRETURN)

enum ERROR_TYPES {
	Error = 0,
	EvalError,
	RangeError,
	ReferenceError,
	SyntaxError,
	TypeError
};
#define ERROR_MAX TypeError
#define ERROR_COUNT (ERROR_MAX+1)
extern const char *ERROR_NAME[];


#define TINYJS_RETURN_VAR					"return"
#define TINYJS_LOKALE_VAR					"__locale__"
#define TINYJS_ANONYMOUS_VAR				"__anonymous__"
#define TINYJS_ARGUMENTS_VAR				"arguments"
#define TINYJS___PROTO___VAR				"__proto__"
#define TINYJS_PROTOTYPE_CLASS			"prototype"
#define TINYJS_FUNCTION_CLOSURE_VAR		"__function_closure__"
#define TINYJS_SCOPE_PARENT_VAR			"__scope_parent__"
#define TINYJS_SCOPE_WITH_VAR				"__scope_with__"
#define TINYJS_ACCESSOR_GET_VAR			"__accessor_get__"
#define TINYJS_ACCESSOR_SET_VAR			"__accessor_set__"
#define TINYJS_TEMP_NAME			""
#define TINYJS_BLANK_DATA			""
#define TINYJS_NEGATIVE_INFINITY_DATA			"-1"
#define TINYJS_POSITIVE_INFINITY_DATA			"+1"

typedef std::vector<std::string> STRING_VECTOR_t;
typedef STRING_VECTOR_t::iterator STRING_VECTOR_it;

/// convert the given string into a quoted string suitable for javascript
std::string getJSString(const std::string &str);
/// convert the given int into a string
std::string int2string(int intData);
/// convert the given double into a string
std::string float2string(const double &floatData);

// ----------------------------------------------------------------------------------- CScriptSmartPointer
template<class T>
class CScriptSmartPointer
{
public:
	CScriptSmartPointer() : pointer(0) {}
	explicit CScriptSmartPointer(T *Pointer)  : pointer(Pointer ? new p(Pointer) : 0) {}
	CScriptSmartPointer(const CScriptSmartPointer &Copy) : pointer(0) {*this = Copy;}
	CScriptSmartPointer &operator=(const CScriptSmartPointer &Copy)
	{
		if(this == &Copy) return *this;
		if(pointer) pointer->unref();
		if(Copy.pointer) 
			pointer = Copy.pointer->ref();
		else
			pointer = 0;
		return *this;
	}
	~CScriptSmartPointer() { release(); }
	void release() { if(pointer) pointer->unref(); pointer = 0;}
	T *operator->() { return operator T*(); }
	operator T*() { return pointer ? pointer->pointer : 0; }
private:
	class p
	{
	public:
		p(T *Pointer) : pointer(Pointer), refs(1) {}
		p *ref() { refs++; return this; }
		void unref() { if(--refs == 0) { delete pointer; delete this; } }
		T *pointer;
		int refs;
	} *pointer;
};

class CScriptException {
public:
	std::string text;
//	int pos;
	std::string file;
	int line;
	int column;
//	CScriptException(const std::string &exceptionText, int Pos=-1);
	CScriptException(const std::string &Text, const std::string &File, int Line=-1, int Column=-1) :
						text(Text), file(File), line(Line), column(Column){}
	CScriptException(const std::string &Text, const char *File="", int Line=-1, int Column=-1) :
						text(Text), file(File), line(Line), column(Column){}
};

// ----------------------------------------------------------------------------------- CSCRIPTLEX
class CScriptLex
{
public:
	CScriptLex(const char *Code, const std::string &File="", int Line=0, int Column=0);

	int tk; ///< The type of the token that we have
	int last_tk; ///< The type of the last token that we have
	const char *tokenStart;
	std::string tkStr; ///< Data contained in the token we have here

	void check(int expected_tk); ///< Lexical check wotsit
	void match(int expected_tk); ///< Lexical match wotsit
	static std::string getTokenStr(int token); ///< Get the string representation of the given token
	void reset(const char *toPos, int line, const char *LineStart); ///< Reset this lex so we can start again

	std::string currentFile;
	int currentLine;
	const char *currentLineStart;
	int currentColumn() { return tokenStart-currentLineStart; }

private:
	const char *data;
	const char *dataPos;
	char currCh, nextCh;

	void getNextCh();
	void getNextToken(); ///< Get the text token from our text string
};

// ----------------------------------------------------------------------------------- CSCRIPTTOKEN
class CScriptToken;
typedef  std::vector<CScriptToken> TOKEN_VECT;
typedef  std::vector<CScriptToken>::iterator TOKEN_VECT_it;
class CScriptTokenData
{
public:
	virtual ~CScriptTokenData() {}
	void ref() { refs++; }
	void unref() { if(--refs == 0) delete this; }
protected:
	CScriptTokenData() : refs(0){}
private:
	int refs;
};
class CScriptTokenDataString : public fixed_size_object<CScriptTokenDataString>, public CScriptTokenData {
public:
	CScriptTokenDataString(const std::string &String) : tokenStr(String) {}
	std::string tokenStr;
private:
};
class CScriptTokenDataFnc : public fixed_size_object<CScriptTokenDataFnc>, public CScriptTokenData {
public:
	std::string file;
	int line;
	std::string name;
	std::vector<std::string> arguments;
	TOKEN_VECT body;
private:
};

class CScriptTokenizer;
/*
	a Token needs 8 Byte
	2 Bytes for the Row-Position of the Token
	2 Bytes for the Token self
	and
	4 Bytes for special Datas in an union
			e.g. an int for interger-literals 
			or pointer for double-literals,
			for string-literals or for functions
*/
class CScriptToken : public fixed_size_object<CScriptToken>
{
public:
	CScriptToken() : line(0), column(0), token(0) {}
	CScriptToken(CScriptLex *l, int Match=-1);
	CScriptToken(uint16_t Tk, int IntData=0);
	CScriptToken(uint16_t Tk, const std::string &TkStr);
	CScriptToken(const  CScriptToken &Copy) : token(0) { *this = Copy; }
	CScriptToken &operator =(const CScriptToken &Copy);
	~CScriptToken() { clear(); }

	int &Int() { ASSERT(LEX_TOKEN_DATA_SIMPLE(token)); return intData; }
	std::string &String() { ASSERT(LEX_TOKEN_DATA_STRING(token)); return stringData->tokenStr; }
	double &Float() { ASSERT(LEX_TOKEN_DATA_FLOAT(token)); return *floatData; }
	CScriptTokenDataFnc &Fnc() { ASSERT(LEX_TOKEN_DATA_FUNCTION(token)); return *fncData; }
	uint16_t			line;
	uint16_t			column;
	uint16_t			token;
	void print(std::string &indent);
	std::string getParsableString(const std::string Indent="");
	std::string getParsableString(std::string &indentString, int &newln, const std::string &indent); ///< newln ==> 1=inject indentString, 2=inject \n, 3=last='{'
private:

	void clear();
	union {
		int							intData;
		double						*floatData;
		CScriptTokenDataString	*stringData;
		CScriptTokenDataFnc		*fncData;
	};
};

// ----------------------------------------------------------------------------------- CSCRIPTTOKENIZER
/*
	the tokenizer converts the code in an Vector with Tokens
*/
class CScriptTokenizer
{
public:
	struct ScriptTokenPosition {
		ScriptTokenPosition(TOKEN_VECT *Tokens) : tokens(Tokens), pos(tokens->begin())/*, currentLine(0)*//*, currentColumn(0)*/ {}
		bool operator ==(const ScriptTokenPosition &eq) { return pos == eq.pos; }
		ScriptTokenPosition &operator =(const ScriptTokenPosition &copy) { 
			tokens=copy.tokens; pos=copy.pos; 
//			currentLine=copy.currentLine; 
			return *this;
		}
		TOKEN_VECT *tokens;
		TOKEN_VECT::iterator pos;
		int currentLine()		{ return pos->line; }
		int currentColumn()	{ return pos->column; }
	};
	CScriptTokenizer();
	CScriptTokenizer(CScriptLex &Lexer);
	CScriptTokenizer(const char *Code, const std::string &File="", int Line=0, int Column=0);
	void tokenizeCode(CScriptLex &Lexer);

	CScriptToken &getToken() { return *(tokenScopeStack.back().pos); }
	void getNextToken();
	bool check(int ExpectedToken);
	void match(int ExpectedToken);
	void pushTokenScope(TOKEN_VECT &Tokens);
	ScriptTokenPosition &getPos() { return tokenScopeStack.back(); }
	void setPos(ScriptTokenPosition &TokenPos);
	ScriptTokenPosition &getPrevPos() { return prevPos; }
	void skip(int Tokens);
	int tk; // current Token
	std::string currentFile;
	int currentLine() { return getPos().currentLine();}
	int currentColumn() { return getPos().currentColumn();}
	std::string &tkStr() { return getToken().String(); }
private:
	void tokenizeCatch(TOKEN_VECT &Tokens, bool &Statement, std::vector<int> &BlockStart, std::vector<int> &Marks);
	void tokenizeTry(TOKEN_VECT &Tokens, bool &Statement, std::vector<int> &BlockStart, std::vector<int> &Marks);
	void tokenizeSwitch(TOKEN_VECT &Tokens, bool &Statement, std::vector<int> &BlockStart, std::vector<int> &Marks);
	void tokenizeWith(TOKEN_VECT &Tokens, bool &Statement, std::vector<int> &BlockStart, std::vector<int> &Marks);
	void tokenizeWhile(TOKEN_VECT &Tokens, bool &Statement, std::vector<int> &BlockStart, std::vector<int> &Marks);
	void tokenizeDo(TOKEN_VECT &Tokens, bool &Statement, std::vector<int> &BlockStart, std::vector<int> &Marks);
	void tokenizeIf(TOKEN_VECT &Tokens, bool &Statement, std::vector<int> &BlockStart, std::vector<int> &Marks);
	void tokenizeForIn(TOKEN_VECT &Tokens, bool &Statement, std::vector<int> &BlockStart, std::vector<int> &Marks);
	void tokenizeFor(TOKEN_VECT &Tokens, bool &Statement, std::vector<int> &BlockStart, std::vector<int> &Marks);
	void tokenizeFunction(TOKEN_VECT &Tokens, bool &Statement, std::vector<int> &BlockStart, std::vector<int> &Marks);
	void tokenizeBlock(TOKEN_VECT &Tokens, bool &Statement, std::vector<int> &BlockStart, std::vector<int> &Marks);
	void tokenizeLet(TOKEN_VECT &Tokens, bool &Statement, std::vector<int> &BlockStart, std::vector<int> &Marks);
	void tokenizeVar(TOKEN_VECT &Tokens, bool &Statement, std::vector<int> &BlockStart, std::vector<int> &Marks);
	void tokenizeStatement(TOKEN_VECT &Tokens, bool &Statement, std::vector<int> &BlockStart, std::vector<int> &Marks);
	void tokenizeToken(TOKEN_VECT &Tokens, bool &Statement, std::vector<int> &BlockStart, std::vector<int> &Marks);
	int pushToken(TOKEN_VECT &Tokens, int Match=-1);
	void throwTokenNotExpected();
	CScriptLex *l;
	TOKEN_VECT tokens;
	ScriptTokenPosition prevPos;
	std::vector<ScriptTokenPosition> tokenScopeStack;
};


////////////////////////////////////////////////////////////////////////// 
/// forward-declaration
//////////////////////////////////////////////////////////////////////////

class CScriptVar;
class CScriptVarPtr;
template<typename C> class CScriptVarPointer;
class CScriptVarLink;
class CScriptVarLinkPtr;

class CScriptVarScopeFnc;
typedef CScriptVarPointer<CScriptVarScopeFnc> CFunctionsScopePtr;
typedef void (*JSCallback)(const CFunctionsScopePtr &var, void *userdata);

class CTinyJS;



//////////////////////////////////////////////////////////////////////////
/// CScriptVar
//////////////////////////////////////////////////////////////////////////

typedef	std::vector<CScriptVarLink*> SCRIPTVAR_CHILDS_t;
typedef	SCRIPTVAR_CHILDS_t::iterator SCRIPTVAR_CHILDS_it;
typedef	SCRIPTVAR_CHILDS_t::const_iterator SCRIPTVAR_CHILDS_cit;

class CScriptVar : public fixed_size_object<CScriptVar> {
protected:
	CScriptVar(CTinyJS *Context, const CScriptVarPtr &Prototype); ///< Create
	CScriptVar(const CScriptVar &Copy); ///< Copy protected -> use clone for public
private:
	CScriptVar & operator=(const CScriptVar &Copy); ///< private -> no assignment-Copy
public:
	virtual ~CScriptVar(void);
	virtual CScriptVarPtr clone()=0;

	/// Type
	virtual bool isObject();	///< is CScriptVarObject
	virtual bool isArray();		///< is CScriptVarArray
	virtual bool isAccessor();	///< is CScriptVarAccessor
	virtual bool isNull();		///< is CScriptVarNull
	virtual bool isUndefined();///< is CScriptVarUndefined
	virtual bool isNaN();		///< is CScriptVarNaN
	virtual bool isString();	///< is CScriptVarString
	virtual bool isInt();		///< is CScriptVarInteger
	virtual bool isBool();		///< is CScriptVarBool
	virtual int isInfinity();	//< is CScriptVarInfinity ///< +1==POSITIVE_INFINITY, -1==NEGATIVE_INFINITY, 0==is not an InfinityVar
	virtual bool isDouble();	///< is CScriptVarDouble

	virtual bool isNumber();	///< is CScriptVarInteger / CScriptVarDouble
	virtual bool isNumeric();	///< is CScriptVarNull
	virtual bool isPrimitive();///< isNull | isUndefined | isNaN | isString | isInt | isDouble | isInfinity

	virtual bool isFunction();	///< is CScriptVarFunction / CScriptVarFunctionNativeCallback / CScriptVarFunctionNativeClass
	virtual bool isNative();	///< is CScriptVarFunctionNativeCallback / CScriptVarFunctionNativeClass

	bool isBasic() { return Childs.empty(); } ///< Is this *not* an array/object/etc


	/// Value
	virtual int getInt(); ///< return 0
	virtual bool getBool(); ///< return false
	virtual double getDouble(); ///< return 0.0
	virtual CScriptTokenDataFnc *getFunctionData(); ///< { return 0; }
	virtual std::string getString(); ///< return ""

	virtual std::string getParsableString(const std::string &indentString, const std::string &indent); ///< get Data as a parsable javascript string
	virtual std::string getVarType()=0;
	CScriptVarPtr getPrimitivVar(); ///< if the var an ObjectType gets the valueOf; if valueOf of an ObjectType gets toString / otherwise gets the Var itself 
	CScriptVarPtr getPrimitivVar(bool execute); ///< if the var an ObjectType gets the valueOf; if valueOf of an ObjectType gets toString / otherwise gets the Var itself 
	virtual CScriptVarPtr getNumericVar(); ///< returns an Integer, a Double, an Infinity or a NaN
	CScriptVarPtr valueOf(bool execute);
	virtual CScriptVarPtr _valueOf(bool execute);
	CScriptVarPtr toString(bool execute, int radix=0);
	virtual CScriptVarPtr _toString(bool execute, int radix=0);
	/// find 
	CScriptVarLink *findChild(const std::string &childName); ///< Tries to find a child with the given name, may return 0
	CScriptVarLink *findChildInPrototypeChain(const std::string &childName);
	CScriptVarLink *findChildWithPrototypeChain(const std::string &childName);
	CScriptVarLink *findChildByPath(const std::string &path); ///< Tries to find a child with the given path (separated by dots)
	CScriptVarLink *findChildOrCreate(const std::string &childName/*, int varFlags=SCRIPTVAR_UNDEFINED*/); ///< Tries to find a child with the given name, or will create it with the given flags
	CScriptVarLink *findChildOrCreateByPath(const std::string &path); ///< Tries to find a child with the given path (separated by dots)

	/// add & remove
	CScriptVarLink *addChild(const std::string &childName, const CScriptVarPtr &child, int linkFlags = SCRIPTVARLINK_DEFAULT);
	CScriptVarLink *addChildNoDup(const std::string &childName, const CScriptVarPtr &child, int linkFlags = SCRIPTVARLINK_DEFAULT); ///< add a child overwriting any with the same name
	bool removeLink(CScriptVarLinkPtr &link); ///< Remove a specific link (this is faster than finding via a child)
	bool removeLink(CScriptVarLink *&link); ///< Remove a specific link (this is faster than finding via a child)
	void removeAllChildren();

	/// ARRAY
	CScriptVarPtr getArrayIndex(int idx); ///< The the value at an array index
	void setArrayIndex(int idx, const CScriptVarPtr &value); ///< Set the value at an array index
	int getArrayLength(); ///< If this is an array, return the number of items in it (else 0)
	
	//////////////////////////////////////////////////////////////////////////
	int getChildren() { return Childs.size(); } ///< Get the number of children
	CTinyJS *getContext() { return context; }
	CScriptVarPtr mathsOp(const CScriptVarPtr &b, int op); ///< do a maths op with another script variable

	void trace(const std::string &name = ""); ///< Dump out the contents of this using trace
	void trace(std::string &indentStr, uint32_t uniqueID, const std::string &name = ""); ///< Dump out the contents of this using trace
	std::string getFlagsAsString(); ///< For debugging - just dump a string version of the flags
//	void getJSON(std::ostringstream &destination, const std::string linePrefix=""); ///< Write out all the JS code needed to recreate this script variable to the stream (as JSON)

	SCRIPTVAR_CHILDS_t Childs;

	/// For memory management/garbage collection
private:
	CScriptVar *ref(); ///< Add reference to this variable
	void unref(); ///< Remove a reference, and delete this variable if required
public:
	int getRefs(); ///< Get the number of references to this script variable
	template<class T>
	operator T *(){ T* ret = dynamic_cast<T*>(this); ASSERT(ret!=0); return ret; }
	template<class T>
	T *get(){ T* ret = dynamic_cast<T*>(this); ASSERT(ret!=0); return ret; }

	template<typename T>	CScriptVarPtr newScriptVar(T t); // { return ::newScriptVar(context, t); }
	template<typename T>	const CScriptVarPtr &constScriptVar(T t); // { return ::newScriptVar(context, t); }
	void setTemporaryID(uint32_t ID) { temporaryID = ID; }
	virtual void setTemporaryID_recursive(uint32_t ID);
	uint32_t getTempraryID() { return temporaryID; }
protected:
	CTinyJS *context;
	int refs; ///< The number of references held to this - used for garbage collection
	CScriptVar *prev;
public:
	CScriptVar *next;
	uint32_t temporaryID;

	friend class CScriptVarPtr;
};

//////////////////////////////////////////////////////////////////////////
/// CScriptVarPrimitive
//////////////////////////////////////////////////////////////////////////

class CScriptVarPrimitive : public CScriptVar {
protected:
	CScriptVarPrimitive(CTinyJS *Context, const CScriptVarPtr &Prototype) : CScriptVar(Context, Prototype) {}
	CScriptVarPrimitive(const CScriptVarPrimitive &Copy) : CScriptVar(Copy) {}
public:

};
////////////////////////////////////////////////////////////////////////// 
/// CScriptVarPtr 
//////////////////////////////////////////////////////////////////////////

class CScriptVarPtr { 
public: 
	// construct
	CScriptVarPtr() : var(0) {} ///< 0-Pointer 
	CScriptVarPtr(CScriptVar *Var) : var(Var) { if(var) var->ref(); } // creates a new CScriptVar (from new);


	CScriptVarPtr(const CScriptVarLink *Link);
//	CScriptVarPtr(bool &execute, const CScriptVarLink *Link);
	CScriptVarPtr(const CScriptVarLinkPtr &Link);

	// from CScriptVarLinkPtr
//	CScriptVarPtr(bool &execute, const CScriptVarLinkPtr &Link);// : var(0) { *this = Link.get(execute); } 
//	CScriptVarPtr(const CScriptVarLinkPtr &Link);// : var(0) { *this = Link.get(); } 
//	CScriptVarPtr & operator =(const CScriptVarLinkPtr &Link); // { *this = 10; } 
public:
	//	CScriptVarPtr & operator =(const CScriptVarLinkPtr &Link, bool &execute) : { 
	//		*this = Link.get(execute)); 
	//	} 

	// copy
	CScriptVarPtr(const CScriptVarPtr &Copy) : var(0) { *this = Copy; } 
	CScriptVarPtr& operator=(const CScriptVarPtr &Copy) { 
		if(var != Copy.var) { 
			if(var) var->unref(); 
			var = Copy.var; if(var) var->ref(); 
		} 
		return *this; 
	}
	// deconstruct 
	~CScriptVarPtr() { if(var) var->unref(); } 

	operator bool() const { return var!=0; } 

	bool operator ==(const CScriptVarPtr &Other) const { return var == Other.var; } 
	bool operator !=(const CScriptVarPtr &Other) const { return var != Other.var; } 

	CScriptVar * operator ->() const { return var; } 
	CScriptVar *getVar() const { return var; } 

	/// temporary
	//	CScriptVarPtr(CScriptVarLink *Var);

	void clear() { if(var) var->unref(); var=0; }
protected: 
	CScriptVar *var; 
}; 

////////////////////////////////////////////////////////////////////////// 
/// CScriptVarPointer - template
//////////////////////////////////////////////////////////////////////////

template<typename C> 
class CScriptVarPointer : public CScriptVarPtr { 
public:
	CScriptVarPointer() {}
	CScriptVarPointer(const CScriptVarPtr &Copy) : CScriptVarPtr(Copy) { if(var) { var = dynamic_cast<C*>(var); } }
//	CScriptVarPointer(const CScriptVarPointer &Copy) : CScriptVarPtr(Copy) { if(var) { var = dynamic_cast<C*>(var); } }
	C * operator ->() const { C *Var = dynamic_cast<C*>(var); ASSERT(var && Var); return Var; }
};

//////////////////////////////////////////////////////////////////////////
/// CScriptVarLink
//////////////////////////////////////////////////////////////////////////


class CScriptVarLink : public fixed_size_object<CScriptVarLink>
{
public:
	CScriptVarLink(const CScriptVarPtr &var, const std::string &name = TINYJS_TEMP_NAME, int flags = SCRIPTVARLINK_DEFAULT);
	CScriptVarLink(const CScriptVarLink &link); ///< Copy constructor
	~CScriptVarLink();

	const std::string &getName() const { return name; }

	int getFlags() { return flags; }

	const CScriptVarPtr &getVarPtr() const { return var; }
	CScriptVarPtr &setVarPtr(const CScriptVarPtr &Var) { return var = Var; }

	void replaceWith(const CScriptVarPtr &newVar); ///< simple Replace the Variable pointed to

	bool isOwned() const { return (flags & SCRIPTVARLINK_OWNED) != 0; }
	void setOwned(bool On) { On ? (flags |= SCRIPTVARLINK_OWNED) : (flags &= ~SCRIPTVARLINK_OWNED); }

	bool isOwner() const { return owner!=0; }

	bool isWritable() const { return (flags & SCRIPTVARLINK_WRITABLE) != 0; }
	void setWritable(bool On) { On ? (flags |= SCRIPTVARLINK_WRITABLE) : (flags &= ~SCRIPTVARLINK_DELETABLE); }
	bool isDeletable() const { return (flags & SCRIPTVARLINK_DELETABLE) != 0; }
	void setDeletable(bool On) { On ? (flags |= SCRIPTVARLINK_DELETABLE) : (flags &= ~SCRIPTVARLINK_DELETABLE); }
	bool isEnumerable() const { return (flags & SCRIPTVARLINK_ENUMERABLE) != 0; }
	void setEnumerable(bool On) { On ? (flags |= SCRIPTVARLINK_ENUMERABLE) : (flags &= ~SCRIPTVARLINK_ENUMERABLE); }
	bool isHidden() const { return (flags & SCRIPTVARLINK_HIDDEN) != 0; }
	void setHidden(bool On) { On ? (flags |= SCRIPTVARLINK_HIDDEN) : (flags &= ~SCRIPTVARLINK_HIDDEN); }

	const CScriptVarPtr &operator ->() const { return var; }

	// TODO remove this (make's saver)
	CScriptVar *getOwner() { return owner; };
	void setOwner(CScriptVar *Owner) { owner = Owner; }
protected:
	std::string name;
	CScriptVar *owner; // pointer to the owner CScriptVar
	uint32_t flags;
	CScriptVarPtr var;
	char dummy[24];
	friend class CScriptVarLinkPtr;
};

// to create a CScriptVarPtr always call getValue to call getter if's needed
inline CScriptVarPtr::CScriptVarPtr(const CScriptVarLink *Link) : var(0) { if(Link) { var = Link->getVarPtr()->ref(); } }


//////////////////////////////////////////////////////////////////////////
/// CScriptVarLinkTmp / CScriptVarLinkTmpPtr
//////////////////////////////////////////////////////////////////////////
class CScriptVarLinkTmp : public CScriptVarLink{
private:
	CScriptVarLinkTmp(const CScriptVarPtr &var, const std::string &name = TINYJS_TEMP_NAME, int flags = SCRIPTVARLINK_DEFAULT) : CScriptVarLink(var, name, flags) {
		refs=1;
	}
	int refs;
	CScriptVarLinkTmp *ref() { refs++; return this;}
	void unref() { if(--refs == 0) delete this; }
	friend class CScriptVarLinkTmpPtr;
};
class CScriptVarLinkTmpPtr{
public:
	CScriptVarLinkTmpPtr() : link(0) {}
	CScriptVarLinkTmpPtr(const CScriptVarPtr &var, const std::string &name = TINYJS_TEMP_NAME, int flags = SCRIPTVARLINK_DEFAULT) {
		link = new CScriptVarLinkTmp(var, name, flags);
	}
	CScriptVarLinkTmpPtr(const CScriptVarLinkTmpPtr &Copy) : link(Copy.link) { if(link) link->ref(); }
	CScriptVarLinkTmpPtr &operator=(const CScriptVarLinkTmpPtr &Copy) { 
		if(link!=Copy.link) { if(link) link->unref(); link=Copy.link; if(link) link->ref(); } return *this; }
	~CScriptVarLinkTmpPtr() { if(link) link->unref(); }
	operator bool() const { return link != 0; }
	int refs() { return link ? link->refs : 0; }
	CScriptVarLink *operator->() const { return link; }
	operator CScriptVarLink *() const { return link; }

private:
	CScriptVarLinkTmp *link;
};




//////////////////////////////////////////////////////////////////////////
/// CScriptVarSmartLink
//////////////////////////////////////////////////////////////////////////

class CScriptVarLinkPtr : public fixed_size_object<CScriptVarLinkPtr>
{
public:
	// constructors
	CScriptVarLinkPtr() : link(0) {}
	CScriptVarLinkPtr(CScriptVarLink *Link) : link(0) { *this = Link; }
	CScriptVarLinkPtr (CScriptVarPtr Var, const std::string &Name = TINYJS_TEMP_NAME, int Flags = SCRIPTVARLINK_DEFAULT) : link(0),tmp_link(Var, Name, Flags) { }

	// copy
	CScriptVarLinkPtr (const CScriptVarLinkPtr &Link)  : link(0) { *this = Link; }
	CScriptVarLinkPtr &operator = (const CScriptVarLinkPtr &Link) { link = Link.link; tmp_link = Link.tmp_link; return *this; }

	// assign
	CScriptVarLinkPtr &operator = (CScriptVarLink *Link) { return (*this)(Link); }
	CScriptVarLinkPtr &operator = (const CScriptVarPtr &Var) { link = 0; set_tmp_link(Var); return *this; }
	CScriptVarLinkPtr &operator()(CScriptVarLink *Link);
	CScriptVarLinkPtr &operator()(bool &execute, CScriptVarLink *Link);
	CScriptVarLinkPtr &operator()(const CScriptVarPtr &Var, const std::string &Name = TINYJS_TEMP_NAME, int Flags = SCRIPTVARLINK_DEFAULT) { link = 0; set_tmp_link(Var, Name, Flags); return *this; }


//	void setLink(CScriptVarLink *Link);
//	void setLink(bool &execute, CScriptVarLink *Link);

//	void setVarPtr(const CScriptVarPtr &Var, const std::string &Name = TINYJS_TEMP_NAME, int Flags = SCRIPTVARLINK_DEFAULT) { link = 0; set_tmp_link(Var, Name, Flags); }
	// operator
	CScriptVarLink *operator ->() { return getLink(); }
	const CScriptVarLink *operator ->() const { return getLink(); }
	CScriptVarLink &operator *() { return *getLink(); }

	void replaceVar(bool &execute, const CScriptVarPtr &Var);

	operator bool() const { return getLink()!=0; }
	//	operator CScriptVarLink *() { return link; }
	CScriptVarLink *getLink() { return (tmp_link && tmp_link->var)?tmp_link:link; }; 
	CScriptVarLink *getRealLink() { return link; }; 
	const CScriptVarLink *getLink() const { return (tmp_link && tmp_link->var)?tmp_link:link; }; 
	void clear() { link=0; clear_tmp_link(); }
	void swap(CScriptVarLinkPtr &Link);
private:
	void set_tmp_link(const CScriptVarPtr &Var, const std::string &Name = TINYJS_TEMP_NAME, int Flags = SCRIPTVARLINK_DEFAULT);
	void clear_tmp_link();

	CScriptVarLink *link;
	CScriptVarLinkTmpPtr tmp_link;
};
inline CScriptVarPtr::CScriptVarPtr(const CScriptVarLinkPtr &Link) : var(0) { if(Link) { var = Link->getVarPtr()->ref(); } }

//////////////////////////////////////////////////////////////////////////
#define define_dummy_t(t1) struct t1##_t{}; extern t1##_t t1
#define declare_dummy_t(t1) t1##_t t1
#define define_newScriptVar_Fnc(t1, ...) CScriptVarPtr newScriptVar(__VA_ARGS__)
#define define_newScriptVar_NamedFnc(t1, ...) CScriptVarPtr newScriptVar##t1(__VA_ARGS__)
#define define_ScriptVarPtr_Type(t1) class CScriptVar##t1; typedef CScriptVarPointer<CScriptVar##t1> CScriptVar##t1##Ptr

typedef struct{}ct_assert_t;
static ct_assert_t ct_assert;
template<typename T> CScriptVarPtr newScriptVar(CTinyJS*, T, ct_assert_t);

////////////////////////////////////////////////////////////////////////// 
/// CScriptVarObject
//////////////////////////////////////////////////////////////////////////

define_dummy_t(Object);
define_ScriptVarPtr_Type(Object);

class CScriptVarObject : public CScriptVar {
protected:
	CScriptVarObject(CTinyJS *Context);
	CScriptVarObject(CTinyJS *Context, const CScriptVarPtr &Prototype) : CScriptVar(Context, Prototype) {}
	CScriptVarObject(const CScriptVarObject &Copy) : CScriptVar(Copy) {} ///< Copy protected -> use clone for public
public:
	virtual ~CScriptVarObject();
	virtual CScriptVarPtr clone();

	virtual bool isObject(); // { return true; }
	virtual bool isPrimitive(); // { return false; } 

	virtual int getInt();
	virtual bool getBool();
	virtual double getDouble();
	virtual std::string getString(); // { return "[ Object ]"; };
	virtual std::string getParsableString(const std::string &indentString, const std::string &indent);
	virtual std::string getVarType(); // { return "object"; }

	virtual CScriptVarPtr _toString(bool execute, int radix=0);
private:
	friend define_newScriptVar_Fnc(Object, CTinyJS* Context, Object_t);
	friend define_newScriptVar_Fnc(Object, CTinyJS* Context, Object_t, const CScriptVarPtr &);
};
inline define_newScriptVar_Fnc(Object, CTinyJS* Context, Object_t) { return new CScriptVarObject(Context); }
inline define_newScriptVar_Fnc(Object, CTinyJS* Context, Object_t, const CScriptVarPtr &Prototype) { return new CScriptVarObject(Context, Prototype); }


////////////////////////////////////////////////////////////////////////// 
/// CScriptVarObjectWrap
////////////////////////////////////////////////////////////////////////// 

define_dummy_t(ObjectWrap);
define_ScriptVarPtr_Type(ObjectWrap);

class CScriptVarObjectWrap : public CScriptVarObject {
protected:
	CScriptVarObjectWrap(CTinyJS *Context, const CScriptVarPtr &Value);// : CScriptVarObject(Context), value(Value) {}
	CScriptVarObjectWrap(const CScriptVarObjectWrap &Copy) : CScriptVarObject(Copy), value(Copy.value) {} ///< Copy protected -> use clone for public
public:
	virtual ~CScriptVarObjectWrap();
	virtual CScriptVarPtr clone();

	virtual std::string getParsableString(const std::string &indentString, const std::string &indent); ///< get Data as a parsable javascript string

	virtual CScriptVarPtr _valueOf(bool execute);
	virtual CScriptVarPtr _toString(bool execute, int radix=0);
	virtual void setTemporaryID_recursive(uint32_t ID);
private:
	CScriptVarPtr value;
	friend define_newScriptVar_Fnc(ObjectWrap, CTinyJS* Context, ObjectWrap_t, const CScriptVarPtr);
};
inline define_newScriptVar_Fnc(ObjectWrap, CTinyJS* Context, ObjectWrap_t, const CScriptVarPtr Value) { return new CScriptVarObjectWrap(Context, Value); }

////////////////////////////////////////////////////////////////////////// 
/// CScriptVarError
////////////////////////////////////////////////////////////////////////// 

define_ScriptVarPtr_Type(Error);

class CScriptVarError : public CScriptVarObject {
protected:
	CScriptVarError(CTinyJS *Context, ERROR_TYPES type, const char *message, const char *file, int line, int column);// : CScriptVarObject(Context), value(Value) {}
	CScriptVarError(const CScriptVarError &Copy) : CScriptVarObject(Copy) {} ///< Copy protected -> use clone for public
public:
	virtual ~CScriptVarError();
	virtual CScriptVarPtr clone();
	//	virtual int getInt();
	//	virtual bool getBool();
	//	virtual double getDouble();
	//	virtual std::string getString();

//	virtual std::string getParsableString(const std::string &indentString, const std::string &indent); ///< get Data as a parsable javascript string

	virtual CScriptVarPtr _toString(bool execute, int radix=0);
private:
	friend define_newScriptVar_NamedFnc(Error, CTinyJS* Context, ERROR_TYPES type, const char *message, const char *file, int line, int column);
};
inline define_newScriptVar_NamedFnc(Error, CTinyJS* Context, ERROR_TYPES type, const char *message=0, const char *file=0, int line=-1, int column=-1) { return new CScriptVarError(Context, type, message, file, line, column); }


////////////////////////////////////////////////////////////////////////// 
/// CScriptVarAccessor
//////////////////////////////////////////////////////////////////////////

define_dummy_t(Accessor);
define_ScriptVarPtr_Type(Accessor);

class CScriptVarAccessor : public CScriptVar {
protected:
	CScriptVarAccessor(CTinyJS *Context);
	CScriptVarAccessor(const CScriptVarAccessor &Copy) : CScriptVar(Copy) {} ///< Copy protected -> use clone for public
public:
	virtual ~CScriptVarAccessor();
	virtual CScriptVarPtr clone();
	virtual bool isAccessor(); // { return true; }
	virtual bool isPrimitive(); // { return false; } 

	virtual std::string getString(); // { return "[ Object ]"; };
	virtual std::string getParsableString(const std::string &indentString, const std::string &indent);
	virtual std::string getVarType(); // { return "object"; }

	CScriptVarPtr getValue();

	friend define_newScriptVar_Fnc(Accessor, CTinyJS* Context, Accessor_t);

};
inline define_newScriptVar_Fnc(Accessor, CTinyJS* Context, Accessor_t) { return new CScriptVarAccessor(Context); }


////////////////////////////////////////////////////////////////////////// 
/// CScriptVarArray
//////////////////////////////////////////////////////////////////////////

define_dummy_t(Array);
define_ScriptVarPtr_Type(Array);
class CScriptVarArray : public CScriptVar {
protected:
	CScriptVarArray(CTinyJS *Context);
	CScriptVarArray(const CScriptVarArray &Copy) : CScriptVar(Copy) {} ///< Copy protected -> use clone for public
public:
	virtual ~CScriptVarArray();
	virtual CScriptVarPtr clone();
	virtual bool isArray(); // { return true; }
	virtual bool isPrimitive(); // { return false; } 

	virtual std::string getString();
	virtual std::string getVarType(); // { return "object"; }
	virtual std::string getParsableString(const std::string &indentString, const std::string &indent);
	friend define_newScriptVar_Fnc(Array, CTinyJS* Context, Array_t);
private:
	void native_Length(const CFunctionsScopePtr &c, void *data);
};
inline define_newScriptVar_Fnc(Array, CTinyJS* Context, Array_t) { return new CScriptVarArray(Context); } 


////////////////////////////////////////////////////////////////////////// 
/// CScriptVarNull
//////////////////////////////////////////////////////////////////////////

define_dummy_t(Null);
define_ScriptVarPtr_Type(Null);
class CScriptVarNull : public CScriptVar {
protected:
	CScriptVarNull(CTinyJS *Context);
	CScriptVarNull(const CScriptVarNull &Copy) : CScriptVar(Copy) {} ///< Copy protected -> use clone for public
public:
	virtual ~CScriptVarNull();
	virtual CScriptVarPtr clone();
	virtual bool isNull(); // { return true; }
	virtual bool isNumeric(); // { return true; }
	virtual std::string getString(); // { return "null"; };
	virtual std::string getVarType(); // { return "null"; }
	virtual CScriptVarPtr getNumericVar(); ///< returns an Integer, a Double, an Infinity or a NaN
	friend define_newScriptVar_Fnc(Null, CTinyJS* Context, Null_t);
};
inline define_newScriptVar_Fnc(Null, CTinyJS* Context, Null_t) { return new CScriptVarNull(Context); }


////////////////////////////////////////////////////////////////////////// 
/// CScriptVarUndefined
//////////////////////////////////////////////////////////////////////////

define_dummy_t(Undefined);
define_ScriptVarPtr_Type(Undefined);
class CScriptVarUndefined : public CScriptVar {
protected:
	CScriptVarUndefined(CTinyJS *Context);
	CScriptVarUndefined(const CScriptVarUndefined &Copy) : CScriptVar(Copy) {} ///< Copy protected -> use clone for public
public:
	virtual ~CScriptVarUndefined();
	virtual CScriptVarPtr clone();
	virtual bool isUndefined(); // { return true; }
	virtual bool isNumeric(); // { return true; }
	virtual std::string getString(); // { return "undefined"; };
	virtual std::string getVarType(); // { return "undefined"; }
//	friend define_newScriptVar_Fnc(Undefined, CTinyJS* Context, Undefined_t);
	friend define_newScriptVar_NamedFnc(Undefined, CTinyJS* Context);
};
//inline define_newScriptVar_Fnc(Undefined, CTinyJS* Context, Undefined_t) { return new CScriptVarUndefined(Context); }
inline define_newScriptVar_NamedFnc(Undefined, CTinyJS* Context) { return new CScriptVarUndefined(Context); }


////////////////////////////////////////////////////////////////////////// 
/// CScriptVarNaN
//////////////////////////////////////////////////////////////////////////

define_dummy_t(NaN);
define_ScriptVarPtr_Type(NaN);
class CScriptVarNaN : public CScriptVar {
protected:
	CScriptVarNaN(CTinyJS *Context);
	CScriptVarNaN(const CScriptVarNaN &Copy) : CScriptVar(Copy) {} ///< Copy protected -> use clone for public
public:
	virtual ~CScriptVarNaN();
	virtual CScriptVarPtr clone();
	virtual bool isNaN();// { return true; }
	virtual bool isNumeric(); // { return true; }
	virtual std::string getString(); // { return "NaN"; };
	virtual std::string getVarType(); // { return "number"; }
	friend define_newScriptVar_NamedFnc(NaN, CTinyJS* Context);
};
inline define_newScriptVar_NamedFnc(NaN, CTinyJS* Context) { return new CScriptVarNaN(Context); }


////////////////////////////////////////////////////////////////////////// 
/// CScriptVarString
//////////////////////////////////////////////////////////////////////////

define_ScriptVarPtr_Type(String);
class CScriptVarString : public CScriptVar {
protected:
	CScriptVarString(CTinyJS *Context, const std::string &Data);
	CScriptVarString(const CScriptVarString &Copy) : CScriptVar(Copy), data(Copy.data) {} ///< Copy protected -> use clone for public
public:
	virtual ~CScriptVarString();
	virtual CScriptVarPtr clone();
	virtual bool isString(); // { return true; }
	virtual int getInt(); // {return strtol(data.c_str(),0,0); }
	virtual bool getBool(); // {return data.length()!=0;}
	virtual double getDouble(); // {return strtod(data.c_str(),0);}
	virtual std::string getString(); // { return data; }
	virtual std::string getParsableString(); // { return getJSString(data); }
	virtual std::string getVarType(); // { return "string"; }
	virtual CScriptVarPtr getNumericVar(); ///< returns an Integer, a Double, an Infinity or a NaN
protected:
	std::string data;
private:
	void native_Length(const CFunctionsScopePtr &c, void *data);

	friend define_newScriptVar_Fnc(String, CTinyJS* Context, const std::string &);
	friend define_newScriptVar_Fnc(String, CTinyJS* Context, const char *);
	friend define_newScriptVar_Fnc(String, CTinyJS* Context, char *);
};
inline define_newScriptVar_Fnc(String, CTinyJS* Context, const std::string &Obj) { return new CScriptVarString(Context, Obj); }
inline define_newScriptVar_Fnc(String, CTinyJS* Context, const char *Obj) { return new CScriptVarString(Context, Obj); }
inline define_newScriptVar_Fnc(String, CTinyJS* Context, char *Obj) { return new CScriptVarString(Context, Obj); }


////////////////////////////////////////////////////////////////////////// 
/// CScriptVarIntegerBase
//////////////////////////////////////////////////////////////////////////

class CScriptVarIntegerBase : public CScriptVar {
protected:
	CScriptVarIntegerBase(CTinyJS *Context, const CScriptVarPtr &Prototype, int Data);
	CScriptVarIntegerBase(const CScriptVarIntegerBase &Copy) : CScriptVar(Copy), data(Copy.data) {} ///< Copy protected -> use clone for public
public:
	virtual ~CScriptVarIntegerBase();
	virtual bool isNumeric(); // { return true; }
	virtual int getInt(); // {return data; }
	virtual bool getBool(); // {return data!=0;}
	virtual double getDouble(); // {return data;}
	virtual std::string getString(); // {return int2string(data);}
	virtual std::string getVarType(); // { return "number"; }
	virtual CScriptVarPtr getNumericVar(); ///< returns an Integer, a Double, an Infinity or a NaN
protected:
	int data;
};


////////////////////////////////////////////////////////////////////////// 
/// CScriptVarInteger
//////////////////////////////////////////////////////////////////////////

define_dummy_t(Zero);
define_dummy_t(One);
define_ScriptVarPtr_Type(Integer);
class CScriptVarInteger : public CScriptVarIntegerBase {
protected:
	CScriptVarInteger(CTinyJS *Context, int Data);
	CScriptVarInteger(const CScriptVarInteger &Copy) : CScriptVarIntegerBase(Copy) {} ///< Copy protected -> use clone for public
public:
	virtual ~CScriptVarInteger();
	virtual CScriptVarPtr clone();
	virtual bool isNumber(); // { return true; }
	virtual bool isInt(); // { return true; }
	virtual CScriptVarPtr _toString(bool execute, int radix=0);
	friend define_newScriptVar_Fnc(Integer, CTinyJS* Context, int);
	friend define_newScriptVar_Fnc(Integer, CTinyJS* Context, char);
};
inline define_newScriptVar_Fnc(Integer, CTinyJS* Context, int Obj) { return new CScriptVarInteger(Context, Obj); }
inline define_newScriptVar_Fnc(Integer, CTinyJS* Context, char Obj) { return new CScriptVarInteger(Context, Obj); }


////////////////////////////////////////////////////////////////////////// 
/// CScriptVarBool
//////////////////////////////////////////////////////////////////////////

define_ScriptVarPtr_Type(Bool);
class CScriptVarBool : public CScriptVarIntegerBase {
protected:
	CScriptVarBool(CTinyJS *Context, bool Data);
	CScriptVarBool(const CScriptVarBool &Copy) : CScriptVarIntegerBase(Copy) {} ///< Copy protected -> use clone for public
public:
	virtual ~CScriptVarBool();
	virtual CScriptVarPtr clone();
	virtual bool isBool(); // { return true; }
	virtual std::string getString(); // {return data!=0?"true":"false";}
	virtual std::string getVarType(); // { return "boolean"; }
	virtual CScriptVarPtr getNumericVar(); ///< returns an Integer, a Double, an Infinity or a NaN
//	friend define_newScriptVar_Fnc(Bool, CTinyJS* Context, bool);
	friend define_newScriptVar_NamedFnc(Bool, CTinyJS* Context, bool);
};
/*
inline define_newScriptVar_Fnc(Bool, CTinyJS* Context, bool Obj) { 
	//return new CScriptVarBool(Context, Obj); 
	i_fnc_t(true);
	return CScriptVarPtr();
}
*/
//inline void newCScriptVar(CTinyJS* Context, bool Obj) {}
inline define_newScriptVar_NamedFnc(Bool, CTinyJS* Context, bool Obj) { return new CScriptVarBool(Context, Obj); }


////////////////////////////////////////////////////////////////////////// 
/// CScriptVarInfinity
//////////////////////////////////////////////////////////////////////////

class Infinity{public:Infinity(int Sig=1):sig(Sig){} int Sig(){return sig;} private:int sig; } ;
extern Infinity InfinityPositive;
extern Infinity InfinityNegative;
define_ScriptVarPtr_Type(Infinity);
class CScriptVarInfinity : public CScriptVarIntegerBase {
protected:
	CScriptVarInfinity(CTinyJS *Context, int Data);
	CScriptVarInfinity(const CScriptVarInfinity &Copy) : CScriptVarIntegerBase(Copy) {} ///< Copy protected -> use clone for public
public:
	virtual ~CScriptVarInfinity();
	virtual CScriptVarPtr clone();
	virtual int isInfinity(); // { return data; }
	virtual std::string getString(); // {return data<0?"-Infinity":"Infinity";}
//	friend define_newScriptVar_Fnc(Infinity, CTinyJS* Context, Infinity);
	friend define_newScriptVar_NamedFnc(Infinity, CTinyJS* Context, Infinity);
};
//inline define_newScriptVar_Fnc(Infinity, CTinyJS* Context, Infinity Obj) { return new CScriptVarInfinity(Context, Obj.sig); } 
inline define_newScriptVar_NamedFnc(Infinity, CTinyJS* Context, Infinity Obj) { return new CScriptVarInfinity(Context, Obj.Sig()); } 


////////////////////////////////////////////////////////////////////////// 
/// CScriptVarDouble
//////////////////////////////////////////////////////////////////////////

define_ScriptVarPtr_Type(Double);
class CScriptVarDouble : public CScriptVar {
protected:
	CScriptVarDouble(CTinyJS *Context, double Data);
	CScriptVarDouble(const CScriptVarDouble &Copy) : CScriptVar(Copy), data(Copy.data) {} ///< Copy protected -> use clone for public
public:
	virtual ~CScriptVarDouble();
	virtual CScriptVarPtr clone();
	virtual bool isDouble(); // { return true; }
	virtual bool isNumber(); // { return true; }
	virtual bool isNumeric(); // { return true; }
	virtual int getInt(); // {return (int)data; }
	virtual bool getBool(); // {return data!=0.0;}
	virtual double getDouble(); // {return data;}
	virtual std::string getString(); // {return float2string(data);}
	virtual std::string getVarType(); // { return "number"; }
	virtual CScriptVarPtr getNumericVar(); ///< returns an Integer, a Double, an Infinity or a NaN
	virtual CScriptVarPtr _toString(bool execute, int radix=0);
private:
	double data;
	friend define_newScriptVar_Fnc(Double, CTinyJS* Context, double);
};
inline define_newScriptVar_Fnc(Double, CTinyJS* Context, double Obj) { return new CScriptVarDouble(Context, Obj); }


////////////////////////////////////////////////////////////////////////// 
/// CScriptVarFunction
//////////////////////////////////////////////////////////////////////////

define_ScriptVarPtr_Type(Function);
class CScriptVarFunction : public CScriptVar {
protected:
	CScriptVarFunction(CTinyJS *Context, CScriptTokenDataFnc *Data);
	CScriptVarFunction(const CScriptVarFunction &Copy) : CScriptVar(Copy), data(Copy.data) { data->ref(); } ///< Copy protected -> use clone for public
public:
	virtual ~CScriptVarFunction();
	virtual CScriptVarPtr clone();
	virtual bool isFunction(); // { return true; }
	virtual bool isPrimitive(); // { return false; } 

	virtual std::string getString(); // {return "[ Function ]";}
	virtual std::string getVarType(); // { return "function"; }
	virtual std::string getParsableString(const std::string &indentString, const std::string &indent);
	virtual CScriptTokenDataFnc *getFunctionData();
	void setFunctionData(CScriptTokenDataFnc *Data);
private:
	CScriptTokenDataFnc *data;
	std::string getParsableBlockString(TOKEN_VECT::iterator &it, TOKEN_VECT::iterator end, const std::string indentString, const std::string indent);


	friend define_newScriptVar_Fnc(Function, CTinyJS* Context, CScriptTokenDataFnc *);
};
inline define_newScriptVar_Fnc(Function, CTinyJS* Context, CScriptTokenDataFnc *Obj) { return new CScriptVarFunction(Context, Obj); }


////////////////////////////////////////////////////////////////////////// 
/// CScriptVarFunctionNative
//////////////////////////////////////////////////////////////////////////

define_ScriptVarPtr_Type(FunctionNative);
class CScriptVarFunctionNative : public CScriptVarFunction {
protected:
	CScriptVarFunctionNative(CTinyJS *Context, void *Userdata) : CScriptVarFunction(Context, 0), jsUserData(Userdata) { }
	CScriptVarFunctionNative(const CScriptVarFunctionNative &Copy) : CScriptVarFunction(Copy), jsUserData(Copy.jsUserData) { } ///< Copy protected -> use clone for public
public:
	virtual ~CScriptVarFunctionNative();
	virtual CScriptVarPtr clone()=0;
	virtual bool isNative(); // { return true; }

	virtual std::string getString(); // {return "[ Function Native ]";}

	virtual void callFunction(const CFunctionsScopePtr &c)=0;// { jsCallback(c, jsCallbackUserData); }
protected:
	void *jsUserData; ///< user data passed as second argument to native functions
};


////////////////////////////////////////////////////////////////////////// 
/// CScriptVarFunctionNativeCallback
//////////////////////////////////////////////////////////////////////////

define_ScriptVarPtr_Type(FunctionNativeCallback);
class CScriptVarFunctionNativeCallback : public CScriptVarFunctionNative {
protected:
	CScriptVarFunctionNativeCallback(CTinyJS *Context, JSCallback Callback, void *Userdata) : CScriptVarFunctionNative(Context, Userdata), jsCallback(Callback) { }
	CScriptVarFunctionNativeCallback(const CScriptVarFunctionNativeCallback &Copy) : CScriptVarFunctionNative(Copy), jsCallback(Copy.jsCallback) { } ///< Copy protected -> use clone for public
public:
	virtual ~CScriptVarFunctionNativeCallback();
	virtual CScriptVarPtr clone();
	virtual void callFunction(const CFunctionsScopePtr &c);
private:
	JSCallback jsCallback; ///< Callback for native functions
	friend define_newScriptVar_Fnc(FunctionNativeCallback, CTinyJS* Context, JSCallback Callback, void*);
};
inline define_newScriptVar_Fnc(FunctionNativeCallback, CTinyJS* Context, JSCallback Callback, void* Userdata) { return new CScriptVarFunctionNativeCallback(Context, Callback, Userdata); }


////////////////////////////////////////////////////////////////////////// 
/// CScriptVarFunctionNativeClass
//////////////////////////////////////////////////////////////////////////

template<class native>
class CScriptVarFunctionNativeClass : public CScriptVarFunctionNative {
protected:
	CScriptVarFunctionNativeClass(CTinyJS *Context, native *ClassPtr, void (native::*ClassFnc)(const CFunctionsScopePtr &, void *), void *Userdata) : CScriptVarFunctionNative(Context, Userdata), classPtr(ClassPtr), classFnc(ClassFnc) { }
	CScriptVarFunctionNativeClass(const CScriptVarFunctionNativeClass &Copy) : CScriptVarFunctionNative(Copy), classPtr(Copy.classPtr), classFnc(Copy.classFnc) { } ///< Copy protected -> use clone for public
public:
	virtual CScriptVarPtr clone() { return new CScriptVarFunctionNativeClass(*this); }

	virtual void callFunction(const CFunctionsScopePtr &c) { (classPtr->*classFnc)(c, jsUserData); }
private:
	native *classPtr;
	void (native::*classFnc)(const CFunctionsScopePtr &c, void *userdata);
	template<typename native2>
	friend define_newScriptVar_Fnc(FunctionNativeCallback, CTinyJS*, native2 *, void (native2::*)(const CFunctionsScopePtr &, void *), void *);
};
template<typename native>
define_newScriptVar_Fnc(FunctionNativeCallback, CTinyJS* Context, native *ClassPtr, void (native::*ClassFnc)(const CFunctionsScopePtr &, void *), void *Userdata) { return new CScriptVarFunctionNativeClass<native>(Context, ClassPtr, ClassFnc, Userdata); }

////////////////////////////////////////////////////////////////////////// 
/// CScriptVarScope
//////////////////////////////////////////////////////////////////////////

define_dummy_t(Scope);
define_ScriptVarPtr_Type(Scope);
class CScriptVarScope : public CScriptVarObject {
protected: // only derived classes or friends can be created
	CScriptVarScope(CTinyJS *Context) // constructor for rootScope
		: CScriptVarObject(Context) {}
	virtual CScriptVarPtr clone();
	virtual bool isObject(); // { return false; }
public:
	virtual ~CScriptVarScope();
	virtual CScriptVarPtr scopeVar(); ///< to create var like: var a = ...
	virtual CScriptVarPtr scopeLet(); ///< to create var like: let a = ...
	virtual CScriptVarLink *findInScopes(const std::string &childName);
	virtual CScriptVarScopePtr getParent();
	friend define_newScriptVar_Fnc(Scope, CTinyJS* Context, Scope_t);
};
inline define_newScriptVar_Fnc(Scope, CTinyJS* Context, Scope_t) { return new CScriptVarScope(Context); }


////////////////////////////////////////////////////////////////////////// 
/// CScriptVarScopeFnc
//////////////////////////////////////////////////////////////////////////

define_dummy_t(ScopeFnc);
define_ScriptVarPtr_Type(ScopeFnc);
class CScriptVarScopeFnc : public CScriptVarScope {
protected: // only derived classes or friends can be created
	CScriptVarScopeFnc(CTinyJS *Context, const CScriptVarScopePtr &Closure) // constructor for FncScope
		: CScriptVarScope(Context), closure(Closure ? addChild(TINYJS_FUNCTION_CLOSURE_VAR, Closure, 0) : 0) {}
public:
	virtual ~CScriptVarScopeFnc();
	virtual CScriptVarLink *findInScopes(const std::string &childName);
	
	void setReturnVar(const CScriptVarPtr &var); ///< Set the result value. Use this when setting complex return data as it avoids a deepCopy()
	
	#define DEPRECATED_getParameter DEPRECATED("getParameter is deprecated use getArgument instead")
	DEPRECATED_getParameter CScriptVarPtr getParameter(const std::string &name); 
	DEPRECATED_getParameter CScriptVarPtr getParameter(int Idx); 
	CScriptVarPtr getArgument(const std::string &name); ///< If this is a function, get the parameter with the given name (for use by native functions)
	CScriptVarPtr getArgument(int Idx); ///< If this is a function, get the parameter with the given index (for use by native functions)
	DEPRECATED("getParameterLength is deprecated use getArgumentsLength instead") int getParameterLength(); ///< If this is a function, get the count of parameters
	int getArgumentsLength(); ///< If this is a function, get the count of parameters
protected:
	CScriptVarLink *closure;
	friend define_newScriptVar_Fnc(ScopeFnc, CTinyJS* Context, ScopeFnc_t, const CScriptVarScopePtr &Closure);
};
inline define_newScriptVar_Fnc(ScopeFnc, CTinyJS* Context, ScopeFnc_t, const CScriptVarScopePtr &Closure) { return new CScriptVarScopeFnc(Context, Closure); }


////////////////////////////////////////////////////////////////////////// 
/// CScriptVarScopeLet
//////////////////////////////////////////////////////////////////////////

define_dummy_t(ScopeLet);
define_ScriptVarPtr_Type(ScopeLet);
class CScriptVarScopeLet : public CScriptVarScope {
protected: // only derived classes or friends can be created
	CScriptVarScopeLet(const CScriptVarScopePtr &Parent); // constructor for LetScope
//		: CScriptVarScope(Parent->getContext()), parent( context->getRoot() != Parent ? addChild(TINYJS_SCOPE_PARENT_VAR, Parent, 0) : 0) {}
public:
	virtual ~CScriptVarScopeLet();
	virtual CScriptVarLink *findInScopes(const std::string &childName);
	virtual CScriptVarPtr scopeVar(); ///< to create var like: var a = ...
	virtual CScriptVarScopePtr getParent();
protected:
	CScriptVarLink *parent;
	friend define_newScriptVar_Fnc(ScopeLet, CTinyJS* Context, ScopeLet_t, const CScriptVarScopePtr &Parent);
};
inline define_newScriptVar_Fnc(ScopeLet, CTinyJS* , ScopeLet_t, const CScriptVarScopePtr &Parent) { return new CScriptVarScopeLet(Parent); }


////////////////////////////////////////////////////////////////////////// 
/// CScriptVarScopeWith
//////////////////////////////////////////////////////////////////////////

define_dummy_t(ScopeWith);
define_ScriptVarPtr_Type(ScopeWith);
class CScriptVarScopeWith : public CScriptVarScopeLet {
protected:
	CScriptVarScopeWith(const CScriptVarScopePtr &Parent, const CScriptVarPtr &With) 
		: CScriptVarScopeLet(Parent), with(addChild(TINYJS_SCOPE_WITH_VAR, With, 0)) {}

public:
	virtual ~CScriptVarScopeWith();
	virtual CScriptVarPtr scopeLet(); ///< to create var like: let a = ...
	virtual CScriptVarLink *findInScopes(const std::string &childName);
private:
	CScriptVarLink *with;
	friend define_newScriptVar_Fnc(ScopeWith, CTinyJS* Context, ScopeWith_t, const CScriptVarScopePtr &Parent, const CScriptVarPtr &With);
};
inline define_newScriptVar_Fnc(ScopeWith, CTinyJS* , ScopeWith_t, const CScriptVarScopePtr &Parent, const CScriptVarPtr &With) { return new CScriptVarScopeWith(Parent, With); }

//////////////////////////////////////////////////////////////////////////
template<typename T>
inline CScriptVarPtr CScriptVar::newScriptVar(T t) { return ::newScriptVar(context, t); }
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
/// CTinyJS
//////////////////////////////////////////////////////////////////////////

class CTinyJS {
public:
	CTinyJS();
	~CTinyJS();

	void execute(CScriptTokenizer &Tokenizer);
	void execute(const char *Code, const std::string &File="", int Line=0, int Column=0);
	void execute(const std::string &Code, const std::string &File="", int Line=0, int Column=0);
	/** Evaluate the given code and return a link to a javascript object,
	 * useful for (dangerous) JSON parsing. If nothing to return, will return
	 * 'undefined' variable type. CScriptVarLink is returned as this will
	 * automatically unref the result as it goes out of scope. If you want to
	 * keep it, you must use ref() and unref() */
	CScriptVarLink evaluateComplex(CScriptTokenizer &Tokenizer);
	/** Evaluate the given code and return a link to a javascript object,
	 * useful for (dangerous) JSON parsing. If nothing to return, will return
	 * 'undefined' variable type. CScriptVarLink is returned as this will
	 * automatically unref the result as it goes out of scope. If you want to
	 * keep it, you must use ref() and unref() */
	CScriptVarLink evaluateComplex(const char *code, const std::string &File="", int Line=0, int Column=0);
	/** Evaluate the given code and return a link to a javascript object,
	 * useful for (dangerous) JSON parsing. If nothing to return, will return
	 * 'undefined' variable type. CScriptVarLink is returned as this will
	 * automatically unref the result as it goes out of scope. If you want to
	 * keep it, you must use ref() and unref() */
	CScriptVarLink evaluateComplex(const std::string &code, const std::string &File="", int Line=0, int Column=0);
	/** Evaluate the given code and return a string. If nothing to return, will return
	 * 'undefined' */
	std::string evaluate(CScriptTokenizer &Tokenizer);
	/** Evaluate the given code and return a string. If nothing to return, will return
	 * 'undefined' */
	std::string evaluate(const char *code, const std::string &File="", int Line=0, int Column=0);
	/** Evaluate the given code and return a string. If nothing to return, will return
	 * 'undefined' */
	std::string evaluate(const std::string &code, const std::string &File="", int Line=0, int Column=0);

	/// add a native function to be called from TinyJS
	/** example:
		\code
			void scRandInt(const CFunctionsScopePtr &c, void *userdata) { ... }
			tinyJS->addNative("function randInt(min, max)", scRandInt, 0);
		\endcode

		or

		\code
			void scSubstring(const CFunctionsScopePtr &c, void *userdata) { ... }
			tinyJS->addNative("function String.substring(lo, hi)", scSubstring, 0);
		\endcode
		or

		\code
			class Class
			{
			public:
				void scSubstring(const CFunctionsScopePtr &c, void *userdata) { ... }
			};
			Class Instanz;
			tinyJS->addNative("function String.substring(lo, hi)", &Instanz, &Class::*scSubstring, 0);
		\endcode
	*/

	CScriptVarFunctionNativePtr addNative(const std::string &funcDesc, JSCallback ptr, void *userdata=0);
	template<class C>
	CScriptVarFunctionNativePtr addNative(const std::string &funcDesc, C *class_ptr, void(C::*class_fnc)(const CFunctionsScopePtr &, void *), void *userdata=0)
	{
		return addNative(funcDesc, ::newScriptVar<C>(this, class_ptr, class_fnc, userdata));
	}

	/// Send all variables to stdout
	void trace();

	const CScriptVarScopePtr &getRoot() { return root; };   /// gets the root of symbol table
	//	CScriptVar *root;   /// root of symbol table

	/// newVars & constVars
	template<typename T>	CScriptVarPtr newScriptVar(T t) { return ::newScriptVar(this, t); }
	template<typename T1, typename T2>	CScriptVarPtr newScriptVar(T1 t1, T2 t2) { return ::newScriptVar(this, t1, t2); }
	const CScriptVarPtr &constScriptVar(Undefined_t)	{ return constUndefined; }
	const CScriptVarPtr &constScriptVar(NaN_t)			{ return constNaN; }
	const CScriptVarPtr &constScriptVar(Infinity t)		{ return t.Sig()<0 ? constInfinityNegative : constInfinityPositive; }
	const CScriptVarPtr &constScriptVar(bool Val)		{ return Val?constTrue:constFalse; }
	const CScriptVarPtr &constScriptVar(Zero_t)			{ return constZero; }
	const CScriptVarPtr &constScriptVar(One_t)			{ return constOne; }


private:
	static bool noexecute;
	CScriptTokenizer *t;       /// current tokenizer
	int runtimeFlags;
	std::vector<std::string> loop_labels;
	std::vector<CScriptVarScopePtr>scopes;
	CScriptVarScopePtr root;

	const CScriptVarScopePtr &scope() { return scopes.back(); }

	class CScopeControl {
	private:
		CScopeControl(const CScopeControl& Copy); // no copy
		CScopeControl& operator =(const CScopeControl& Copy);
	public:
		CScopeControl(CTinyJS* Context) : context(Context), count(0) {} 
		~CScopeControl() { while(count--) {CScriptVarScopePtr parent = context->scopes.back()->getParent(); if(parent) context->scopes.back() = parent; else context->scopes.pop_back() ;} } 
		void addFncScope(const CScriptVarScopePtr &Scope) { context->scopes.push_back(Scope); count++; }
		void addLetScope() {	context->scopes.back() = ::newScriptVar(context, ScopeLet, context->scopes.back()); count++; }
		void addWithScope(const CScriptVarPtr &With) { context->scopes.back() = ::newScriptVar(context, ScopeWith, context->scopes.back(), With); count++; }  
	private:
		CTinyJS *context;
		int		count;
	};
	friend class CScopeControl;
public:
	CScriptVarPtr objectPrototype; /// Built in object class
	CScriptVarPtr objectPrototype_valueOf; /// Built in object class
	CScriptVarPtr objectPrototype_toString; /// Built in object class
	CScriptVarPtr arrayPrototype; /// Built in array class
	CScriptVarPtr stringPrototype; /// Built in string class
	CScriptVarPtr numberPrototype; /// Built in number class
	CScriptVarPtr booleanPrototype; /// Built in boolean class
	CScriptVarPtr functionPrototype; /// Built in function class
	const CScriptVarPtr &getErrorPrototype(ERROR_TYPES Type) { return errorPrototypes[Type]; }
private:
	CScriptVarPtr errorPrototypes[ERROR_COUNT]; /// Built in error class
	CScriptVarPtr constUndefined;
	CScriptVarPtr constNaN;
	CScriptVarPtr constInfinityPositive;
	CScriptVarPtr constInfinityNegative;
	CScriptVarPtr constTrue;
	CScriptVarPtr constFalse;
	CScriptVarPtr constOne;
	CScriptVarPtr constZero;
	std::vector<CScriptVarPtr *> pseudo_refered;
	CScriptVarPtr exceptionVar; /// containing the exception var by (runtimeFlags&RUNTIME_THROW) == true; 

	void CheckRightHandVar(bool &execute, CScriptVarLinkPtr &link)
	{
		if(execute && link && !link->isOwned() && !link->isOwner() && !link->getName().empty())
			throwError(execute, link->getName() + " is not defined", t->getPrevPos());
	}

	void CheckRightHandVar(bool &execute, CScriptVarLinkPtr &link, CScriptTokenizer::ScriptTokenPosition &Pos)
	{
		if(execute && link && !link->isOwned() && !link->isOwner() && !link->getName().empty())
			throwError(execute, link->getName() + " is not defined", Pos);
	}

public:
	// function call
	CScriptVarPtr callFunction(bool &execute, const CScriptVarPtr &Function, std::vector<CScriptVarPtr> &Arguments, const CScriptVarPtr &This, CScriptVarPtr *newThis=0);
	//
	// parsing - in order of precedence

	CScriptVarPtr mathsOp(bool &execute, const CScriptVarPtr &a, const CScriptVarPtr &b, int op);
private:

	CScriptVarLinkPtr execute_literals(bool &execute);
	CScriptVarLinkPtr execute_member(CScriptVarLinkPtr &parent, bool &execute);
	CScriptVarLinkPtr execute_function_call(bool &execute);
	CScriptVarLinkPtr execute_unary(bool &execute);
	CScriptVarLinkPtr execute_term(bool &execute);
	CScriptVarLinkPtr execute_expression(bool &execute);
	CScriptVarLinkPtr execute_binary_shift(bool &execute);
	CScriptVarLinkPtr execute_relation(bool &execute, int set=LEX_EQUAL, int set_n='<');
	CScriptVarLinkPtr execute_binary_logic(bool &execute, int op='|', int op_n1='^', int op_n2='&');
	CScriptVarLinkPtr execute_logic(bool &execute, int op=LEX_OROR, int op_n=LEX_ANDAND);
	CScriptVarLinkPtr execute_condition(bool &execute);
	CScriptVarLinkPtr execute_assignment(bool &execute);
	CScriptVarLinkPtr execute_base(bool &execute);
	void execute_block(bool &execute);
	CScriptVarLinkPtr execute_statement(bool &execute);
	// parsing utility functions
	CScriptVarLinkPtr parseFunctionDefinition(CScriptToken &FncToken);
//	CScriptVarSmartLink parseFunctionDefinition();
//	void parseFunctionArguments(CScriptVar *funcVar);
	CScriptVarLinkPtr parseFunctionsBodyFromString(const std::string &Parameter, const std::string &FncBody);
public:
	CScriptVarLink *findInScopes(const std::string &childName); ///< Finds a child, looking recursively up the scopes
	/// Get all Keynames of en given object (optionial look up the prototype-chain)
	void keys(STRING_VECTOR_t &Keys, CScriptVarPtr object, bool WithPrototypeChain);
	/// Look up in any parent classes of the given object
//	CScriptVarLink *findInPrototypeChain(const CScriptVarPtr &object, const std::string &name);
private:
	CScriptVarFunctionNativePtr addNative(const std::string &funcDesc, CScriptVarFunctionNativePtr Var);

	/// throws an Error
	void throwError(bool &execute, const std::string &message);
	void throwError(bool &execute, const std::string &message, CScriptTokenizer::ScriptTokenPosition &Pos);

	//////////////////////////////////////////////////////////////////////////
	/// native Object-Constructors & prototype-functions

	void native_Object(const CFunctionsScopePtr &c, void *data);
	void native_Object_hasOwnProperty(const CFunctionsScopePtr &c, void *data);
	void native_Object_getPrototypeOf(const CFunctionsScopePtr &c, void *data);
	void native_Object_valueOf(const CFunctionsScopePtr &c, void *data);
	void native_Object_toString(const CFunctionsScopePtr &c, void *data);

	void native_Array(const CFunctionsScopePtr &c, void *data);

	void native_String(const CFunctionsScopePtr &c, void *data);

	void native_Number(const CFunctionsScopePtr &c, void *data);

	void native_Boolean(const CFunctionsScopePtr &c, void *data);

	void native_Function(const CFunctionsScopePtr &c, void *data);
	void native_Function_call(const CFunctionsScopePtr &c, void *data);
	void native_Function_apply(const CFunctionsScopePtr &c, void *data);

	void native_Error(const CFunctionsScopePtr &c, void *data);
	void native_EvalError(const CFunctionsScopePtr &c, void *data);
	void native_RangeError(const CFunctionsScopePtr &c, void *data);
	void native_ReferenceError(const CFunctionsScopePtr &c, void *data);
	void native_SyntaxError(const CFunctionsScopePtr &c, void *data);
	void native_TypeError(const CFunctionsScopePtr &c, void *data);


	//////////////////////////////////////////////////////////////////////////
	/// global function

	void native_eval(const CFunctionsScopePtr &c, void *data);
	void native_isNAN(const CFunctionsScopePtr &c, void *data);
	void native_isFinite(const CFunctionsScopePtr &c, void *data);
	void native_parseInt(const CFunctionsScopePtr &c, void *data);
	void native_parseFloat(const CFunctionsScopePtr &c, void *data);



	void native_JSON_parse(const CFunctionsScopePtr &c, void *data);


	uint32_t uniqueID;
public:
	uint32_t getUniqueID() { return ++uniqueID; }
	CScriptVar *first;
	void setTemporaryID_recursive(uint32_t ID);
	void ClearUnreferedVars(const CScriptVarPtr &extra=CScriptVarPtr());
};

//////////////////////////////////////////////////////////////////////////
template<typename T>
inline const CScriptVarPtr &CScriptVar::constScriptVar(T t) { return context->constScriptVar(t); }
//////////////////////////////////////////////////////////////////////////
/*
inline define_newScriptVar_Fnc(NaN, CTinyJS* Context, NaN_t) { 
	//char error[] = "please use constScriptVar(NaN) or newScriptVarNaN(Context)";
	//return Context->constScriptVar(NaN); 
}
*/
template<typename T> CScriptVarPtr CT_ASSERT(CTinyJS* Context, T t){
	return Context->constScriptVar(t); 
}
template<typename T>
inline CScriptVarPtr newScriptVar(CTinyJS* Context, T Val, ct_assert_t Assert=ct_assert)
{
	return CT_ASSERT(Context, Assert); 
}

/*
	inline define_newScriptVar_Fnc(Bool, CTinyJS* Context, bool Var) { 
	return Fnc__(Context, Var); 
}
*/
#endif

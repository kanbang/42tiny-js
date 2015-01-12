#pragma once
#include "TypeList.h"
#include "TypeTraits.h"
#include <typeinfo>
#include <memory>

namespace Loki
{
	////////////////////////////////////////////////////////////////////////////////
	// class EmptyType
	// Used as a class type that doesn't hold anything
	// Useful as a strawman class
	////////////////////////////////////////////////////////////////////////////////

	class EmptyType {};

	////////////////////////////////////////////////////////////////////////////////
	// class template FunctorImpl (internal)
	////////////////////////////////////////////////////////////////////////////////

	namespace Private
	{
		template <typename R>
		struct FunctorImplBase
		{
			typedef R ResultType;

			typedef EmptyType Parm1;
			typedef EmptyType Parm2;
			typedef EmptyType Parm3;
			typedef EmptyType Parm4;
			typedef EmptyType Parm5;
			typedef EmptyType Parm6;
			typedef EmptyType Parm7;
			typedef EmptyType Parm8;
			typedef EmptyType Parm9;
			typedef EmptyType Parm10;
			typedef EmptyType Parm11;
			typedef EmptyType Parm12;
			typedef EmptyType Parm13;
			typedef EmptyType Parm14;
			typedef EmptyType Parm15;

			virtual FunctorImplBase* DoClone() const = 0;
			template <class U>
			static U* Clone(U* pObj)
			{
				if (!pObj) return 0;
				U* pClone = static_cast<U*>(pObj->DoClone());
				assert(typeid(*pClone) == typeid(*pObj));
				return pClone;
			}
		};
	}

	////////////////////////////////////////////////////////////////////////////////
	// macro DEFINE_CLONE_FUNCTORIMPL
	// Implements the DoClone function for a functor implementation
	////////////////////////////////////////////////////////////////////////////////

#define DEFINE_CLONE_FUNCTORIMPL(Cls) \
	virtual Cls* DoClone() const { return new Cls(*this); }

	////////////////////////////////////////////////////////////////////////////////
	// class template FunctorImpl
	// The base class for a hierarchy of functors. The FunctorImpl class is not used
	//     directly; rather, the Functor class manages and forwards to a pointer to
	//     FunctorImpl
	// You may want to derive your own functors from FunctorImpl.
	// Specializations of FunctorImpl for up to 15 parameters follow
	////////////////////////////////////////////////////////////////////////////////

	template <typename R, class TList>
	class FunctorImpl;

	////////////////////////////////////////////////////////////////////////////////
	// class template FunctorImpl
	// Specialization for 0 (zero) parameters
	////////////////////////////////////////////////////////////////////////////////

	template <typename R>
	class FunctorImpl<R, NullType>
		: public Private::FunctorImplBase<R>
	{
	public:
		typedef R ResultType;
		virtual R operator()() = 0;
	};

	////////////////////////////////////////////////////////////////////////////////
	// class template FunctorImpl
	// Specialization for 1 parameter
	////////////////////////////////////////////////////////////////////////////////

	template <typename R, typename P1>
	class FunctorImpl<R, TYPELIST_1(P1)>
		: public Private::FunctorImplBase<R>
	{
	public:
		typedef R ResultType;
		typedef typename TypeTraits<P1>::ParameterType Parm1;
		virtual R operator()(Parm1) = 0;
	};

	////////////////////////////////////////////////////////////////////////////////
	// class template FunctorImpl
	// Specialization for 2 parameters
	////////////////////////////////////////////////////////////////////////////////

	template <typename R, typename P1, typename P2>
	class FunctorImpl<R, TYPELIST_2(P1, P2)>
		: public Private::FunctorImplBase<R>
	{
	public:
		typedef R ResultType;
		typedef typename TypeTraits<P1>::ParameterType Parm1;
		typedef typename TypeTraits<P2>::ParameterType Parm2;
		virtual R operator()(Parm1, Parm2) = 0;
	};

	////////////////////////////////////////////////////////////////////////////////
	// class template FunctorImpl
	// Specialization for 3 parameters
	////////////////////////////////////////////////////////////////////////////////

	template <typename R, typename P1, typename P2, typename P3>
	class FunctorImpl<R, TYPELIST_3(P1, P2, P3)>
		: public Private::FunctorImplBase<R>
	{
	public:
		typedef R ResultType;
		typedef typename TypeTraits<P1>::ParameterType Parm1;
		typedef typename TypeTraits<P2>::ParameterType Parm2;
		typedef typename TypeTraits<P3>::ParameterType Parm3;
		virtual R operator()(Parm1, Parm2, Parm3) = 0;
	};

	////////////////////////////////////////////////////////////////////////////////
	// class template FunctorImpl
	// Specialization for 4 parameters
	////////////////////////////////////////////////////////////////////////////////

	template <typename R, typename P1, typename P2, typename P3, typename P4>
	class FunctorImpl<R, TYPELIST_4(P1, P2, P3, P4)>
		: public Private::FunctorImplBase<R>
	{
	public:
		typedef R ResultType;
		typedef typename TypeTraits<P1>::ParameterType Parm1;
		typedef typename TypeTraits<P2>::ParameterType Parm2;
		typedef typename TypeTraits<P3>::ParameterType Parm3;
		typedef typename TypeTraits<P4>::ParameterType Parm4;
		virtual R operator()(Parm1, Parm2, Parm3, Parm4) = 0;
	};

	////////////////////////////////////////////////////////////////////////////////
	// class template FunctorImpl
	// Specialization for 5 parameters
	////////////////////////////////////////////////////////////////////////////////

	template <typename R, typename P1, typename P2, typename P3, typename P4,
		typename P5>
	class FunctorImpl<R, TYPELIST_5(P1, P2, P3, P4, P5)>
		: public Private::FunctorImplBase<R>
	{
	public:
		typedef R ResultType;
		typedef typename TypeTraits<P1>::ParameterType Parm1;
		typedef typename TypeTraits<P2>::ParameterType Parm2;
		typedef typename TypeTraits<P3>::ParameterType Parm3;
		typedef typename TypeTraits<P4>::ParameterType Parm4;
		typedef typename TypeTraits<P5>::ParameterType Parm5;
		virtual R operator()(Parm1, Parm2, Parm3, Parm4, Parm5) = 0;
	};

	////////////////////////////////////////////////////////////////////////////////
	// class template FunctorImpl
	// Specialization for 6 parameters
	////////////////////////////////////////////////////////////////////////////////

	template <typename R, typename P1, typename P2, typename P3, typename P4,
		typename P5, typename P6>
	class FunctorImpl<R, TYPELIST_6(P1, P2, P3, P4, P5, P6)>
		: public Private::FunctorImplBase<R>
	{
	public:
		typedef R ResultType;
		typedef typename TypeTraits<P1>::ParameterType Parm1;
		typedef typename TypeTraits<P2>::ParameterType Parm2;
		typedef typename TypeTraits<P3>::ParameterType Parm3;
		typedef typename TypeTraits<P4>::ParameterType Parm4;
		typedef typename TypeTraits<P5>::ParameterType Parm5;
		typedef typename TypeTraits<P6>::ParameterType Parm6;
		virtual R operator()(Parm1, Parm2, Parm3, Parm4, Parm5, Parm6) = 0;
	};

	////////////////////////////////////////////////////////////////////////////////
	// class template FunctorImpl
	// Specialization for 7 parameters
	////////////////////////////////////////////////////////////////////////////////

	template <typename R, typename P1, typename P2, typename P3, typename P4,
		typename P5, typename P6, typename P7>
	class FunctorImpl<R, TYPELIST_7(P1, P2, P3, P4, P5, P6, P7)>
		: public Private::FunctorImplBase<R>
	{
	public:
		typedef R ResultType;
		typedef typename TypeTraits<P1>::ParameterType Parm1;
		typedef typename TypeTraits<P2>::ParameterType Parm2;
		typedef typename TypeTraits<P3>::ParameterType Parm3;
		typedef typename TypeTraits<P4>::ParameterType Parm4;
		typedef typename TypeTraits<P5>::ParameterType Parm5;
		typedef typename TypeTraits<P6>::ParameterType Parm6;
		typedef typename TypeTraits<P7>::ParameterType Parm7;
		virtual R operator()(Parm1, Parm2, Parm3, Parm4, Parm5, Parm6,
			Parm7) = 0;
	};

	////////////////////////////////////////////////////////////////////////////////
	// class template FunctorImpl
	// Specialization for 8 parameters
	////////////////////////////////////////////////////////////////////////////////

	template <typename R, typename P1, typename P2, typename P3, typename P4,
		typename P5, typename P6, typename P7, typename P8>
	class FunctorImpl<R, TYPELIST_8(P1, P2, P3, P4, P5, P6, P7, P8)>
		: public Private::FunctorImplBase<R>
	{
	public:
		typedef R ResultType;
		typedef typename TypeTraits<P1>::ParameterType Parm1;
		typedef typename TypeTraits<P2>::ParameterType Parm2;
		typedef typename TypeTraits<P3>::ParameterType Parm3;
		typedef typename TypeTraits<P4>::ParameterType Parm4;
		typedef typename TypeTraits<P5>::ParameterType Parm5;
		typedef typename TypeTraits<P6>::ParameterType Parm6;
		typedef typename TypeTraits<P7>::ParameterType Parm7;
		typedef typename TypeTraits<P8>::ParameterType Parm8;
		virtual R operator()(Parm1, Parm2, Parm3, Parm4, Parm5, Parm6,
			Parm7, Parm8) = 0;
	};

	////////////////////////////////////////////////////////////////////////////////
	// class template FunctorImpl
	// Specialization for 9 parameters
	////////////////////////////////////////////////////////////////////////////////

	template <typename R, typename P1, typename P2, typename P3, typename P4,
		typename P5, typename P6, typename P7, typename P8, typename P9>
	class FunctorImpl<R, TYPELIST_9(P1, P2, P3, P4, P5, P6, P7, P8, P9)>
		: public Private::FunctorImplBase<R>
	{
	public:
		typedef R ResultType;
		typedef typename TypeTraits<P1>::ParameterType Parm1;
		typedef typename TypeTraits<P2>::ParameterType Parm2;
		typedef typename TypeTraits<P3>::ParameterType Parm3;
		typedef typename TypeTraits<P4>::ParameterType Parm4;
		typedef typename TypeTraits<P5>::ParameterType Parm5;
		typedef typename TypeTraits<P6>::ParameterType Parm6;
		typedef typename TypeTraits<P7>::ParameterType Parm7;
		typedef typename TypeTraits<P8>::ParameterType Parm8;
		typedef typename TypeTraits<P9>::ParameterType Parm9;
		virtual R operator()(Parm1, Parm2, Parm3, Parm4, Parm5, Parm6,
			Parm7, Parm8, Parm9) = 0;
	};

	////////////////////////////////////////////////////////////////////////////////
	// class template FunctorImpl
	// Specialization for 10 parameters
	////////////////////////////////////////////////////////////////////////////////

	template <typename R, typename P1, typename P2, typename P3, typename P4,
		typename P5, typename P6, typename P7, typename P8, typename P9,
		typename P10>
	class FunctorImpl<R, TYPELIST_10(P1, P2, P3, P4, P5, P6, P7, P8, P9, P10)>
		: public Private::FunctorImplBase<R>
	{
	public:
		typedef R ResultType;
		typedef typename TypeTraits<P1>::ParameterType Parm1;
		typedef typename TypeTraits<P2>::ParameterType Parm2;
		typedef typename TypeTraits<P3>::ParameterType Parm3;
		typedef typename TypeTraits<P4>::ParameterType Parm4;
		typedef typename TypeTraits<P5>::ParameterType Parm5;
		typedef typename TypeTraits<P6>::ParameterType Parm6;
		typedef typename TypeTraits<P7>::ParameterType Parm7;
		typedef typename TypeTraits<P8>::ParameterType Parm8;
		typedef typename TypeTraits<P9>::ParameterType Parm9;
		typedef typename TypeTraits<P10>::ParameterType Parm10;
		virtual R operator()(Parm1, Parm2, Parm3, Parm4, Parm5, Parm6,
			Parm7, Parm8, Parm9, Parm10) = 0;
	};

	////////////////////////////////////////////////////////////////////////////////
	// class template FunctorImpl
	// Specialization for 11 parameters
	////////////////////////////////////////////////////////////////////////////////

	template <typename R, typename P1, typename P2, typename P3, typename P4,
		typename P5, typename P6, typename P7, typename P8, typename P9,
		typename P10, typename P11>
	class FunctorImpl<R,
		TYPELIST_11(P1, P2, P3, P4, P5, P6, P7, P8, P9, P10, P11)>
		: public Private::FunctorImplBase<R>
	{
	public:
		typedef R ResultType;
		typedef typename TypeTraits<P1>::ParameterType Parm1;
		typedef typename TypeTraits<P2>::ParameterType Parm2;
		typedef typename TypeTraits<P3>::ParameterType Parm3;
		typedef typename TypeTraits<P4>::ParameterType Parm4;
		typedef typename TypeTraits<P5>::ParameterType Parm5;
		typedef typename TypeTraits<P6>::ParameterType Parm6;
		typedef typename TypeTraits<P7>::ParameterType Parm7;
		typedef typename TypeTraits<P8>::ParameterType Parm8;
		typedef typename TypeTraits<P9>::ParameterType Parm9;
		typedef typename TypeTraits<P10>::ParameterType Parm10;
		typedef typename TypeTraits<P11>::ParameterType Parm11;
		virtual R operator()(Parm1, Parm2, Parm3, Parm4, Parm5, Parm6,
			Parm7, Parm8, Parm9, Parm10, Parm11) = 0;
	};

	////////////////////////////////////////////////////////////////////////////////
	// class template FunctorImpl
	// Specialization for 12 parameters
	////////////////////////////////////////////////////////////////////////////////

	template <typename R, typename P1, typename P2, typename P3, typename P4,
		typename P5, typename P6, typename P7, typename P8, typename P9,
		typename P10, typename P11, typename P12>
	class FunctorImpl<R,
		TYPELIST_12(P1, P2, P3, P4, P5, P6, P7, P8, P9, P10, P11, P12)>
		: public Private::FunctorImplBase<R>
	{
	public:
		typedef R ResultType;
		typedef typename TypeTraits<P1>::ParameterType Parm1;
		typedef typename TypeTraits<P2>::ParameterType Parm2;
		typedef typename TypeTraits<P3>::ParameterType Parm3;
		typedef typename TypeTraits<P4>::ParameterType Parm4;
		typedef typename TypeTraits<P5>::ParameterType Parm5;
		typedef typename TypeTraits<P6>::ParameterType Parm6;
		typedef typename TypeTraits<P7>::ParameterType Parm7;
		typedef typename TypeTraits<P8>::ParameterType Parm8;
		typedef typename TypeTraits<P9>::ParameterType Parm9;
		typedef typename TypeTraits<P10>::ParameterType Parm10;
		typedef typename TypeTraits<P11>::ParameterType Parm11;
		typedef typename TypeTraits<P12>::ParameterType Parm12;
		virtual R operator()(Parm1, Parm2, Parm3, Parm4, Parm5, Parm6,
			Parm7, Parm8, Parm9, Parm10, Parm11, Parm12) = 0;
	};

	////////////////////////////////////////////////////////////////////////////////
	// class template FunctorImpl
	// Specialization for 13 parameters
	////////////////////////////////////////////////////////////////////////////////

	template <typename R, typename P1, typename P2, typename P3, typename P4,
		typename P5, typename P6, typename P7, typename P8, typename P9,
		typename P10, typename P11, typename P12, typename P13>
	class FunctorImpl<R,
		TYPELIST_13(P1, P2, P3, P4, P5, P6, P7, P8, P9, P10, P11, P12, P13)>
		: public Private::FunctorImplBase<R>
	{
	public:
		typedef R ResultType;
		typedef typename TypeTraits<P1>::ParameterType Parm1;
		typedef typename TypeTraits<P2>::ParameterType Parm2;
		typedef typename TypeTraits<P3>::ParameterType Parm3;
		typedef typename TypeTraits<P4>::ParameterType Parm4;
		typedef typename TypeTraits<P5>::ParameterType Parm5;
		typedef typename TypeTraits<P6>::ParameterType Parm6;
		typedef typename TypeTraits<P7>::ParameterType Parm7;
		typedef typename TypeTraits<P8>::ParameterType Parm8;
		typedef typename TypeTraits<P9>::ParameterType Parm9;
		typedef typename TypeTraits<P10>::ParameterType Parm10;
		typedef typename TypeTraits<P11>::ParameterType Parm11;
		typedef typename TypeTraits<P12>::ParameterType Parm12;
		typedef typename TypeTraits<P13>::ParameterType Parm13;
		virtual R operator()(Parm1, Parm2, Parm3, Parm4, Parm5, Parm6,
			Parm7, Parm8, Parm9, Parm10, Parm11, Parm12, Parm13) = 0;
	};

	////////////////////////////////////////////////////////////////////////////////
	// class template FunctorImpl
	// Specialization for 14 parameters
	////////////////////////////////////////////////////////////////////////////////

	template <typename R, typename P1, typename P2, typename P3, typename P4,
		typename P5, typename P6, typename P7, typename P8, typename P9,
		typename P10, typename P11, typename P12, typename P13, typename P14>
	class FunctorImpl<R,
		TYPELIST_14(P1, P2, P3, P4, P5, P6, P7, P8, P9, P10, P11, P12, P13,
		P14)>
		: public Private::FunctorImplBase<R>
	{
	public:
		typedef R ResultType;
		typedef typename TypeTraits<P1>::ParameterType Parm1;
		typedef typename TypeTraits<P2>::ParameterType Parm2;
		typedef typename TypeTraits<P3>::ParameterType Parm3;
		typedef typename TypeTraits<P4>::ParameterType Parm4;
		typedef typename TypeTraits<P5>::ParameterType Parm5;
		typedef typename TypeTraits<P6>::ParameterType Parm6;
		typedef typename TypeTraits<P7>::ParameterType Parm7;
		typedef typename TypeTraits<P8>::ParameterType Parm8;
		typedef typename TypeTraits<P9>::ParameterType Parm9;
		typedef typename TypeTraits<P10>::ParameterType Parm10;
		typedef typename TypeTraits<P11>::ParameterType Parm11;
		typedef typename TypeTraits<P12>::ParameterType Parm12;
		typedef typename TypeTraits<P13>::ParameterType Parm13;
		typedef typename TypeTraits<P14>::ParameterType Parm14;
		virtual R operator()(Parm1, Parm2, Parm3, Parm4, Parm5, Parm6,
			Parm7, Parm8, Parm9, Parm10, Parm11, Parm12, Parm13, Parm14) = 0;
	};

	////////////////////////////////////////////////////////////////////////////////
	// class template FunctorImpl
	// Specialization for 15 parameters
	////////////////////////////////////////////////////////////////////////////////

	template <typename R, typename P1, typename P2, typename P3, typename P4,
		typename P5, typename P6, typename P7, typename P8, typename P9,
		typename P10, typename P11, typename P12, typename P13, typename P14,
		typename P15>
	class FunctorImpl<R,
		TYPELIST_15(P1, P2, P3, P4, P5, P6, P7, P8, P9, P10, P11, P12, P13,
		P14, P15)>
		: public Private::FunctorImplBase<R>
	{
	public:
		typedef R ResultType;
		typedef typename TypeTraits<P1>::ParameterType Parm1;
		typedef typename TypeTraits<P2>::ParameterType Parm2;
		typedef typename TypeTraits<P3>::ParameterType Parm3;
		typedef typename TypeTraits<P4>::ParameterType Parm4;
		typedef typename TypeTraits<P5>::ParameterType Parm5;
		typedef typename TypeTraits<P6>::ParameterType Parm6;
		typedef typename TypeTraits<P7>::ParameterType Parm7;
		typedef typename TypeTraits<P8>::ParameterType Parm8;
		typedef typename TypeTraits<P9>::ParameterType Parm9;
		typedef typename TypeTraits<P10>::ParameterType Parm10;
		typedef typename TypeTraits<P11>::ParameterType Parm11;
		typedef typename TypeTraits<P12>::ParameterType Parm12;
		typedef typename TypeTraits<P13>::ParameterType Parm13;
		typedef typename TypeTraits<P14>::ParameterType Parm14;
		typedef typename TypeTraits<P15>::ParameterType Parm15;
		virtual R operator()(Parm1, Parm2, Parm3, Parm4, Parm5, Parm6,
			Parm7, Parm8, Parm9, Parm10, Parm11, Parm12, Parm13, Parm14,
			Parm15) = 0;
	};

	////////////////////////////////////////////////////////////////////////////////
	// class template FunctorHandler
	// Wraps functors and pointers to functions
	////////////////////////////////////////////////////////////////////////////////

	template <class ParentFunctor, typename Fun>
	class FunctorHandler
		: public ParentFunctor::Impl
	{
		typedef typename ParentFunctor::Impl Base;

	public:
		typedef typename Base::ResultType ResultType;
		typedef typename Base::Parm1 Parm1;
		typedef typename Base::Parm2 Parm2;
		typedef typename Base::Parm3 Parm3;
		typedef typename Base::Parm4 Parm4;
		typedef typename Base::Parm5 Parm5;
		typedef typename Base::Parm6 Parm6;
		typedef typename Base::Parm7 Parm7;
		typedef typename Base::Parm8 Parm8;
		typedef typename Base::Parm9 Parm9;
		typedef typename Base::Parm10 Parm10;
		typedef typename Base::Parm11 Parm11;
		typedef typename Base::Parm12 Parm12;
		typedef typename Base::Parm13 Parm13;
		typedef typename Base::Parm14 Parm14;
		typedef typename Base::Parm15 Parm15;

		FunctorHandler(const Fun& fun) : f_(fun) {}

		DEFINE_CLONE_FUNCTORIMPL(FunctorHandler)

			// operator() implementations for up to 15 arguments

			ResultType operator()()
		{
				return f_();
			}

		ResultType operator()(Parm1 p1)
		{
			return f_(p1);
		}

		ResultType operator()(Parm1 p1, Parm2 p2)
		{
			return f_(p1, p2);
		}

		ResultType operator()(Parm1 p1, Parm2 p2, Parm3 p3)
		{
			return f_(p1, p2, p3);
		}

		ResultType operator()(Parm1 p1, Parm2 p2, Parm3 p3, Parm4 p4)
		{
			return f_(p1, p2, p3, p4);
		}

		ResultType operator()(Parm1 p1, Parm2 p2, Parm3 p3, Parm4 p4, Parm5 p5)
		{
			return f_(p1, p2, p3, p4, p5);
		}

		ResultType operator()(Parm1 p1, Parm2 p2, Parm3 p3, Parm4 p4, Parm5 p5,
			Parm6 p6)
		{
			return f_(p1, p2, p3, p4, p5, p6);
		}

		ResultType operator()(Parm1 p1, Parm2 p2, Parm3 p3, Parm4 p4, Parm5 p5,
			Parm6 p6, Parm7 p7)
		{
			return f_(p1, p2, p3, p4, p5, p6, p7);
		}

		ResultType operator()(Parm1 p1, Parm2 p2, Parm3 p3, Parm4 p4, Parm5 p5,
			Parm6 p6, Parm7 p7, Parm8 p8)
		{
			return f_(p1, p2, p3, p4, p5, p6, p7, p8);
		}

		ResultType operator()(Parm1 p1, Parm2 p2, Parm3 p3, Parm4 p4, Parm5 p5,
			Parm6 p6, Parm7 p7, Parm8 p8, Parm9 p9)
		{
			return f_(p1, p2, p3, p4, p5, p6, p7, p8, p9);
		}

		ResultType operator()(Parm1 p1, Parm2 p2, Parm3 p3, Parm4 p4, Parm5 p5,
			Parm6 p6, Parm7 p7, Parm8 p8, Parm9 p9, Parm10 p10)
		{
			return f_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10);
		}

		ResultType operator()(Parm1 p1, Parm2 p2, Parm3 p3, Parm4 p4, Parm5 p5,
			Parm6 p6, Parm7 p7, Parm8 p8, Parm9 p9, Parm10 p10, Parm11 p11)
		{
			return f_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11);
		}

		ResultType operator()(Parm1 p1, Parm2 p2, Parm3 p3, Parm4 p4, Parm5 p5,
			Parm6 p6, Parm7 p7, Parm8 p8, Parm9 p9, Parm10 p10, Parm11 p11,
			Parm12 p12)
		{
			return f_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12);
		}

		ResultType operator()(Parm1 p1, Parm2 p2, Parm3 p3, Parm4 p4, Parm5 p5,
			Parm6 p6, Parm7 p7, Parm8 p8, Parm9 p9, Parm10 p10, Parm11 p11,
			Parm12 p12, Parm13 p13)
		{
			return f_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13);
		}

		ResultType operator()(Parm1 p1, Parm2 p2, Parm3 p3, Parm4 p4, Parm5 p5,
			Parm6 p6, Parm7 p7, Parm8 p8, Parm9 p9, Parm10 p10, Parm11 p11,
			Parm12 p12, Parm13 p13, Parm14 p14)
		{
			return f_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13,
				p14);
		}

		ResultType operator()(Parm1 p1, Parm2 p2, Parm3 p3, Parm4 p4, Parm5 p5,
			Parm6 p6, Parm7 p7, Parm8 p8, Parm9 p9, Parm10 p10, Parm11 p11,
			Parm12 p12, Parm13 p13, Parm14 p14, Parm15 p15)
		{
			return f_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13,
				p14, p15);
		}

	private:
		Fun f_;
	};

	////////////////////////////////////////////////////////////////////////////////
	// class template FunctorHandler
	// Wraps pointers to member functions
	////////////////////////////////////////////////////////////////////////////////

	template <class ParentFunctor, typename PointerToObj,
		typename PointerToMemFn>
	class MemFunHandler : public ParentFunctor::Impl
	{
		typedef typename ParentFunctor::Impl Base;

	public:
		typedef typename Base::ResultType ResultType;
		typedef typename Base::Parm1 Parm1;
		typedef typename Base::Parm2 Parm2;
		typedef typename Base::Parm3 Parm3;
		typedef typename Base::Parm4 Parm4;
		typedef typename Base::Parm5 Parm5;
		typedef typename Base::Parm6 Parm6;
		typedef typename Base::Parm7 Parm7;
		typedef typename Base::Parm8 Parm8;
		typedef typename Base::Parm9 Parm9;
		typedef typename Base::Parm10 Parm10;
		typedef typename Base::Parm11 Parm11;
		typedef typename Base::Parm12 Parm12;
		typedef typename Base::Parm13 Parm13;
		typedef typename Base::Parm14 Parm14;
		typedef typename Base::Parm15 Parm15;

		MemFunHandler(const PointerToObj& pObj, PointerToMemFn pMemFn)
			: pObj_(pObj), pMemFn_(pMemFn)
		{}

		DEFINE_CLONE_FUNCTORIMPL(MemFunHandler)

			ResultType operator()()
		{
				return ((*pObj_).*pMemFn_)();
			}

		ResultType operator()(Parm1 p1)
		{
			return ((*pObj_).*pMemFn_)(p1);
		}

		ResultType operator()(Parm1 p1, Parm2 p2)
		{
			return ((*pObj_).*pMemFn_)(p1, p2);
		}

		ResultType operator()(Parm1 p1, Parm2 p2, Parm3 p3)
		{
			return ((*pObj_).*pMemFn_)(p1, p2, p3);
		}

		ResultType operator()(Parm1 p1, Parm2 p2, Parm3 p3, Parm4 p4)
		{
			return ((*pObj_).*pMemFn_)(p1, p2, p3, p4);
		}

		ResultType operator()(Parm1 p1, Parm2 p2, Parm3 p3, Parm4 p4, Parm5 p5)
		{
			return ((*pObj_).*pMemFn_)(p1, p2, p3, p4, p5);
		}

		ResultType operator()(Parm1 p1, Parm2 p2, Parm3 p3, Parm4 p4, Parm5 p5,
			Parm6 p6)
		{
			return ((*pObj_).*pMemFn_)(p1, p2, p3, p4, p5, p6);
		}

		ResultType operator()(Parm1 p1, Parm2 p2, Parm3 p3, Parm4 p4, Parm5 p5,
			Parm6 p6, Parm7 p7)
		{
			return ((*pObj_).*pMemFn_)(p1, p2, p3, p4, p5, p6, p7);
		}

		ResultType operator()(Parm1 p1, Parm2 p2, Parm3 p3, Parm4 p4, Parm5 p5,
			Parm6 p6, Parm7 p7, Parm8 p8)
		{
			return ((*pObj_).*pMemFn_)(p1, p2, p3, p4, p5, p6, p7, p8);
		}

		ResultType operator()(Parm1 p1, Parm2 p2, Parm3 p3, Parm4 p4, Parm5 p5,
			Parm6 p6, Parm7 p7, Parm8 p8, Parm9 p9)
		{
			return ((*pObj_).*pMemFn_)(p1, p2, p3, p4, p5, p6, p7, p8, p9);
		}

		ResultType operator()(Parm1 p1, Parm2 p2, Parm3 p3, Parm4 p4, Parm5 p5,
			Parm6 p6, Parm7 p7, Parm8 p8, Parm9 p9, Parm10 p10)
		{
			return ((*pObj_).*pMemFn_)(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10);
		}

		ResultType operator()(Parm1 p1, Parm2 p2, Parm3 p3, Parm4 p4, Parm5 p5,
			Parm6 p6, Parm7 p7, Parm8 p8, Parm9 p9, Parm10 p10, Parm11 p11)
		{
			return ((*pObj_).*pMemFn_)(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10,
				p11);
		}

		ResultType operator()(Parm1 p1, Parm2 p2, Parm3 p3, Parm4 p4, Parm5 p5,
			Parm6 p6, Parm7 p7, Parm8 p8, Parm9 p9, Parm10 p10, Parm11 p11,
			Parm12 p12)
		{
			return ((*pObj_).*pMemFn_)(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10,
				p11, p12);
		}

		ResultType operator()(Parm1 p1, Parm2 p2, Parm3 p3, Parm4 p4, Parm5 p5,
			Parm6 p6, Parm7 p7, Parm8 p8, Parm9 p9, Parm10 p10, Parm11 p11,
			Parm12 p12, Parm13 p13)
		{
			return ((*pObj_).*pMemFn_)(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10,
				p11, p12, p13);
		}

		ResultType operator()(Parm1 p1, Parm2 p2, Parm3 p3, Parm4 p4, Parm5 p5,
			Parm6 p6, Parm7 p7, Parm8 p8, Parm9 p9, Parm10 p10, Parm11 p11,
			Parm12 p12, Parm13 p13, Parm14 p14)
		{
			return ((*pObj_).*pMemFn_)(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10,
				p11, p12, p13, p14);
		}

		ResultType operator()(Parm1 p1, Parm2 p2, Parm3 p3, Parm4 p4, Parm5 p5,
			Parm6 p6, Parm7 p7, Parm8 p8, Parm9 p9, Parm10 p10, Parm11 p11,
			Parm12 p12, Parm13 p13, Parm14 p14, Parm15 p15)
		{
			return ((*pObj_).*pMemFn_)(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10,
				p11, p12, p13, p14, p15);
		}

	private:
		PointerToObj pObj_;
		PointerToMemFn pMemFn_;
	};

	////////////////////////////////////////////////////////////////////////////////
	// class template Functor
	// A generalized functor implementation with value semantics
	////////////////////////////////////////////////////////////////////////////////

	template <typename R, class TList = NullType>
	class Functor
	{
	public:
		// Handy type definitions for the body type
		typedef FunctorImpl<R, TList> Impl;
		typedef R ResultType;
		typedef TList ParmList;
		typedef typename Impl::Parm1 Parm1;
		typedef typename Impl::Parm2 Parm2;
		typedef typename Impl::Parm3 Parm3;
		typedef typename Impl::Parm4 Parm4;
		typedef typename Impl::Parm5 Parm5;
		typedef typename Impl::Parm6 Parm6;
		typedef typename Impl::Parm7 Parm7;
		typedef typename Impl::Parm8 Parm8;
		typedef typename Impl::Parm9 Parm9;
		typedef typename Impl::Parm10 Parm10;
		typedef typename Impl::Parm11 Parm11;
		typedef typename Impl::Parm12 Parm12;
		typedef typename Impl::Parm13 Parm13;
		typedef typename Impl::Parm14 Parm14;
		typedef typename Impl::Parm15 Parm15;

		// Member functions

		Functor() : spImpl_(0)
		{}

		Functor(const Functor& rhs) : spImpl_(Impl::Clone(rhs.spImpl_.get()))
		{}

		Functor(std::auto_ptr<Impl> spImpl) : spImpl_(spImpl)
		{}

		template <typename Fun>
		Functor(Fun fun)
			: spImpl_(new FunctorHandler<Functor, Fun>(fun))
		{}

		template <class PtrObj, typename MemFn>
		Functor(const PtrObj& p, MemFn memFn)
			: spImpl_(new MemFunHandler<Functor, PtrObj, MemFn>(p, memFn))
		{}

		typedef Impl * (std::auto_ptr<Impl>::*unspecified_bool_type)() const;

		operator unspecified_bool_type() const
		{
			return spImpl_.get() ? &std::auto_ptr<Impl>::get : 0;
		}

		Functor& operator=(const Functor& rhs)
		{
			Functor copy(rhs);
			// swap auto_ptrs by hand
			Impl* p = spImpl_.release();
			spImpl_.reset(copy.spImpl_.release());
			copy.spImpl_.reset(p);
			return *this;
		}

		ResultType operator()() const
		{
			return (*spImpl_)();
		}

		ResultType operator()(Parm1 p1) const
		{
			return (*spImpl_)(p1);
		}

		ResultType operator()(Parm1 p1, Parm2 p2) const
		{
			return (*spImpl_)(p1, p2);
		}

		ResultType operator()(Parm1 p1, Parm2 p2, Parm3 p3) const
		{
			return (*spImpl_)(p1, p2, p3);
		}

		ResultType operator()(Parm1 p1, Parm2 p2, Parm3 p3, Parm4 p4) const
		{
			return (*spImpl_)(p1, p2, p3, p4);
		}

		ResultType operator()(Parm1 p1, Parm2 p2, Parm3 p3, Parm4 p4, Parm5 p5) const
		{
			return (*spImpl_)(p1, p2, p3, p4, p5);
		}

		ResultType operator()(Parm1 p1, Parm2 p2, Parm3 p3, Parm4 p4, Parm5 p5,
			Parm6 p6) const
		{
			return (*spImpl_)(p1, p2, p3, p4, p5, p6);
		}

		ResultType operator()(Parm1 p1, Parm2 p2, Parm3 p3, Parm4 p4, Parm5 p5,
			Parm6 p6, Parm7 p7) const
		{
			return (*spImpl_)(p1, p2, p3, p4, p5, p6, p7);
		}

		ResultType operator()(Parm1 p1, Parm2 p2, Parm3 p3, Parm4 p4, Parm5 p5,
			Parm6 p6, Parm7 p7, Parm8 p8) const
		{
			return (*spImpl_)(p1, p2, p3, p4, p5, p6, p7, p8);
		}

		ResultType operator()(Parm1 p1, Parm2 p2, Parm3 p3, Parm4 p4, Parm5 p5,
			Parm6 p6, Parm7 p7, Parm8 p8, Parm9 p9) const
		{
			return (*spImpl_)(p1, p2, p3, p4, p5, p6, p7, p8, p9);
		}

		ResultType operator()(Parm1 p1, Parm2 p2, Parm3 p3, Parm4 p4, Parm5 p5,
			Parm6 p6, Parm7 p7, Parm8 p8, Parm9 p9, Parm10 p10) const
		{
			return (*spImpl_)(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10);
		}

		ResultType operator()(Parm1 p1, Parm2 p2, Parm3 p3, Parm4 p4, Parm5 p5,
			Parm6 p6, Parm7 p7, Parm8 p8, Parm9 p9, Parm10 p10, Parm11 p11) const
		{
			return (*spImpl_)(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11);
		}

		ResultType operator()(Parm1 p1, Parm2 p2, Parm3 p3, Parm4 p4, Parm5 p5,
			Parm6 p6, Parm7 p7, Parm8 p8, Parm9 p9, Parm10 p10, Parm11 p11,
			Parm12 p12) const
		{
			return (*spImpl_)(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11,
				p12);
		}

		ResultType operator()(Parm1 p1, Parm2 p2, Parm3 p3, Parm4 p4, Parm5 p5,
			Parm6 p6, Parm7 p7, Parm8 p8, Parm9 p9, Parm10 p10, Parm11 p11,
			Parm12 p12, Parm13 p13) const
		{
			return (*spImpl_)(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11,
				p12, p13);
		}

		ResultType operator()(Parm1 p1, Parm2 p2, Parm3 p3, Parm4 p4, Parm5 p5,
			Parm6 p6, Parm7 p7, Parm8 p8, Parm9 p9, Parm10 p10, Parm11 p11,
			Parm12 p12, Parm13 p13, Parm14 p14) const
		{
			return (*spImpl_)(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11,
				p12, p13, p14);
		}

		ResultType operator()(Parm1 p1, Parm2 p2, Parm3 p3, Parm4 p4, Parm5 p5,
			Parm6 p6, Parm7 p7, Parm8 p8, Parm9 p9, Parm10 p10, Parm11 p11,
			Parm12 p12, Parm13 p13, Parm14 p14, Parm15 p15) const
		{
			return (*spImpl_)(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11,
				p12, p13, p14, p15);
		}

	private:
		std::auto_ptr<Impl> spImpl_;
	};
} // namespace Loki

////////////////////////////////////////////////////////////////////////////////
// Change log:
// June 20, 2001: ported by Nick Thurn to gcc 2.95.3. Kudos, Nick!!!
////////////////////////////////////////////////////////////////////////////////
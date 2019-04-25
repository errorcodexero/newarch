#ifndef QUICK_H
#define QUICK_H

#define EQ1(A,B) if(a.B!=b.B) return 0;

#define LT1(A,B) if(a.B<b.B) return 1; if(b.B<a.B) return 0;

#define PRINT1(A,B) o<<a.B<<" ";

#define DECL1(A,B,...) A B;

#define TYPES(A,B,...) A,

#define STRUCT_MEMBERS(LIST)\
	LIST(DECL1)\
	bool dummy[0];

#define DECLARE_STRUCT(NAME,LIST)\
	struct NAME{\
		STRUCT_MEMBERS(LIST)\
		NAME( LIST(TYPES) bool=0);\
	};

#define ARGS(A,B,...) A B##1,
#define FDECL(A,B,...) B(B##1),

#define IMPL_STRUCT(NAME,LIST)\
	NAME( LIST(ARGS) bool):LIST(FDECL) dummy{}{\
		\
	}

#define IMPL_STRUCT_DECLARE(NAME,LIST) NAME( LIST(TYPES) bool);

#define ITEMS(A,B) B,

#define CONSTRUCT_STRUCT_PARAMS(LIST) LIST(ITEMS) false

#define CMP_OPS(NAME,LIST) \
	bool operator==(NAME const& a,NAME const& b){\
		LIST(EQ1)\
		return 1;\
	}\
	bool operator!=(NAME const& a,NAME const& b){\
		return !(a==b);\
	}\
	bool operator<(NAME const& a,NAME const& b){\
		LIST(LT1)\
		return 0;\
	}\
	std::ostream& operator<<(std::ostream& o,NAME const& a){\
		o<<""#NAME<<"( ";\
		LIST(PRINT1)\
		return o<<")";\
	}

#define CMP1(NAME) \
	bool operator==(NAME const&,NAME const&); \
	bool operator!=(NAME const&,NAME const&); \
	bool operator<(NAME const&,NAME const&); \
	std::ostream& operator<<(std::ostream&,NAME const&);

#define SINGLE_ARG(...) __VA_ARGS__

#endif

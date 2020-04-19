#pragma once

/**
 * Variable memory obfuscator
 *
 * This is a variable memory obfuscator used to prevent 
 * from memory hacking.
 *
 * (c) 2015 Damian Nowakowski
 */

#include <windows.h>
#include <ctime>
#include <stdint.h>
#include <assert.h>

/* Macro for easy defining binary operators */
#define DEF_BINARY_OPERATOR(_op)	safe_type& operator _op (const T& rhs)	\
									{										\
										val ^= key;							\
										assert(val == (chk ^ mask));		\
										T v = *(T*)&val;					\
										v _op rhs;							\
										val = *(uint64_t*)&v;				\
										chk = val ^ mask;					\
										genKey();							\
										val ^= key;							\
										return *this;						\
									}										\

/* Macro for easy defining unary oeprators */
#define DEF_UNARY_OPERATOR(_op)		safe_type& operator _op ()				\
									{										\
										val ^= key;							\
										assert(val == (chk ^ mask));		\
										T v = *(T*)&val;					\
										v _op;								\
										val = *(uint64_t*)&v;				\
										chk = val ^ mask;					\
										genKey();							\
										val ^= key;							\
										return *this;						\
									}										\
									safe_type operator _op (int)			\
									{										\
										safe_type tmp(*this);				\
										operator _op();						\
										return tmp;							\
									}										\

/* Random 64bit number as a mask for creating check copy */
static uint64_t mask = 7591541521614551595;

template<typename T>
class safe_type
{

private:

	uint64_t val;	///< The value that will be stored in memory.
	uint64_t key;	///< The key used to encrypt/decrypt the value.
	uint64_t chk;	///< The check copy for validating the value.

public:

	/* Constructor */
	safe_type(T v = 0)
	{
		genKey();

		val = *(uint64_t*)&v;
		chk = val ^ mask;
		val ^= key;
	}

	/* Copying constructor */
	safe_type(safe_type& v)
	{
		val = v.val;
		key = v.key;
		chk = v.chk;
	}

	/* Assign operator for new value */
	safe_type &operator=(T &v)
	{
		val = *(uint64_t*)&v;
		chk = val ^ mask;
		genKey();
		val ^= key;
		return *this;
	}

	/* Assign operator for safe type */
	safe_type &operator=(const safe_type &v)
	{
		val = v.val;
		key = v.key;
		chk = v.chk;
		return *this;
	}

	/* Call operator */
	operator T() const
	{
		uint64_t ret = val ^ key;
		assert(ret == (chk ^ mask));
		return *(T*)&ret;
	}

	/* Generate key */
	void genKey()
	{
		key = (uint64_t)time(0);
		key |= (key << 32); // We wanna full 64 bits for xoring.
	}

	/* Binary Arithmetic operators **/
	DEF_BINARY_OPERATOR(*=)
	DEF_BINARY_OPERATOR(/=)
	DEF_BINARY_OPERATOR(%=)
	DEF_BINARY_OPERATOR(-=)
	DEF_BINARY_OPERATOR(+=)
	DEF_BINARY_OPERATOR(^=)
	DEF_BINARY_OPERATOR(<<)
	DEF_BINARY_OPERATOR(>>)

	/* Unary Arithmetic operators */
	DEF_UNARY_OPERATOR(++)
	DEF_UNARY_OPERATOR(--)
};


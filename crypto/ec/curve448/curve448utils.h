/*
 * Copyright 2017 The OpenSSL Project Authors. All Rights Reserved.
 * Copyright 2015 Cryptography Research, Inc.
 *
 * Licensed under the OpenSSL license (the "License").  You may not use
 * this file except in compliance with the License.  You can obtain a copy
 * in the file LICENSE in the source distribution or at
 * https://www.openssl.org/source/license.html
 *
 * Originally written by Mike Hamburg
 */

#ifndef __DECAF_COMMON_H__
# define __DECAF_COMMON_H__ 1

# include <openssl/e_os2.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Internal word types. Somewhat tricky.  This could be decided separately per
 * platform.  However, the structs do need to be all the same size and
 * alignment on a given platform to support dynamic linking, since even if you
 * header was built with eg arch_neon, you might end up linking a library built
 * with arch_arm32.
 */
# ifndef DECAF_WORD_BITS
#  if (defined(__ILP64__) || defined(__amd64__) || defined(__x86_64__) \
      || (((__UINT_FAST32_MAX__)>>30)>>30))
#   define DECAF_WORD_BITS 64      /* The number of bits in a word */
#  else
#   define DECAF_WORD_BITS 32      /* The number of bits in a word */
#  endif
# endif

# if DECAF_WORD_BITS == 64
/* Word size for internal computations */
typedef uint64_t decaf_word_t;
/* Signed word size for internal computations */
typedef int64_t decaf_sword_t;
/* "Boolean" type, will be set to all-zero or all-one (i.e. -1u) */
typedef uint64_t decaf_bool_t;
/* Double-word size for internal computations */
typedef __uint128_t decaf_dword_t;
/* Signed double-word size for internal computations */
typedef __int128_t decaf_dsword_t;
# elif DECAF_WORD_BITS == 32
/* Word size for internal computations */
typedef uint32_t decaf_word_t;
/* Signed word size for internal computations */
typedef int32_t decaf_sword_t;
/* "Boolean" type, will be set to all-zero or all-one (i.e. -1u) */
typedef uint32_t decaf_bool_t;
/* Double-word size for internal computations */
typedef uint64_t decaf_dword_t;
/* Signed double-word size for internal computations */
typedef int64_t decaf_dsword_t;
# else
#  error "Only supporting DECAF_WORD_BITS = 32 or 64 for now"
# endif

/* DECAF_TRUE = -1 so that DECAF_TRUE & x = x */
static const decaf_bool_t DECAF_TRUE = -(decaf_bool_t) 1;

/* DECAF_FALSE = 0 so that DECAF_FALSE & x = 0 */
static const decaf_bool_t DECAF_FALSE = 0;

/* Another boolean type used to indicate success or failure. */
typedef enum {
    DECAF_SUCCESS = -1, /**< The operation succeeded. */
    DECAF_FAILURE = 0   /**< The operation failed. */
} decaf_error_t;

/* Return success if x is true */
static ossl_inline decaf_error_t decaf_succeed_if(decaf_bool_t x)
{
    return (decaf_error_t) x;
}

/* Return DECAF_TRUE iff x == DECAF_SUCCESS */
static ossl_inline decaf_bool_t decaf_successful(decaf_error_t e)
{
    decaf_dword_t w = ((decaf_word_t) e) ^ ((decaf_word_t) DECAF_SUCCESS);
    return (w - 1) >> DECAF_WORD_BITS;
}

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif                          /* __DECAF_COMMON_H__ */
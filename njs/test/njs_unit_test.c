
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) NGINX, Inc.
 */

#include <nxt_types.h>
#include <nxt_clang.h>
#include <nxt_stub.h>
#include <nxt_malloc.h>
#include <nxt_array.h>
#include <nxt_lvlhsh.h>
#include <nxt_mem_cache_pool.h>
#include <njscript.h>
#include <string.h>
#include <stdio.h>


#define  NXT_FIBOBENCH  0


typedef struct {
    nxt_str_t  script;
    nxt_str_t  ret;
} nxt_jscript_test_t;


extern char  **environ;


static nxt_jscript_test_t  js_test[] =
{
#if (NXT_FIBOBENCH == 1)

    { nxt_string("function fibo(n) {                        \
                      if (n > 1)                            \
                          return fibo(n - 1) + fibo(n - 2)  \
                      return 'α'                            \
                  }                                         \
                  fibo(32).length"),
      nxt_string("3524578") },

#elif (NXT_FIBOBENCH > 1)

    { nxt_string("function fibo(n) {                        \
                      if (n > 1)                            \
                          return fibo(n - 1) + fibo(n - 2)  \
                      return 1                              \
                  }                                         \
                  fibo(32)"),
      nxt_string("3524578") },

#else /* !(NXT_FIBOBENCH) */

    { nxt_string("0 == '000'"),
      nxt_string("true") },

    { nxt_string("999999999999999999999"),
      nxt_string("1e+21") },

#if 0
    { nxt_string("9223372036854775808"),
      nxt_string("9223372036854775808") },

    { nxt_string("18446744073709551616"),
      nxt_string("18446744073709552000") },

    { nxt_string("1.7976931348623157E+308"),
      nxt_string("1.7976931348623157e+308") },
#endif

#if 0
    { nxt_string("var a = 'a\\'b'"),
      nxt_string("a'b") },
#endif

    { nxt_string("+1"),
      nxt_string("1") },

    { nxt_string("+1\n"),
      nxt_string("1") },

#if 0
    { nxt_string(""),
      nxt_string("SyntaxError") },

    { nxt_string("\n"),
      nxt_string("SyntaxError") },
#endif

    { nxt_string("\n +1"),
      nxt_string("1") },

    { nxt_string("1 + undefined"),
      nxt_string("NaN") },

    { nxt_string("1 + ''"),
      nxt_string("1") },

    { nxt_string("undefined + undefined"),
      nxt_string("NaN") },

    { nxt_string("1.2 + 5.7"),
      nxt_string("6.9") },

    { nxt_string("1 + 1 + '2' + 1 + 1"),
      nxt_string("2211") },

    { nxt_string("1.2 - '5.7'"),
      nxt_string("-4.5") },

    { nxt_string("1.2 + -'5.7'"),
      nxt_string("-4.5") },

    { nxt_string("1 + +'3'"),
      nxt_string("4") },

    { nxt_string("1 - undefined"),
      nxt_string("NaN") },

    { nxt_string("1 - ''"),
      nxt_string("1") },

    { nxt_string("undefined - undefined"),
      nxt_string("NaN") },

    { nxt_string("12 | 6"),
      nxt_string("14") },

    { nxt_string("12 | 'abc'"),
      nxt_string("12") },

    { nxt_string("-1 | 0"),
      nxt_string("-1") },

    { nxt_string("-2147483648 | 0"),
      nxt_string("-2147483648") },

    { nxt_string("1024.9 | 0"),
      nxt_string("1024") },

    { nxt_string("-1024.9 | 0"),
      nxt_string("-1024") },

    { nxt_string("9007199254740991 | 0"),
      nxt_string("-1") },

    { nxt_string("9007199254740992 | 0"),
      nxt_string("0") },

    { nxt_string("9007199254740993 | 0"),
      nxt_string("0") },

#if 0
    { nxt_string("9223372036854775808 | 0"),
      nxt_string("0") },
#endif

    { nxt_string("9223372036854777856 | 0"),
      nxt_string("2048") },

    { nxt_string("-9223372036854777856 | 0"),
      nxt_string("-2048") },

    { nxt_string("NaN | 0"),
      nxt_string("0") },

    { nxt_string("-NaN | 0"),
      nxt_string("0") },

    { nxt_string("Infinity | 0"),
      nxt_string("0") },

    { nxt_string("-Infinity | 0"),
      nxt_string("0") },

    { nxt_string("+0 | 0"),
      nxt_string("0") },

    { nxt_string("-0 | 0"),
      nxt_string("0") },

    { nxt_string("32.5 << 2.4"),
      nxt_string("128") },

    { nxt_string("32.5 << 'abc'"),
      nxt_string("32") },

    { nxt_string("'abc' << 2"),
      nxt_string("0") },

    { nxt_string("-1 << 0"),
      nxt_string("-1") },

    { nxt_string("-1 << -1"),
      nxt_string("-2147483648") },

    { nxt_string("-2147483648 << 0"),
      nxt_string("-2147483648") },

#if 0
    { nxt_string("9223372036854775808 << 0"),
      nxt_string("0") },
#endif

    { nxt_string("9223372036854777856 << 0"),
      nxt_string("2048") },

    { nxt_string("-9223372036854777856 << 0"),
      nxt_string("-2048") },

    { nxt_string("NaN << 0"),
      nxt_string("0") },

    { nxt_string("32.5 >> 2.4"),
      nxt_string("8") },

    { nxt_string("-1 >> 30"),
      nxt_string("-1") },

    { nxt_string("'abc' >> 2"),
      nxt_string("0") },

    { nxt_string("-1 >> 0"),
      nxt_string("-1") },

    { nxt_string("-1 >> -1"),
      nxt_string("-1") },

    { nxt_string("-2147483648 >> 0"),
      nxt_string("-2147483648") },

    { nxt_string("-2147483648 >> -1"),
      nxt_string("-1") },

#if 0
    { nxt_string("9223372036854775808 >> 0"),
      nxt_string("0") },
#endif

    { nxt_string("9223372036854777856 >> 0"),
      nxt_string("2048") },

    { nxt_string("-9223372036854777856 >> 0"),
      nxt_string("-2048") },

    { nxt_string("NaN >> 0"),
      nxt_string("0") },

    { nxt_string("-1 >>> 30"),
      nxt_string("3") },

    { nxt_string("NaN >>> 1"),
      nxt_string("0") },

#if 0
    { nxt_string("9223372036854775808 >>> 1"),
      nxt_string("0") },
#endif

    { nxt_string("-1 >>> 0"),
      nxt_string("4294967295") },

    { nxt_string("-1 >>> -1"),
      nxt_string("1") },

    { nxt_string("-2147483648 >>> 0"),
      nxt_string("2147483648") },

    { nxt_string("-2147483648 >>> -1"),
      nxt_string("1") },

#if 0
    { nxt_string("9223372036854775808 >>> 0"),
      nxt_string("0") },
#endif

    { nxt_string("9223372036854777856 >>> 0"),
      nxt_string("2048") },

    { nxt_string("-9223372036854777856 >>> 0"),
      nxt_string("4294965248") },

    { nxt_string("NaN >>> 0"),
      nxt_string("0") },

    { nxt_string("!2"),
      nxt_string("false") },

    { nxt_string("1 || 2"),
      nxt_string("1") },

    { nxt_string("1 && 2"),
      nxt_string("2") },

    { nxt_string("a = true; a = -~!a"),
      nxt_string("1") },

    { nxt_string("12 & 6"),
      nxt_string("4") },

    { nxt_string("-1 & 65536"),
      nxt_string("65536") },

    { nxt_string("-2147483648 & 65536"),
      nxt_string("0") },

#if 0
    { nxt_string("9223372036854775808 & 65536"),
      nxt_string("0") },
#endif

    { nxt_string("NaN & 65536"),
      nxt_string("0") },

    { nxt_string("12 ^ 6"),
      nxt_string("10") },

    { nxt_string("-1 ^ 65536"),
      nxt_string("-65537") },

    { nxt_string("-2147483648 ^ 65536"),
      nxt_string("-2147418112") },

#if 0
    { nxt_string("9223372036854775808 ^ 65536"),
      nxt_string("65536") },
#endif

    { nxt_string("NaN ^ 65536"),
      nxt_string("65536") },

    { nxt_string("x = '1'; +x + 2"),
      nxt_string("3") },

    { nxt_string("'3' -+-+-+ '1' + '1' / '3' * '6' + '2'"),
      nxt_string("42") },

    { nxt_string("'true' == true"),
      nxt_string("false") },

    { nxt_string("null == false"),
      nxt_string("false") },

    { nxt_string("!null"),
      nxt_string("true") },

    { nxt_string("0 === -0"),
      nxt_string("true") },

    { nxt_string("1/-0"),
      nxt_string("-Infinity") },

    { nxt_string("1/0 === 1/-0"),
      nxt_string("false") },

    { nxt_string("1 == true"),
      nxt_string("true") },

    { nxt_string("NaN === NaN"),
      nxt_string("false") },

    { nxt_string("NaN !== NaN"),
      nxt_string("true") },

    { nxt_string("NaN == NaN"),
      nxt_string("false") },

    { nxt_string("NaN != NaN"),
      nxt_string("true") },

    { nxt_string("NaN == false"),
      nxt_string("false") },

    { nxt_string("Infinity == Infinity"),
      nxt_string("true") },

    { nxt_string("-Infinity == -Infinity"),
      nxt_string("true") },

    { nxt_string("-Infinity < Infinity"),
      nxt_string("true") },

    { nxt_string("Infinity - Infinity"),
      nxt_string("NaN") },

    { nxt_string("Infinity - -Infinity"),
      nxt_string("Infinity") },

    { nxt_string("undefined == 0"),
      nxt_string("false") },

    { nxt_string("undefined == null"),
      nxt_string("true") },

    { nxt_string("'1' == 1"),
      nxt_string("true") },

    { nxt_string("'1a' == '1'"),
      nxt_string("false") },

    { nxt_string("'abc' == 'abc'"),
      nxt_string("true") },

    { nxt_string("'abc' < 'abcde'"),
      nxt_string("true") },

    { nxt_string("0 == ''"),
      nxt_string("true") },

    { nxt_string("0 == ' '"),
      nxt_string("true") },

    { nxt_string("0 == '  '"),
      nxt_string("true") },

    { nxt_string("0 == '0'"),
      nxt_string("true") },

    { nxt_string("0 == ' 0 '"),
      nxt_string("true") },

    { nxt_string("0 == '000'"),
      nxt_string("true") },

    { nxt_string("'0' == ''"),
      nxt_string("false") },

    { nxt_string("1 < 2"),
      nxt_string("true") },

    { nxt_string("NaN < NaN"),
      nxt_string("false") },

    { nxt_string("NaN > NaN"),
      nxt_string("false") },

    { nxt_string("undefined < 1"),
      nxt_string("false") },

    { nxt_string("[] == false"),
      nxt_string("true") },

    { nxt_string("[0] == false"),
      nxt_string("true") },

    { nxt_string("[0,0] == false"),
      nxt_string("false") },

    { nxt_string("({}) == false"),
      nxt_string("false") },

    { nxt_string("var a = { valueOf: function() { return 5 } }; a == 5"),
      nxt_string("true") },

    { nxt_string("var a = { valueOf: function() { return '5' } }; a == 5"),
      nxt_string("true") },

    { nxt_string("var a = { valueOf: function() { return '5' } }; a == '5'"),
      nxt_string("true") },

    /* Comparisions. */

    { nxt_string("null === null"),
      nxt_string("true") },

    { nxt_string("null !== null"),
      nxt_string("false") },

    { nxt_string("null == null"),
      nxt_string("true") },

    { nxt_string("null != null"),
      nxt_string("false") },

    { nxt_string("null < null"),
      nxt_string("false") },

    { nxt_string("null > null"),
      nxt_string("false") },

    { nxt_string("null <= null"),
      nxt_string("true") },

    { nxt_string("null >= null"),
      nxt_string("true") },

    /**/

    { nxt_string("null === undefined"),
      nxt_string("false") },

    { nxt_string("null !== undefined"),
      nxt_string("true") },

    { nxt_string("null == undefined"),
      nxt_string("true") },

    { nxt_string("null != undefined"),
      nxt_string("false") },

    { nxt_string("null < undefined"),
      nxt_string("false") },

    { nxt_string("null > undefined"),
      nxt_string("false") },

    { nxt_string("null <= undefined"),
      nxt_string("false") },

    { nxt_string("null >= undefined"),
      nxt_string("false") },

    /**/

    { nxt_string("null === false"),
      nxt_string("false") },

    { nxt_string("null !== false"),
      nxt_string("true") },

    { nxt_string("null == false"),
      nxt_string("false") },

    { nxt_string("null != false"),
      nxt_string("true") },

    { nxt_string("null < false"),
      nxt_string("false") },

    { nxt_string("null > false"),
      nxt_string("false") },

    { nxt_string("null <= false"),
      nxt_string("true") },

    { nxt_string("null >= false"),
      nxt_string("true") },

    /**/

    { nxt_string("null === true"),
      nxt_string("false") },

    { nxt_string("null !== true"),
      nxt_string("true") },

    { nxt_string("null == true"),
      nxt_string("false") },

    { nxt_string("null != true"),
      nxt_string("true") },

    { nxt_string("null < true"),
      nxt_string("true") },

    { nxt_string("null > true"),
      nxt_string("false") },

    { nxt_string("null <= true"),
      nxt_string("true") },

    { nxt_string("null >= true"),
      nxt_string("false") },

    /**/

    { nxt_string("Infinity === Infinity"),
      nxt_string("true") },

    { nxt_string("Infinity !== Infinity"),
      nxt_string("false") },

    { nxt_string("Infinity == Infinity"),
      nxt_string("true") },

    { nxt_string("Infinity != Infinity"),
      nxt_string("false") },

    { nxt_string("Infinity < Infinity"),
      nxt_string("false") },

    { nxt_string("Infinity > Infinity"),
      nxt_string("false") },

    { nxt_string("Infinity <= Infinity"),
      nxt_string("true") },

    { nxt_string("Infinity >= Infinity"),
      nxt_string("true") },

    /**/

    { nxt_string("-Infinity === Infinity"),
      nxt_string("false") },

    { nxt_string("-Infinity !== Infinity"),
      nxt_string("true") },

    { nxt_string("-Infinity == Infinity"),
      nxt_string("false") },

    { nxt_string("-Infinity != Infinity"),
      nxt_string("true") },

    { nxt_string("-Infinity < Infinity"),
      nxt_string("true") },

    { nxt_string("-Infinity > Infinity"),
      nxt_string("false") },

    { nxt_string("-Infinity <= Infinity"),
      nxt_string("true") },

    { nxt_string("-Infinity >= Infinity"),
      nxt_string("false") },

    /**/

    { nxt_string("NaN === NaN"),
      nxt_string("false") },

    { nxt_string("NaN !== NaN"),
      nxt_string("true") },

    { nxt_string("NaN == NaN"),
      nxt_string("false") },

    { nxt_string("NaN != NaN"),
      nxt_string("true") },

    { nxt_string("NaN < NaN"),
      nxt_string("false") },

    { nxt_string("NaN > NaN"),
      nxt_string("false") },

    { nxt_string("NaN >= NaN"),
      nxt_string("false") },

    { nxt_string("NaN <= NaN"),
      nxt_string("false") },

    /**/

    { nxt_string("null < 0"),
      nxt_string("false") },

    { nxt_string("null < 1"),
      nxt_string("true") },

    { nxt_string("null < NaN"),
      nxt_string("false") },

    { nxt_string("null < -Infinity"),
      nxt_string("false") },

    { nxt_string("null < Infinity"),
      nxt_string("true") },

    { nxt_string("null < 'null'"),
      nxt_string("false") },

    { nxt_string("null < '1'"),
      nxt_string("true") },

    { nxt_string("null < [1]"),
      nxt_string("true") },

    { nxt_string("null < ({})"),
      nxt_string("false") },

    { nxt_string("var a = { valueOf: function() { return 1 } }; null < a"),
      nxt_string("true") },

    { nxt_string("var a = { valueOf: function() { return 'null' } }; null < a"),
      nxt_string("false") },

    { nxt_string("var a = { valueOf: function() { return '1' } }; null < a"),
      nxt_string("true") },

    /**/

    { nxt_string("undefined < null"),
      nxt_string("false") },

    { nxt_string("undefined < undefined"),
      nxt_string("false") },

    { nxt_string("undefined < false"),
      nxt_string("false") },

    { nxt_string("undefined < true"),
      nxt_string("false") },

    { nxt_string("undefined < 0"),
      nxt_string("false") },

    { nxt_string("undefined < 1"),
      nxt_string("false") },

    { nxt_string("undefined < NaN"),
      nxt_string("false") },

    { nxt_string("undefined < -Infinity"),
      nxt_string("false") },

    { nxt_string("undefined < Infinity"),
      nxt_string("false") },

    { nxt_string("undefined < 'undefined'"),
      nxt_string("false") },

    { nxt_string("undefined < '1'"),
      nxt_string("false") },

    { nxt_string("undefined < [1]"),
      nxt_string("false") },

    { nxt_string("undefined < ({})"),
      nxt_string("false") },

    { nxt_string("var a = { valueOf: function() { return 1 } } undefined < a"),
      nxt_string("false") },

    { nxt_string("var a = { valueOf: function() { return 'undefined' } }      \
                  undefined < a"),
      nxt_string("false") },

    { nxt_string("var a = { valueOf: function() { return '1' } }              \
                  undefined < a"),
      nxt_string("false") },

    /**/

    { nxt_string("false < 1"),
      nxt_string("true") },

    { nxt_string("true < 1"),
      nxt_string("false") },

    { nxt_string("-1 < 1"),
      nxt_string("true") },

    { nxt_string("-1 < '1'"),
      nxt_string("true") },

    { nxt_string("NaN < NaN"),
      nxt_string("false") },

    { nxt_string("-Infinity < Infinity"),
      nxt_string("true") },

    { nxt_string("Infinity < -Infinity"),
      nxt_string("false") },

    { nxt_string("1 < 'abc'"),
      nxt_string("false") },

    /**/

    { nxt_string("[] === []"),
      nxt_string("false") },

    { nxt_string("[] !== []"),
      nxt_string("true") },

    { nxt_string("[] == []"),
      nxt_string("false") },

    { nxt_string("[] != []"),
      nxt_string("true") },

    { nxt_string("[] < []"),
      nxt_string("false") },

    { nxt_string("[] > []"),
      nxt_string("false") },

    { nxt_string("[] >= []"),
      nxt_string("true") },

    { nxt_string("[] <= []"),
      nxt_string("true") },

    /**/

    { nxt_string("({}) === ({})"),
      nxt_string("false") },

    { nxt_string("({}) !== ({})"),
      nxt_string("true") },

    { nxt_string("({}) == ({})"),
      nxt_string("false") },

    { nxt_string("({}) != ({})"),
      nxt_string("true") },

    { nxt_string("({}) > ({})"),
      nxt_string("false") },

    { nxt_string("({}) <= ({})"),
      nxt_string("true") },

    { nxt_string("({}) >= ({})"),
      nxt_string("true") },

    /**/

    { nxt_string("[0] == ({})"),
      nxt_string("false") },

    { nxt_string("[0] != ({})"),
      nxt_string("true") },

    { nxt_string("[0] <= ({})"),
      nxt_string("true") },

    { nxt_string("[0] >= ({})"),
      nxt_string("false") },

    /**/

    { nxt_string("a = 1 ? 2 : 3"),
      nxt_string("2") },

    { nxt_string("a = 1 ? 2 : 3 ? 4 : 5"),
      nxt_string("2") },

    { nxt_string("a = 0 ? 2 : 3 ? 4 : 5"),
      nxt_string("4") },

    { nxt_string("0 ? 2 ? 3 : 4 : 5"),
      nxt_string("5") },

    { nxt_string("1 ? 2 ? 3 : 4 : 5"),
      nxt_string("3") },

    { nxt_string("1 ? 0 ? 3 : 4 : 5"),
      nxt_string("4") },

    { nxt_string("(1 ? 0 : 3) ? 4 : 5"),
      nxt_string("5") },

    { nxt_string("a = (1 + 2) ? 2 ? 3 + 4 : 5 : 6"),
      nxt_string("7") },

    { nxt_string("a = (1 ? 2 : 3) + 4"),
      nxt_string("6") },

    { nxt_string("a = 1 ? b = 2 + 4 : b = 3"),
      nxt_string("6") },

    /* Increment. */

    { nxt_string("var a = 1;   ++a"),
      nxt_string("2") },

    { nxt_string("var a = '1'; ++a"),
      nxt_string("2") },

    { nxt_string("var a = [1]; ++a"),
      nxt_string("2") },

    { nxt_string("var a = {};  ++a"),
      nxt_string("NaN") },

    { nxt_string("var a = { valueOf: function() { return 1 } };   ++a"),
      nxt_string("2") },

    { nxt_string("var a = { valueOf: function() { return '1' } }; ++a"),
      nxt_string("2") },

    { nxt_string("var a = { valueOf: function() { return [1] } }; ++a"),
      nxt_string("NaN") },

    { nxt_string("var a = { valueOf: function() { return {} } }; ++a"),
      nxt_string("NaN") },

    /**/

    { nxt_string("var a = 1;   a = ++a"),
      nxt_string("2") },

    { nxt_string("var a = '1'; a = ++a"),
      nxt_string("2") },

    { nxt_string("var a = [1]; a = ++a"),
      nxt_string("2") },

    { nxt_string("var a = {};  a = ++a"),
      nxt_string("NaN") },

    { nxt_string("var a = { valueOf: function() { return 1 } }   a = ++a"),
      nxt_string("2") },

    { nxt_string("var a = { valueOf: function() { return '1' } } a = ++a"),
      nxt_string("2") },

    { nxt_string("var a = { valueOf: function() { return [1] } } a = ++a"),
      nxt_string("NaN") },

    { nxt_string("var a = { valueOf: function() { return {} } } a = ++a"),
      nxt_string("NaN") },

    /**/

    { nxt_string("var a = 1;   var b = ++a; a +' '+ b"),
      nxt_string("2 2") },

    { nxt_string("var a = '1'; var b = ++a; a +' '+ b"),
      nxt_string("2 2") },

    { nxt_string("var a = [1]; var b = ++a; a +' '+ b"),
      nxt_string("2 2") },

    { nxt_string("var a = {};  var b = ++a; a +' '+ b"),
      nxt_string("NaN NaN") },

    { nxt_string("var a = { valueOf: function() { return 1 } }                \
                  var b = ++a; a +' '+ b"),
      nxt_string("2 2") },

    { nxt_string("var a = { valueOf: function() { return '1' } }              \
                  var b = ++a; a +' '+ b"),
      nxt_string("2 2") },

    { nxt_string("var a = { valueOf: function() { return [1] } }              \
                  var b = ++a; a +' '+ b"),
      nxt_string("NaN NaN") },

    { nxt_string("var a = { valueOf: function() { return {} } }               \
                  var b = ++a; a +' '+ b"),
      nxt_string("NaN NaN") },

    /* Post increment. */

    { nxt_string("var a = 1;   a++"),
      nxt_string("1") },

    { nxt_string("var a = '1'; a++"),
      nxt_string("1") },

    { nxt_string("var a = [1]; a++"),
      nxt_string("1") },

    { nxt_string("var a = {};  a++"),
      nxt_string("NaN") },

    { nxt_string("var a = { valueOf: function() { return 1 } }    a++"),
      nxt_string("1") },

    { nxt_string("var a = { valueOf: function() { return '1' } }  a++"),
      nxt_string("1") },

    { nxt_string("var a = { valueOf: function() { return [1] } }  a++"),
      nxt_string("NaN") },

    { nxt_string("var a = { valueOf: function() { return {} } }  a++"),
      nxt_string("NaN") },

    /**/

    { nxt_string("var a = 1;   a = a++"),
      nxt_string("1") },

    { nxt_string("var a = '1'; a = a++"),
      nxt_string("1") },

    { nxt_string("var a = [1]; a = a++"),
      nxt_string("1") },

    { nxt_string("var a = {};  a = a++"),
      nxt_string("NaN") },

    { nxt_string("var a = { valueOf: function() { return 1 } }   a = a++"),
      nxt_string("1") },

    { nxt_string("var a = { valueOf: function() { return '1' } } a = a++"),
      nxt_string("1") },

    { nxt_string("var a = { valueOf: function() { return [1] } } a = a++"),
      nxt_string("NaN") },

    { nxt_string("var a = { valueOf: function() { return {} } } a = a++"),
      nxt_string("NaN") },

    /**/

    { nxt_string("var a = 1;   var b = a++; a +' '+ b"),
      nxt_string("2 1") },

    { nxt_string("var a = '1'; var b = a++; a +' '+ b"),
      nxt_string("2 1") },

    { nxt_string("var a = [1]; var b = a++; a +' '+ b"),
      nxt_string("2 1") },

    { nxt_string("var a = {};  var b = a++; a +' '+ b"),
      nxt_string("NaN NaN") },

    { nxt_string("var a = { valueOf: function() { return 1 } }                \
                  var b = a++; a +' '+ b"),
      nxt_string("2 1") },

    { nxt_string("var a = { valueOf: function() { return '1' } }              \
                  var b = a++; a +' '+ b"),
      nxt_string("2 1") },

    { nxt_string("var a = { valueOf: function() { return [1] } }              \
                  var b = a++; a +' '+ b"),
      nxt_string("NaN NaN") },

    { nxt_string("var a = { valueOf: function() { return {} } }               \
                  var b = a++; a +' '+ b"),
      nxt_string("NaN NaN") },

    /* Decrement. */

    { nxt_string("var a = 1;   --a"),
      nxt_string("0") },

    { nxt_string("var a = '1'; --a"),
      nxt_string("0") },

    { nxt_string("var a = [1]; --a"),
      nxt_string("0") },

    { nxt_string("var a = {};  --a"),
      nxt_string("NaN") },

    { nxt_string("var a = { valueOf: function() { return 1} };   --a"),
      nxt_string("0") },

    { nxt_string("var a = { valueOf: function() { return '1'} }; --a"),
      nxt_string("0") },

    { nxt_string("var a = { valueOf: function() { return [1]} }; --a"),
      nxt_string("NaN") },

    { nxt_string("var a = { valueOf: function() { return {} } }; --a"),
      nxt_string("NaN") },

    /**/

    { nxt_string("var a = 1;   a = --a"),
      nxt_string("0") },

    { nxt_string("var a = '1'; a = --a"),
      nxt_string("0") },

    { nxt_string("var a = [1]; a = --a"),
      nxt_string("0") },

    { nxt_string("var a = {};  a = --a"),
      nxt_string("NaN") },

    { nxt_string("var a = { valueOf: function() { return 1} }   a = --a"),
      nxt_string("0") },

    { nxt_string("var a = { valueOf: function() { return '1'} } a = --a"),
      nxt_string("0") },

    { nxt_string("var a = { valueOf: function() { return [1]} } a = --a"),
      nxt_string("NaN") },

    { nxt_string("var a = { valueOf: function() { return {} } } a = --a"),
      nxt_string("NaN") },

    /**/

    { nxt_string("var a = 1;   var b = --a; a +' '+ b"),
      nxt_string("0 0") },

    { nxt_string("var a = '1'; var b = --a; a +' '+ b"),
      nxt_string("0 0") },

    { nxt_string("var a = [1]; var b = --a; a +' '+ b"),
      nxt_string("0 0") },

    { nxt_string("var a = {};  var b = --a; a +' '+ b"),
      nxt_string("NaN NaN") },

    { nxt_string("var a = { valueOf: function() { return 1 } }                \
                  var b = --a; a +' '+ b"),
      nxt_string("0 0") },

    { nxt_string("var a = { valueOf: function() { return '1' } }              \
                  var b = --a; a +' '+ b"),
      nxt_string("0 0") },

    { nxt_string("var a = { valueOf: function() { return [1] } }              \
                  var b = --a; a +' '+ b"),
      nxt_string("NaN NaN") },

    { nxt_string("var a = { valueOf: function() { return {} } }               \
                  var b = --a; a +' '+ b"),
      nxt_string("NaN NaN") },

    /* Post decrement. */

    { nxt_string("var a = 1;   a--"),
      nxt_string("1") },

    { nxt_string("var a = '1'; a--"),
      nxt_string("1") },

    { nxt_string("var a = [1]; a--"),
      nxt_string("1") },

    { nxt_string("var a = {};  a--"),
      nxt_string("NaN") },

    { nxt_string("var a = { valueOf: function() { return 1 } }    a--"),
      nxt_string("1") },

    { nxt_string("var a = { valueOf: function() { return '1' } }  a--"),
      nxt_string("1") },

    { nxt_string("var a = { valueOf: function() { return [1] } }  a--"),
      nxt_string("NaN") },

    { nxt_string("var a = { valueOf: function() { return {} } }  a--"),
      nxt_string("NaN") },

    /**/

    { nxt_string("var a = 1;   a = a--"),
      nxt_string("1") },

    { nxt_string("var a = '1'; a = a--"),
      nxt_string("1") },

    { nxt_string("var a = [1]; a = a--"),
      nxt_string("1") },

    { nxt_string("var a = {};  a = a--"),
      nxt_string("NaN") },

    { nxt_string("var a = { valueOf: function() { return 1 } }   a = a--"),
      nxt_string("1") },

    { nxt_string("var a = { valueOf: function() { return '1' } } a = a--"),
      nxt_string("1") },

    { nxt_string("var a = { valueOf: function() { return [1] } } a = a--"),
      nxt_string("NaN") },

    { nxt_string("var a = { valueOf: function() { return {} } } a = a--"),
      nxt_string("NaN") },

    /**/

    { nxt_string("var a = 1;   var b = a--; a +' '+ b"),
      nxt_string("0 1") },

    { nxt_string("var a = '1'; var b = a--; a +' '+ b"),
      nxt_string("0 1") },

    { nxt_string("var a = [1]; var b = a--; a +' '+ b"),
      nxt_string("0 1") },

    { nxt_string("var a = {};  var b = a--; a +' '+ b"),
      nxt_string("NaN NaN") },

    { nxt_string("var a = { valueOf: function() { return 1 } }                \
                  var b = a--; a +' '+ b"),
      nxt_string("0 1") },

    { nxt_string("var a = { valueOf: function() { return '1' } }              \
                  var b = a--; a +' '+ b"),
      nxt_string("0 1") },

    { nxt_string("var a = { valueOf: function() { return [1] } }              \
                  var b = a--; a +' '+ b"),
      nxt_string("NaN NaN") },

    { nxt_string("var a = { valueOf: function() { return {} } }               \
                  var b = a--; a +' '+ b"),
      nxt_string("NaN NaN") },

    /**/

    { nxt_string("a = 2; b = ++a + ++a; a + ' ' + b"),
      nxt_string("4 7") },

    { nxt_string("a = 2; b = a++ + a++; a + ' ' + b"),
      nxt_string("4 5") },

    { nxt_string("a = b = 7; a +' '+ b"),
      nxt_string("7 7") },

    { nxt_string("a = b = c = 5; a +' '+ b +' '+ c"),
      nxt_string("5 5 5") },

    { nxt_string("a = b = (c = 5) + 2; a +' '+ b +' '+ c"),
      nxt_string("7 7 5") },

    { nxt_string("1, 2 + 5, 3"),
      nxt_string("3") },

    { nxt_string("a = 1 /* YES */\n b = a + 2 \n \n + 1 \n + 3"),
      nxt_string("7") },

    { nxt_string("a = 1 // YES \n b = a + 2 \n \n + 1 \n + 3"),
      nxt_string("7") },

    { nxt_string("a = 0; ++ \n a"),
      nxt_string("1") },

    { nxt_string("a = 0; a \n ++"),
      nxt_string("SyntaxError") },

    { nxt_string("a = 1 ? 2 \n : 3"),
      nxt_string("2") },

    { nxt_string("a = 0 / 0; b = 1 / 0; c = -1 / 0; a +' '+ b +' '+ c"),
      nxt_string("NaN Infinity -Infinity") },

    { nxt_string("a = (b = 7) + 5; var c; a +' '+ b +' '+ c"),
      nxt_string("12 7 undefined") },

    { nxt_string("var a, b = 1, c; a +' '+ b +' '+ c"),
      nxt_string("undefined 1 undefined") },

    { nxt_string("var a = 1, b = a + 1; a +' '+ b"),
      nxt_string("1 2") },

    { nxt_string("a = a = 1"),
      nxt_string("1") },

    { nxt_string("var a = 1, \n b; a +' '+ b"),
      nxt_string("1 undefined") },

    { nxt_string("a = b + 1; var b; a +' '+ b"),
      nxt_string("NaN undefined") },

    { nxt_string("var a += 1"),
      nxt_string("SyntaxError") },

    { nxt_string("var a = a + 1"),
      nxt_string("NaN") },

    { nxt_string("a = b + 1; var b = 1; a +' '+ b"),
      nxt_string("NaN 1") },

    { nxt_string("(a) = 1"),
      nxt_string("1") },

    { nxt_string("a"),
      nxt_string("ReferenceError") },

    { nxt_string("a + a"),
      nxt_string("ReferenceError") },

    { nxt_string("a = b + 1"),
      nxt_string("ReferenceError") },

    { nxt_string("a = a + 1"),
      nxt_string("ReferenceError") },

    { nxt_string("a += 1"),
      nxt_string("ReferenceError") },

    { nxt_string("a += 1; var a = 2"),
      nxt_string("2") },

    { nxt_string("var a = 1"),
      nxt_string("1") },

    { nxt_string("var a = 1; a = (a = 2) + a"),
      nxt_string("4") },

    { nxt_string("var a = 1; a = a + (a = 2)"),
      nxt_string("3") },

    { nxt_string("var a = 1; a += (a = 2)"),
      nxt_string("3") },

    { nxt_string("var a = b = 1; a +' '+ b"),
      nxt_string("1 1") },

    { nxt_string("var a \n if (!a) a = 3; a"),
      nxt_string("3") },

    { nxt_string("a = 3; if (true) if (false); else a = 2; a"),
      nxt_string("2") },

    { nxt_string("for (i = 0; i < 10; i++) { i += 1 } i"),
      nxt_string("10") },

    /* Factorial. */

    { nxt_string("n = 5; f = 1; while (n--) f *= n + 1; f"),
      nxt_string("120") },

    { nxt_string("n = 5; f = 1; while (n) { f *= n; n-- } f"),
      nxt_string("120") },

    /* Fibonacci. */

    { nxt_string("var n = 50, x; \
                  for(i=0,j=1,k=0; k<n; i=j,j=x,k++ ){ x=i+j } x"),
      nxt_string("20365011074") },

    { nxt_string("3 + 'abc' + 'def' + null + true + false + undefined"),
      nxt_string("3abcdefnulltruefalseundefined") },

    { nxt_string("a = 0; do a++; while (a < 5) if (a == 5) a = 7.33 \n\
                  else a = 8; while (a < 10) a++; a"),
      nxt_string("10.33") },

    /* typeof. */

    { nxt_string("typeof null"),
      nxt_string("object") },

    { nxt_string("typeof undefined"),
      nxt_string("undefined") },

    { nxt_string("typeof false"),
      nxt_string("boolean") },

    { nxt_string("typeof true"),
      nxt_string("boolean") },

    { nxt_string("typeof 0"),
      nxt_string("number") },

    { nxt_string("typeof -1"),
      nxt_string("number") },

    { nxt_string("typeof Infinity"),
      nxt_string("number") },

    { nxt_string("typeof NaN"),
      nxt_string("number") },

    { nxt_string("typeof 'a'"),
      nxt_string("string") },

    { nxt_string("typeof {}"),
      nxt_string("object") },

    { nxt_string("typeof Object()"),
      nxt_string("object") },

    { nxt_string("typeof []"),
      nxt_string("object") },

    { nxt_string("typeof function(){}"),
      nxt_string("function") },

    { nxt_string("typeof Object"),
      nxt_string("function") },

    { nxt_string("typeof /./i"),
      nxt_string("object") },

    { nxt_string("typeof $r"),
      nxt_string("undefined") },

    { nxt_string("typeof a"),
      nxt_string("undefined") },

    { nxt_string("typeof a; a"),
      nxt_string("ReferenceError") },

    { nxt_string("typeof a; a = 1"),
      nxt_string("1") },

    /**/

    { nxt_string("void 0"),
      nxt_string("undefined") },

    { nxt_string("undefined = 1"),
      nxt_string("SyntaxError") },

    { nxt_string("var a; b = a; a = 1; a +' '+ b"),
      nxt_string("1 undefined") },

    { nxt_string("a = 1"),
      nxt_string("1") },

    { nxt_string("a; a = 1; a"),
      nxt_string("ReferenceError") },

    { nxt_string("a = {}; typeof a +' '+ a"),
      nxt_string("object [object Object]") },

    { nxt_string("a = {}; a.b"),
      nxt_string("undefined") },

    { nxt_string("a = {}; a.b = 1 + 2; a.b"),
      nxt_string("3") },

    { nxt_string("a = {}; a['b']"),
      nxt_string("undefined") },

    { nxt_string("a = {}; a.b.c"),
      nxt_string("TypeError") },

    { nxt_string("a = {}; a.b = 1; a.b"),
      nxt_string("1") },

    { nxt_string("a = {}; a.b = 1; a.b += 2"),
      nxt_string("3") },

    { nxt_string("a = {}; a.b = 1; a.b += a.b"),
      nxt_string("2") },

    { nxt_string("a = {}; a.b = 1; x = {}; x.b = 3; a.b += (x.b = 2)"),
      nxt_string("3") },

    { nxt_string("a = {}; a.b = 1; a.b += (a.b = 2)"),
      nxt_string("3") },

    { nxt_string("a = {}; a.b += 1"),
      nxt_string("NaN") },

    { nxt_string("a = 1; b = 2; a = b += 1"),
      nxt_string("3") },

    { nxt_string("a = 1; b = { x:2 }; a = b.x += 1"),
      nxt_string("3") },

    { nxt_string("a = 1; b = { x:2 }; a = b.x += (a = 1)"),
      nxt_string("3") },

    { nxt_string("a = 1; a.b++; a.b"),
      nxt_string("undefined") },

    { nxt_string("a = {}; a.b = {}; a.b.c = 1; a.b['c']"),
      nxt_string("1") },

    { nxt_string("a = {}; a.b = {}; a.b.c = 1; a['b']['c']"),
      nxt_string("1") },

    { nxt_string("a = {}; a.b = {}; c = 'd'; a.b.d = 1; a['b'][c]"),
      nxt_string("1") },

    { nxt_string("a = {}; a.b = 1; c = a.b++; a.b +' '+ c"),
      nxt_string("2 1") },

    { nxt_string("a = 2; a.b = 1; c = a.b++; a +' '+ a.b +' '+ c"),
      nxt_string("2 undefined NaN") },

    { nxt_string("x = { a: 1 }; x.a"),
      nxt_string("1") },

    { nxt_string("a = { x:1 }; b = { y:2 }; a.x = b.y; a.x"),
      nxt_string("2") },

    { nxt_string("a = { x:1 }; b = { y:2 }; c = a.x = b.y; c"),
      nxt_string("2") },

    { nxt_string("a = { x:1 }; b = { y:2 }; a.x = b.y"),
      nxt_string("2") },

    { nxt_string("a = { x:1 }; b = a.x = 1 + 2; a.x +' '+ b"),
      nxt_string("3 3") },

    { nxt_string("a = { x:1 }; b = { y:2 }; c = {}; c.x = a.x = b.y; c.x"),
      nxt_string("2") },

    { nxt_string("y = 2; x = { a:1, b: y + 5, c:3 }; x.a +' '+ x.b +' '+ x.c"),
      nxt_string("1 7 3") },

    { nxt_string("x = { a: 1, b: { a:2, c:5 } }; x.a +' '+ x.b.a +' '+ x.b.c"),
      nxt_string("1 2 5") },

    { nxt_string("var y = 5; x = { a:y }; x.a"),
      nxt_string("5") },

    { nxt_string("x = { a: 1, b: x.a }"),
      nxt_string("ReferenceError") },

    { nxt_string("a = { b: 2 }; a.b += 1"),
      nxt_string("3") },

    { nxt_string("o = {a:1}; c = o; o.a = o = {b:5}; \
                  o.a +' '+ o.b +' '+ c.a.b"),
      nxt_string("undefined 5 5") },

    { nxt_string("y = { a: 2 }; x = { a: 1, b: y.a }; x.a +' '+ x.b"),
      nxt_string("1 2") },

    { nxt_string("y = { a: 1 }; x = { a: y.a++, b: y.a++ }\n\
                 x.a +' '+ x.b +' '+ y.a"),
      nxt_string("1 2 3") },

    { nxt_string("var a=''; var o = {a:1, b:2} \
                  for (p in o) { a += p +':'+ o[p] +',' } a"),
      nxt_string("a:1,b:2,") },

    { nxt_string("x = { a: 1 }; b = delete x.a; x.a +' '+ b"),
      nxt_string("undefined true") },

    { nxt_string("delete null"),
      nxt_string("true") },

    { nxt_string("var a; delete a"),
      nxt_string("false") },

    { nxt_string("delete undefined"),
      nxt_string("false") },

    { nxt_string("delete NaN"),
      nxt_string("false") },

    { nxt_string("delete Infinity"),
      nxt_string("false") },

    { nxt_string("delete -Infinity"),
      nxt_string("true") },

    { nxt_string("delete (1/0)"),
      nxt_string("true") },

    { nxt_string("delete 1"),
      nxt_string("true") },

    { nxt_string("delete (a = 1); a"),
      nxt_string("1") },

    { nxt_string("delete a"),
      nxt_string("true") },

    { nxt_string("a = 1; delete a"),
      nxt_string("true") },

    { nxt_string("a = 1; delete a; typeof a"),
      nxt_string("undefined") },

    { nxt_string("a = { x:1 }; ('x' in a) +' '+ (1 in a)"),
      nxt_string("true false") },

    { nxt_string("a = {}; 1 in a"),
      nxt_string("false") },

    { nxt_string("a = 1; 1 in a"),
      nxt_string("TypeError") },

    { nxt_string("a = [ 1, 2, 3 ]; a[0] + a[1] + a[2]"),
      nxt_string("6") },

    { nxt_string("a = [ 1, 2, 3 ]; a[0] +' '+ a[1] +' '+ a[2] +' '+ a[3]"),
      nxt_string("1 2 3 undefined") },

    { nxt_string("[] - 2"),
      nxt_string("-2") },

    { nxt_string("[1] - 2"),
      nxt_string("-1") },

    { nxt_string("[[1]] - 2"),
      nxt_string("-1") },

    { nxt_string("[[[1]]] - 2"),
      nxt_string("-1") },

    { nxt_string("a = []; a - 2"),
      nxt_string("-2") },

    { nxt_string("a = [1]; a - 2"),
      nxt_string("-1") },

    { nxt_string("a = []; a[0] = 1; a - 2"),
      nxt_string("-1") },

    { nxt_string("[] + 2 + 3"),
      nxt_string("23") },

    { nxt_string("[1] + 2 + 3"),
      nxt_string("123") },

    { nxt_string("a = []; a + 2 + 3"),
      nxt_string("23") },

    { nxt_string("a = [1]; a + 2 + 3"),
      nxt_string("123") },

    { nxt_string("a = [1,2]; i = 0; a[i++] += a[0] = 5 + i; a[0] +' '+ a[1]"),
      nxt_string("7 2") },

    { nxt_string("a = []; a[0] = 1; a + 2 + 3"),
      nxt_string("123") },

    { nxt_string("a = []; a['0'] = 1; a + 2 + 3"),
      nxt_string("123") },

    { nxt_string("a = []; a[2] = 1; a[2]"),
      nxt_string("1") },

    { nxt_string("a = [1, 2]; 1 in a"),
      nxt_string("true") },

    { nxt_string("a = [1, 2]; 2 in a"),
      nxt_string("false") },

    { nxt_string("a = [1, 2]; delete a[0]; 0 in a"),
      nxt_string("false") },

    { nxt_string("var s = '', a = [5,1,2]; \
                  a[null] = null; \
                  a[undefined] = 'defined'; \
                  a[false] = false; \
                  a[true] = true; \
                  a[-0] = 0; \
                  a[Infinity] = Infinity; \
                  a[-Infinity] = -Infinity; \
                  a[NaN] = NaN; \
                  a[-NaN] = -NaN; \
                  for (i in a) { s += i +':'+ a[i] +',' } s"),
      nxt_string("0:0,1:1,2:2,null:null,undefined:defined,false:false,true:true,Infinity:Infinity,-Infinity:-Infinity,NaN:NaN,") },

    { nxt_string("[].length"),
      nxt_string("0") },

    { nxt_string("[1,2].length"),
      nxt_string("2") },

    { nxt_string("a = [1,2]; a.length"),
      nxt_string("2") },

    { nxt_string("a = [1,2,3]; a.join()"),
      nxt_string("1,2,3") },

    { nxt_string("a = [1,2,3]; a.join(':')"),
      nxt_string("1:2:3") },

    { nxt_string("a = []; a[5] = 5; a.join()"),
      nxt_string(",,,,,5") },

    { nxt_string("a = []; a[5] = 5; a +''"),
      nxt_string(",,,,,5") },

    { nxt_string("a = []; a.concat([]) +''"),
      nxt_string("") },

    { nxt_string("a = [1,2,3]; a.concat(4, [5, 6, 7], 8) +''"),
      nxt_string("1,2,3,4,5,6,7,8") },

    { nxt_string("a = []; a[100] = a.length; a[100] +' '+ a.length"),
      nxt_string("0 101") },

    { nxt_string("a = [1,2]; a[100] = 100; a[100] +' '+ a.length"),
      nxt_string("100 101") },

    { nxt_string("a = [1,2,3,4,5]; b = a.slice(3); b[0] +' '+ b[1] +' '+ b[2]"),
      nxt_string("4 5 undefined") },

    { nxt_string("a = [1,2]; a.pop() +' '+ a.length +' '+ a"),
      nxt_string("2 1 1") },

    { nxt_string("a = [1,2]; len = a.push(3); len +' '+ a.pop() +' '+ a"),
      nxt_string("3 3 1,2") },

    { nxt_string("a = [1,2]; len = a.push(3,4,5); len +' '+ a.pop() +' '+ a"),
      nxt_string("5 5 1,2,3,4") },

    { nxt_string("a = [1,2,3]; a.shift() +' '+ a[0] +' '+ a.length"),
      nxt_string("1 2 2") },

    { nxt_string("a = [1,2]; len = a.unshift(3); len +' '+ a.shift()"),
      nxt_string("3 3") },

    { nxt_string("a = [1,2]; len = a.unshift(3,4,5); len +' '+ a.shift()"),
      nxt_string("5 3") },

    { nxt_string("var a = []; var s = { sum: 0 }; \
                  a.forEach(function(v, i, a) { this.sum += v }, s); s.sum"),
      nxt_string("0") },

    { nxt_string("var a = new Array(3); var s = { sum: 0 }; \
                  a.forEach(function(v, i, a) { this.sum += v }, s); s.sum"),
      nxt_string("0") },

#if 0
    { nxt_string("var a = [,,,]; var s = { sum: 0 }; \
                  a.forEach(function(v, i, a) { this.sum += v }, s); s.sum"),
      nxt_string("0") },
#endif

    { nxt_string("var a = [1,2,3]; var s = { sum: 0 }; \
                  a.forEach(function(v, i, a) { this.sum += v }, s); s.sum"),
      nxt_string("6") },

    { nxt_string("var a = [1,2,3]; \
                  a.forEach(function(v, i, a) { a[i+3] = a.length }); \
                  a +''"),
      nxt_string("1,2,3,3,4,5") },

    { nxt_string("var a = []; \
                  a.some(function(v, i, a) { return v > 1 })"),
      nxt_string("false") },

    { nxt_string("var a = [1,2,3]; \
                  a.some(function(v, i, a) { return v > 1 })"),
      nxt_string("true") },

    { nxt_string("var a = [1,2,3]; \
                  a.some(function(v, i, a) { return v > 2 })"),
      nxt_string("true") },

    { nxt_string("var a = [1,2,3]; \
                  a.some(function(v, i, a) { return v > 3 })"),
      nxt_string("false") },

    { nxt_string("var a = []; \
                  a.every(function(v, i, a) { return v > 1 })"),
      nxt_string("true") },

    { nxt_string("var a = [3,2,1]; \
                  a.every(function(v, i, a) { return v > 3 })"),
      nxt_string("false") },

    { nxt_string("var a = [3,2,1]; \
                  a.every(function(v, i, a) { return v > 2 })"),
      nxt_string("false") },

    { nxt_string("var a = [3,2,1]; \
                  a.every(function(v, i, a) { return v > 0 })"),
      nxt_string("true") },

    { nxt_string("var a = '0123456789' + '012345'  \
                  var b = 'abcdefghij' + 'klmnop'  \
                      a = b"),
      nxt_string("abcdefghijklmnop") },

    { nxt_string("''.length"),
      nxt_string("0") },

    { nxt_string("'abc'.length"),
      nxt_string("3") },

    { nxt_string("'絵文字'.length"),
      nxt_string("3") },

    { nxt_string("'えもじ'.length"),
      nxt_string("3") },

    { nxt_string("'囲碁織'.length"),
      nxt_string("3") },

    { nxt_string("a = 'abc'; a.length"),
      nxt_string("3") },

    { nxt_string("a = 'abc'; a['length']"),
      nxt_string("3") },

    { nxt_string("a = 'абв'; a.length"),
      nxt_string("3") },

    { nxt_string("a = 'abc' + 'абв'; a.length"),
      nxt_string("6") },

    { nxt_string("a = 'abc' + 1 + 'абв'; a +' '+ a.length"),
      nxt_string("abc1абв 7") },

    /* TODO: '\u00C2\u00B6'.bytes */

    { nxt_string("a = '\xC3\x82\xC2\xB6'.bytes; u = a.utf8; \
                  a.length +' '+ a +' '+ u.length +' '+ u"),
      nxt_string("2 \xC2\xB6 1 \xC2\xB6") },

    { nxt_string("a = 1; a.length"),
      nxt_string("undefined") },

    { nxt_string("a = 'abc'; a.concat('абв', 123)"),
      nxt_string("abcабв123") },

    { nxt_string("a = $r.uri; s = a.utf8; s.length +' '+ s"),
      nxt_string("3 АБВ") },

    { nxt_string("a = $r.uri; a +' '+ a.length +' '+ a"),
      nxt_string("АБВ 6 АБВ") },

    { nxt_string("$r.uri = 'αβγ'; a = $r.uri; a.length +' '+ a"),
      nxt_string("6 αβγ") },

    { nxt_string("$r.uri.length +' '+ $r.uri"),
      nxt_string("6 αβγ") },

    { nxt_string("$r.uri = $r.uri.substr(2); $r.uri.length +' '+ $r.uri"),
      nxt_string("4 βγ") },

    { nxt_string("a = $r.host; a +' '+ a.length +' '+ a"),
      nxt_string("АБВГДЕЁЖЗИЙ 22 АБВГДЕЁЖЗИЙ") },

    { nxt_string("a = $r.host; a.substr(2, 2)"),
      nxt_string("Б") },

    { nxt_string("a = $r.header['User-Agent']; a +' '+ a.length +' '+ a"),
      nxt_string("User-Agent|АБВ 17 User-Agent|АБВ") },

    { nxt_string("var a=''; \
                  for (p in $r.header) { a += p +':'+ $r.header[p] +',' }\
                  a"),
      nxt_string("01:01|АБВ,02:02|АБВ,03:03|АБВ,") },

#if 1
    { nxt_string("$r.nonexistent"),
      nxt_string("undefined") },
#endif

    { nxt_string("a = 'abcdefgh'; a.substr(3, 15)"),
      nxt_string("defgh") },

    { nxt_string("'abcdefgh'.substr(3, 15)"),
      nxt_string("defgh") },

    { nxt_string("'abcdefghijklmno'.substr(3, 4)"),
      nxt_string("defg") },

    { nxt_string("'abcdefghijklmno'.substr(-3, 2)"),
      nxt_string("mn") },

    { nxt_string("'abcdefgh'.substr(100, 120)"),
      nxt_string("") },

    { nxt_string("('abc' + 'defgh').substr(1, 4)"),
      nxt_string("bcde") },

    { nxt_string("'abcdefghijklmno'.substring(3, 5)"),
      nxt_string("de") },

    { nxt_string("'abcdefgh'.substring(3)"),
      nxt_string("defgh") },

    { nxt_string("'abcdefgh'.substring(5, 3)"),
      nxt_string("de") },

    { nxt_string("'abcdefgh'.substring(100, 120)"),
      nxt_string("") },

    { nxt_string("'abcdefghijklmno'.slice(NaN, 5)"),
      nxt_string("abcde") },

    { nxt_string("'abcdefghijklmno'.slice('0', '5')"),
      nxt_string("abcde") },

    { nxt_string("'abcdefghijklmno'.slice(3, 5)"),
      nxt_string("de") },

    { nxt_string("'abcdefgh'.slice(3)"),
      nxt_string("defgh") },

    { nxt_string("'abcdefgh'.slice(3, -2)"),
      nxt_string("def") },

    { nxt_string("'abcdefgh'.slice(5, 3)"),
      nxt_string("") },

    { nxt_string("'abcdefgh'.slice(100, 120)"),
      nxt_string("") },

    { nxt_string("'abc'.charAt(1 + 1)"),
      nxt_string("c") },

    { nxt_string("'abc'.charAt(3)"),
      nxt_string("") },

    { nxt_string("'abc'.charCodeAt(1 + 1)"),
      nxt_string("99") },

    { nxt_string("'abc'.charCodeAt(3)"),
      nxt_string("NaN") },

    { nxt_string("a = 'abcdef'; a.3"),
      nxt_string("SyntaxError") },

    { nxt_string("'abcdef'[3]"),
      nxt_string("d") },

    { nxt_string("'abcdef'[0]"),
      nxt_string("a") },

    { nxt_string("'abcdef'[-1]"),
      nxt_string("undefined") },

    { nxt_string("'abcdef'[NaN]"),
      nxt_string("undefined") },

    { nxt_string("'abcdef'[3.5]"),
      nxt_string("undefined") },

    { nxt_string("'abcdef'[8]"),
      nxt_string("undefined") },

    { nxt_string("a = 'abcdef'; b = 1 + 2; a[b]"),
      nxt_string("d") },

    { nxt_string("'абвгдеёжзийклмнопрстуфхцчшщъыьэюя'.charCodeAt(5)"),
      nxt_string("1077") },

    { nxt_string("'12345абвгдеёжзийклмнопрстуфхцчшщъыьэюя'.charCodeAt(35)"),
      nxt_string("1101") },

    { nxt_string("'12345абвгдеёжзийклмнопрстуфхцчшщъыьэюя'.substring(35)"),
      nxt_string("эюя") },

    { nxt_string("'abcdef'.substr(-5, 4).substring(3, 1).charAt(1)"),
      nxt_string("d") },

    { nxt_string("'abcdef'.substr(2, 4).charAt(2)"),
      nxt_string("e") },

    { nxt_string("a = 'abcdef'.substr(2, 4).charAt(2).length"),
      nxt_string("1") },

    { nxt_string("a = 'abcdef'.substr(2, 4).charAt(2) + '1234'"),
      nxt_string("e1234") },

    { nxt_string("a = ('abcdef'.substr(2, 5 * 2 - 6).charAt(2) + '1234') \
                      .length"),
      nxt_string("5") },

    { nxt_string("a = 'abcdef'; function f(a) { \
                  return a.slice(a.indexOf('cd')) } f(a)"),
      nxt_string("cdef") },

    { nxt_string("a = 'abcdef'; a.slice(a.indexOf('cd'))"),
      nxt_string("cdef") },

    { nxt_string("'abcdef'.indexOf('de', 2)"),
      nxt_string("3") },

    { nxt_string("'абвгдеёжзийклмнопрстуфхцчшщъыьэюя'.indexOf('лмно', 10)"),
      nxt_string("12") },

    { nxt_string("'abcdef'.indexOf('a', 10)"),
      nxt_string("-1") },

    { nxt_string("'abcdef'.indexOf('q', 0)"),
      nxt_string("-1") },

    { nxt_string("'abcdef'.indexOf('', 10)"),
      nxt_string("6") },

    { nxt_string("'abcdef'.indexOf('', 3)"),
      nxt_string("3") },

    { nxt_string("'abc abc abc abc'.lastIndexOf('abc')"),
      nxt_string("12") },

    { nxt_string("'abc abc abc abc'.lastIndexOf('abc', 11)"),
      nxt_string("8") },

    { nxt_string("'abc abc abc abc'.lastIndexOf('abc', 0)"),
      nxt_string("-1") },

    { nxt_string("'abcdefgh'.search()"),
      nxt_string("0") },

    { nxt_string("'abcdefgh'.search(/def/)"),
      nxt_string("3") },

    { nxt_string("''.match(/^$/) +''"),
      nxt_string("") },

    { nxt_string("''.match(/^$/g) +''"),
      nxt_string("") },

    { nxt_string("'abcdefgh'.match(/def/) +''"),
      nxt_string("def") },

    { nxt_string("'abc ABC aBc'.match(/abc/ig) +''"),
      nxt_string("abc,ABC,aBc") },

    { nxt_string("var q = 1; function x(a, b, c) { q = a } x(5); q"),
      nxt_string("5") },

    { nxt_string("function x(a) { while (a < 2) a++; return a + 1 } x(1) "),
      nxt_string("3") },

    /* Recursive factorial. */

    { nxt_string("function f(a) {                       \
                      if (a > 1)                        \
                          return a * f(a - 1)           \
                      return 1                          \
                  }                                     \
                  f(10)"),
      nxt_string("3628800") },

    /* Recursive factorial. */

    { nxt_string("function f(a) { return (a > 1) ? a * f(a - 1) : 1 } f(10)"),
      nxt_string("3628800") },

    /* Recursive fibonacci. */

    { nxt_string("function fibo(n) {                    \
                      if (n > 1)                        \
                          return fibo(n-1) + fibo(n-2)  \
                       return 1                         \
                  }                                     \
                  fibo(10)"),
      nxt_string("89") },

    { nxt_string("function fibo(n) {                    \
                      if (n > 1)                        \
                          return fibo(n-1) + fibo(n-2)  \
                       return '.'                       \
                  }                                     \
                  fibo(10).length"),
      nxt_string("89") },

    { nxt_string("function fibo(n) {                    \
                      if (n > 1)                        \
                          return fibo(n-1) + fibo(n-2)  \
                       return 1                         \
                  }                                     \
                  fibo('10')"),
      nxt_string("89") },

    { nxt_string("function add(a, b) { return a + b }           \
                  function mul(a, b) { return a * b }           \
                  function f(a, b) {                            \
                      return a + mul(add(1, 2), add(2, 3)) + b  \
                  }                                             \
                  f(30, 70)"),
      nxt_string("115") },

    { nxt_string("function a(x, y) { return x + y }     \
                  function b(x, y) { return x * y }     \
                  a(3, b(4, 5))"),
      nxt_string("23") },

    { nxt_string("function x(n) { return n }; x('12'.substr(1))"),
      nxt_string("2") },

    { nxt_string("function f(a) { a *= 2 } f(10)"),
      nxt_string("undefined") },

    { nxt_string("function f() { return 5 } f()"),
      nxt_string("5") },

    { nxt_string("function f() { return 5 } f(1)"),
      nxt_string("5") },

    { nxt_string("function f() {} f()"),
      nxt_string("undefined") },

    { nxt_string("function f(a) { return a + 1 } b = f(2)"),
      nxt_string("3") },

    { nxt_string("f = function(a) { a *= 2; return a }; f(10)"),
      nxt_string("20") },

    { nxt_string("var f = function b(a) { a *= 2; return a }; f(10)"),
      nxt_string("20") },

    { nxt_string("var f = function b(a) { a *= 2; return a }; b(10)"),
      nxt_string("ReferenceError") },

    { nxt_string("var f = function(a) { a *= 2; return a }; f(10)"),
      nxt_string("20") },

    { nxt_string("f = function b(a) { a *= 2; return a }; b(10)"),
      nxt_string("ReferenceError") },

    { nxt_string("f = function b(a) { a *= 2; return a }; f(10)"),
      nxt_string("20") },

    { nxt_string("f = a = function(a) { a *= 2; return a }; f(10)"),
      nxt_string("20") },

    { nxt_string("f = a = function(a) { a *= 2; return a }; a(10)"),
      nxt_string("20") },

    { nxt_string("var f = function b(a) { a *= 2; return a } = 5"),
      nxt_string("SyntaxError") },

    { nxt_string("function a() { return { x:2} }; var b = a(); b.x"),
      nxt_string("2") },

    { nxt_string("a = {}; function f(a) { return a + 1 } a.b = f(2); a.b"),
      nxt_string("3") },

    { nxt_string("a = (function() { return 1 })(); a"),
      nxt_string("1") },

    { nxt_string("a = (function(a) { return a + 1 })(2); a"),
      nxt_string("3") },

    { nxt_string("a = (function(a) { return a + 1 }(2)); a"),
      nxt_string("3") },

    { nxt_string("a = !function(a) { return a + 1 }(2); a"),
      nxt_string("false") },

    { nxt_string("a = +function(a) { return a + 1 }(2); a"),
      nxt_string("3") },

    { nxt_string("a = true && function(a) { return a + 1 }(2); a"),
      nxt_string("3") },

    { nxt_string("a = 0, function(a) { return a + 1 }(2); a"),
      nxt_string("0") },

    { nxt_string("var o = { f: function(a) { return a * 2 } }; o.f(5)"),
      nxt_string("10") },

    { nxt_string("var o = {}; o.f = function(a) { return a * 2 }; o.f(5)"),
      nxt_string("10") },

    { nxt_string("var o = { x: 1, f: function() { return this.x } } o.f()"),
      nxt_string("1") },

    { nxt_string("var o = { x: 1, f: function(a) { return this.x += a } } \
                  o.f(5) +' '+ o.x"),
      nxt_string("6 6") },

    { nxt_string("var f = function(a) { return 3 } f.call()"),
      nxt_string("3") },

    { nxt_string("var f = function(a) { return this } f.call(5)"),
      nxt_string("5") },

    { nxt_string("var f = function(a, b) { return this + a } f.call(5, 1)"),
      nxt_string("6") },

    { nxt_string("var f = function(a, b) { return this + a + b } \
                  f.call(5, 1, 2)"),
      nxt_string("8") },

    { nxt_string("var f = function(a) { return 3 } f.apply()"),
      nxt_string("3") },

    { nxt_string("var f = function(a) { return this } f.apply(5)"),
      nxt_string("5") },

    { nxt_string("var f = function(a) { return this + a } f.apply(5, 1)"),
      nxt_string("TypeError") },

    { nxt_string("var f = function(a, b) { return this + a + b } \
                  f.apply(5, [1, 2])"),
      nxt_string("8") },

    { nxt_string("var f = function(a, b) { return this + a + b } \
                  f.apply(5, [1, 2], 3)"),
      nxt_string("8") },

    { nxt_string("''.concat.call()"),
      nxt_string("TypeError") },

    { nxt_string("''.concat.call('a', 'b', 'c')"),
      nxt_string("abc") },

    { nxt_string("''.concat.call('a')"),
      nxt_string("a") },

    { nxt_string("''.concat.call('a', [ 'b', 'c' ])"),
      nxt_string("ab,c") },

    { nxt_string("''.concat.call('a', [ 'b', 'c' ], 'd')"),
      nxt_string("ab,cd") },

    { nxt_string("''.concat.apply()"),
      nxt_string("TypeError") },

    { nxt_string("''.concat.apply('a')"),
      nxt_string("a") },

    { nxt_string("''.concat.apply('a', 'b')"),
      nxt_string("TypeError") },

    { nxt_string("''.concat.apply('a', [ 'b', 'c' ])"),
      nxt_string("abc") },

    { nxt_string("''.concat.apply('a', [ 'b', 'c' ], 'd')"),
      nxt_string("abc") },

    { nxt_string("[].join.call([1,2,3])"),
      nxt_string("1,2,3") },

    { nxt_string("[].join.call([1,2,3], ':')"),
      nxt_string("1:2:3") },

    { nxt_string("[].join.call()"),
      nxt_string("TypeError") },

    { nxt_string("function F(a, b) { this.a = a + b } \
                  var o = new F(1, 2) \
                  o.a"),
      nxt_string("3") },

    { nxt_string("function F(a, b) { this.a = a + b; return { a: 7 } } \
                  var o = new F(1, 2) \
                  o.a"),
      nxt_string("7") },

    { nxt_string("function a() { return function(x) { return x + 1 } } \
                  b = a(); b(2)"),
      nxt_string("3") },

    { nxt_string("/^$/.test('')"),
      nxt_string("true") },

    { nxt_string("var a = /\\d/; a.test('123')"),
      nxt_string("true") },

    { nxt_string("var a = /\\d/; a.test('abc')"),
      nxt_string("false") },

    { nxt_string("/\\d/.test('123')"),
      nxt_string("true") },

    { nxt_string("/\\d/.test('abc')"),
      nxt_string("false") },

    { nxt_string("/abc/i.test('ABC')"),
      nxt_string("true") },

    { nxt_string("/абв/i.test('АБВ')"),
      nxt_string("true") },

    /* TODO: '\u00C2\u00B6".bytes */

    { nxt_string("/\xC2\xB6/.test('\xC3\x82\xC2\xB6'.bytes)"),
      nxt_string("true") },

    { nxt_string("/\\x80/.test('\x80'.bytes)"),
      nxt_string("true") },

    { nxt_string("var a = /^$/.exec(''); a.length +' '+ a"),
      nxt_string("1 ") },

    { nxt_string("var r = /бв/ig; var a = r.exec('АБВ'); r.lastIndex +' '+ a"),
      nxt_string("3 БВ") },

    { nxt_string("var r = /\\x80/g; r.exec('\x81\x80'.bytes); r.lastIndex"),
      nxt_string("1") },

    /*
     * It seems that "/стоп/ig" fails on early PCRE versions.
     * It fails at least in 8.1 and works at least in 8.31.
     */

    { nxt_string("var r = /Стоп/ig; \
                  var a = r.exec('АБВДЕЁЖЗИКЛМНОПРСТУФХЦЧШЩЬЫЪЭЮЯСТОП'); \
                  r.lastIndex +' '+ a"),
      nxt_string("35 СТОП") },

    { nxt_string("var r = /quick\\s(brown).+?(jumps)/ig \
                 var a = r.exec('The Quick Brown Fox Jumps Over The Lazy Dog') \
                 a[0] +' '+ a[1] +' '+ a[2] +' '+ a[3] +' '+ \
                 a.index +' '+ r.lastIndex +' '+ a.input"),
      nxt_string("Quick Brown Fox Jumps Brown Jumps undefined 4 25 The Quick Brown Fox Jumps Over The Lazy Dog") },

    { nxt_string("var s; var r = /./g; while (s = r.exec('abc')); s"),
      nxt_string("null") },

    { nxt_string("var r = /LS/i.exec(false); r[0]"),
      nxt_string("ls") },

    /* Non-standard ECMA-262 features. */

    /* 0x10400 is not a surrogate pair of 0xD801 and 0xDC00. */

    { nxt_string("var chars = '𐐀'; chars.length +' '+ chars.charCodeAt(0)"),
      nxt_string("1 66560") },

    /* es5id: 6.1, 0x104A0 is not a surrogate pair of 0xD801 and 0xDCA0. */

    { nxt_string("var chars = '𐒠'; chars.length +' '+ chars.charCodeAt(0)"),
      nxt_string("1 66720") },

    /* Exceptions. */

    { nxt_string("throw null"),
      nxt_string("") },

    { nxt_string("var a; try { throw null } catch (e) { a = e } a"),
      nxt_string("null") },

    { nxt_string("try { throw null } catch (e) { throw e }"),
      nxt_string("") },

    { nxt_string("var a = 0; try { a = 5 } \
                  catch (e) { a = 9 } finally { a++ } a"),
      nxt_string("6") },

    { nxt_string("var a = 0; try { throw 3 } \
                  catch (e) { a = e } finally { a++ } a"),
      nxt_string("4") },

    { nxt_string("var a = 0; try { throw 3 } \
                  catch (e) { throw e + 1 } finally { a++ }"),
      nxt_string("") },

    { nxt_string("var a = 0; try { throw 3 } \
                  catch (e) { a = e } finally { throw a }"),
      nxt_string("") },

    { nxt_string("try { throw null } catch (e) { } finally { }"),
      nxt_string("undefined") },

    { nxt_string("var a = 0; try { throw 3 } \
                  catch (e) { throw 4 } finally { throw a }"),
      nxt_string("") },

    { nxt_string("var a = 0; try { a = 5 } finally { a++ } a"),
      nxt_string("6") },

    { nxt_string("var a = 0; try { throw 5 } finally { a++ }"),
      nxt_string("") },

    { nxt_string("var a = 0; try { a = 5 } finally { throw 7 }"),
      nxt_string("") },

    { nxt_string("function f(a) {                    \
                     if (a > 1) return f(a - 1);     \
                     throw 9; return a }             \
                  var a = 0; try { a = f(5); a++ } catch(e) { a = e } a"),
      nxt_string("9") },

    { nxt_string("var a; try { try { throw 5 } catch (e) { a = e } throw 3 } \
                         catch(x) { a += x } a"),
      nxt_string("8") },

    { nxt_string("var o = { valueOf: function() { return '3' } }; --o"),
      nxt_string("2") },

    { nxt_string("var o = { valueOf: function() { return [3] } }; --o"),
      nxt_string("NaN") },

    { nxt_string("var o = { valueOf: function() { return '3' } } 10 - o"),
      nxt_string("7") },

    { nxt_string("var o = { valueOf: function() { return [3] } } 10 - o"),
      nxt_string("NaN") },

    { nxt_string("var o = { toString: function() { return 'OK' } } 'o:' + o"),
      nxt_string("o:OK") },

    { nxt_string("var o = { toString: function() { return [1] } } 'o:' + o"),
      nxt_string("TypeError") },

    { nxt_string("var a = { valueOf: function() { return '3' } }             \
                  var b = { toString: function() { return 10 - a + 'OK' } }  \
                  var c = { toString: function() { return b + 'YES' } }      \
                  'c:' + c"),
      nxt_string("c:7OKYES") },

    { nxt_string("[1,2,3].valueOf()"),
      nxt_string("1,2,3") },

    { nxt_string("var o = { valueOf: function() { return 'OK' } } o.valueOf()"),
      nxt_string("OK") },

    { nxt_string("[].__proto__ === [1,2].__proto__"),
      nxt_string("true") },

    { nxt_string("/./.__proto__ === /a/.__proto__"),
      nxt_string("true") },

    { nxt_string("''.__proto__ === 'abc'.__proto__"),
      nxt_string("true") },

    { nxt_string("[].__proto__.join.call([1,2,3], ':')"),
      nxt_string("1:2:3") },

    { nxt_string("''.__proto__.concat.call('a', 'b', 'c')"),
      nxt_string("abc") },

    { nxt_string("/./.__proto__.test.call(/a{2}/, 'aaa')"),
      nxt_string("true") },

    { nxt_string("({}) instanceof Object"),
      nxt_string("true") },

    { nxt_string("[] instanceof Array"),
      nxt_string("true") },

    { nxt_string("[] instanceof Object"),
      nxt_string("true") },

    { nxt_string("var o = Object(); o"),
      nxt_string("[object Object]") },

    { nxt_string("var o = new Object(); o"),
      nxt_string("[object Object]") },

    { nxt_string("var o = new Object(1); o +''"),
      nxt_string("1") },

    { nxt_string("var o = {}; o === Object(o)"),
      nxt_string("true") },

    { nxt_string("var o = {}; o === new Object(o)"),
      nxt_string("true") },

    { nxt_string("Object.name"),
      nxt_string("Object") },

    { nxt_string("Object.prototype.constructor === Object"),
      nxt_string("true") },

    { nxt_string("Object.constructor === Function"),
      nxt_string("true") },

    { nxt_string("var a = Array(3); a +''"),
      nxt_string(",,") },

    { nxt_string("var a = Array(); a.length"),
      nxt_string("0") },

    { nxt_string("var a = Array(0); a.length"),
      nxt_string("0") },

    { nxt_string("var a = Array(true); a +''"),
      nxt_string("true") },

    { nxt_string("var a = Array(1,'two',3); a +''"),
      nxt_string("1,two,3") },

    { nxt_string("var a = Array(-1)"),
      nxt_string("RangeError") },

    { nxt_string("var a = Array(2.5)"),
      nxt_string("RangeError") },

    { nxt_string("var a = Array(NaN)"),
      nxt_string("RangeError") },

    { nxt_string("var a = Array(Infinity)"),
      nxt_string("RangeError") },

    { nxt_string("var a = new Array(3); a +''"),
      nxt_string(",,") },

    { nxt_string("Array.name"),
      nxt_string("Array") },

    { nxt_string("Array.length"),
      nxt_string("1") },

    { nxt_string("Array.prototype.constructor === Array"),
      nxt_string("true") },

    { nxt_string("Array.constructor === Function"),
      nxt_string("true") },

    { nxt_string("var a = []; a.join = 'OK'; a +''"),
      nxt_string("[object Array]") },

    { nxt_string("[].__proto__ === Array.prototype"),
      nxt_string("true") },

    { nxt_string("[].__proto__.constructor === Array"),
      nxt_string("true") },

    { nxt_string("[].constructor === Array"),
      nxt_string("true") },

    /* TODO: Boolean */

    { nxt_string("Number()"),
      nxt_string("0") },

    { nxt_string("typeof Number(1)"),
      nxt_string("number") },

    { nxt_string("typeof new Number(1)"),
      nxt_string("object") },

    { nxt_string("Number.name"),
      nxt_string("Number") },

    { nxt_string("Number.length"),
      nxt_string("1") },

    { nxt_string("Number.prototype.constructor === Number"),
      nxt_string("true") },

    { nxt_string("Number.constructor === Function"),
      nxt_string("true") },

    { nxt_string("String()"),
      nxt_string("") },

    { nxt_string("typeof String('abc')"),
      nxt_string("string") },

    { nxt_string("typeof new String('abc')"),
      nxt_string("object") },

    { nxt_string("String.name"),
      nxt_string("String") },

    { nxt_string("String.length"),
      nxt_string("1") },

    { nxt_string("String.prototype.length"),
      nxt_string("0") },

    { nxt_string("String.prototype.constructor === String"),
      nxt_string("true") },

    { nxt_string("String.constructor === Function"),
      nxt_string("true") },

    { nxt_string("'test'.__proto__ === String.prototype"),
      nxt_string("true") },

    { nxt_string("'test'.constructor === String"),
      nxt_string("true") },

    { nxt_string("'test'.constructor.prototype === String.prototype"),
      nxt_string("true") },

    { nxt_string("Function.name"),
      nxt_string("Function") },

    { nxt_string("Function.length"),
      nxt_string("0") },

    { nxt_string("Function.prototype.constructor === Function"),
      nxt_string("true") },

    { nxt_string("Function.constructor === Function"),
      nxt_string("true") },

    { nxt_string("RegExp.name"),
      nxt_string("RegExp") },

    { nxt_string("RegExp.length"),
      nxt_string("2") },

    { nxt_string("RegExp.prototype.constructor === RegExp"),
      nxt_string("true") },

    { nxt_string("RegExp.constructor === Function"),
      nxt_string("true") },

#if 0
    { nxt_string("Object.prototype.toString.call()"),
      nxt_string("[object Undefined]") },
#endif

    { nxt_string("Object.prototype.toString.call(undefined)"),
      nxt_string("[object Undefined]") },

    { nxt_string("Object.prototype.toString.call(null)"),
      nxt_string("[object Null]") },

    { nxt_string("Object.prototype.toString.call(true)"),
      nxt_string("[object Boolean]") },

    { nxt_string("Object.prototype.toString.call(1)"),
      nxt_string("[object Number]") },

    { nxt_string("Object.prototype.toString.call('')"),
      nxt_string("[object String]") },

    { nxt_string("Object.prototype.toString.call({})"),
      nxt_string("[object Object]") },

    { nxt_string("Object.prototype.toString.call([])"),
      nxt_string("[object Array]") },

    { nxt_string("Object.prototype.toString.call(new Object(true))"),
      nxt_string("[object Boolean]") },

    { nxt_string("Object.prototype.toString.call(new Number(1))"),
      nxt_string("[object Number]") },

    { nxt_string("Object.prototype.toString.call(new Object(1))"),
      nxt_string("[object Number]") },

    { nxt_string("Object.prototype.toString.call(new Object(''))"),
      nxt_string("[object String]") },

    { nxt_string("Object.prototype.toString.call(function(){})"),
      nxt_string("[object Function]") },

    { nxt_string("Object.prototype.toString.call(/./)"),
      nxt_string("[object RegExp]") },

    { nxt_string("var p = { a:5 }; var o = Object.create(p); o.a"),
      nxt_string("5") },

    { nxt_string("var p = { a:5 }; var o = Object.create(p); \
                  o.__proto__ === p"),
      nxt_string("true") },

    { nxt_string("var o = Object.create(Object.prototype); \
                  o.__proto__ === Object.prototype"),
      nxt_string("true") },

    { nxt_string("var o = Object.create(null); '__proto__' in o"),
      nxt_string("false") },

    /*  es5id: 8.2_A1_T1 */
    /*  es5id: 8.2_A1_T2 */

    { nxt_string("var x = null;"),
      nxt_string("") },

#if 0
    /*  es5id: 8.2_A2 */

    { nxt_string("var null;"),
      nxt_string("SyntaxError") },
#endif

    /*  es5id: 8.2_A3 */

    { nxt_string("typeof(null) === \"object\""),
      nxt_string("true") },

#endif  /* NXT_FIBOBENCH */

};


typedef struct {
    nxt_mem_cache_pool_t  *mem_cache_pool;
    nxt_str_t             uri;
} nxt_jscript_unit_test_req;


static njs_ret_t
nxt_jscript_unit_test_r_get_uri_external(njs_vm_t *vm, njs_value_t *value,
    void *obj, uintptr_t data)
{
    nxt_jscript_unit_test_req  *r;

    r = (nxt_jscript_unit_test_req *) obj;

    return njs_string_create(vm, value, r->uri.data, r->uri.len, 0);
}


static njs_ret_t
nxt_jscript_unit_test_r_set_uri_external(njs_vm_t *vm,
    void *obj, uintptr_t data, nxt_str_t *value)
{
    nxt_jscript_unit_test_req  *r;

    r = (nxt_jscript_unit_test_req *) obj;
    r->uri = *value;

    return NXT_OK;
}


static njs_ret_t
nxt_jscript_unit_test_host_external(njs_vm_t *vm, njs_value_t *value,
    void *obj, uintptr_t data)
{
    return njs_string_create(vm, value, (u_char *) "АБВГДЕЁЖЗИЙ", 22, 0);
}


static njs_ret_t
nxt_jscript_unit_test_header_external(njs_vm_t *vm, njs_value_t *value,
    void *obj, uintptr_t data)
{
    u_char                     *s, *p;
    uint32_t                   size;
    nxt_str_t                  *h;
    nxt_jscript_unit_test_req  *r;

    r = (nxt_jscript_unit_test_req *) obj;
    h = (nxt_str_t *) data;

    size = 7 + h->len;

    s = nxt_mem_cache_alloc(r->mem_cache_pool, size);
    if (nxt_slow_path(s == NULL)) {
        return NXT_ERROR;
    }

    p = memcpy(s, h->data, h->len);
    p += h->len;
    *p++ = '|';
    memcpy(p, "АБВ", 6);

    return njs_string_create(vm, value, s, size, 0);
}


static njs_ret_t
nxt_jscript_unit_test_header_each_start_external(njs_vm_t *vm, void *obj,
    void *each)
{
    u_char  *s;

    s = each;
    s[0] = '0';
    s[1] = '0';

    return NXT_OK;
}


static njs_ret_t
nxt_jscript_unit_test_header_each_external(njs_vm_t *vm, njs_value_t *value,
    void *obj, void *each)
{
    u_char  *s;

    s = each;
    s[1]++;

    if (s[1] == '4') {
        return NXT_DONE;
    }

    return njs_string_create(vm, value, s, 2, 0);
}


static njs_ret_t
nxt_jscript_unit_test_undefined_external(njs_vm_t *vm, njs_value_t *value,
    void *obj, uintptr_t data)
{
    njs_void_set(value);

    return NJS_OK;
}


static njs_external_t  nxt_test_r_external[] = {

    { nxt_string("uri"),
      NJS_EXTERN_PROPERTY,
      NULL,
      0,
      nxt_jscript_unit_test_r_get_uri_external,
      nxt_jscript_unit_test_r_set_uri_external,
      NULL,
      NULL,
      NULL,
      NULL,
      0 },

    { nxt_string("host"),
      NJS_EXTERN_PROPERTY,
      NULL,
      0,
      nxt_jscript_unit_test_host_external,
      NULL,
      NULL,
      NULL,
      NULL,
      NULL,
      0 },

    { nxt_string("header"),
      NJS_EXTERN_OBJECT,
      NULL,
      0,
      nxt_jscript_unit_test_header_external,
      NULL,
      NULL,
      nxt_jscript_unit_test_header_each_start_external,
      nxt_jscript_unit_test_header_each_external,
      NULL,
      0 },

};


static njs_external_t  nxt_test_external[] = {

    { nxt_string("$r"),
      NJS_EXTERN_OBJECT,
      nxt_test_r_external,
      nxt_nitems(nxt_test_r_external),
      nxt_jscript_unit_test_undefined_external,
      NULL,
      NULL,
      NULL,
      NULL,
      NULL,
      0 },

};


static nxt_int_t
nxt_jscript_unit_test_externals(nxt_lvlhsh_t *externals, nxt_mem_cache_pool_t *mcp)
{
    nxt_lvlhsh_init(externals);

    return njs_add_external(externals, mcp, 0, nxt_test_external,
                            nxt_nitems(nxt_test_external));
}


static void *
njs_alloc(void *mem, size_t size)
{
    return nxt_malloc(size);
}


static void *
njs_zalloc(void *mem, size_t size)
{
    void  *p;

    p = nxt_malloc(size);

    if (p != NULL) {
        memset(p, 0, size);
    }

    return p;
}


static void *
njs_align(void *mem, size_t alignment, size_t size)
{
    return nxt_memalign(alignment, size);
}


static void
njs_free(void *mem, void *p)
{
    nxt_free(p);
}


static const nxt_mem_proto_t  nxt_jscript_mem_cache_pool_proto = {
    njs_alloc,
    njs_zalloc,
    njs_align,
    NULL,
    njs_free,
    NULL,
    NULL,
};


static nxt_int_t
nxt_jscript_unit_test(void)
{
    void                      *ext_object;
    u_char                    *start;
    njs_vm_t                  *vm, *nvm;
    nxt_int_t                  ret;
    nxt_str_t                  s;
    nxt_bool_t                 ok;
    nxt_uint_t                 i;
    nxt_lvlhsh_t               externals;
    njs_vm_shared_t           *shared;
    nxt_mem_cache_pool_t       *mcp;
    nxt_jscript_unit_test_req  r;

    shared = NULL;

    mcp = nxt_mem_cache_pool_create(&nxt_jscript_mem_cache_pool_proto, NULL, NULL,
                                   2 * nxt_pagesize(), 128, 512, 16);
    if (nxt_slow_path(mcp == NULL)) {
        return NXT_ERROR;
    }

    r.mem_cache_pool = mcp;
    r.uri.len = 6;
    r.uri.data = (u_char *) "АБВ";

    ext_object = &r;

    if (nxt_jscript_unit_test_externals(&externals, mcp) != NXT_OK) {
        return NXT_ERROR;
    }

    for (i = 0; i < nxt_nitems(js_test); i++) {

#if 1
        printf("\"%.*s\"\n",
               (int) js_test[i].script.len, js_test[i].script.data);
#endif

        vm = njs_vm_create(mcp, &shared, &externals);
        if (vm == NULL) {
            return NXT_ERROR;
        }

        start = js_test[i].script.data;

        ret = njs_vm_compile(vm, &start, start + js_test[i].script.len);

        if (ret == NXT_OK) {
            nvm = njs_vm_clone(vm, NULL, &ext_object);
            if (nvm == NULL) {
                return NXT_ERROR;
            }

            if (njs_vm_run(nvm) == NXT_OK) {
                if (njs_vm_retval(nvm, &s) != NXT_OK) {
                    return NXT_ERROR;
                }

            } else {
                njs_vm_exception(nvm, &s);
            }

        } else {
            njs_vm_exception(vm, &s);
        }

        ok = nxt_strstr_eq(&js_test[i].ret, &s);

        if (!ok) {
            printf("jscript(\"%.*s\") failed: \"%.*s\" vs \"%.*s\"\n",
                   (int) js_test[i].script.len, js_test[i].script.data,
                   (int) js_test[i].ret.len, js_test[i].ret.data,
                   (int) s.len, s.data);

            return NXT_ERROR;
        }
    }

    nxt_mem_cache_pool_destroy(mcp);

    printf("jscript unit tests passed\n");

    return NXT_OK;
}


int nxt_cdecl
main(int argc, char **argv)
{
    return nxt_jscript_unit_test();
}
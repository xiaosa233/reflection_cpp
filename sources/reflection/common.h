#pragma once

#define _namespace reflection::

#define _unpack(...) __VA_ARGS__

// limiter
#define _empty()
#define _semicolon() ;
#define _comma() ,
#define _f_select(                                                                                 \
    _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20,     \
    _21, _22, _23, _24, _25, _26, _27, _28, _29, _30, _31, _32, _33, _34, _35, _36, _37, _38, _39, \
    _40, _41, _42, _43, _44, _45, _46, _47, _48, _49, _50, _51, _52, _53, _54, _55, _56, _57, _58, \
    _59, _60, _61, _62, _63, _64, _65, _66, _67, _68, _69, _70, _71, _72, _73, _74, _75, _76, _77, \
    _78, _79, _80, _81, _82, _83, _84, _85, _86, _87, _88, _89, _90, _91, _92, _93, _94, _95, _96, \
    _97, _98, _99, _100, _101, _102, _103, _104, _105, _106, _107, _108, _109, _110, _111, _112,   \
    _113, _114, _115, _116, _117, _118, _119, _120, _121, _122, _123, _124, _125, _126, _127,      \
    _128, _129, _130, _131, _132, _133, _134, _135, _136, _137, _138, _139, _140, _141, _142,      \
    _143, _144, _145, _146, _147, _148, _149, _150, _151, _152, _153, _154, _155, _156, _157,      \
    _158, _159, _160, _161, _162, _163, _164, _165, _166, _167, _168, _169, _170, _171, _172,      \
    _173, _174, _175, _176, _177, _178, _179, _180, _181, _182, _183, _184, _185, _186, _187,      \
    _188, _189, _190, _191, _192, _193, _194, _195, _196, _197, _198, _199, _200, _201, _202,      \
    _203, _204, _205, _206, _207, _208, _209, _210, _211, _212, _213, _214, _215, _216, _217,      \
    _218, _219, _220, _221, _222, _223, _224, _225, _226, _227, _228, _229, _230, _231, _232,      \
    _233, _234, _235, _236, _237, _238, _239, _240, _241, _242, _243, _244, _245, _246, _247,      \
    _248, _249, _250, _251, _252, _253, _254, _255, _256, n, ...)                                  \
  n
#define _n                                                                                         \
  256, 255, 254, 253, 252, 251, 250, 249, 248, 247, 246, 245, 244, 243, 242, 241, 240, 239, 238,   \
      237, 236, 235, 234, 233, 232, 231, 230, 229, 228, 227, 226, 225, 224, 223, 222, 221, 220,    \
      219, 218, 217, 216, 215, 214, 213, 212, 211, 210, 209, 208, 207, 206, 205, 204, 203, 202,    \
      201, 200, 199, 198, 197, 196, 195, 194, 193, 192, 191, 190, 189, 188, 187, 186, 185, 184,    \
      183, 182, 181, 180, 179, 178, 177, 176, 175, 174, 173, 172, 171, 170, 169, 168, 167, 166,    \
      165, 164, 163, 162, 161, 160, 159, 158, 157, 156, 155, 154, 153, 152, 151, 150, 149, 148,    \
      147, 146, 145, 144, 143, 142, 141, 140, 139, 138, 137, 136, 135, 134, 133, 132, 131, 130,    \
      129, 128, 127, 126, 125, 124, 123, 122, 121, 120, 119, 118, 117, 116, 115, 114, 113, 112,    \
      111, 110, 109, 108, 107, 106, 105, 104, 103, 102, 101, 100, 99, 98, 97, 96, 95, 94, 93, 92,  \
      91, 90, 89, 88, 87, 86, 85, 84, 83, 82, 81, 80, 79, 78, 77, 76, 75, 74, 73, 72, 71, 70, 69,  \
      68, 67, 66, 65, 64, 63, 62, 61, 60, 59, 58, 57, 56, 55, 54, 53, 52, 51, 50, 49, 48, 47, 46,  \
      45, 44, 43, 42, 41, 40, 39, 38, 37, 36, 35, 34, 33, 32, 31, 30, 29, 28, 27, 26, 25, 24, 23,  \
      22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1

#define _f_dispatch_n2(n) _f##n
#define _f_dispatch_n(n) _f_dispatch_n2(n)
#define _f_helper(N) _f_select N

// _fn(1,2,3,n) --> _fn
#define _fn(...) _f_dispatch_n(_f_helper((__VA_ARGS__, _n)))

#define _invoke(func, args) func args
#define _f1(func, limiter, class, args) _invoke(func, (class, _unpack args))
#define _f2(func, limiter, class, args, ...)                                                       \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f3(func, limiter, class, args, ...)                                                       \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f4(func, limiter, class, args, ...)                                                       \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f5(func, limiter, class, args, ...)                                                       \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f6(func, limiter, class, args, ...)                                                       \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f7(func, limiter, class, args, ...)                                                       \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f8(func, limiter, class, args, ...)                                                       \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f9(func, limiter, class, args, ...)                                                       \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f10(func, limiter, class, args, ...)                                                      \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f11(func, limiter, class, args, ...)                                                      \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f12(func, limiter, class, args, ...)                                                      \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f13(func, limiter, class, args, ...)                                                      \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f14(func, limiter, class, args, ...)                                                      \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f15(func, limiter, class, args, ...)                                                      \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f16(func, limiter, class, args, ...)                                                      \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f17(func, limiter, class, args, ...)                                                      \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f18(func, limiter, class, args, ...)                                                      \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f19(func, limiter, class, args, ...)                                                      \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f20(func, limiter, class, args, ...)                                                      \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f21(func, limiter, class, args, ...)                                                      \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f22(func, limiter, class, args, ...)                                                      \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f23(func, limiter, class, args, ...)                                                      \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f24(func, limiter, class, args, ...)                                                      \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f25(func, limiter, class, args, ...)                                                      \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f26(func, limiter, class, args, ...)                                                      \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f27(func, limiter, class, args, ...)                                                      \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f28(func, limiter, class, args, ...)                                                      \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f29(func, limiter, class, args, ...)                                                      \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f30(func, limiter, class, args, ...)                                                      \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f31(func, limiter, class, args, ...)                                                      \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f32(func, limiter, class, args, ...)                                                      \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f33(func, limiter, class, args, ...)                                                      \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f34(func, limiter, class, args, ...)                                                      \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f35(func, limiter, class, args, ...)                                                      \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f36(func, limiter, class, args, ...)                                                      \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f37(func, limiter, class, args, ...)                                                      \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f38(func, limiter, class, args, ...)                                                      \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f39(func, limiter, class, args, ...)                                                      \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f40(func, limiter, class, args, ...)                                                      \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f41(func, limiter, class, args, ...)                                                      \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f42(func, limiter, class, args, ...)                                                      \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f43(func, limiter, class, args, ...)                                                      \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f44(func, limiter, class, args, ...)                                                      \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f45(func, limiter, class, args, ...)                                                      \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f46(func, limiter, class, args, ...)                                                      \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f47(func, limiter, class, args, ...)                                                      \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f48(func, limiter, class, args, ...)                                                      \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f49(func, limiter, class, args, ...)                                                      \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f50(func, limiter, class, args, ...)                                                      \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f51(func, limiter, class, args, ...)                                                      \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f52(func, limiter, class, args, ...)                                                      \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f53(func, limiter, class, args, ...)                                                      \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f54(func, limiter, class, args, ...)                                                      \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f55(func, limiter, class, args, ...)                                                      \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f56(func, limiter, class, args, ...)                                                      \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f57(func, limiter, class, args, ...)                                                      \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f58(func, limiter, class, args, ...)                                                      \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f59(func, limiter, class, args, ...)                                                      \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f60(func, limiter, class, args, ...)                                                      \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f61(func, limiter, class, args, ...)                                                      \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f62(func, limiter, class, args, ...)                                                      \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f63(func, limiter, class, args, ...)                                                      \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f64(func, limiter, class, args, ...)                                                      \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f65(func, limiter, class, args, ...)                                                      \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f66(func, limiter, class, args, ...)                                                      \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f67(func, limiter, class, args, ...)                                                      \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f68(func, limiter, class, args, ...)                                                      \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f69(func, limiter, class, args, ...)                                                      \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f70(func, limiter, class, args, ...)                                                      \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f71(func, limiter, class, args, ...)                                                      \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f72(func, limiter, class, args, ...)                                                      \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f73(func, limiter, class, args, ...)                                                      \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f74(func, limiter, class, args, ...)                                                      \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f75(func, limiter, class, args, ...)                                                      \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f76(func, limiter, class, args, ...)                                                      \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f77(func, limiter, class, args, ...)                                                      \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f78(func, limiter, class, args, ...)                                                      \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f79(func, limiter, class, args, ...)                                                      \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f80(func, limiter, class, args, ...)                                                      \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f81(func, limiter, class, args, ...)                                                      \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f82(func, limiter, class, args, ...)                                                      \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f83(func, limiter, class, args, ...)                                                      \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f84(func, limiter, class, args, ...)                                                      \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f85(func, limiter, class, args, ...)                                                      \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f86(func, limiter, class, args, ...)                                                      \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f87(func, limiter, class, args, ...)                                                      \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f88(func, limiter, class, args, ...)                                                      \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f89(func, limiter, class, args, ...)                                                      \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f90(func, limiter, class, args, ...)                                                      \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f91(func, limiter, class, args, ...)                                                      \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f92(func, limiter, class, args, ...)                                                      \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f93(func, limiter, class, args, ...)                                                      \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f94(func, limiter, class, args, ...)                                                      \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f95(func, limiter, class, args, ...)                                                      \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f96(func, limiter, class, args, ...)                                                      \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f97(func, limiter, class, args, ...)                                                      \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f98(func, limiter, class, args, ...)                                                      \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f99(func, limiter, class, args, ...)                                                      \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f100(func, limiter, class, args, ...)                                                     \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f101(func, limiter, class, args, ...)                                                     \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f102(func, limiter, class, args, ...)                                                     \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f103(func, limiter, class, args, ...)                                                     \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f104(func, limiter, class, args, ...)                                                     \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f105(func, limiter, class, args, ...)                                                     \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f106(func, limiter, class, args, ...)                                                     \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f107(func, limiter, class, args, ...)                                                     \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f108(func, limiter, class, args, ...)                                                     \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f109(func, limiter, class, args, ...)                                                     \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f110(func, limiter, class, args, ...)                                                     \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f111(func, limiter, class, args, ...)                                                     \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f112(func, limiter, class, args, ...)                                                     \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f113(func, limiter, class, args, ...)                                                     \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f114(func, limiter, class, args, ...)                                                     \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f115(func, limiter, class, args, ...)                                                     \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f116(func, limiter, class, args, ...)                                                     \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f117(func, limiter, class, args, ...)                                                     \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f118(func, limiter, class, args, ...)                                                     \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f119(func, limiter, class, args, ...)                                                     \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f120(func, limiter, class, args, ...)                                                     \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f121(func, limiter, class, args, ...)                                                     \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f122(func, limiter, class, args, ...)                                                     \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f123(func, limiter, class, args, ...)                                                     \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f124(func, limiter, class, args, ...)                                                     \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f125(func, limiter, class, args, ...)                                                     \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f126(func, limiter, class, args, ...)                                                     \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f127(func, limiter, class, args, ...)                                                     \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f128(func, limiter, class, args, ...)                                                     \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f129(func, limiter, class, args, ...)                                                     \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f130(func, limiter, class, args, ...)                                                     \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f131(func, limiter, class, args, ...)                                                     \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f132(func, limiter, class, args, ...)                                                     \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f133(func, limiter, class, args, ...)                                                     \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f134(func, limiter, class, args, ...)                                                     \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f135(func, limiter, class, args, ...)                                                     \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f136(func, limiter, class, args, ...)                                                     \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f137(func, limiter, class, args, ...)                                                     \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f138(func, limiter, class, args, ...)                                                     \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f139(func, limiter, class, args, ...)                                                     \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f140(func, limiter, class, args, ...)                                                     \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f141(func, limiter, class, args, ...)                                                     \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f142(func, limiter, class, args, ...)                                                     \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f143(func, limiter, class, args, ...)                                                     \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f144(func, limiter, class, args, ...)                                                     \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f145(func, limiter, class, args, ...)                                                     \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f146(func, limiter, class, args, ...)                                                     \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f147(func, limiter, class, args, ...)                                                     \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f148(func, limiter, class, args, ...)                                                     \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f149(func, limiter, class, args, ...)                                                     \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f150(func, limiter, class, args, ...)                                                     \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f151(func, limiter, class, args, ...)                                                     \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f152(func, limiter, class, args, ...)                                                     \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f153(func, limiter, class, args, ...)                                                     \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f154(func, limiter, class, args, ...)                                                     \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f155(func, limiter, class, args, ...)                                                     \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f156(func, limiter, class, args, ...)                                                     \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f157(func, limiter, class, args, ...)                                                     \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f158(func, limiter, class, args, ...)                                                     \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f159(func, limiter, class, args, ...)                                                     \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f160(func, limiter, class, args, ...)                                                     \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f161(func, limiter, class, args, ...)                                                     \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f162(func, limiter, class, args, ...)                                                     \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f163(func, limiter, class, args, ...)                                                     \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f164(func, limiter, class, args, ...)                                                     \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f165(func, limiter, class, args, ...)                                                     \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f166(func, limiter, class, args, ...)                                                     \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f167(func, limiter, class, args, ...)                                                     \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f168(func, limiter, class, args, ...)                                                     \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f169(func, limiter, class, args, ...)                                                     \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f170(func, limiter, class, args, ...)                                                     \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f171(func, limiter, class, args, ...)                                                     \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f172(func, limiter, class, args, ...)                                                     \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f173(func, limiter, class, args, ...)                                                     \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f174(func, limiter, class, args, ...)                                                     \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f175(func, limiter, class, args, ...)                                                     \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f176(func, limiter, class, args, ...)                                                     \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f177(func, limiter, class, args, ...)                                                     \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f178(func, limiter, class, args, ...)                                                     \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f179(func, limiter, class, args, ...)                                                     \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f180(func, limiter, class, args, ...)                                                     \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f181(func, limiter, class, args, ...)                                                     \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f182(func, limiter, class, args, ...)                                                     \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f183(func, limiter, class, args, ...)                                                     \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f184(func, limiter, class, args, ...)                                                     \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f185(func, limiter, class, args, ...)                                                     \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f186(func, limiter, class, args, ...)                                                     \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f187(func, limiter, class, args, ...)                                                     \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f188(func, limiter, class, args, ...)                                                     \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f189(func, limiter, class, args, ...)                                                     \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f190(func, limiter, class, args, ...)                                                     \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f191(func, limiter, class, args, ...)                                                     \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f192(func, limiter, class, args, ...)                                                     \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f193(func, limiter, class, args, ...)                                                     \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f194(func, limiter, class, args, ...)                                                     \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f195(func, limiter, class, args, ...)                                                     \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f196(func, limiter, class, args, ...)                                                     \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f197(func, limiter, class, args, ...)                                                     \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f198(func, limiter, class, args, ...)                                                     \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f199(func, limiter, class, args, ...)                                                     \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f200(func, limiter, class, args, ...)                                                     \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f201(func, limiter, class, args, ...)                                                     \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f202(func, limiter, class, args, ...)                                                     \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f203(func, limiter, class, args, ...)                                                     \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f204(func, limiter, class, args, ...)                                                     \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f205(func, limiter, class, args, ...)                                                     \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f206(func, limiter, class, args, ...)                                                     \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f207(func, limiter, class, args, ...)                                                     \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f208(func, limiter, class, args, ...)                                                     \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f209(func, limiter, class, args, ...)                                                     \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f210(func, limiter, class, args, ...)                                                     \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f211(func, limiter, class, args, ...)                                                     \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f212(func, limiter, class, args, ...)                                                     \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f213(func, limiter, class, args, ...)                                                     \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f214(func, limiter, class, args, ...)                                                     \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f215(func, limiter, class, args, ...)                                                     \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f216(func, limiter, class, args, ...)                                                     \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f217(func, limiter, class, args, ...)                                                     \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f218(func, limiter, class, args, ...)                                                     \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f219(func, limiter, class, args, ...)                                                     \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f220(func, limiter, class, args, ...)                                                     \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f221(func, limiter, class, args, ...)                                                     \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f222(func, limiter, class, args, ...)                                                     \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f223(func, limiter, class, args, ...)                                                     \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f224(func, limiter, class, args, ...)                                                     \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f225(func, limiter, class, args, ...)                                                     \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f226(func, limiter, class, args, ...)                                                     \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f227(func, limiter, class, args, ...)                                                     \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f228(func, limiter, class, args, ...)                                                     \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f229(func, limiter, class, args, ...)                                                     \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f230(func, limiter, class, args, ...)                                                     \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f231(func, limiter, class, args, ...)                                                     \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f232(func, limiter, class, args, ...)                                                     \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f233(func, limiter, class, args, ...)                                                     \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f234(func, limiter, class, args, ...)                                                     \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f235(func, limiter, class, args, ...)                                                     \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f236(func, limiter, class, args, ...)                                                     \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f237(func, limiter, class, args, ...)                                                     \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f238(func, limiter, class, args, ...)                                                     \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f239(func, limiter, class, args, ...)                                                     \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f240(func, limiter, class, args, ...)                                                     \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f241(func, limiter, class, args, ...)                                                     \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f242(func, limiter, class, args, ...)                                                     \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f243(func, limiter, class, args, ...)                                                     \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f244(func, limiter, class, args, ...)                                                     \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f245(func, limiter, class, args, ...)                                                     \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f246(func, limiter, class, args, ...)                                                     \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f247(func, limiter, class, args, ...)                                                     \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f248(func, limiter, class, args, ...)                                                     \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f249(func, limiter, class, args, ...)                                                     \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f250(func, limiter, class, args, ...)                                                     \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f251(func, limiter, class, args, ...)                                                     \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f252(func, limiter, class, args, ...)                                                     \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f253(func, limiter, class, args, ...)                                                     \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f254(func, limiter, class, args, ...)                                                     \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f255(func, limiter, class, args, ...)                                                     \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)
#define _f256(func, limiter, class, args, ...)                                                     \
  _invoke(func, (class, _unpack args)) limiter() _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)

#define _func_fn(func, limiter, class, ...) _fn(__VA_ARGS__)(func, limiter, class, __VA_ARGS__)

#define _repair(none, A, B, ...) (A, B)

// convert three to two
#define _convert(...) _func_fn(_repair, _comma, none, __VA_ARGS__)

#define DISALLOW_COPY_AND_MOVE(X)                                                                  \
private:                                                                                           \
  X(const X&) = delete;                                                                            \
  X(X&&) = delete

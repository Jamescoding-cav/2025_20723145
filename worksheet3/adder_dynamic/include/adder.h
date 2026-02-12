#ifndef MATHSLIB_ADDER_H
#define MATHSLIB_ADDER_H

#ifdef maths_EXPORTS
#define MATHSLIB_API __declspec(dllexport)
#else
#define MATHSLIB_API __declspec(dllimport)
#endif

MATHSLIB_API int add(int a, int b);

#endif
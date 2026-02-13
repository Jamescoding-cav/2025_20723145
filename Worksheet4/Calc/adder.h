#ifndef MATHSLIB_ADDER_H
#define MATHSLIB_ADDER_H

#ifdef maths_STATIC
// Static library: no import/export needed
#define MATHSLIB_API
#elif defined(maths_EXPORTS)
// Building the DLL
#define MATHSLIB_API __declspec(dllexport)
#else
// Using the DLL
#define MATHSLIB_API __declspec(dllimport)
#endif

MATHSLIB_API int add(int a, int b);

#endif // MATHSLIB_ADDER_H

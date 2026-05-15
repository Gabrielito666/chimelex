#include <stddef.h>

#define WAVE_FN_PARAMS_SIZE 64

typedef double (*WaveBaseFn)(double t, void *params);


typedef struct
{
	WaveBaseFn wave_base_fn;
	unsigned char params[WAVE_FN_PARAMS_SIZE];
} WaveFn;

typedef struct
{
	int length;
	WaveFn *wave_fns;
} WaveFnsStack;

typedef WaveFn (*WaveFn__createFn)(WaveBaseFn wave_base_fn, void *params, size_t params_size);
typedef WaveFnsStack (*WaveFn__create_stackFn)(int length, WaveFn *wave_fns);
typedef WaveFn (* WaveFn__addFn)(WaveFnsStack wave_fns_stack);
typedef double(*WaveFn__evalFn)(WaveFn *wave_fn, double t);

typedef struct
{
	WaveFn__createFn create;
	WaveFn__create_stackFn create_stack;
	WaveFn__addFn add;
	WaveFn__evalFn eval;
} WaveFnOpts;

extern WaveFnOpts wave_fn_opts;

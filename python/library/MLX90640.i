%module MLX90640
%include "stdint.i"

%{
int setup(int newAddr, int fps);
float * get_frame(void);
uint16_t * get_raw(void);
%}

%typemap(out) float *get_frame %{
    $result = PyList_New(768);
    for (int i = 0; i < 768; ++i) {
        PyList_SetItem($result, i, PyFloat_FromDouble($1[i]));
    }
%}

%typemap(out) uint16_t *get_raw %{
    $result = PyList_New(834);
    for (int i = 0; i < 834; ++i) {
        PyList_SetItem($result, i, PyLong_FromUnsignedLong($1[i]));
    }
%}

int setup(int newAddr, int fps);
float * get_frame(void);
uint16_t * get_raw(void);

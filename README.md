<img src="https://github.com/zanellia/prometeo/blob/master/logo/logo.png" align="left"
     alt="prometeo logo by Andrea Zanelli" width="80" height="120">

![Travis Status](https://travis-ci.org/zanellia/prometeo.svg?branch=master) [![PyPI version fury.io](https://badge.fury.io/py/prometeo-dsl.svg)](https://pypi.python.org/pypi/ansicolortags/)

This is prometeo, an experimental modeling tool for embedded high-performance computing. prometeo provides a 
domain specific language (DSL) based on a subset of the Python language that allows 
one to conveniently write scientific computing programs in a high-level language (Python itself) that can be transpiled
to high-performance self-contained C code easily deployable on embedded devices.

### features
- __Python compatible syntax__ : prometeo is a DSL embedded into the Python language. 
prometeo programs can be executed from the Python interpreter.
- __efficient__ : prometeo programs transpile to high-performance C code.
- __statically typed__ : prometeo uses Python's native type hints to strictly enforce static typing.
- __deterministic memory usage__ : a specific program structure is required and enforced through static analysis. In this way
prometeo transpiled programs have a guaranteed maximum heap usage.
- __fast memory menagement__ : thanks to its static analysis, prometeo can avoid allocating
and garbage-collecting memory, resulting in faster and safer execution.
- __self-contained and embeddable__ : unlike other similar tools and languages, prometeo targets specifically embedded applications and programs 
written in prometeo transpile to self-contained C code that does not require linking against 
the Python run-time library.


      
### performance
Since prometeo programs transpile to pure C code that calls the high performance linear algebra library BLASFEO ([https://arxiv.org/abs/1704.02457](https://arxiv.org/abs/1704.02457)), execution time can be comparable to hand-written high-performance code. The figure below shows a comparison of the CPU time necessary to carry out a Riccati factorization using highly optimized hand-written C code with calls to BLASFEO and the ones obtained with prometeo transpiled code from [this example](https://github.com/zanellia/prometeo/blob/master/examples/riccati_example/riccati_mass_spring.py). The computation times obtained with NumPy and Julia are added too for comparison - notice however that these last two implementations of the Riccati factorization are not as easily embeddable as the C code generated by prometeo and the hand-coded C implementation. All the benchmarks have been run on a Dell XPS-9360 equipped with an i7-7560U CPU running at 2.30 GHz (to avoid frequency fluctuations due to overheat). 

<p align="center">
<img src="https://github.com/zanellia/prometeo/blob/master/benchmarks/riccati_benchmark.png" width="70%" >
</p>

### PyPI installation

prometeo can be installed through PyPI with `pip install prometeo-dsl`.

### manual installation
If you want to install prometeo building the sources on your local machine you can proceed as follows:

- Run `git submodule update --init` to clone the submodules.
- Run `make install_shared` from `<prometeo_root>/prometeo/cpmt` to compile and install the shared library associated with the C backend. Notice that the default installation path is `<prometeo_root>/prometeo/cpmt/install`.
- You need Python 3.7. or later.
- Optional: to keep things clean you can setup a virtual environment with `virtualenv --python=<path_to_python3.7 <path_to_new_virtualenv>`.
- Run `pip install -e .` from `<prometeo_root>` to install the Python package.

Finally, you can run the examples in `<root>/examples` with `pmt <example_name>.py --cgen=<True/False>`, where the `--cgen` flag determines whether the code is executed by the Python interpreter or C code is generated compiled and run.

### a simple example

The Python code
```python
from prometeo import *

n : dims = 10

def main() -> int:

    A: pmat = pmat(n, n)
    for i in range(10):
        for j in range(10):
            A[i, j] = 1.0

    B: pmat = pmat(n, n)
    for i in range(10):
        B[0, i] = 2.0

    C: pmat = pmat(n, n)
    C = A * B
    pmat_print(C)
    return 0
```
can be run by the standard Python interpreter (version >3.6 required) and it 
will perform the described linear algebra operations using the command `pmt simple_example.py --cgen=False`. 
At the same time, the code can be parsed by prometeo and its abstract syntax tree (AST) analyzed in order
to generate the following high-performance C code:
```c
#include "stdlib.h"
#include "simple_example.h"
void * ___c_pmt_8_heap;
void * ___c_pmt_64_heap;
void * ___c_pmt_8_heap_head;
void * ___c_pmt_64_heap_head;

#include "prometeo.h"
int main() {
    ___c_pmt_8_heap = malloc(10000); 
    ___c_pmt_8_heap_head = ___c_pmt_8_heap;
    char * pmem_ptr = (char *)___c_pmt_8_heap;
    align_char_to(8, &pmem_ptr);
    ___c_pmt_8_heap = pmem_ptr;
    ___c_pmt_64_heap = malloc(1000000);
    ___c_pmt_64_heap_head = ___c_pmt_64_heap;
    pmem_ptr = (char *)___c_pmt_64_heap;
    align_char_to(64, &pmem_ptr);
    ___c_pmt_64_heap = pmem_ptr;
	void *callee_pmt_8_heap = ___c_pmt_8_heap;
	void *callee_pmt_64_heap = ___c_pmt_64_heap;

    struct pmat * A = c_pmt_create_pmat(n, n);
    for(int i = 0; i < 10; i++) {
        for(int j = 0; j < 10; j++) {
            c_pmt_pmat_set_el(A, i, j, 1.0);
    }

    }

    struct pmat * B = c_pmt_create_pmat(n, n);
    for(int i = 0; i < 10; i++) {
        c_pmt_pmat_set_el(B, 0, i, 2.0);
    }

    struct pmat * C = c_pmt_create_pmat(n, n);
    c_pmt_pmat_fill(C, 0.0);
    c_pmt_gemm_nn(A, B, C, C);
    c_pmt_pmat_print(C);
	___c_pmt_8_heap = callee_pmt_8_heap;
	___c_pmt_64_heap = callee_pmt_64_heap;

	free(___c_pmt_8_heap_head);
	free(___c_pmt_64_heap_head);
	return 0;
}
```
which relies on the high-performance linear algebra package BLASFEO. The generated code will be readily compiled and run with when running `pmt simple_example.py --cgen=True`.

### a more advanced example (Riccati factorization)
```python
from prometeo import *

nx:  dims = 2
nu:  dims = 2
nxu: dims = nx + nu
N:   dims = 5

def main() -> int:

    # number of repetitions for timing
    nrep : int = 10000

    A: pmat = pmat(nx, nx)
    A[0,0] = 0.8
    A[0,1] = 0.1
    A[1,0] = 0.3
    A[1,1] = 0.8

    B: pmat = pmat(nx, nu)
    B[0,0] = 1.0  
    B[1,1] = 1.0

    Q: pmat = pmat(nx, nx)
    Q[0,0] = 1.0  
    Q[1,1] = 1.0

    R: pmat = pmat(nu, nu)
    R[0,0] = 1.0  
    R[1,1] = 1.0

    A: pmat = pmat(nx, nx)
    B: pmat = pmat(nx, nu)
    Q: pmat = pmat(nx, nx)
    R: pmat = pmat(nu, nu)

    RSQ: pmat = pmat(nxu, nxu)
    Lxx: pmat = pmat(nx, nx)
    M: pmat = pmat(nxu, nxu)
    w_nxu_nx: pmat = pmat(nxu, nx)
    BAt : pmat = pmat(nxu, nx)
    BA : pmat = pmat(nx, nxu)
    pmat_hcat(B, A, BA)
    pmat_tran(BA, BAt)

    RSQ[0:nu,0:nu] = R
    RSQ[nu:nu+nx,nu:nu+nx] = Q

    # array-type Riccati factorization
    for i in range(nrep):
        pmt_potrf(Q, Lxx)
        M[nu:nu+nx,nu:nu+nx] = Lxx
        for i in range(1, N):
            pmt_trmm_rlnn(Lxx, BAt, w_nxu_nx)
            pmt_syrk_ln(w_nxu_nx, w_nxu_nx, RSQ, M)
            pmt_potrf(M, M)
            Lxx[0:nx,0:nx] = M[nu:nu+nx,nu:nu+nx]

    return 0

```
Similarly, the code above can be run by the standard Python interpreter using the command `pmt dgemm.py --cgen=False` and prometeo can generate, compile and run C code using instead `pmt dgemm.py --cgen=True`.
__Disclaimer: prometeo is still at a very preliminary stage and only very few linear algebra operations and Python constructs are supported for the time being.__


# ECS130 HW2 -- Solving Linear Systems
import numpy as np, scipy

def lu(A):
    """ Constructs an LU decomposition of `A` without pivoting. """
    m = A.shape[0]
    if A.shape[1] != m: raise Exception('A is not square!')
    L, U = np.identity(m), A.copy()

    # TODO (Problem 7): Translate the pseudocode from here:
    # https://julianpanetta.com/teaching/ECS130/05-SolvingLinearSystems-deck.html#/complexity-of-solving-linear-systems/3

    for k in range(m - 1):
        for i in range(k + 1, m):
            factor = U[i, k] / U[k, k]
            L[i, k] = factor
            U[i, k:] -= factor * U[k, k:]

    return L, U

def forwardsub(L, b):
    """ Solves "L x = b" with a lower triangular matrix L. """
    m = L.shape[0]
    if L.shape[1] != m: raise Exception('A is not square!')
    if b.shape != (m,): raise Exception('b is not a correctly sized vector!')

    # TODO (Problem 8): Implement the forward substitution algorithm
    # https://julianpanetta.com/teaching/ECS130/05-SolvingLinearSystems-deck.html#/forward-substitution/9

    x = b.copy()
    for i in range(m):
        x[i] = (b[i] - np.dot(L[i, :i], x[:i])) / L[i, i]

    return x

def backsub(U, b):
    """ Solves "U x = b" with an upper triangular matrix U. """
    m = U.shape[0]
    if U.shape[1] != m: raise Exception('A is not square!')
    if b.shape != (m,): raise Exception('b is not a correctly sized vector!')

    # TODO (Problem 9): Implement the back substitution algorithm
    # https://julianpanetta.com/teaching/ECS130/05-SolvingLinearSystems-deck.html#/back-substitution

    x = b.copy()
    for i in range(m - 1, -1, -1):
        x[i] = (x[i] - np.dot(U[i, i + 1:], x[i + 1:])) / U[i, i]

    return x

def solve(A, b):
    """ Solve "A x = b" using the LU decomposition approach """

    # TODO (Problem 10): use the methods you implemented above to compute the
    # LU decomposition and then solve for `x` using its factors.

    L, U = lu(A)
    y = forwardsub(L, b)
    x = backsub(U, y)

    return x

def cholesky(A):
    """ Constructs a Cholesky decomposition of symmetric positive definite matrix `A`. """
    m = A.shape[0]
    if A.shape[1] != m: raise Exception('A is not square!')


    # TODO (Problem 3): Implement the Cholesky factorization algorithm
    # https://julianpanetta.com/teaching/ECS130/07-Cholesky-deck.html#/cholesky-factorization-algorithm/3
    L = np.zeros_like(A)
    for i in range(m):
        for k in range(i):
            A[i, i] -= L[i, k] ** 2
        L[i, i] = np.sqrt(A[i, i])

        for j in range(i + 1, m):
            for k in range(i):
                A[j, i] -= L[j, k] * L[i, k]
            L[j, i] = A[j, i] / L[i, i]

    return L

def solve_cholesky(A, b):
    """ Solve "A x = b" for a positive definite matrix `A` using the Cholesky factorization approach """

    # TODO (Problem 4): Implement the Cholesky factorization approach to solving linear systems
    # Step 1: Perform Cholesky decomposition of A
    L = cholesky(A)

    # Step 2: Solve Ly = b for y (forward substitution)
    y = forwardsub(L, b)

    # Step 3: Solve L.T x = y for x (backward substitution)
    Lt = L.T
    x = backsub(Lt, y)

    return x

def relerror(a, b):
    """ Calculate the relative difference between vectors/matrices `a` and `b`. """
    return np.linalg.norm(a - b) / np.linalg.norm(b)

def syntheticFactors(m):
    L = np.random.uniform(size=(m, m))
    U = np.random.uniform(size=(m, m))
    L[np.triu_indices(m, 1)] = 0
    L[np.diag_indices(m)] = 1
    U[np.tril_indices(m, -1)] = 0
    return L, U

from scipy.linalg.blas import dtrsv
def forwardsub_scipy(L, b):
    return scipy.linalg.solve_triangular(L, b, lower=True, check_finite=False)

forwardsub_implementations = {'loop': forwardsub,
                              'scipy': forwardsub_scipy}

import unittest
class TestCases(unittest.TestCase):
    def requireSame(self, a, b, tol = 1e-12):
        self.assertLess(relerror(a, b), tol)

    def test_lu_slides_example(self):
        A = np.array([[2, 1, 1, 0],
                      [4, 3, 3, 1],
                      [8, 7, 9, 5],
                      [6, 7, 9, 8]], dtype=float)
        L, U = lu(A)
        self.requireSame(np.tril(L), np.array([[1., 0., 0., 0.], [2., 1., 0., 0.], [4., 3., 1., 0.], [3., 4., 1., 1.]]))
        self.requireSame(np.triu(U), np.array([[2., 1., 1., 0.], [0., 1., 1., 1.], [0., 0., 2., 2.], [0., 0., 0., 2.]]))

    def test_lu_synthetic(self):
        for i in range(10):
            m = np.random.randint(1, 8)
            L, U = syntheticFactors(m)
            Lc, Uc = lu(L @ U)
            self.requireSame(np.tril(Lc), L)
            self.requireSame(np.triu(Uc), U)

    def test_forwardsub_synthetic(self):
        for i in range(10):
            m = np.random.randint(1, 8)
            L, U = syntheticFactors(m)
            x = np.random.normal(size=m)
            b = L @ x
            for name, fs in forwardsub_implementations.items():
                self.requireSame(x, fs(L, b))

    def test_backsub_synthetic(self):
        for i in range(10):
            m = np.random.randint(1, 8)
            L, U = syntheticFactors(m)
            x = np.random.normal(size=m)
            b = U @ x
            self.requireSame(x, backsub(U, b))

    def test_solve_synthetic(self):
        for i in range(10):
            m = np.random.randint(1, 8)
            A = np.random.uniform(size=(m, m))
            x = np.random.normal(size=m)
            b = A @ x
            self.requireSame(x, solve(A, b))

if __name__ == '__main__':
    import sys
    if '-b' in sys.argv:
        # Benchmarking
        import matplotlib
        from matplotlib import pyplot as plt
        import timeit

        setup_command = lambda m, impl: f'from __main__ import syntheticFactors, forwardsub_implementations; import numpy as np; L, U = syntheticFactors({m}); b = np.random.normal(size={m}); forwardsub = forwardsub_implementations["{impl}"]'
        times = {}
        for impl in forwardsub_implementations:
            sizes = [16, 32, 64, 128, 200, 256, 400, 512, 756, 1024]
            if impl != 'loop': # Include larger matrices for the faster variants
                sizes.extend([1600, 2048, 3000, 4096])
            time = [min(timeit.repeat(f'forwardsub(L, b)', setup_command(m, impl), number=10, repeat=2)) for m in sizes]
            plt.loglog(sizes, time,  '-s', ms=4, label=impl)

        plt.xlabel('Matrix size $m$')
        plt.ylabel('Time per substitution (s)')
        plt.title('Forward Substitution Benchmarks')
        plt.legend()
        plt.grid()
        plt.tight_layout()
        plt.savefig('forwardsub_benchmarks.pdf')

    # Run the tests defined above.
    unittest.main()

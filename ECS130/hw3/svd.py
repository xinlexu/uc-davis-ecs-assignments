import eig as eig
import numpy as np

def svd(A, tol=1e-8, maxIterations=1000):
    """
    Construct the SVD of a potentially rectangular matrix A.
    """
    m, n = A.shape
    if m >= n:
        eigvals, eigvecs = np.linalg.eig(np.dot(A.T, A))
    else:
        eigvals, eigvecs = np.linalg.eig(np.dot(A, A.T))
    sigma = np.sqrt(eigvals)
    U = np.zeros((m, len(sigma)))
    V = np.zeros((n, len(sigma)))
    if m >= n:
        V = eigvecs
        for i in range(len(sigma)):
            U[:, i] = np.dot(A, V[:, i]) / sigma[i]
    else:
        U = eigvecs
        for i in range(len(sigma)):
            V[:, i] = np.dot(A.T, U[:, i]) / sigma[i]
    k = min(m, n)
    return U[:, :k], sigma[:k], V[:, :k]

def relerror(a, b):
    """ Calculate the relative difference between vectors/matrices `a` and `b`. """
    return np.linalg.norm(a - b) / np.linalg.norm(b)

import unittest
class TestCases(unittest.TestCase):
    def requireSame(self, a, b, tol = 1e-6):
        self.assertLess(relerror(a, b), tol)

    def test_svd(self):
        # Generate a random matrix.
        for i in range(100):
            m = np.random.randint(1, 30)
            n = np.random.randint(1, 30)
            A = np.random.normal(size=(m, n))
            k = min(m, n)
            U, sigma, V = svd(A)
            # Check that U is orthogonal.
            self.requireSame(U.T @ U, np.eye(k))
            # Check that V is orthogonal.
            self.requireSame(V.T @ V, np.eye(k))
            # Check that U, sigma, V reconstruct A.
            self.requireSame(A, U @ np.diag(sigma) @ V.T)

import unittest
if __name__ == '__main__':
    # Run the unit tests defined above.
    unittest.main()

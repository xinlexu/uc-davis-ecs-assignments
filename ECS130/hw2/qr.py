# ECS130 HW2 -- QR Factorization Algorithms
import linear_systems
import numpy as np
from numpy.linalg import norm

def modified_gram_schmidt(A):
    """ Constructs a reduced QR decomposition of `A` using the modified Gram-Schmidt algorithm. """
    m, n = A.shape
    if (m < n): raise ValueError("A must have at least as many rows as columns")

    Q = np.empty((m, n))
    R = np.empty((n, n))

    # TODO (Problem 9): Implement the modified Gram-Schmidt algorithm
    # https://julianpanetta.com/teaching/ECS130/10-QR-part2-deck.html#/modified-gram-schmidt-algorithm/5
    for j in range(n):
        vj = A[:, j]
        for i in range(j):
            R[i, j] = np.dot(Q[:, i].T, A[:, j])
            vj = vj - R[i, j] * Q[:, i]
        R[j, j] = np.linalg.norm(vj)
        Q[:, j] = vj / R[j, j]

    return Q, R

def householder(A):
    """
    Constructs a full QR decomposition of `A` using the Householder algorithm.
    The Q factor is represented in implicit form as a list of reflectors "v".
    """
    m, n = A.shape
    if (m < n): raise ValueError("A must have at least as many rows as columns")

    # TODO (Problem 11): Implement the Householder algorithm
    # https://julianpanetta.com/teaching/ECS130/10-QR-part2-deck.html#/householder-qr-algorithm/9
    Q = []
    R = A.copy()
    for k in range(n):
        x = R[k:m, k]
        e1 = np.zeros_like(x)
        e1[0] = 1.0
        v = x + np.sign(x[0]) * np.linalg.norm(x) * e1
        v = v / np.linalg.norm(v)
        R[k:m, k:n] -= 2 * np.outer(v, np.dot(v, R[k:m, k:n]))
        Q.append(v)

    return Q, R

# The different methods for solving a least-squares problem $A x = b$
# and their corresponding factorization algorithms.
methods = {
    'ModifiedGramSchmidt':         modified_gram_schmidt,
    'Householder':                 householder,
    'ModifiedGramSchmidtImproved': modified_gram_schmidt
}

def least_squares(A, b, method):
    """ Solves "A x = b" using the QR decomposition approach """

    if method not in methods: raise ValueError(f'Unknown method: {method}')
    factorizer = methods[method]

    m, n = A.shape
    Q, R = factorizer(A)
    result = np.empty(A.shape[1])
    result[:] = np.nan # NaN signals to the caller that the method is unimplemented

    if method == 'ModifiedGramSchmidt':
        # TODO (Problem 10): Implement the least-squares solution using the
        # reduced QR decomposition from the modified Gram-Schmidt algorithm.
        y = np.dot(Q.T, b)
        result = linear_systems.backsub(R, y)
    elif method == 'Householder':
        # TODO (Problem 12): Implement the least-squares solution using the
        # full QR decomposition from the Householder algorithm.
        Qtb = b.copy()
        for k, v in enumerate(Q):
            v_padded = np.zeros_like(Qtb)
            v_padded[k:] = v

            v_dot_Qtb = np.dot(v_padded, Qtb)
            Qtb -= 2 * v_dot_Qtb * v_padded
        result = linear_systems.backsub(R[:n, :], Qtb[:n])
    elif method == 'ModifiedGramSchmidtImproved':
        y = np.zeros(n)
        b_residual = b.copy()
        for i in range(n):
            y[i] = np.dot(Q[:, i], b_residual)
            b_residual -= y[i] * Q[:, i]
        result = linear_systems.backsub(R, y)

    return result

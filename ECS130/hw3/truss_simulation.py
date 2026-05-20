# ECS 130 HW2: Truss Simulation
import visualization
from matplotlib import pyplot as plt
import numpy as np
import pickle
import linear_systems

def construct_K(X, E, k):
    """
    Constructs the stiffness matrix K for a truss structure
    given the rest joint positions in the rows of X,
    edge endpoint indices in the rows of E,
    and the stiffness k of each edge.
    """
    numNodes = X.shape[0] # referred to as n in the handout
    numEdges = E.shape[0] # referred to as m in the handout

    # TODO (Problem 14): Construct the stiffness matrix K

    B = np.zeros((2 * numNodes, numEdges))
    for e in range(numEdges):
        i, j = E[e]
        edge = X[j] - X[i]
        length = np.linalg.norm(edge)
        a_e = edge / length
        B[2*i:2*(i+1), e] = a_e
        B[2*j:2*(j+1), e] = -a_e
    D = np.diag(k)
    K = B @ D @ B.T
    return K

def simulate(X, E, C, F, k):
    """
    Simulates the deformation of a truss structure given the rest joint positions
    in the rows of X, edge endpoint indices in the rows of E, indices of fixed
    joints in the array C, and the stiffness k of each edge.
    """
    numNodes = X.shape[0] # referred to as n in the handout
    numEdges = E.shape[0] # referred to as m in the handout

    # TODO (Problem 15): Simulate the deformation of the truss structure
    K = construct_K(X, E, k)
    for c in C:
        K[2*c:2*(c+1), :] = 0
        K[:, 2*c:2*(c+1)] = 0
        K[2*c, 2*c] = 1
        K[2*c+1, 2*c+1] = 1
    F_mod = F.copy()
    for c in C:
        F_mod[2*c:2*(c+1)] = 0
    U = linear_systems.solve_cholesky(K, F_mod)
    return U

if __name__ == '__main__':
    import sys
    if len(sys.argv) != 2:
        print("Usage: python3 truss_simulation.py <data file>")
        sys.exit(1)

    data = pickle.load(open(sys.argv[1], 'rb'))

    name = data['name']
    X = data['X']
    E = data['E']
    C = np.array(data['C'])
    F = data['F']
    k = data['k']

    # Visualize the truss's rest configuration
    axisLimits = visualization.visualizeTruss(X, E, C, F)
    plt.savefig(f'{name}_init.pdf')
    plt.close()

    # Simulate the truss's deformation
    U = simulate(X, E, C, F.ravel(), k).reshape((-1, 2))
    x = X + U

    # Visualize the truss's deformed configuration
    visualization.visualizeTruss(x, E, C, F, axisLimits=axisLimits)
    plt.savefig(f'{name}_deformed.pdf')
    plt.close()

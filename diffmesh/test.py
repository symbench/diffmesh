import numpy as np
import pygalmesh


def _row_dot(a, b):
    # http://stackoverflow.com/a/26168677/353337
    return np.einsum("ij, ij->i", a, b)


def compute_volumes(vertices, tets):
    cell_coords = vertices[tets]

    a = cell_coords[:, 1, :] - cell_coords[:, 0, :]
    b = cell_coords[:, 2, :] - cell_coords[:, 0, :]
    c = cell_coords[:, 3, :] - cell_coords[:, 0, :]

    # omega = <a, b x c>
    omega = _row_dot(a, np.cross(b, c))

    # https://en.wikipedia.org/wiki/Tetrahedron#Volume
    return abs(omega) / 6.0


def compute_triangle_areas(vertices, triangles):
    e0 = vertices[triangles[:, 1]] - vertices[triangles[:, 0]]
    e1 = vertices[triangles[:, 2]] - vertices[triangles[:, 1]]

    assert e0.shape == e1.shape
    if e0.shape[1] == 2:
        z_component_of_e0_cross_e1 = np.cross(e0, e1)
        cross_magnitude = z_component_of_e0_cross_e1
    else:
        assert e0.shape[1] == 3
        e0_cross_e1 = np.cross(e0, e1)
        cross_magnitude = np.sqrt(_row_dot(e0_cross_e1, e0_cross_e1))

    return 0.5 * cross_magnitude


def test_rectangle():
    points = np.array([[0.0, 0.0], [1.0, 0.0], [1.0, 1.0], [0.0, 1.0]])
    constraints = [[0, 1], [1, 2], [2, 3], [3, 0]]

    mesh = pygalmesh.generate_2d(
        points, constraints, max_edge_size=1.0e-1, num_lloyd_steps=10
    )

    assert mesh.points.shape == (276, 2)
    assert mesh.get_cells_type("triangle").shape == (486, 3)

    import matplotlib.pyplot as plt
    for e in mesh.get_cells_type("triangle"):
        p0 = mesh.points[e[0]]
        p1 = mesh.points[e[1]]
        p2 = mesh.points[e[2]]
        plt.plot([p0[0], p1[0]], [p0[1], p1[1]], "-k")
        plt.plot([p1[0], p2[0]], [p1[1], p2[1]], "-k")
        plt.plot([p2[0], p0[0]], [p2[1], p0[1]], "-k")
    for p in mesh.points:
        plt.plot(p[0], p[1], "or")
    plt.gca().set_aspect("equal")
    plt.show()

    # mesh.points *= 100
    # mesh.write("rect.svg")


def test_disk():
    h = 0.2
    n = int(2 * np.pi / h)
    alpha = np.linspace(0.0, 2 * np.pi, n + 1, endpoint=False)
    points = np.column_stack([np.cos(alpha), np.sin(alpha)])

    constraints = [[k, k + 1] for k in range(n)] + [[n, 0]]
    mesh = pygalmesh.generate_2d(
        points, constraints, max_edge_size=h, num_lloyd_steps=0
    )
    areas = compute_triangle_areas(
        mesh.points, mesh.get_cells_type("triangle"))
    assert np.all(areas > 1.0e-5)

    import matplotlib.pyplot as plt
    for e in mesh.get_cells_type("triangle"):
        p0 = mesh.points[e[0]]
        p1 = mesh.points[e[1]]
        p2 = mesh.points[e[2]]
        plt.plot([p0[0], p1[0]], [p0[1], p1[1]], "-k")
        plt.plot([p1[0], p2[0]], [p1[1], p2[1]], "-k")
        plt.plot([p2[0], p0[0]], [p2[1], p0[1]], "-k")
    for p in mesh.points:
        plt.plot(p[0], p[1], "or")
    plt.gca().set_aspect("equal")
    plt.show()


if __name__ == "__main__":
    test_disk()
    # test_rectangle()

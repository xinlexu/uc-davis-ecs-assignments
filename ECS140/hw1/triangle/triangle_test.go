package triangle

import "testing"

func TestGetTriangleType(t *testing.T) {
	type Test struct {
		a, b, c  int
		expected triangleType
	}

	var tests = []Test{
		{30001, 6, 2, UnknownTriangle},
		{15, 20001, 5, UnknownTriangle},
		{5, 20, 10001, UnknownTriangle},
		{0, 5, 6, UnknownTriangle},
		{6, -1, 6, UnknownTriangle},
		{6, 5, 0, UnknownTriangle},
		{1, 2, 3, InvalidTriangle},
		{3, 1, 2, InvalidTriangle},
		{2, 3, 1, InvalidTriangle},
		{5, 4, 3, RightTriangle},
		{5, 5, 5, AcuteTriangle},
		{7, 3, 5, ObtuseTriangle},
	}

	for _, test := range tests {
		actual := getTriangleType(test.a, test.b, test.c)
		if actual != test.expected {
			t.Errorf("getTriangleType(%d, %d, %d)=%v; want %v", test.a, test.b, test.c, actual, test.expected)
		}
	}
}

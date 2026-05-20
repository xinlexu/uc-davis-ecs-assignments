package min

import "testing"

type Test struct {
	in  []int
	out int
}

var tests = []Test{
	{
		in:  []int{-1, 0, 1, 2, 4},
		out: -1,
	},
	{
		in:  []int{1},
		out: 1,
	},
	{
		in:  []int{},
		out: 0,
	},
	{
		in:  nil,
		out: 0,
	},
	{
		in:  []int{1000000, 2000000, 3000000, -4000000, 5000000},
		out: -4000000,
	},
	{
		in:  []int{2147483647, -2147483648, 0},
		out: -2147483648,
	},
	{
		in:  []int{-10, -20, -30, -40, -50},
		out: -50,
	},
}

func TestMin(t *testing.T) {
	for i, test := range tests {
		m := Min(test.in)
		if m != test.out {
			t.Errorf("#%d: Min(%v)=%d; want %d", i, test.in, m, test.out)
		}
	}
}

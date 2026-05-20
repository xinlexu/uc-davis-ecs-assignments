package depth

import (
	"hw1/expr"
)

func max(x, y uint) uint {
	if x > y {
		return x
	}
	return y
}

// Depth returns the maximum number of AST nodes between the root of the
// tree and any leaf (literal or variable) in the tree.
func Depth(e expr.Expr) uint {
	switch e := e.(type) {
	case expr.Literal:
		return 1
	case expr.Var:
		return 1
	case expr.Unary:
		return 1 + Depth(e.X)
	case expr.Binary:
		return 1 + max(Depth(e.X), Depth(e.Y))
	default:
		panic("Depth: unknown Expr")
	}
}

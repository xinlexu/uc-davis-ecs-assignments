package simplify

import (
	"hw1/expr"
)

func Simplify(e expr.Expr, env expr.Env) expr.Expr {
	switch e := e.(type) {
	case expr.Literal:
		return e
	case expr.Var:
		if val, ok := env[e]; ok {
			return expr.Literal(val)
		}
		return e
	case expr.Unary:
		switch e.Op {
		case '+':
			return Simplify(e.X, env)
		case '-':
			x := Simplify(e.X, env)
			if litX, ok := x.(expr.Literal); ok {
				return expr.Literal(-litX)
			}
			return expr.Unary{Op: e.Op, X: x}
		}
	case expr.Binary:
		x := Simplify(e.X, env)
		y := Simplify(e.Y, env)
		litX, okX := x.(expr.Literal)
		litY, okY := y.(expr.Literal)

		if okX && okY {
			switch e.Op {
			case '+':
				return expr.Literal(litX + litY)
			case '-':
				return expr.Literal(litX - litY)
			case '*':
				return expr.Literal(litX * litY)
			case '/':
				return expr.Literal(litX / litY)
			}
		}

		if okX || okY {
			switch e.Op {
			case '+':
				if okX && litX == 0 {
					return y
				}
				if okY && litY == 0 {
					return x
				}
			case '*':
				if okX {
					if litX == 0 {
						return expr.Literal(0)
					}
					if litX == 1 {
						return y
					}
				}
				if okY {
					if litY == 0 {
						return expr.Literal(0)
					}
					if litY == 1 {
						return x
					}
				}
			}
		}
		return expr.Binary{Op: e.Op, X: x, Y: y}
	default:
		panic("Simplify: unknown Expr")
	}
	return e
}

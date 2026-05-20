package rewrite

import (
	"bytes"
	"fmt"
	"go/ast"
	"go/format"
	"go/parser"
	"go/token"
	"hw1/expr"
	"hw1/simplify"
	"strconv"
)

// rewriteCalls should modify the passed AST
func rewriteCalls(node ast.Node) {
	ast.Inspect(node, func(n ast.Node) bool {
		callExpr, ok := n.(*ast.CallExpr)
		if !ok {
			return true
		}
		selExpr, ok := callExpr.Fun.(*ast.SelectorExpr)
		if !ok {
			return true
		}
		if selExpr.Sel.Name != "ParseAndEval" {
			return true
		}
		if len(callExpr.Args) != 2 {
			return true
		}
		lit, ok := callExpr.Args[0].(*ast.BasicLit)
		if !ok || lit.Kind != token.STRING {
			return true
		}
		exprStr, _ := strconv.Unquote(lit.Value)
		parsedExpr, err := expr.Parse(exprStr)
		if err != nil {
			fmt.Printf("Failed to parse expression: %s\n", exprStr)
			return true // Skip rewriting if parsing fails
		}
		simplifiedExpr := simplify.Simplify(parsedExpr, expr.Env{})
		simplifiedStr := expr.Format(simplifiedExpr)
		callExpr.Args[0] = &ast.BasicLit{
			Kind:  token.STRING,
			Value: strconv.Quote(simplifiedStr),
		}
		return false
	})
}

func SimplifyParseAndEval(src string) string {
	fset := token.NewFileSet()
	f, err := parser.ParseFile(fset, "src.go", src, 0)
	if err != nil {
		panic(err)
	}

	rewriteCalls(f)

	var buf bytes.Buffer
	format.Node(&buf, fset, f)
	return buf.String()
}

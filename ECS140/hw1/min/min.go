package min

// Min returns the minimum value in the arr,
// and 0 if arr is nil.
func Min(arr []int) int {
	if arr == nil || len(arr) == 0 {
		return 0
	}
	min := arr[0]
	for _, value := range arr {
		if value < min {
			min = value
		}
	}
	return min
}

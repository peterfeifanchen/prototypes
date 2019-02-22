package main

import (
	"fmt"
	"reflect"
)

type Input struct {
	jsons map[string][]byte
}

// Compares the things that they point to
func compareInput(a, b *Input) bool {
	return reflect.DeepEqual(a, b)
}

func main() {
	a := &Input{
		jsons: map[string][]byte{
			"a": []byte("a string"),
			"b": []byte("another string"),
		},
	}
	b := &Input{
		jsons: map[string][]byte{
			"a": []byte("a string"),
			"b": []byte("another string"),
		},
	}
	c := &Input{
		jsons: map[string][]byte{
			"b": []byte("another string"),
			"a": []byte("a string"),
		},
	}
	d := &Input{
		jsons: map[string][]byte{
			"b": []byte("another string"),
			"c": []byte("a string"),
		},
	}
	e := &Input{
		jsons: map[string][]byte{
			"b": []byte("another string"),
			"a": []byte("different string"),
		},
	}

	fmt.Println(compareInput(a, b)) //true
	fmt.Println(compareInput(a, c)) //true
	fmt.Println(compareInput(a, d)) //false
	fmt.Println(compareInput(a, e)) //false
}

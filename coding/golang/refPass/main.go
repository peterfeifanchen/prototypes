package main

import "fmt"

func mapPassByRef(m map[string][]byte) {
	m["newkey1"] = []byte("I added a new key")
}

func mapReturnByRef() map[string][]byte {
	m := make(map[string][]byte)
	m["newkey2"] = []byte("I added a new key")
	return m
}

func main() {
	// Are maps passed by reference?
	m := make(map[string][]byte)
	mapPassByRef(m)
	fmt.Println("newkey1:", string(m["newkey1"]))

	m = mapReturnByRef()
	fmt.Println("newkey2:", string(m["newkey2"]))
}

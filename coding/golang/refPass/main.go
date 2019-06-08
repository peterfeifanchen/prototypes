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
	mOriginal := make(map[string][]byte)
	mapPassByRef(mOriginal)
	fmt.Println("newkey1:", string(mOriginal["newkey1"]))

	mOriginal = mapReturnByRef()
	fmt.Println("newkey1:", string(mOriginal["newkey1"]))
	fmt.Println("newkey2:", string(mOriginal["newkey2"]))
}

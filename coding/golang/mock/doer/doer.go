package doer

// Doer implements SendTo and ReceiveFrom, two functions that we will mock for
// purposes such as requiring a network connection, non-deterministic output or speed.

type Doer interface {
	SendTo(addr string, port int, msg string) error
	ReceiveFrom(addr string, port int) string
}

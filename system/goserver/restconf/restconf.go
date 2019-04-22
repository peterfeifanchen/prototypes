package main

import (
	"crypto/tls"
	"fmt"
)

type Config struct {
	restCertFile string
	restKeyFile  string
}

type Status struct {
	cert *tls.Certificate
}

func main() {
	cfg := &Config{
		restCertFile: "abcdef",
		restKeyFile:  "abcdef",
	}

	status := &Status{}

	if err = initTLSCerts(cfg, status); err != nil {
		t.Fatal(err)
	}

}

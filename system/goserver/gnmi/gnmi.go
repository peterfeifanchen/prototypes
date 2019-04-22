package main

import (
	"fmt"
)

type Server struct {
	ctx context.Context

	// add services (aaa, data models, etc.,) here

}

type Config struct {
	grpcCertFile string
	grpcKeyFile  string
}

type Status struct {
	cert *tls.Certificate
}

func main() {

	if err = initTLSCerts(cfg, status); err != nil {
		t.Fatal(err)
	}
}

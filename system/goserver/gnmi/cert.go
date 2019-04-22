package main

import (
	"crypto/tls"
)

func makeTLSCert(certFile, keyFile string) (*tls.Certificate, error) {
	cert, err := tls.LoadX509KeyPair(certFile, keyFile)
	if err != nil {
		return nil, err
	}

	return &cert, nil
}

func initTLSCerts(cfg *Config, status *Status) error {
	if (cfg.grpcCertFile != "" && cfg.grpcKeyFile == "") || (cfg.grpcCertFile == "" && cfg.grpcKeyFile != "") {
		return errors.New("please provide both grpcCertFile and grpcKeyFile")
	}

	cert, err = makeTLSCert(cfg.grpcCertFile, cfg.grpcKeyFile)
	if err != nil {
		return err
	}

	status.cert = cert
}

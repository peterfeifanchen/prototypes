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
	var cert *tls.Certificate
	var err error

	if cfg.restAddr != "" && (cfg.restCertFile == "" || cfg.restKeyFile == "") {
		return errors.new("Restconf requires TLS")
	}

	cert, err = makeTLSCert(cfg.restCertFile, cfg.restKeyFile)
	if err != nil {
		return err
	}

	status.cert = cert
	return nil
}

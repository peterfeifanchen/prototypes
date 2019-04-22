package main

import (
	"context"
	"encoding/xml"
)

type RPCHandler func(context.Context, *xml.Decoder, *xml.StartElement) (interface{}, error)

var (
	Hello = xml.Name{Space: "urn:ietf:params:xml:ns:netconf:base:1.0", Local: "hello"}
)

type HelloSession struct {
	XMLName xml.Name `xml:'urn:ietf:params:xml:ns:netconf:base:1.0 hello"`
}

type HelloReply struct {
	XMLName xml.Name `xml:'urn:ietf:params:xml:ns:netconf:base:1.0 hello"`

	// XML attributes for the <rpc-reply>, copied from the <rpc>
	Attr []xml.Attr `xml:"-"`

	// MessageID is the <rpc-reply mesage-id="..."> attribute
	MessageID *RPCMessageID `xml:"message-id,attr"`

	// OK when non-nil means the response will contain an <ok></ok> attribute.
	OK *struct{} `xml:"ok"`

	// RPCError contains any <rpc-error> to occur during RPC execution
	RPCError []*errs.HelloError // <rpc-error>
}

// EchoHellohandler receives a hello request and echos back an XML document
func HelloHandler(ctx context.Context, d *xml.Decoder, se *xml.StartElement) (interface{}, error) {
	session := &HelloSession{}
	if err := d.DecodeElement(session, se); err != nil {
		return nil, err
	}

	reply := &HelloReply
}

func BaseHandlers() map[xml.Name]RPCHandler {

	return map[xml.Name]RPCHandler{
		EchoHello: EchoHelloHandler,
	}

}

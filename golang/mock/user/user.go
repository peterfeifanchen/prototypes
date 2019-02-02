package user

import (
	"prototypes/mock/doer"

	"github.com/golang/mock/gomock"
)

type User struct {
	Doer doer.Doer
}

func (u *User) SendTo(addr string, port int, msg string) error {
	return u.Doer.SendTo(addr, port, msg)
}

func (u *User) ReceiveFrom(addr string, port int) string {
	return u.Doer.ReceiveFrom(addr, port)
}

type ipMatcher struct {
}

func NewIPMatcher() gomock.Matcher {
	return &ipMatcher{}
}

func (m *ipMatcher) String() string {
	return ""
}

func (m *ipMatcher) Matches(x interface{}) bool {
	return true
}

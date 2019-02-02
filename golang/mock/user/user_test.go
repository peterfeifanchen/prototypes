package user

import (
	"fmt"
	"testing"

	"github.com/golang/mock/gomock"
	"prototypes/mock/mocks"
)

func TestUse(t *testing.T) {
	// Create a mock controller responsible for tracking and asserting EXPECT of mock objects
	mockCtrl := gomock.NewController(t)
	// Programming pattern, Finish asserts at the end that all the methods that were expected
	// to be called were called
	defer mockCtrl.Finish()

	mockDoer := mocks.NewMockDoer(mockCtrl)
	testUser := &User{Doer: mockDoer}

	// Expect Do to be called once with the following parameters and return nil at the end.
	// Each EXPECT() is one singular instance of an expected call. Notice testUser is in
	// different order. Order between EXPECT must by specified (see TestUse4).
	mockDoer.EXPECT().SendTo("127.0.0.2", 22, "Hello GoMock").Return(nil).Times(1)
	mockDoer.EXPECT().SendTo(NewIPMatcher(), gomock.Not(0), gomock.Any()).Return(nil)
	// Expect Do to be called once with the following parameters and return "ok" at the end
	mockDoer.EXPECT().ReceiveFrom("127.0.0.2", 22).Return("ok").Times(1)

	// If these calls were absent, mockDoer's sendTo/receiveFrom would not be called and
	// mockCtrl.Finish would assert that error
	testUser.SendTo("127.0.0.1", 22, "Hello GoMock")
	testUser.SendTo("127.0.0.2", 22, "Hello GoMock")
	msg := testUser.ReceiveFrom("127.0.0.2", 22)

	if msg != "ok" {
		t.Fatal("Did not receive ok")
	}
}

func TestUse2(t *testing.T) {
	mockCtrl := gomock.NewController(t)
	defer mockCtrl.Finish()

	mockDoer := mocks.NewMockDoer(mockCtrl)
	testUser := &User{Doer: mockDoer}

	// Matchers can be used to match parameters. This can be the default: gomock.Eq, gomock.Any,
	// gomock.Not, gomock.Nil or a custom matcher
	mockDoer.EXPECT().SendTo(NewIPMatcher(), gomock.Not(0), gomock.Any()).Return(nil)
	// This will cause mockCtrl.Finish() to assert failure
	// testUser.SendTo("127.0.0.1", 0, "Hello")
	testUser.SendTo("127.0.0.1", 22, "Hello GoMock")
}

func TestUse3(t *testing.T) {
	mockCtrl := gomock.NewController(t)
	defer mockCtrl.Finish()

	mockDoer := mocks.NewMockDoer(mockCtrl)
	testUser := &User{Doer: mockDoer}

	// Call order can also be specified
	callFirst := mockDoer.EXPECT().SendTo("localhost", 22, "Hello")
	mockDoer.EXPECT().ReceiveFrom("localhost", 22).After(callFirst).Return("Hi")

	// SendTo must preceed ReceiveFrom with the given parameters
	testUser.SendTo("localhost", 22, "Hello")
	msg := testUser.ReceiveFrom("localhost", 22)

	if msg != "Hi" {
		t.Fatal("Did not receive Hi")
	}

	// More cleanly this can be written as
	gomock.InOrder(
		mockDoer.EXPECT().SendTo("localhost", 22, "Hello"),
		mockDoer.EXPECT().ReceiveFrom("localhost", 22).Return("Hi1"),
		mockDoer.EXPECT().SendTo("localhost", 22, "Hello"),
		mockDoer.EXPECT().ReceiveFrom("localhost", 22).Return("Hi2"),
	)

	testUser.SendTo("localhost", 22, "Hello")
	msg1 := testUser.ReceiveFrom("localhost", 22)
	testUser.SendTo("localhost", 22, "Hello")
	msg2 := testUser.ReceiveFrom("localhost", 22)

	if msg1 != "Hi1" || msg2 != "Hi2" {
		t.Fatal("Did not receive correct Hi1 and Hi2")
	}
}

func TestUse4(t *testing.T) {
	mockCtrl := gomock.NewController(t)
	defer mockCtrl.Finish()

	mockDoer := mocks.NewMockDoer(mockCtrl)
	testUser := &User{Doer: mockDoer}

	// So can specialized mock functions for performing some general action.
	mockDoer.EXPECT().
		SendTo(gomock.Any(), gomock.Any(), "Hello").
		Return(nil).
		Do(func(addr string, port int, msg string) {
			fmt.Println("Called with addr =", addr, ":", port, "and msg =", msg)
		})

	testUser.SendTo("localhost", 22, "Hello")
}

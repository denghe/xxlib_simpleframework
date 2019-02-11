package main

import (
	"fmt"
	"net"
	"os"
)

var input = make(chan interface{})

type Connection struct {
	conn net.Conn
	buf []byte
}

func Server() {
	listener, err := net.Listen("tcp", ":12345")
	if err != nil {
		fmt.Println("connect error: ", err)
		os.Exit(-1)
	}
	fmt.Println("listening at 12345")
	for {
		conn, err := listener.Accept()
		if err != nil {
			fmt.Println("accept error: ", err)
			continue
		}
		//err = conn.SetDeadline(time.Now().Add(time.Duration(10) * time.Second))
		//if err != nil {
		//	fmt.Println("conn.SetDeadline error: ", err)
		//	continue
		//}
		defer conn.Close()
		fmt.Println(conn.RemoteAddr(), " accepted.")
		n := int(0)
		buf := make([]byte, 10)
		for {
			n, err = conn.Read(buf)
			if err != nil {
				fmt.Println("conn.Read error: ", err)
				break
			} else if len(buf) > 0 {
				fmt.Println(n, buf)
			}
		}
	}
}

func Client() {
	conn, err := net.Dial("tcp", "127.0.0.1:12345")
	if err != nil {
		fmt.Println("accept error: ", err)
		return
	}
	defer conn.Close()
	conn.Write([]byte("asdf"))

}



func main() {
	go Server()
	Client()
}


//
//input := make(chan interface{})
//
////producer - produce the messages
//go func() {
//	for i := 0; i < 5; i++ {
//		input <- i
//	}
//	input <- "hello, world"
//}()
//
//t1 := time.NewTimer(time.Second * 5)
//t2 := time.NewTimer(time.Second * 10)
//
//for {
//select {
////consumer - consume the messages
//case msg := <-input:
//fmt.Println(msg)
//
//case <-t1.C:
//println("5s timer")
//t1.Reset(time.Second * 5)
//
//case <-t2.C:
//println("10s timer")
//t2.Reset(time.Second * 10)
//}
//}

//
//func sum(s []int, c chan int) {
//	sum := 0
//	for _, v := range s {
//		sum += v
//		time.Sleep(time.Second)
//	}
//	c <- sum // send sum to c
//}
//
//func main() {
//	s := []int{7, 2, 8, -9, 4, 0}
//	c := make(chan int)
//	go sum(s[:len(s)/2], c)
//	go sum(s[len(s)/2:], c)
//	x, y := <-c, <-c // receive from c
//	fmt.Println(x, y, x+y)
//}

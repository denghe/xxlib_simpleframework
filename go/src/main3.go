package main

import (
	"fmt"
	"net"
	"sync"
	"time"
)

func test1() {
	var wg sync.WaitGroup
	wg.Add(1000)
	ticker := time.NewTicker(time.Microsecond * 1)
	lastTime := time.Now()
	go func() {
		for i := 0; i < 1000; i++ {
			<- ticker.C
			wg.Done()
		}
	}()
	wg.Wait()
	fmt.Println("done. secs = ", time.Now().Sub(lastTime).Seconds())
}

type RPC struct {
	timer time.Timer
}



var recvs = make(chan interface{}, 3)

// go
func initNetwork() {
	listener, err := net.Listen("tcp", ":12345")
	if err != nil {
		panic(err)
	}
	for {
		conn, err := listener.Accept()
		if err != nil {
			fmt.Println("accept error: ", err)
			continue
		}
		go func() {
			defer func() {
				conn.Close()
				fmt.Println("conn.Close.")
			}()
			fmt.Println(conn.RemoteAddr(), " accepted.")
			n := 0
			buf := make([]byte, 10)
			for {
				n, err = conn.Read(buf)
				if err != nil {
					fmt.Println("conn.Read error: ", err)
					goto AfterFor
				} else if n > 0 {
					//fmt.Println(n, buf)
					select {
					case recvs <- buf[0]:
					default:
						goto AfterFor
					}

				}
			}
			AfterFor:
		}()
	}
}

var frameNumber = 0
func frameUpdate() {
	println("logic: frameNumber = ", frameNumber)

	for {
		select {
		case recv := <-recvs:
			fmt.Println(recv)
		default:
			goto AfterFor
		}
	}
	AfterFor:

	frameNumber++
}

func mainLoop() {
	ticksPerFrame := time.Second	// / 60
	var ticksPool time.Duration
	lastTicks := time.Now()

	for {
		currTicks := time.Now()
		ticksPool += currTicks.Sub(lastTicks)
		lastTicks = currTicks

		for ; ticksPool > ticksPerFrame; {
			frameUpdate()
			ticksPool -= ticksPerFrame
		}

		time.Sleep(time.Microsecond * 1)
	}
}
func main() {
	go initNetwork()
	mainLoop()
}

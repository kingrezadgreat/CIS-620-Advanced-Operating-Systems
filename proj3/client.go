
package main
 
import (
    "os"
    "bufio"
    "fmt"
    "net"
    "time"
    "strconv"
    //"bytes"
    "encoding/binary"
    "math"
    "strings"

)

 
func CheckError(err error) {
    if err  != nil {
        fmt.Println("Error: " , err)
    }
}
 
func main() {
    // UDP start to service map
    
    // setting up the address of servicemap
    ServerAddr,err := net.ResolveUDPAddr("udp","10.219.255.255:28062")
    CheckError(err)

    // setting up the address of local machine which is client
    LocalAddr, err := net.ResolveUDPAddr("udp", ":0")
    CheckError(err)
    
    // binding to socket   
    CLNS, err := net.ListenUDP("udp", LocalAddr)
    CheckError(err)
    
    // close at the end of program 
    defer CLNS.Close()
    
    // buffer to hold values from service map
    buff_in := make([]byte, 1024) 


    buf := []byte("GET CISBANK")
    _,err1 := CLNS.WriteToUDP(buf, ServerAddr)
    if err1 != nil {
        fmt.Println("Error", err1)
    }
    n,_,err := CLNS.ReadFromUDP(buff_in)
    fmt.Println("Service provided by", string(buff_in[0:n-7]), "port", string(buff_in[n-6:n]))
    time.Sleep(time.Second * 1)

    //  UDP end to service map
    //---------------------------------------------------------------------------
    // TCP start to server
    
    // start a while loop to get command from user
    for{

        // setup a reader
        reader := bufio.NewReader(os.Stdin)
        fmt.Print("> ")

        // take input from user as soon as put enter    
        text, _ := reader.ReadString('\n')
        
        // if the command has query inside do the following --> this might have error if query is not at the right location    
        if strings.Contains(text, "query"){
            // split the input text and remove \n at the end
            input_txt := strings.Split(text, " ")
            acct_num_txt := strings.Split(input_txt[1], "\n")
            
            // convert the input value (in charachters) to Uint (4 byte) value
            acct_num_64, err := strconv.ParseUint(acct_num_txt[0], 10, 64)
            if err != nil {
                fmt.Println(err)
            }
            
            // set up the 2 values sent to server
            var msg1 uint32 = 1000
            var msg2 uint32 = uint32(acct_num_64)
            
            // setup the outgoing buffer to server with 8 byte length
            buff_out := make ([]byte,8)
            
            // make tcp connection 
            conn, err := net.Dial("tcp", string(buff_in[0:n-1]))
            if err != nil {
                fmt.Println("fail!")
	        return 
            }
            
            // defer exiting the TCP to the end of the program once "quit" is entered
            defer conn.Close()
            
            // converting using big endian
            binary.BigEndian.PutUint32(buff_out[0:], msg1)
            binary.BigEndian.PutUint32(buff_out[4:], msg2)

            // sending buff_out to server
            conn.Write([]byte(buff_out))
            
            // take the result back from server and print 
            buffer := make([]byte, 1024)
            n, err := conn.Read(buffer)
            fmt.Println(string(buffer[:n]))
            
        } else if strings.Contains(text, "update"){
        
            // split the input text and remove \n at the end        
            input_txt := strings.Split(text, " ")
            acct_num_txt := strings.Split(input_txt[1], "\n")

            // convert the input value (in charachters) to Uint (4 byte) value
            acct_num_64, err := strconv.ParseUint(acct_num_txt[0], 10, 64)
            if err != nil {
                fmt.Println(err)
            }
            
            // remove the enter \n from the end of user command
            amount_txt := strings.Split(input_txt[2], "\n")

            // converting the 3rd value into a float32
            amount_float, err := strconv.ParseFloat(amount_txt[0], 32)
            if err != nil {
                fmt.Println(err)
            }
            var new_amount_float float32 = float32(amount_float)
            
            // set up the 3 values sent to server
            var msg1 uint32 = 1001
            var msg2 uint32 = uint32(acct_num_64)
            var msg3 uint32 = math.Float32bits(new_amount_float)
            
            // setup the outgoing buffer to server with 8 byte length
            buff_out := make ([]byte,12)
            
            // setup the TCP connection and make connection
            conn, err := net.Dial("tcp", string(buff_in[0:n-1]))
            if err != nil {
                fmt.Println("fail!")
	        return 
            }
            // defer closing connection to when user enter quit
            defer conn.Close()
            
            // big endian conversion
            binary.BigEndian.PutUint32(buff_out[0:], msg1)
            binary.BigEndian.PutUint32(buff_out[4:], msg2)
            binary.BigEndian.PutUint32(buff_out[8:], msg3)
            
            // write buff_out to TCP        
            conn.Write([]byte(buff_out))
            
            // setup read buffer from server which contans data 
            buffer := make([]byte, 1024)
            n, err := conn.Read(buffer)
            
            // print data from server
            fmt.Println(string(buffer[:n]))
        } else if strings.Contains(text, "quit"){
            // if user enters quit, BREAK , which exits the loop which is the end of program. TCP closes here
            break
        }
    }
    
}







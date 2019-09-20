
package main
 
import (
    "fmt"
    "net"
    "os"
    "strings"
)

// the hash (map) of input data from server 1)port 2)addr
type MAP struct {
    port string
    addr string
}
 
// error handling
func CheckError(err error) {
    if err  != nil {
        fmt.Println("Error: " , err)
        os.Exit(0)
    }
}
 

func main() {

    // initilize map
    db_map := make(map[string]MAP)

    /* Let's prepare an address at any address at port 28062*/   
    ServerAddr,err := net.ResolveUDPAddr("udp",":28062")
    CheckError(err)
 
    /* Now listen. actually binds at port */
    ServerCLNS, err := net.ListenUDP("udp", ServerAddr)
    CheckError(err)
    
    // defer closing the port to the end of program which only happens when ctr+c is entered 
    defer ServerCLNS.Close()
    
    // set an input buffer 
    buf := make([]byte, 1024)
    
    
    for {
        // read from UDP port from outside --> this can be server which is sending its info to service map to save
        n,addr,err := ServerCLNS.ReadFromUDP(buf)
        
        // receive info from udp and print data
        fmt.Println("Received from ",addr.IP.String(),addr.Port,":" ,string(buf[0:n]))
        result := strings.Split(string(buf[0:n]), " ")
        
        // if data has PUT in the command then save the address and port into map
        if strings.Contains(string(buf[0:n]), "PUT"){
            db_map[string(result[1])] = MAP{string(result[2]), addr.IP.String()}
            // send back OK to let server know data was received
            ServerCLNS.WriteToUDP([]byte("OK"),addr)
        }
        
        //if command contains GET, generate the data according to format and send ot bacl to origine
        if strings.Contains(string(buf[0:n]), "GET"){
            for db, target := range db_map {
                // if the name of the server matches with one of the saved data--> send info of server bacl to client
                if db == string(result[1]){
                    ret_msg := ""
                    ret_msg += target.addr
                    ret_msg += ":"
                    ret_msg += target.port
                    // write back to UDP the info of server
                    ServerCLNS.WriteToUDP([]byte(ret_msg),addr)                                        
                }
            }
        }
        
        if err != nil {
            fmt.Println("Error: ",err)
        } 
    }
}

package main
import "fmt"
import "gopkg.in/mgo.v2/bson"

func main() {
	var k = bson.M{
		"rpm": 10000,
	}
	//print k in hex format	
	fmt.Printf("%x", k)

}

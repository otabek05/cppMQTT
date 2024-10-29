package main

import (
	"fmt"

	"github.com/gin-gonic/gin"
)


func main(){
	router := gin.Default()

    router.GET("/test", func(c *gin.Context) {
		c.JSON(200, "Server is working")
	})

	router.POST("/data", func (c *gin.Context)  {
		var body any

		err := c.BindJSON(&body)
		if err!= nil {
            c.JSON(400, gin.H{"error": err.Error()})
            return
        }
		fmt.Println(body)
		c.JSON(200, gin.H{"data": body})
	})

	router.Run(":8080")

}
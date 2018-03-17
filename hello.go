package hello

import (
	// "fmt"
	"net/http"
)

func init() {
	http.Handle("/", http.FileServer(http.Dir("./portfolio")))
	// http.HandleFunc("/portfolio", portfolioHandler)
}

// func handler(w http.ResponseWriter, r *http.Request) {
// 	fmt.Fprint(w, "Yolo, world!")
// }

// func portfolioHandler(w http.ResponseWriter, r *http.Request) {
// 	fmt.Fprint(w, "Portfolio, world!")
// }
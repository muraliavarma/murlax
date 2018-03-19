package hello

import (
	// "fmt"
	"net/http"
)

func init() {
	http.Handle("/", http.FileServer(http.Dir("./public")))
	http.HandleFunc("/portfolio", portfolioHandler)
}

// func handler(w http.ResponseWriter, r *http.Request) {
// 	fmt.Fprint(w, "Yolo, world!")
// }

func portfolioHandler(w http.ResponseWriter, r *http.Request) {
	http.Redirect(w, r, "/", http.StatusMovedPermanently)
}

/*

Slots Challenges Framework in ReactJS and PHP

AWS Elasticache Migration from Memcached and Lambda functions

YouSpot Chrome Extension

RayTracer Engine in NodeJS

Shapes AI game in Unity

Kinect Finger Soccer in C++

Rendering Pipeline in pure JavaScript

Forest Generation using L-System in Three.js

Dogfighting Airplane game in Unity

CommonWealth Games Network Visualization in Flash

*/
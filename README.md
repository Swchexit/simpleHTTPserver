> This is a class project developed by Sheng Wei Chen and Hong Wen Shen during computer network(fall 2022), NTU.

### Feature:
This web server is a simple bulletin which you can sign in and leave comments. 

In this buletin, you can also play video through online streaming.

### Why its special
This server does not rely on any HTTP client packages, but manually parse HTTP packets. It's capable of managing multiple HTTP connections while utilizing self-signed TLS to upgrade unsafe http connection to https. It also offer an unsafe http website, but it will try to upgrade the connection if possible.

This server leverages cookies to maintain login status.

Lastly, this server is capable of streaming videos and audios through HLS protocol.

This server does not use any packages that process HTTP packages.

To run this server, simply do `python3 main.py` with all the required packages installed.
Then, a demostration website would be set up on localhost at port 8787. Feel free to change this site. 

---

Beside this python web server, there is also a simple http server written in C included in this repo.

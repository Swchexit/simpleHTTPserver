### This is a class project developed by Sheng Wei Chen and Hong Wen Shen during computer network(fall 2022), NTU.

This web server is a simple bulletin which you can sign in and leave comments. 
Also, it utilizes https to boost security but also offer an unsafe http website, and tries to upgrade the connection if possible.
This server leverages cookies to maintain login status.
Lastly, this server is capable of streaming videos and audios through HLS protocol.
This server doee not use any packages that process HTTP packages.

To run this server, simply do `python3 main.py` with all the packages installed.
Then, a demostration website would be set up on localhost at port 8787. Feel free to explore this. 

---

Beside this python web server, there is also a simple http server written in C included in this repo.

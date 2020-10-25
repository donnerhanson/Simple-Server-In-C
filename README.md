<!--
*** Thanks for checking out this README Template. If you have a suggestion that would
*** make this better, please fork the repo and create a pull request or simply open
*** an issue with the tag "enhancement".
*** Thanks again! Now go create something AMAZING! :D
***
***
***
*** To avoid retyping too much info. Do a search and replace for the following:
*** donnerhanson, Simple-Server-In-C, donnerhanson, hanso127@mail.chapman.edu
-->





<!-- PROJECT SHIELDS -->
<!--
*** I'm using markdown "reference style" links for readability.
*** Reference links are enclosed in brackets [ ] instead of parentheses ( ).
*** See the bottom of this document for the declaration of the reference variables
*** for contributors-url, forks-url, etc. This is an optional, concise syntax you may use.
*** https://www.markdownguide.org/basic-syntax/#reference-style-links
-->

[![LinkedIn][linkedin-shield]][linkedin-url]



<!-- PROJECT LOGO -->
<br />
<p align="center">
  <a href="https://github.com/donnerhanson/Simple-Server-In-C">
    <img src="images/logo.png" alt="Logo" width="80" height="80">
  </a>

  <h3 align="center">Simple Server in C</h3>

  <p align="center">
  A program written in C to instantiate a simple server
    <br />
  </p>
</p>



<!-- TABLE OF CONTENTS -->
## Table of Contents

* [About the Project](#about-the-project)
* [Getting Started](#getting-started)
  * [Prerequisites](#prerequisites)
  * [Installation](#installation)
* [Usage](#usage)
* [Example](#example)
* [Contact](#contact)



<!-- ABOUT THE PROJECT -->
## About The Program

This program instantiates a server at a user defined IP address and user-defined, or default, port number that gives the client access to the page "index.html", or other hosted documents by interpreting the client issued GET request from the terminal or FireFox Browser. The client can perform the GET request using a telnet connection or browser.

The server connects to multiple client sessions using threading.

When a user sends a properly formatted request, the server returns the requested data if the data exists and also displays information about the received request. If the user supplies an invalid request in terminal, the server responds with the error and expected input. In both the terminal and browser if the user inputs an invalid file name or bad permissions file, but properly formatted request, the type of error is parsed and displayed by the server terminal and the client is sent a 404 not found splash page or 403 or to a 403 FORBIDDEN ACCESS page. The type of error can be viewed in the server terminal window. The connection is closed upon both a successful or unsuccessful GET request.

The server is equipped to send html and txt, i.e. "plain text" data.

Supported requests are:  
1. "GET / HTTP/1.1"
2. "GET /filename HTTP/1.1"
3. "GET / HTTP/1.0"
4. "GET /filename HTTP/1.0"

Supported file extension types are:
1. .html
2. .txt


<!-- GETTING STARTED -->
## Getting Started

To get a local copy up and running follow these simple steps.

### Prerequisites

* Interfacing with the server
```sh
GNU Compiler Collection (GCC)
```
```sh
telnet or an internet browser -- currently tested and functional in FireFox
```

### Installation

1. Clone the repo
```sh
git clone https://github.com/donnerhanson/Simple-Server-In-C.git
```
2. Ensure that index.html, 404.html, and server.c exist within the same directory on your machine
3. Compile the program
```sh
LINUX:
gcc server.c -o server -lpthread -lrt
MacOS:
gcc server.c -o server -lpthread
```

<!-- USAGE EXAMPLES -->
## Usage

1. Start the server using terminal -- recommended to test on your local ip address and using a port number greater than 1024 and less than 65536. If no port number is specified port 6789 is chosen by the program.
```sh
./server [IP ADDRESS] [PORT NUMBER]
```
2. Sending the Get Command Using a Browser
* Open the internet browser and in the address bar type and enter
```sh
http://[IP ADDRESS]:[PORT NUMBER]/
```
3. Sending the Get Command Using a *nix Terminal
* Open a separate Terminal window, type and enter
```sh
telnet [IP ADDRESS] [PORT NUMBER]
```
Then type
```sh
GET / HTTP/1.0
```
followed by enter.

Incorrect input will receive an error message with the expected user input.

4. Shutting Down the Server  

Because the server is meant to exist in a "forever loop" the server process must be shut down with an interrupt signal. In the server terminal window press the keys <Ctrl> and <C> simultaneously to end the server process.

## Example

1. Example 1:  Basic Retrieval no file specified

```
Donner$ telnet 127.0.0.1 5001
Trying 127.0.0.1...
Connected to localhost.
Escape character is '^]'.
GET / HTTP/1.1
HTTP/1.1 200 OK


<html>
<!-- Text between angle brackets is an HTML tag and is not displayed.
Most tags, such as the HTML and /HTML tags that surround the contents of
a page, come in pairs; some tags, like HR, for a horizontal rule, stand
alone. Comments, such as the text you're reading, are not displayed when
the Web page is shown. The information between the HEAD and /HEAD tags is
not displayed. The information between the BODY and /BODY tags is displayed.-->
<head>
<title>Enter a title, displayed at the top of the window.</title>
</head>
<!-- The information between the BODY and /BODY tags is displayed.-->
<body>
<h1>Enter the main heading, usually the same as the title.</h1>
<p>Be <b>bold</b> in stating your key points. Put them in a list: </p>
<ul>
<li>The first item in your list</li>
<li>The second item; <i>italicize</i> key words</li>
</ul>
<p>Improve your image by including an image. </p>
<p><img src="http://www.mygifs.com/CoverImage.gif" alt="A Great HTML Resource"></p>
<p>Add a link to your favorite <a href="https://www.dummies.com/">Web site</a>.  
Break up your page with a horizontal rule or two. </p>  
<hr>  
<p>Finally, link to <a href="page2.html">another page</a> in your own Web site.</p>  
<!-- And add a copyright notice.-->  
<p>&#169; Wiley Publishing, 2011</p>  
</body>  
</html>  
Connection closed by foreign host.  
```

*Server*  
```
GET comm: GET /, currIndex : 0
FNAME: , fname_Index: 5
PROT/NUM: HTTP/1.1, end_index: 13
```

2. Example 2 - File Not Found
*Client*
```
Donner$ telnet 127.0.0.1 5001
Trying 127.0.0.1...
Connected to localhost.
Escape character is '^]'.
GET /notarealfile.txt HTTP/1.0
HTTP/1.0 404 NOT FOUND


<!doctype html>
<!-- https://github.com/h5bp/html5-boilerplate/blob/master/src/404.html -->
<html lang="en">

<head>
  <meta charset="utf-8">
  <title>Page Not Found</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <style>
    * {
      line-height: 1.2;
      margin: 0;
    }

    html {
      color: #888;
      display: table;
      font-family: sans-serif;
      height: 100%;
      text-align: center;
      width: 100%;
    }

    body {
      display: table-cell;
      vertical-align: middle;
      margin: 2em auto;
    }

    h1 {
      color: #555;
      font-size: 2em;
      font-weight: 400;
    }

    p {
      margin: 0 auto;
      width: 280px;
    }

    @media only screen and (max-width: 280px) {

      body,
      p {
        width: 95%;
      }

      h1 {
        font-size: 1.5em;
        margin: 0 0 0.3em;
      }

    }
  </style>
</head>

<body>
  <h1>Page Not Found</h1>
  <p>Sorry, but the page you were trying to view does not exist.</p>
</body>

</html>
<!-- IE needs 512+ bytes: https://docs.microsoft.com/archive/blogs/ieinternals/friendly-http-error-pages -->

Connection closed by foreign host.
```
*Server*
```
GET comm: GET /, currIndex : 0
FNAME: notarealfile.txt, fname_Index: 21
PROT/NUM: HTTP/1.0, end_index: 29
```
3. Example 3 - File Specified
*Client*
```
Donner$ telnet 127.0.0.1 5001
Trying 127.0.0.1...
Connected to localhost.
Escape character is '^]'.
GET /text.txt HTTP/1.1
HTTP/1.1 200 OK


The goal of this assignment is to build a functional HTTP server. This assignment will teach you the basics At a high level, a web server listens for connections on a socket (bound to a specific port on a host machine). Clients connect to this socket and use a simple text-based protocol to retrieve files from the server. FOne of the key things to keep in mind in building your web server is that the server is translating relative filenames (such as index.html) to absolute filenames in a local file system. For example, you may decide to keep all the files for your server in cstudent/server/files/, which we call the root. When your server gets a request for /index.html, it will pre-pend the root to the specified file and determine if the file exists, and if the proper permissions are set on the file (typically the file has to be world readable). If the file does not exist, a file not found error is returned. If a file is present but the proper permissions are not set, a permission denied error is returned. Otherwise, an HTTP OK message is returned along with the contYou should also note that web servers typically translate "GET /" to "GET /index.html". That is, index.html is assumed to be the filename if no explicit filename is present. The default filename can also be overriddeFor this assignment, you will need to support enough of the HTTP protocol to allow an existing web browser (Firefox, Safari or Internet Explorer) to connect to your web server and retrieve the contents of a sample page from your server. (Of course, this will require that you copy the appropriate files to your server's docu•ou yourA multi-threaded approach will spawn a new thread for each incoming connection. That is, once the sePlease refer to the HTTP 1.0 RFC to learn more about the method syntax and error codes. You can check the validity of your implementation by issuing queries from a telnet session or from the client described in next Implement your assignment in either C or C++. You will want to become familiar with the interactions of the following system calls to build your system: socket(), select(), listen(), accept(), connect(). We outline a number of resources below with additional information on these system calls. Several books are also availabIf <port> is not passed, you will choose your own default port number. Make sure it is larger than 1024 and You will write C++ (or C) code that compiles under the GCC (GNU Compiler Collection) environment. You have to make sure your code will compile and run correctly on a Linux Host. Please submit the source code with a mConnection closed by foreign host.t file describing how to compile and run your program from a terminal.
```

*Server*
```
GET comm: GET /, currIndex : 0
FNAME: text.txt, fname_Index: 13
PROT/NUM: HTTP/1.1, end_index: 21
```

<!-- CONTACT -->
## Contact

Donner Hanson - [@hansondonner](https://twitter.com/hansondonner) - hanso127@mail.chapman.edu - ChapmanID - 001276484

Project Link: [https://github.com/donnerhanson/Simple-Server-In-C](https://github.com/donnerhanson/Simple-Server-In-C)



<!-- MARKDOWN LINKS & IMAGES -->
<!-- https://www.markdownguide.org/basic-syntax/#reference-style-links -->

[linkedin-shield]: https://img.shields.io/badge/-LinkedIn-black.svg?style=flat-square&logo=linkedin&colorB=555
[linkedin-url]: https://linkedin.com/in/donner-hanson

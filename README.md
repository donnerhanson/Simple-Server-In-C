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
  * [Built With](#built-with)
* [Getting Started](#getting-started)
  * [Prerequisites](#prerequisites)
  * [Installation](#installation)
* [Usage](#usage)
* [Roadmap](#roadmap)
* [Contributing](#contributing)
* [License](#license)
* [Contact](#contact)
* [Acknowledgements](#acknowledgements)



<!-- ABOUT THE PROJECT -->
## About The Program

This program instantiates a server at a user defined IP address
and port number that gives a client access to the page
"index.html", OR some other developer-user defined page,
when the client issues a GET request in the terminal or FireFox Browser.

The server can connect to multiple client sessions using threading.

When a user sends a request the server notifies the user if there is an invalid request and the type of error otherwise will send
the user the intended html document

The server closes the connection upon sending either
the document or the error information

The supported requests are:  
1. "GET / HTTP/1.1"
2. "GET /filename HTTP/1.1"
3. "GET / HTTP/1.0"
4. "GET /filename HTTP/1.0"

Supported file extension types are:
1. html
2. txt






<!-- GETTING STARTED -->
## Getting Started

To get a local copy up and running follow these simple steps.

### Prerequisites

This is an example of how to list things you need to use the software and how to install them.
* npm
```sh
npm install npm@latest -g
```

### Installation

1. Clone the repo
```sh
git clone https://github.com/donnerhanson/Simple-Server-In-C.git
```
2. Compile the program
```sh
gcc server.c -o server -lpthread -lrt
```



<!-- USAGE EXAMPLES -->
## Usage

Use this space to show useful examples of how a project can be used. Additional screenshots, code examples and demos work well in this space. You may also link to more resources.

_For more examples, please refer to the [Documentation](https://example.com)_


<!-- CONTACT -->
## Contact

Donner Hanson - [@donnerhanson](https://twitter.com/donnerhanson) - hanso127@mail.chapman.edu

Project Link: [https://github.com/donnerhanson/Simple-Server-In-C](https://github.com/donnerhanson/Simple-Server-In-C)



<!-- MARKDOWN LINKS & IMAGES -->
<!-- https://www.markdownguide.org/basic-syntax/#reference-style-links -->

[linkedin-shield]: https://img.shields.io/badge/-LinkedIn-black.svg?style=flat-square&logo=linkedin&colorB=555
[linkedin-url]: https://linkedin.com/in/donner-hanson

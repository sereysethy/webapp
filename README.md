An incomplete web server to showcase socket connection in C. It is by no means
a web server. It is just an example. The program only accepts numeric route
`/1, /2, /..`, any other than that it will reply `404 Not Found`.

More info on HTTP response and request protocol:
- https://tools.ietf.org/html/rfc7230#section-2.1
- https://www.w3.org/Protocols/rfc2616/rfc2616-sec4.html

To compile, type

```
$ make
```

By default, the server port is set to `8080`

```
$ ./webapp
```

To change the default port to `1234`

```
$ ./webapp 1234
```

To stop execution, type `CRTL+C`

To test, type following

```
$  curl -D - http://localhost:8080/1
```

or type `http://localhost:8080/1` in any browsers.

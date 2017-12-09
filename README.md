An incomplete web server to showcase socket connection in C. It is by no means
a web server. It is just an example.

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
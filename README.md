# TS

## CREDIT 
C# TLS_CLIENT: https://stackoverflow.com/questions/39304612/c-sharp-ssl-tls-with-socket-tcp

Simple TLS_server: https://wiki.openssl.org/index.php/Simple_TLS_Server

## install submodule
We use [JSMN](https://github.com/zserge/jsmn/) to parse JSON for c
```
   git clone https://github.com/zserge/jsmn/
   eiei
```
## To make a tls_server
```use gcc -o tls_server tls_server.c -lcrypto -lssl```

## To make a json_server
```use gcc -o json_server json_server.c -lcrypto -lssl -ljsmn```

## License
[MIT](https://choosealicense.com/licenses/mit/)

##by BMI

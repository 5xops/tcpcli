# tcpcli

## what can do

- to test an ip address and port is open or not
- send text to a port and get output

## how to build

```
docker buildx build . --platform linux/amd64 --target bin --output .
docker buildx build . --platform linux/arm64 --target bin --output .
```

## how to use

```
CONN_TIMEOUT=500 ./tcpcli 127.0.0.1 2181 stat
CONN_TIMEOUT=500 ./tcpcli 127.0.0.1 2181
echo $?
```
## license

MIT

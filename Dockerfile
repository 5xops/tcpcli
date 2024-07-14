FROM ubuntu as build
WORKDIR /opt
RUN if [  -e /etc/apt/sources.list ];then sed -ri 's/[a-zA-Z0-9.]+(debian.org|ubuntu.com)/mirrors.volces.com/g' /etc/apt/sources.list; fi && \
    export DEBIAN_FRONTEND=noninteractive && \
    apt-get update && \
    apt-get install -y gcc make cmake
COPY ./tcpcli.c /
RUN cd / && gcc -static -o tcpcli tcpcli.c && \
    strip tcpcli && cp tcpcli /tcpcli-$(dpkg --print-architecture)

FROM scratch AS bin
COPY --from=build /tcpcli-* /

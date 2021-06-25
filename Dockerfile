FROM alpine:latest

RUN apk add --update ncurses-dev libc-dev gcc make

COPY . /pixel
WORKDIR /pixel
RUN make clean && make all
ENTRYPOINT [ "./pixel" ]

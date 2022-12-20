FROM gcc:latest

COPY lib/ /workdir/lib/
COPY bin/ /workdir/bin
COPY src/ /workdir/src/
COPY Makefile /workdir/Makefile
COPY project.conf /workdir/project.conf

WORKDIR /workdir/

RUN make clean

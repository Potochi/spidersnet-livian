FROM --platform=amd64 ubuntu:22.04 as builder

RUN apt update -y && apt install -y gcc make

RUN mkdir /artifacts
COPY ./src/* /artifacts/
WORKDIR /artifacts

RUN make

FROM --platform=amd64 ubuntu:22.04 as runner

RUN apt update -y && apt install -y xinetd


RUN echo "Blocked by xinetd" > /etc/banner_fail
COPY ./config/chall.xinetd /etc/xinetd.d/chall

COPY ./start.sh /start.sh
RUN chmod +x start.sh

ENV CHALL_USER=ctf
ENV CHALL_PATH=/home/${CHALL_USER}

RUN useradd -m ${CHALL_USER}

WORKDIR ${CHALL_PATH}

COPY --from=builder /artifacts/sirop .
COPY ./flag.txt .

RUN chown root:${CHALL_USER} ./sirop
RUN chmod 750 ./sirop

RUN chown root:${CHALL_USER} ./flag.txt
RUN chmod 740 ./flag.txt

EXPOSE 2000

CMD ["/start.sh"]

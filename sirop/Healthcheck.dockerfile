FROM --platform=amd64 ubuntu:22.04

RUN apt update -y && apt install -y python3 python3-pip
RUN pip3 install pwntools

RUN useradd -m health

WORKDIR /home/health

COPY ./healthcheck/health.py .
COPY ./artifacts/sirop .
COPY ./flag.txt .

RUN chown root:health sirop 
RUN chmod 740 sirop

RUN chown root:health flag.txt 
RUN chmod 740 flag.txt

RUN chown root:health health.py
RUN chmod 750 health.py

USER health

CMD ["sleep", "infinity"]

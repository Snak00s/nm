FROM debian:bookworm

RUN dpkg --add-architecture i386 && \
    apt-get update && \
    apt-get install -y gcc gcc-multilib && \
	apt-get -y install make \
    rm -rf /var/lib/apt/lists/*

WORKDIR /srcs
FROM ubuntu:latest
ENV DEBIAN_FRONTEND noninteractive
RUN apt-get update \
    && apt-get --no-install-recommends -y install \
    siege \
    iputils-ping \
    ufw \
    && apt-get clean \
    && rm -rf /var/lib/apt/lists
EXPOSE 6969
ENTRYPOINT ["tail", "-f"]


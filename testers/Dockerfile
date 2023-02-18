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

#docker build . -t siege
#docker run siege
#docker ps -> to get 'CONTAINER_ID'
#docker exec -it 'CONTAINER_ID' bash
#docker exec -it 'container_name' bash
#ifconfig | grep "inet"      -> to know private ip address
#run server AND siege on host private ip address
#siege http://IP_ADDRESS:PORT


# docker system prune --all -> to remove all stored container data
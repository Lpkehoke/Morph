FROM ubuntu:bionic
RUN apt-get update && apt-get -y install g++-8 ninja-build pkg-config python3 python3-pip python3-dev
RUN pip3 install conan meson && conan user
COPY . .
CMD ./docker-build.sh

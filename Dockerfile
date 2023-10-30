# Start with the Ubuntu 16.04 base image
FROM ubuntu:22.04

# Set the working directory for building
WORKDIR /boundless

# Avoid interaction with the user for apt-get install
ENV DEBIAN_FRONTEND noninteractive

# Run system update
RUN apt-get update -y \
    && apt-get install -y apt-utils software-properties-common

# Install required packages
RUN add-apt-repository -y ppa:ubuntuhandbook1/ppa \
    && apt-get update -y \
    && apt-get install -y build-essential libtool autotools-dev automake pkg-config libssl-dev libevent-dev bsdmainutils python3 \
    libboost-system-dev libboost-filesystem-dev libboost-chrono-dev libboost-program-options-dev libboost-test-dev libboost-thread-dev \
    libqt5gui5 libqt5core5a libqt5dbus5 qttools5-dev qttools5-dev-tools libprotobuf-dev protobuf-compiler libboost-all-dev \
    software-properties-common git libminiupnpc-dev libzmq3-dev libqt4-dev wget unzip

# Copy sources to working directory
COPY . .

# Download libdb4.8 sources
RUN wget http://download.oracle.com/berkeley-db/db-4.8.30.zip \
    && unzip db-4.8.30.zip

# Step into libdb build directory
WORKDIR db-4.8.30/build_unix

# Fix overwrite of gcc builtin
RUN sed -i 's/__atomic_compare_exchange/db_atomic_compare_exchange_db/g' ../dbinc/atomic.h

# Build libdb and install
RUN ../dist/configure --prefix=/usr --enable-cxx \
    && make \
    && make install

# Back to root directory
WORKDIR ../../

# Build the software
RUN cd src \
    && make -f makefile.unix \
    && cd .. \
    && qmake && make

# Set the default command for the container
CMD ["/bin/bash"]
